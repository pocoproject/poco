//
// Client.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  Client
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Client.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include <algorithm>
#include <limits>


namespace Poco::AI::MCP {


namespace
{
	MCPException errorOf(const Poco::JSON::Object::Ptr& pResponse, const std::string& method)
		/// The exception for an error reply; an error member that is not a
		/// JSON-RPC error object, or whose code is not an int, breaks the
		/// protocol.
	{
		const Poco::JSON::Object::Ptr pError = pResponse->isObject("error") ? pResponse->getObject("error") : Poco::JSON::Object::Ptr();
		const std::string notAnError("The error reply to " + method + " is not a JSON-RPC error object");
		if (pError == nullptr || !pError->get("message").isString())
		{
			throw Poco::ProtocolException(notAnError);
		}
		const Poco::Dynamic::Var code = pError->get("code");
		if (!code.isInteger() || code.isBoolean())
		{
			throw Poco::ProtocolException(notAnError);
		}
		Poco::Int64 value = 0;
		try
		{
			value = code.convert<Poco::Int64>();
		}
		catch (const Poco::Exception&)
		{
			throw Poco::ProtocolException(notAnError, "its code is out of range");
		}
		if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max())
		{
			throw Poco::ProtocolException(notAnError, "its code is out of range");
		}
		std::string message = pError->getValue<std::string>("message");
		if (pError->has("data") && !pError->isNull("data"))
		{
			message += " " + pError->get("data").toString();
		}
		return MCPException(message, static_cast<int>(value));
	}


	bool asksForInput(const std::string& method)
		/// The requests a server may answer with input_required.
	{
		return method == "tools/call" || method == "resources/read" || method == "prompts/get";
	}
}


Client::Client():
	_clientInfo(Implementation::library()),
	_timeout(60, 0),
	_maxPages(DEFAULT_MAX_PAGES),
	_maxInputRounds(DEFAULT_MAX_INPUT_ROUNDS),
	_nextId(1)
{
}


Client::~Client()
{
}


void Client::setClientInfo(const Implementation& info)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_clientInfo = info;
}


Implementation Client::getClientInfo() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _clientInfo;
}


void Client::setTimeout(const Poco::Timespan& timeout)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_timeout = timeout;
}


Poco::Timespan Client::getTimeout() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _timeout;
}


void Client::setMaxPages(int maxPages)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_maxPages = maxPages;
}


int Client::getMaxPages() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _maxPages;
}


void Client::setMaxInputRounds(int maxRounds)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_maxInputRounds = maxRounds;
}


int Client::getMaxInputRounds() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _maxInputRounds;
}


bool Client::isReply(const Poco::JSON::Object::Ptr& message, const Id& id)
{
	if (message == nullptr || message->has("method")) return false;
	const Poco::Dynamic::Var messageId = message->get("id");
	if (messageId.isEmpty()) return message->has("error");
	return sameId(messageId, id);
}


void Client::toolsListed(std::vector<Tool>& /*tools*/)
{
}


Poco::JSON::Object::Ptr Client::call(const std::string& method, const Poco::JSON::Object::Ptr& params)
{
	Poco::JSON::Object::Ptr pParams = params != nullptr ? Poco::JSON::Object::Ptr(new Poco::JSON::Object(*params)) : Poco::JSON::Object::Ptr(new Poco::JSON::Object);
	Poco::JSON::Object::Ptr pMeta;
	if (pParams->has("_meta"))
	{
		if (!pParams->isObject("_meta"))
		{
			throw Poco::InvalidArgumentException("The _meta member of the params of " + method + " must be an object");
		}
		pMeta = new Poco::JSON::Object(*pParams->getObject("_meta"));
	}
	else pMeta = new Poco::JSON::Object;
	pMeta->set(META_PROTOCOL_VERSION, PROTOCOL_VERSION);
	pMeta->set(META_CLIENT_CAPABILITIES, Poco::JSON::Object::Ptr(new Poco::JSON::Object));
	const Implementation clientInfo = getClientInfo();
	if (!clientInfo.empty())
	{
		pMeta->set(META_CLIENT_INFO, clientInfo.toObject());
	}
	else pMeta->remove(META_CLIENT_INFO);
	pParams->set("_meta", pMeta);

	const int maxRounds = getMaxInputRounds();
	Poco::FastMutex::ScopedLock lock(_callMutex);
	for (int round = 1;; ++round)
	{
		const Id id(_nextId++);
		const Poco::JSON::Object::Ptr pResponse = exchange(Request::make(id, method, pParams));
		if (pResponse == nullptr)
		{
			throw Poco::ProtocolException("No reply to " + method);
		}
		if (pResponse->has("error"))
		{
			throw errorOf(pResponse, method);
		}
		if (!pResponse->has("result"))
		{
			throw Poco::ProtocolException("The reply to " + method + " has neither a result nor an error");
		}
		if (!pResponse->isObject("result"))
		{
			throw Poco::ProtocolException("The reply to " + method + " has no result object");
		}
		const Poco::JSON::Object::Ptr pResult = pResponse->getObject("result");
		const Poco::Dynamic::Var resultType = pResult->get("resultType");
		if (resultType.isEmpty() || (resultType.isString() && resultType.extract<std::string>() == "complete"))
		{
			return pResult;
		}
		if (!resultType.isString() || resultType.extract<std::string>() != "input_required" || !asksForInput(method))
		{
			throw Poco::ProtocolException("The reply to " + method + " has a result type this client does not handle", resultType.toString());
		}
		// The client declares no capabilities, so there is no input it may be
		// asked for; a result that asks for none is a request to send again.
		if (pResult->isObject("inputRequests") ? pResult->getObject("inputRequests")->size() > 0 : pResult->has("inputRequests"))
		{
			throw Poco::ProtocolException("The server asks for input this client does not declare it can give, to complete " + method);
		}
		if (round >= maxRounds)
		{
			throw Poco::ProtocolException("The server still asks to send " + method + " again after " + Poco::NumberFormatter::format(round) + " rounds");
		}
		const Poco::Dynamic::Var state = pResult->get("requestState");
		if (state.isString()) pParams->set("requestState", state);
		else if (state.isEmpty()) pParams->remove("requestState");
		else throw Poco::ProtocolException("The requestState of the reply to " + method + " is not a string");
	}
}


std::vector<Poco::JSON::Object::Ptr> Client::listAll(const std::string& method, const std::string& key)
{
	const int maxPages = getMaxPages();
	std::vector<Poco::JSON::Object::Ptr> items;
	Poco::Dynamic::Var cursor;
	for (int page = 0;; ++page)
	{
		if (page >= maxPages)
		{
			throw Poco::ProtocolException(method + " returns more pages than the limit of " + Poco::NumberFormatter::format(maxPages));
		}
		Poco::JSON::Object::Ptr pParams = new Poco::JSON::Object;
		// An empty string is a valid cursor too.
		if (cursor.isString()) pParams->set("cursor", cursor);
		const Poco::JSON::Object::Ptr pResult = call(method, pParams);
		if (!pResult->isArray(key))
		{
			throw Poco::ProtocolException("The result of " + method + " has no " + key + " array");
		}
		const Poco::JSON::Array::Ptr pItems = pResult->getArray(key);
		for (std::size_t i = 0; i < pItems->size(); ++i)
		{
			const Poco::JSON::Object::Ptr pItem = pItems->getObject(static_cast<unsigned>(i));
			if (pItem != nullptr) items.push_back(pItem);
		}
		cursor = pResult->get("nextCursor");
		if (!cursor.isString()) break;
	}
	return items;
}


ServerDescription Client::discover()
{
	const Poco::JSON::Object::Ptr pResult = call("server/discover");
	ServerDescription description;
	if (!pResult->isArray("supportedVersions"))
	{
		throw Poco::ProtocolException("The result of server/discover has no supportedVersions array");
	}
	const Poco::JSON::Array::Ptr pVersions = pResult->getArray("supportedVersions");
	description.supportedVersions.reserve(pVersions->size());
	for (const auto& version: *pVersions)
	{
		if (!version.isString())
		{
			throw Poco::ProtocolException("The supportedVersions of server/discover are not all strings");
		}
		description.supportedVersions.push_back(version.extract<std::string>());
	}
	description.capabilities = pResult->isObject("capabilities") ? pResult->getObject("capabilities") : Poco::JSON::Object::Ptr(new Poco::JSON::Object);
	description.instructions = pResult->optValue<std::string>("instructions", "");
	const Poco::JSON::Object::Ptr pMeta = pResult->isObject("_meta") ? pResult->getObject("_meta") : Poco::JSON::Object::Ptr();
	if (pMeta != nullptr && pMeta->isObject(META_SERVER_INFO))
	{
		description.serverInfo = Implementation::fromObject(pMeta->getObject(META_SERVER_INFO));
	}
	if (std::find(description.supportedVersions.begin(), description.supportedVersions.end(), PROTOCOL_VERSION) == description.supportedVersions.end())
	{
		std::string supported;
		for (const std::string& version: description.supportedVersions)
		{
			if (!supported.empty()) supported += ", ";
			supported += version;
		}
		throw MCPException(std::string("The server does not implement protocol revision ") + PROTOCOL_VERSION, "it implements " + (supported.empty() ? std::string("none") : supported), static_cast<int>(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION));
	}
	return description;
}


std::vector<Tool> Client::listTools()
{
	std::vector<Tool> tools;
	for (const Poco::JSON::Object::Ptr& pItem: listAll("tools/list", "tools"))
	{
		try
		{
			tools.push_back(Tool::fromObject(pItem));
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}
	toolsListed(tools);
	return tools;
}


ToolResult Client::callTool(const std::string& name, const Poco::JSON::Object::Ptr& arguments)
{
	Poco::JSON::Object::Ptr pParams = new Poco::JSON::Object;
	pParams->set("name", name);
	pParams->set("arguments", arguments != nullptr ? arguments : Poco::JSON::Object::Ptr(new Poco::JSON::Object));
	try
	{
		return ToolResult::fromObject(call("tools/call", pParams));
	}
	catch (const MCPException& exc)
	{
		if (exc.code() != static_cast<int>(ErrorCode::HEADER_MISMATCH)) throw;
	}
	// A transport that carries arguments in headers the tool's definition
	// names (Streamable HTTP) had a definition that is missing or out of date:
	// listed again, the tool is called once more.
	(void) listTools();
	return ToolResult::fromObject(call("tools/call", pParams));
}


std::vector<Resource> Client::listResources()
{
	std::vector<Resource> resources;
	for (const Poco::JSON::Object::Ptr& pItem: listAll("resources/list", "resources"))
	{
		try
		{
			resources.push_back(Resource::fromObject(pItem));
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}
	return resources;
}


std::vector<ResourceContents> Client::readResource(const std::string& uri)
{
	Poco::JSON::Object::Ptr pParams = new Poco::JSON::Object;
	pParams->set("uri", uri);
	const Poco::JSON::Object::Ptr pResult = call("resources/read", pParams);
	if (!pResult->isArray("contents"))
	{
		throw Poco::ProtocolException("The result of resources/read has no contents array");
	}
	std::vector<ResourceContents> contents;
	const Poco::JSON::Array::Ptr pContents = pResult->getArray("contents");
	contents.reserve(pContents->size());
	for (std::size_t i = 0; i < pContents->size(); ++i)
	{
		contents.push_back(ResourceContents::fromObject(pContents->getObject(static_cast<unsigned>(i))));
	}
	return contents;
}


std::vector<Prompt> Client::listPrompts()
{
	std::vector<Prompt> prompts;
	for (const Poco::JSON::Object::Ptr& pItem: listAll("prompts/list", "prompts"))
	{
		try
		{
			prompts.push_back(Prompt::fromObject(pItem));
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}
	return prompts;
}


Poco::JSON::Object::Ptr Client::getPrompt(const std::string& name, const Poco::JSON::Object::Ptr& arguments)
{
	Poco::JSON::Object::Ptr pParams = new Poco::JSON::Object;
	pParams->set("name", name);
	if (arguments != nullptr) pParams->set("arguments", arguments);
	const Poco::JSON::Object::Ptr pResult = call("prompts/get", pParams);
	if (!pResult->isArray("messages"))
	{
		throw Poco::ProtocolException("The result of prompts/get has no messages array");
	}
	return pResult;
}


} // namespace Poco::AI::MCP
