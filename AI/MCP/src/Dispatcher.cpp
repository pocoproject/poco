//
// Dispatcher.cpp
//
// Library: AIMCP
// Package: Core
// Module:  Dispatcher
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Dispatcher.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"
#include <exception>
#include <vector>


namespace Poco {
namespace AI {
namespace MCP {


namespace
{
	Poco::JSON::Object::Ptr emptyObject()
	{
		return new Poco::JSON::Object;
	}
}


Dispatcher::Dispatcher():
	_initialized(false)
{
}


Dispatcher::~Dispatcher()
{
}


void Dispatcher::registerTool(const Tool& tool)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_tools[tool.name] = tool;
}


void Dispatcher::registerResource(const Resource& resource, ResourceReader reader)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	ResourceEntry entry;
	entry.resource = resource;
	entry.reader = reader;
	_resources[resource.uri] = entry;
}


void Dispatcher::registerPrompt(const Prompt& prompt, PromptRenderer renderer)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	PromptEntry entry;
	entry.prompt = prompt;
	entry.renderer = renderer;
	_prompts[prompt.name] = entry;
}


void Dispatcher::setServerInfo(const std::string& name, const std::string& version)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_serverName = name;
	_serverVersion = version;
}


void Dispatcher::setInstructions(const std::string& instructions)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_instructions = instructions;
}


bool Dispatcher::initialized() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _initialized;
}


Poco::JSON::Object::Ptr Dispatcher::handle(const Poco::JSON::Object::Ptr& message)
{
	return handle(message, RequestContext());
}


Poco::JSON::Object::Ptr Dispatcher::handle(const Poco::JSON::Object::Ptr& message, const RequestContext& context)
{
	if (!message)
	{
		return Response::error(Id(), ErrorCode::InvalidRequest, "Invalid Request");
	}

	const bool hasId = message->has("id");
	const Id id = hasId ? message->get("id") : Id();
	const std::string version = message->optValue<std::string>("jsonrpc", "");
	const Poco::Dynamic::Var methodValue = message->get("method");

	// JSON-RPC wants the method to be a string; anything else, a missing
	// method included, is not a request.
	if (version != JSONRPC_VERSION || !methodValue.isString())
	{
		// A malformed notification gets no reply; a malformed request gets an error.
		if (!hasId)
		{
			return Poco::JSON::Object::Ptr();
		}
		return Response::error(id, ErrorCode::InvalidRequest, "Invalid Request");
	}

	const std::string method = methodValue.extract<std::string>();
	Poco::JSON::Object::Ptr params = message->isObject("params") ? message->getObject("params") : Poco::JSON::Object::Ptr();

	if (!hasId)
	{
		onNotification(method, params);
		return Poco::JSON::Object::Ptr();
	}

	if (method == "initialize")
	{
		return onInitialize(id, params);
	}
	if (method == "ping")
	{
		return Response::result(id, emptyObject());
	}
	if (!initialized())
	{
		return Response::error(id, ErrorCode::InvalidRequest, "Server not initialized");
	}
	if (method == "tools/list")
	{
		return onToolsList(id, params);
	}
	if (method == "tools/call")
	{
		return onToolsCall(id, params, context);
	}
	if (method == "resources/list")
	{
		return onResourcesList(id, params);
	}
	if (method == "resources/read")
	{
		return onResourcesRead(id, params);
	}
	if (method == "prompts/list")
	{
		return onPromptsList(id, params);
	}
	if (method == "prompts/get")
	{
		return onPromptsGet(id, params);
	}
	return Response::error(id, ErrorCode::MethodNotFound, "Method not found: " + method);
}


void Dispatcher::onNotification(const std::string& method, const Poco::JSON::Object::Ptr& /*params*/)
{
	if (method == "notifications/initialized")
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_initialized = true;
	}
	// Other notifications (e.g. notifications/cancelled) are accepted and ignored.
}


Poco::JSON::Object::Ptr Dispatcher::onInitialize(const Id& id, const Poco::JSON::Object::Ptr& /*params*/)
{
	std::string name, serverVersion, instructions;
	bool hasResources, hasPrompts;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		// Requests are served from here on. The specification holds the client
		// back only until the server has responded to initialize; the
		// initialized notification is what the server waits for before it
		// sends requests of its own, and this one sends none.
		_initialized = true;
		name = _serverName;
		serverVersion = _serverVersion;
		instructions = _instructions;
		hasResources = !_resources.empty();
		hasPrompts = !_prompts.empty();
	}

	Poco::JSON::Object::Ptr toolsCap = new Poco::JSON::Object;
	toolsCap->set("listChanged", false);

	Poco::JSON::Object::Ptr caps = new Poco::JSON::Object;
	caps->set("tools", toolsCap);

	// Advertise resources/prompts only when the server actually has some, so a
	// tools-only server's initialize result is unchanged.
	if (hasResources)
	{
		Poco::JSON::Object::Ptr resourcesCap = new Poco::JSON::Object;
		resourcesCap->set("listChanged", false);
		caps->set("resources", resourcesCap);
	}
	if (hasPrompts)
	{
		Poco::JSON::Object::Ptr promptsCap = new Poco::JSON::Object;
		promptsCap->set("listChanged", false);
		caps->set("prompts", promptsCap);
	}

	Poco::JSON::Object::Ptr info = new Poco::JSON::Object;
	info->set("name", name);
	info->set("version", serverVersion);

	Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
	result->set("protocolVersion", PROTOCOL_VERSION);
	result->set("capabilities", caps);
	result->set("serverInfo", info);
	if (!instructions.empty())
	{
		result->set("instructions", instructions);
	}
	return Response::result(id, result);
}


Poco::JSON::Object::Ptr Dispatcher::onToolsList(const Id& id, const Poco::JSON::Object::Ptr& /*params*/)
{
	std::vector<Tool> tools;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		for (const auto& entry: _tools)
		{
			tools.push_back(entry.second);
		}
	}

	Poco::JSON::Array::Ptr list = new Poco::JSON::Array;
	for (const Tool& tool: tools)
	{
		Poco::JSON::Object::Ptr t = new Poco::JSON::Object;
		t->set("name", tool.name);
		t->set("description", tool.description);
		t->set("inputSchema", tool.inputSchema ? tool.inputSchema : emptyObject());
		if (tool.outputSchema)
		{
			t->set("outputSchema", tool.outputSchema);
		}
		if (tool.annotations)
		{
			t->set("annotations", tool.annotations);
		}
		list->add(t);
	}

	Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
	result->set("tools", list);
	return Response::result(id, result);
}


Poco::JSON::Object::Ptr Dispatcher::onToolsCall(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context)
{
	if (!params || !params->has("name"))
	{
		return Response::error(id, ErrorCode::InvalidParams, "Missing tool name");
	}
	const std::string name = params->getValue<std::string>("name");

	if (params->has("arguments") && !params->isObject("arguments"))
	{
		return Response::error(id, ErrorCode::InvalidParams, "Tool arguments must be an object");
	}

	ToolHandler handler;
	ContextToolHandler contextHandler;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		std::map<std::string, Tool>::const_iterator it = _tools.find(name);
		if (it == _tools.end())
		{
			return Response::error(id, ErrorCode::InvalidParams, "Unknown tool: " + name);
		}
		handler = it->second.handler;
		contextHandler = it->second.contextHandler;
	}

	Poco::JSON::Object::Ptr arguments = params->isObject("arguments") ? params->getObject("arguments") : Poco::JSON::Object::Ptr(new Poco::JSON::Object);

	ToolResult toolResult;
	try
	{
		// The context-aware handler wins when both are set, so a tool that needs
		// the caller's identity gets it.
		if (contextHandler)
		{
			toolResult = contextHandler(arguments, context);
		}
		else
		{
			toolResult = handler(arguments);
		}
	}
	catch (Poco::Exception& exc)
	{
		toolResult = ToolResult();
		toolResult.setError().addText(exc.displayText());
	}
	catch (std::exception& exc)
	{
		toolResult = ToolResult();
		toolResult.setError().addText(exc.what());
	}

	return Response::result(id, toolResult.toObject());
}


Poco::JSON::Object::Ptr Dispatcher::onResourcesList(const Id& id, const Poco::JSON::Object::Ptr& /*params*/)
{
	std::vector<Resource> resources;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		for (const auto& entry: _resources)
		{
			resources.push_back(entry.second.resource);
		}
	}

	Poco::JSON::Array::Ptr list = new Poco::JSON::Array;
	for (const Resource& r: resources)
	{
		Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
		o->set("uri", r.uri);
		o->set("name", r.name);
		if (!r.description.empty()) o->set("description", r.description);
		if (!r.mimeType.empty()) o->set("mimeType", r.mimeType);
		list->add(o);
	}

	Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
	result->set("resources", list);
	return Response::result(id, result);
}


Poco::JSON::Object::Ptr Dispatcher::onResourcesRead(const Id& id, const Poco::JSON::Object::Ptr& params)
{
	const std::string uri = params ? params->optValue<std::string>("uri", "") : "";

	ResourceReader reader;
	std::string mimeType;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		std::map<std::string, ResourceEntry>::const_iterator it = _resources.find(uri);
		if (it == _resources.end())
		{
			return Response::error(id, ErrorCode::InvalidParams, "Unknown resource: " + uri);
		}
		reader = it->second.reader;
		mimeType = it->second.resource.mimeType;
	}

	std::string text;
	try
	{
		text = reader ? reader(uri) : std::string();
	}
	catch (Poco::Exception& exc)
	{
		return Response::error(id, ErrorCode::InternalError, exc.displayText());
	}
	catch (std::exception& exc)
	{
		return Response::error(id, ErrorCode::InternalError, exc.what());
	}

	Poco::JSON::Object::Ptr content = new Poco::JSON::Object;
	content->set("uri", uri);
	if (!mimeType.empty()) content->set("mimeType", mimeType);
	content->set("text", text);

	Poco::JSON::Array::Ptr contents = new Poco::JSON::Array;
	contents->add(content);
	Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
	result->set("contents", contents);
	return Response::result(id, result);
}


Poco::JSON::Object::Ptr Dispatcher::onPromptsList(const Id& id, const Poco::JSON::Object::Ptr& /*params*/)
{
	std::vector<Prompt> prompts;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		for (const auto& entry: _prompts)
		{
			prompts.push_back(entry.second.prompt);
		}
	}

	Poco::JSON::Array::Ptr list = new Poco::JSON::Array;
	for (const Prompt& p: prompts)
	{
		Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
		o->set("name", p.name);
		if (!p.description.empty()) o->set("description", p.description);
		if (!p.arguments.empty())
		{
			Poco::JSON::Array::Ptr args = new Poco::JSON::Array;
			for (const PromptArgument& a: p.arguments)
			{
				Poco::JSON::Object::Ptr ao = new Poco::JSON::Object;
				ao->set("name", a.name);
				if (!a.description.empty()) ao->set("description", a.description);
				ao->set("required", a.required);
				args->add(ao);
			}
			o->set("arguments", args);
		}
		list->add(o);
	}

	Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
	result->set("prompts", list);
	return Response::result(id, result);
}


Poco::JSON::Object::Ptr Dispatcher::onPromptsGet(const Id& id, const Poco::JSON::Object::Ptr& params)
{
	const std::string name = params ? params->optValue<std::string>("name", "") : "";

	Prompt prompt;
	PromptRenderer renderer;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		std::map<std::string, PromptEntry>::const_iterator it = _prompts.find(name);
		if (it == _prompts.end())
		{
			return Response::error(id, ErrorCode::InvalidParams, "Unknown prompt: " + name);
		}
		prompt = it->second.prompt;
		renderer = it->second.renderer;
	}

	Poco::JSON::Object::Ptr arguments = (params && params->isObject("arguments"))
		? params->getObject("arguments") : Poco::JSON::Object::Ptr(new Poco::JSON::Object);

	Poco::JSON::Array::Ptr messages;
	try
	{
		messages = renderer ? renderer(arguments) : Poco::JSON::Array::Ptr(new Poco::JSON::Array);
	}
	catch (Poco::Exception& exc)
	{
		return Response::error(id, ErrorCode::InternalError, exc.displayText());
	}
	catch (std::exception& exc)
	{
		return Response::error(id, ErrorCode::InternalError, exc.what());
	}

	Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
	if (!prompt.description.empty()) result->set("description", prompt.description);
	result->set("messages", messages ? messages : Poco::JSON::Array::Ptr(new Poco::JSON::Array));
	return Response::result(id, result);
}


} } } // namespace Poco::AI::MCP
