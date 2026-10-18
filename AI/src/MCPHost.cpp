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
#include "Poco/Exception.h"
#include <utility>


namespace Poco::AI {


struct MCPHost::Connection
	/// One MCP server: its client and its tools. The client serializes its
	/// calls itself.
{
	std::string label;
	std::unique_ptr<Poco::AI::MCP::Client> pClient;
	std::vector<Poco::AI::MCP::Tool> tools;
};


namespace
{
	bool isModelToolName(const std::string& name)
		/// Returns true if both model APIs take name as a tool name: 1 to 64
		/// characters from A-Z, a-z, 0-9, '_' and '-'.
	{
		if (name.empty() || name.size() > 64) return false;
		for (const char c: name)
		{
			const bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
			if (!ok) return false;
		}
		return true;
	}
}


MCPHost::MCPHost()
{
}


MCPHost::~MCPHost()
{
}


void MCPHost::addServer(const std::string& label, std::unique_ptr<Poco::AI::MCP::Client> pClient)
{
	if (pClient == nullptr)
	{
		throw Poco::InvalidArgumentException("An MCP server needs a client", label);
	}
	for (const auto& pConnection: _connections)
	{
		if (pConnection->label == label)
		{
			throw Poco::InvalidArgumentException("An MCP server of that label is already added", label);
		}
	}
	auto pConnection = std::make_shared<Connection>();
	pConnection->label = label;
	(void) pClient->discover();
	pConnection->tools = pClient->listTools();
	pConnection->pClient = std::move(pClient);
	_connections.push_back(pConnection);
}


std::size_t MCPHost::registerInto(ToolRegistry& registry, std::vector<std::string>* pSkipped)
{
	std::size_t count = 0;
	for (const auto& pConnection: _connections)
	{
		for (const Poco::AI::MCP::Tool& remote: pConnection->tools)
		{
			if (!isModelToolName(remote.name))
			{
				if (pSkipped != nullptr) pSkipped->push_back(pConnection->label + ": " + remote.name + " (a name the model APIs refuse)");
				continue;
			}
			Poco::AI::MCP::Tool tool(remote);
			const std::shared_ptr<Connection> pBound = pConnection;
			const std::string name = remote.name;
			tool.handler = [pBound, name](const Poco::JSON::Object::Ptr& input, const Poco::AI::MCP::RequestContext&)
			{
				return pBound->pClient->callTool(name, input);
			};
			if (!registry.registerToolIfAbsent(tool))
			{
				if (pSkipped != nullptr)
				{
					const auto owner = _owners.find(name);
					pSkipped->push_back(pConnection->label + ": " + name + (owner != _owners.end() && owner->second != pConnection->label
						? " (the name is taken by " + owner->second + ")"
						: std::string(" (the name is taken)")));
				}
				continue;
			}
			_owners[name] = pConnection->label;
			++count;
		}
	}
	return count;
}


} // namespace Poco::AI
