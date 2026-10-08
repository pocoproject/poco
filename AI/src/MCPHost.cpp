//
// MCPHost.cpp
//
// Library: AI
// Package: Agent
// Module:  MCPHost
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCPHost.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Mutex.h"
#include "Poco/Exception.h"
#include <sstream>
#include <utility>


namespace Poco {
namespace AI {


struct MCPHost::Connection
	/// One live MCP server: the HTTP client, a mutex serializing calls to it
	/// (HTTPClient is not safe for concurrent use), and the cached tool list.
{
	std::string label;
	std::unique_ptr<Poco::AI::MCP::HTTP::HTTPClient> pClient;
	Poco::FastMutex mutex;
	Poco::JSON::Array::Ptr pTools;
};


namespace
{
	std::string serializeJSON(const Poco::Dynamic::Var& value)
	{
		std::ostringstream os;
		Poco::JSON::Stringifier::stringify(value, os);
		return os.str();
	}


	std::string flattenToolResult(const Poco::AI::MCP::ToolResult& result)
		/// Reduces an MCP tool result to the string the ToolRegistry expects:
		/// the joined text blocks, or {"error": ...} when isError, or the raw
		/// content array as JSON when it carries non-text blocks.
	{
		Poco::JSON::Array::Ptr pContent = result.content();
		std::string text;
		bool allText = true;
		if (pContent)
		{
			for (unsigned int i = 0; i < pContent->size(); ++i)
			{
				Poco::JSON::Object::Ptr pBlock = pContent->getObject(i);
				if (pBlock && pBlock->optValue<std::string>("type", "") == "text" && pBlock->has("text"))
				{
					if (!text.empty())
					{
						text += "\n";
					}
					text += pBlock->getValue<std::string>("text");
				}
				else
				{
					allText = false;
				}
			}
		}

		if (result.isError())
		{
			Poco::JSON::Object::Ptr pErr = new Poco::JSON::Object;
			pErr->set("error", text.empty() ? std::string("tool error") : text);
			return serializeJSON(pErr);
		}
		if (allText)
		{
			return text;
		}
		return pContent ? serializeJSON(pContent) : std::string();
	}
}


MCPHost::MCPHost()
{
}


MCPHost::~MCPHost()
{
}


void MCPHost::addServer(const std::string& label, const Poco::URI& endpoint,
	const std::string& clientName, const std::string& clientVersion,
	SessionFactory sessionFactory)
{
	auto pConnection = std::make_shared<Connection>();
	pConnection->label = label;
	pConnection->pClient = std::make_unique<Poco::AI::MCP::HTTP::HTTPClient>(endpoint, std::move(sessionFactory));
	pConnection->pClient->initialize(clientName, clientVersion);
	pConnection->pTools = pConnection->pClient->listTools();
	_connections.push_back(pConnection);
}


std::size_t MCPHost::registerInto(ToolRegistry& registry, std::vector<std::string>* pShadowed)
{
	std::size_t count = 0;
	for (auto& pConnection: _connections)
	{
		if (!pConnection->pTools)
		{
			continue;
		}
		for (unsigned int i = 0; i < pConnection->pTools->size(); ++i)
		{
			Poco::JSON::Object::Ptr pTool = pConnection->pTools->getObject(i);
			if (!pTool || !pTool->has("name"))
			{
				continue;
			}
			const std::string name = pTool->getValue<std::string>("name");

			if (registry.hasTool(name))
			{
				if (pShadowed) pShadowed->push_back(name);
				continue;
			}

			Poco::JSON::Object definition;
			definition.set("name", name);
			definition.set("description", pTool->optValue<std::string>("description", std::string()));
			if (pTool->isObject("inputSchema"))
			{
				definition.set("parameters", pTool->getObject("inputSchema"));
			}

			std::shared_ptr<Connection> pBound = pConnection;
			registry.registerTool(definition, [pBound, name](const Poco::JSON::Object& input) -> std::string
			{
				try
				{
					Poco::FastMutex::ScopedLock lock(pBound->mutex);
					Poco::JSON::Object::Ptr pArgs = new Poco::JSON::Object(input);
					return flattenToolResult(pBound->pClient->callTool(name, pArgs));
				}
				catch (Poco::Exception& exc)
				{
					Poco::JSON::Object::Ptr pErr = new Poco::JSON::Object;
					pErr->set("error", exc.displayText());
					return serializeJSON(pErr);
				}
			});

			_owners[name] = pConnection->label;
			++count;
		}
	}
	return count;
}


} } // namespace Poco::AI
