//
// Server.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  Server
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"
#include "Poco/Logger.h"
#include <algorithm>
#include <exception>
#include <istream>
#include <ostream>
#include <utility>
#include <vector>


namespace Poco::AI::MCP {


namespace
{
	Poco::JSON::Object::Ptr invalidParams(const Id& id, const std::string& message, const Poco::JSON::Object::Ptr& pData = nullptr)
	{
		return Response::error(id, ErrorCode::INVALID_PARAMS, message, pData);
	}


	Poco::JSON::Object::Ptr unsupportedVersion(const Id& id, const Poco::Dynamic::Var& requested)
		/// The error that names the supported revision, in its message and in
		/// its data, and the one the request asked for: empty when it named
		/// none.
	{
		Poco::JSON::Array::Ptr pSupported = new Poco::JSON::Array;
		pSupported->add(std::string(PROTOCOL_VERSION));
		Poco::JSON::Object::Ptr pData = new Poco::JSON::Object;
		pData->set("supported", pSupported);
		pData->set("requested", requested.isString() ? requested.extract<std::string>() : std::string());
		return Response::error(id, ErrorCode::UNSUPPORTED_PROTOCOL_VERSION, std::string("Unsupported protocol version: this server implements ") + PROTOCOL_VERSION, pData);
	}


	bool hasCursor(const Poco::JSON::Object::Ptr& pParams)
	{
		return pParams != nullptr && pParams->has("cursor");
	}


	Poco::JSON::Object::Ptr invalidCursor(const Id& id)
		/// Every list comes in one page, so no cursor is valid.
	{
		return invalidParams(id, "Invalid cursor");
	}
}


Server::Server()
{
	_settings.serverInfo = Implementation::library();
}


Server::~Server()
{
}


void Server::registerTool(const Tool& tool)
{
	if (tool.name.empty())
	{
		throw Poco::InvalidArgumentException("A tool needs a name");
	}
	if (!tool.handler)
	{
		throw Poco::InvalidArgumentException("A tool needs a handler", tool.name);
	}
	if (tool.inputSchema != nullptr)
	{
		const Poco::Dynamic::Var type = tool.inputSchema->get("type");
		if (!type.isString() || type.extract<std::string>() != "object")
		{
			throw Poco::InvalidArgumentException("The input schema of a tool must have the type object", tool.name);
		}
	}
	Poco::FastMutex::ScopedLock lock(_mutex);
	_tools[tool.name] = tool;
}


void Server::registerTool(const std::string& name, const std::string& description, const Poco::JSON::Object::Ptr& inputSchema, ToolHandler handler)
{
	if (!handler)
	{
		throw Poco::InvalidArgumentException("A tool needs a handler", name);
	}
	Tool tool;
	tool.name = name;
	tool.description = description;
	tool.inputSchema = inputSchema;
	tool.handler = [handler = std::move(handler)](const Poco::JSON::Object::Ptr& arguments, const RequestContext&)
	{
		return handler(arguments);
	};
	registerTool(tool);
}


std::optional<Tool> Server::findTool(const std::string& name) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	const auto it = _tools.find(name);
	if (it == _tools.end()) return std::nullopt;
	return it->second;
}


void Server::registerResource(const Resource& resource, ResourceReader reader)
{
	if (resource.uri.empty())
	{
		throw Poco::InvalidArgumentException("A resource needs a URI");
	}
	if (!reader)
	{
		throw Poco::InvalidArgumentException("A resource needs a reader", resource.uri);
	}
	Poco::FastMutex::ScopedLock lock(_mutex);
	_resources[resource.uri] = ResourceEntry{resource, std::move(reader)};
}


void Server::registerPrompt(const Prompt& prompt, PromptRenderer renderer)
{
	if (prompt.name.empty())
	{
		throw Poco::InvalidArgumentException("A prompt needs a name");
	}
	if (!renderer)
	{
		throw Poco::InvalidArgumentException("A prompt needs a renderer", prompt.name);
	}
	Poco::FastMutex::ScopedLock lock(_mutex);
	_prompts[prompt.name] = PromptEntry{prompt, std::move(renderer)};
}


void Server::setServerInfo(const Implementation& info)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_settings.serverInfo = info;
}


void Server::setInstructions(const std::string& instructions)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_settings.instructions = instructions;
}


void Server::setCacheHints(const Poco::Timespan& ttl, CacheScope scope)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_settings.ttl = ttl;
	_settings.scope = scope;
}


void Server::setLoggerName(const std::string& name)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_settings.loggerName = name;
}


Server::Settings Server::settings() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _settings;
}


Poco::JSON::Object::Ptr Server::handle(const Poco::JSON::Object::Ptr& message, const RequestContext& context)
{
	if (message == nullptr)
	{
		return Response::error(Id(), ErrorCode::INVALID_REQUEST, "Invalid Request");
	}

	const bool hasId = message->has("id");
	const Poco::Dynamic::Var idValue = message->get("id");
	if (hasId && !isValidId(idValue))
	{
		// Requests carry a string or an integer id, never null.
		return Response::error(Id(), ErrorCode::INVALID_REQUEST, "Invalid Request: the id must be a string or an integer");
	}
	const Id id = hasId ? idValue : Id();

	const Poco::Dynamic::Var method = message->get("method");
	if (message->optValue<std::string>("jsonrpc", "") != JSONRPC_VERSION || !method.isString())
	{
		// Only a well-formed notification goes without a reply.
		return Response::error(id, ErrorCode::INVALID_REQUEST, "Invalid Request");
	}
	if (!hasId)
	{
		return Poco::JSON::Object::Ptr();
	}
	const std::string& name = method.extract<std::string>();

	if (message->has("params") && !message->isObject("params"))
	{
		return invalidParams(id, "The params must be an object");
	}
	const Poco::JSON::Object::Ptr pParams = message->isObject("params") ? message->getObject("params") : Poco::JSON::Object::Ptr();

	if (name == "initialize")
	{
		// A client of an earlier revision: tell it which revision this server speaks.
		return unsupportedVersion(id, pParams != nullptr ? pParams->get("protocolVersion") : Poco::Dynamic::Var());
	}

	const Poco::JSON::Object::Ptr pMeta = (pParams != nullptr && pParams->isObject("_meta")) ? pParams->getObject("_meta") : Poco::JSON::Object::Ptr();
	if (pMeta == nullptr)
	{
		return invalidParams(id, "The request has no _meta object");
	}
	const Poco::Dynamic::Var version = pMeta->get(META_PROTOCOL_VERSION);
	if (!version.isString())
	{
		return invalidParams(id, std::string("The _meta object has no ") + META_PROTOCOL_VERSION);
	}
	if (version.extract<std::string>() != PROTOCOL_VERSION)
	{
		return unsupportedVersion(id, version);
	}
	if (!pMeta->isObject(META_CLIENT_CAPABILITIES))
	{
		return invalidParams(id, std::string("The _meta object has no ") + META_CLIENT_CAPABILITIES);
	}

	RequestContext requestContext(context);
	requestContext.protocolVersion = version.extract<std::string>();
	requestContext.clientCapabilities = pMeta->getObject(META_CLIENT_CAPABILITIES);
	requestContext.clientInfo = Implementation::fromObject(pMeta->isObject(META_CLIENT_INFO) ? pMeta->getObject(META_CLIENT_INFO) : Poco::JSON::Object::Ptr());

	try
	{
		return dispatch(id, name, pParams, requestContext);
	}
	catch (const MCPException& exc)
	{
		// An error a handler meant for the client.
		return Response::error(id, exc.code() != 0 ? static_cast<ErrorCode>(exc.code()) : ErrorCode::INTERNAL_ERROR, exc.message());
	}
	catch (const Poco::Exception& exc)
	{
		return failed(id, name, exc.displayText());
	}
	catch (const std::exception& exc)
	{
		return failed(id, name, exc.what());
	}
	catch (...)
	{
		return failed(id, name, "unknown exception");
	}
}


Poco::JSON::Object::Ptr Server::failed(const Id& id, const std::string& method, const std::string& what) const
	/// The reply to a request that failed for a reason the client has no
	/// business to know: the reason goes to the log.
{
	Poco::Logger& logger = Poco::Logger::get(settings().loggerName);
	if (logger.error())
	{
		logger.error("MCP request %s failed: %s", method, what);
	}
	return Response::error(id, ErrorCode::INTERNAL_ERROR, "Internal error");
}


Poco::JSON::Object::Ptr Server::dispatch(const Id& id, const std::string& method, const Poco::JSON::Object::Ptr& params, const RequestContext& context)
{
	if (method == "server/discover") return onDiscover(id);
	if (method == "tools/list") return onToolsList(id, params);
	if (method == "tools/call") return onToolsCall(id, params, context);
	bool hasResources = false;
	bool hasPrompts = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		hasResources = !_resources.empty();
		hasPrompts = !_prompts.empty();
	}
	// The methods of a capability server/discover does not advertise are not
	// found.
	if (hasResources)
	{
		if (method == "resources/list") return onResourcesList(id, params);
		if (method == "resources/templates/list") return onResourceTemplatesList(id, params);
		if (method == "resources/read") return onResourcesRead(id, params, context);
	}
	if (hasPrompts)
	{
		if (method == "prompts/list") return onPromptsList(id, params);
		if (method == "prompts/get") return onPromptsGet(id, params, context);
	}
	return Response::error(id, ErrorCode::METHOD_NOT_FOUND, "Method not found: " + method);
}


Poco::JSON::Object::Ptr Server::complete(const Id& id, Poco::JSON::Object::Ptr result, bool cacheable)
{
	const Settings current = settings();
	result->set("resultType", "complete");
	if (cacheable)
	{
		result->set("ttlMs", std::max<Poco::Timespan::TimeDiff>(0, current.ttl.totalMilliseconds()));
		result->set("cacheScope", current.scope == CacheScope::PUBLIC ? "public" : "private");
	}
	if (!current.serverInfo.empty())
	{
		Poco::JSON::Object::Ptr pMeta = new Poco::JSON::Object;
		pMeta->set(META_SERVER_INFO, current.serverInfo.toObject());
		result->set("_meta", pMeta);
	}
	return Response::result(id, result);
}


Poco::JSON::Object::Ptr Server::onDiscover(const Id& id)
{
	bool hasResources = false;
	bool hasPrompts = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		hasResources = !_resources.empty();
		hasPrompts = !_prompts.empty();
	}
	Poco::JSON::Object::Ptr pCapabilities = new Poco::JSON::Object;
	pCapabilities->set("tools", Poco::JSON::Object::Ptr(new Poco::JSON::Object));
	if (hasResources) pCapabilities->set("resources", Poco::JSON::Object::Ptr(new Poco::JSON::Object));
	if (hasPrompts) pCapabilities->set("prompts", Poco::JSON::Object::Ptr(new Poco::JSON::Object));

	Poco::JSON::Array::Ptr pVersions = new Poco::JSON::Array;
	pVersions->add(std::string(PROTOCOL_VERSION));

	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	pResult->set("supportedVersions", pVersions);
	pResult->set("capabilities", pCapabilities);
	const std::string instructions = settings().instructions;
	if (!instructions.empty()) pResult->set("instructions", instructions);
	return complete(id, pResult, true);
}


Poco::JSON::Object::Ptr Server::onToolsList(const Id& id, const Poco::JSON::Object::Ptr& params)
{
	if (hasCursor(params)) return invalidCursor(id);
	std::vector<Tool> tools;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		tools.reserve(_tools.size());
		for (const auto& entry: _tools) tools.push_back(entry.second);
	}
	Poco::JSON::Array::Ptr pList = new Poco::JSON::Array;
	for (const Tool& tool: tools) pList->add(tool.toObject());

	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	pResult->set("tools", pList);
	return complete(id, pResult, true);
}


Poco::JSON::Object::Ptr Server::onToolsCall(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context)
{
	const Poco::Dynamic::Var name = params->get("name");
	if (!name.isString())
	{
		return invalidParams(id, "The tool name must be a string");
	}
	if (params->has("arguments") && !params->isObject("arguments"))
	{
		return invalidParams(id, "The tool arguments must be an object");
	}
	const std::string& toolName = name.extract<std::string>();

	ContextToolHandler handler;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		const auto it = _tools.find(toolName);
		if (it == _tools.end())
		{
			return invalidParams(id, "Unknown tool: " + toolName);
		}
		handler = it->second.handler;
	}

	const Poco::JSON::Object::Ptr pArguments = params->isObject("arguments") ? params->getObject("arguments") : Poco::JSON::Object::Ptr(new Poco::JSON::Object);
	ToolResult result;
	try
	{
		result = handler(pArguments, context);
	}
	catch (const Poco::Exception& exc)
	{
		result = ToolResult::error(exc.displayText());
	}
	catch (const std::exception& exc)
	{
		result = ToolResult::error(exc.what());
	}
	return complete(id, result.toObject(), false);
}


Poco::JSON::Object::Ptr Server::onResourcesList(const Id& id, const Poco::JSON::Object::Ptr& params)
{
	if (hasCursor(params)) return invalidCursor(id);
	std::vector<Resource> resources;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		resources.reserve(_resources.size());
		for (const auto& entry: _resources) resources.push_back(entry.second.resource);
	}
	Poco::JSON::Array::Ptr pList = new Poco::JSON::Array;
	for (const Resource& resource: resources) pList->add(resource.toObject());

	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	pResult->set("resources", pList);
	return complete(id, pResult, true);
}


Poco::JSON::Object::Ptr Server::onResourceTemplatesList(const Id& id, const Poco::JSON::Object::Ptr& params)
{
	if (hasCursor(params)) return invalidCursor(id);
	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	pResult->set("resourceTemplates", Poco::JSON::Array::Ptr(new Poco::JSON::Array));
	return complete(id, pResult, true);
}


Poco::JSON::Object::Ptr Server::onResourcesRead(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context)
{
	const Poco::Dynamic::Var uri = params->get("uri");
	if (!uri.isString())
	{
		return invalidParams(id, "The resource URI must be a string");
	}
	const std::string& resourceURI = uri.extract<std::string>();

	ResourceReader reader;
	std::string mimeType;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		const auto it = _resources.find(resourceURI);
		if (it == _resources.end())
		{
			Poco::JSON::Object::Ptr pData = new Poco::JSON::Object;
			pData->set("uri", resourceURI);
			return invalidParams(id, "Resource not found: " + resourceURI, pData);
		}
		reader = it->second.reader;
		mimeType = it->second.resource.mimeType;
	}

	ResourceContents contents = reader(resourceURI, context);
	if (contents.uri.empty()) contents.uri = resourceURI;
	if (contents.mimeType.empty()) contents.mimeType = mimeType;

	Poco::JSON::Array::Ptr pContents = new Poco::JSON::Array;
	pContents->add(contents.toObject());
	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	pResult->set("contents", pContents);
	return complete(id, pResult, true);
}


Poco::JSON::Object::Ptr Server::onPromptsList(const Id& id, const Poco::JSON::Object::Ptr& params)
{
	if (hasCursor(params)) return invalidCursor(id);
	std::vector<Prompt> prompts;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		prompts.reserve(_prompts.size());
		for (const auto& entry: _prompts) prompts.push_back(entry.second.prompt);
	}
	Poco::JSON::Array::Ptr pList = new Poco::JSON::Array;
	for (const Prompt& prompt: prompts) pList->add(prompt.toObject());

	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	pResult->set("prompts", pList);
	return complete(id, pResult, true);
}


Poco::JSON::Object::Ptr Server::onPromptsGet(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context)
{
	const Poco::Dynamic::Var name = params->get("name");
	if (!name.isString())
	{
		return invalidParams(id, "The prompt name must be a string");
	}
	if (params->has("arguments") && !params->isObject("arguments"))
	{
		return invalidParams(id, "The prompt arguments must be an object");
	}
	const std::string& promptName = name.extract<std::string>();

	Prompt prompt;
	PromptRenderer renderer;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		const auto it = _prompts.find(promptName);
		if (it == _prompts.end())
		{
			return invalidParams(id, "Unknown prompt: " + promptName);
		}
		prompt = it->second.prompt;
		renderer = it->second.renderer;
	}

	const Poco::JSON::Object::Ptr pArguments = params->isObject("arguments") ? params->getObject("arguments") : Poco::JSON::Object::Ptr(new Poco::JSON::Object);
	for (const std::string& argumentName: pArguments->getNames())
	{
		if (!pArguments->get(argumentName).isString())
		{
			return invalidParams(id, "The prompt argument " + argumentName + " must be a string");
		}
	}
	for (const PromptArgument& argument: prompt.arguments)
	{
		if (argument.required && !pArguments->has(argument.name))
		{
			return invalidParams(id, "Missing required argument: " + argument.name);
		}
	}

	const Poco::JSON::Array::Ptr pMessages = renderer(pArguments, context);
	Poco::JSON::Object::Ptr pResult = new Poco::JSON::Object;
	if (!prompt.description.empty()) pResult->set("description", prompt.description);
	pResult->set("messages", pMessages != nullptr ? pMessages : Poco::JSON::Array::Ptr(new Poco::JSON::Array));
	return complete(id, pResult, false);
}


void Server::serve(std::istream& in, std::ostream& out, std::size_t maxMessageSize)
{
	for (;;)
	{
		Poco::JSON::Object::Ptr pReply;
		try
		{
			const Poco::JSON::Object::Ptr pMessage = readMessage(in, maxMessageSize);
			if (pMessage == nullptr) return;
			pReply = handle(pMessage);
		}
		catch (const MCPException& exc)
		{
			pReply = Response::error(Id(), static_cast<ErrorCode>(exc.code()), exc.message());
		}
		if (pReply != nullptr)
		{
			writeMessage(out, pReply);
		}
	}
}


} // namespace Poco::AI::MCP
