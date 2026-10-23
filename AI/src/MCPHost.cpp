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
#include <algorithm>
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
	bool isModelToolCharacter(char c)
		/// Returns true if both model APIs take c in a tool name: A-Z, a-z,
		/// 0-9, '_' and '-'.
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
	}


	bool isModelToolName(const std::string& name)
		/// Returns true if both model APIs take name as a tool name: 1 to 64
		/// of those characters.
	{
		return !name.empty() && name.size() <= 64 && std::all_of(name.begin(), name.end(), isModelToolCharacter);
	}


	std::string usable(const std::string& text)
		/// text with each character the model APIs refuse in a tool name
		/// replaced by '_'.
	{
		std::string result(text);
		std::replace_if(result.begin(), result.end(), [](char c) { return !isModelToolCharacter(c); }, '_');
		return result;
	}
}


MCPHost::MCPHost():
	_naming(Naming::REMOTE)
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
	const Poco::AI::MCP::ServerDescription description = pClient->discover();
	if (description.capabilities->has("tools"))
	{
		pConnection->tools = pClient->listTools();
	}
	pConnection->pClient = std::move(pClient);
	_connections.push_back(pConnection);
}


void MCPHost::setNaming(Naming naming)
{
	_naming = naming;
}


std::size_t MCPHost::registerInto(ToolRegistry& registry, std::vector<std::string>* pSkipped)
{
	std::size_t count = 0;
	for (const auto& pConnection: _connections)
	{
		for (const Poco::AI::MCP::Tool& remote: pConnection->tools)
		{
			const std::string name = _naming == Naming::PREFIXED ? usable(pConnection->label) + "_" + usable(remote.name) : remote.name;
			if (!isModelToolName(name))
			{
				if (pSkipped != nullptr) pSkipped->push_back(pConnection->label + ": " + name + " (a name the model APIs refuse)");
				continue;
			}
			Poco::AI::MCP::Tool tool(remote);
			tool.name = name;
			const std::shared_ptr<Connection> pBound = pConnection;
			const std::string remoteName = remote.name;
			tool.handler = [pBound, remoteName](const Poco::JSON::Object::Ptr& input, const Poco::AI::MCP::RequestContext& context)
			{
				if (context.isCancelled())
				{
					return Poco::AI::MCP::ToolResult::error("The tool was not called: the run was cancelled.");
				}
				return pBound->pClient->callTool(remoteName, input);
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
