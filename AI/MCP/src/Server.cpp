//
// Server.cpp
//
// Library: AIMCP
// Package: Core
// Module:  Server
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include <istream>
#include <ostream>
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


Server::Server()
{
}


Server::~Server()
{
}


void Server::registerTool(const std::string& name, const std::string& description,
	Poco::JSON::Object::Ptr inputSchema, ToolHandler handler)
{
	Tool tool;
	tool.name = name;
	tool.description = description;
	tool.inputSchema = inputSchema;
	tool.handler = handler;
	_dispatcher.registerTool(tool);
}


void Server::registerTool(const Tool& tool)
{
	_dispatcher.registerTool(tool);
}


void Server::registerResource(const Resource& resource, ResourceReader reader)
{
	_dispatcher.registerResource(resource, reader);
}


void Server::registerPrompt(const Prompt& prompt, PromptRenderer renderer)
{
	_dispatcher.registerPrompt(prompt, renderer);
}


void Server::setServerInfo(const std::string& name, const std::string& version)
{
	_dispatcher.setServerInfo(name, version);
}


void Server::setInstructions(const std::string& instructions)
{
	_dispatcher.setInstructions(instructions);
}


Poco::JSON::Object::Ptr Server::handleMessage(const Poco::JSON::Object::Ptr& message)
{
	return _dispatcher.handle(message);
}


Poco::JSON::Object::Ptr Server::handleMessage(const Poco::JSON::Object::Ptr& message, const RequestContext& context)
{
	return _dispatcher.handle(message, context);
}


void Server::serve(std::istream& in, std::ostream& out)
{
	std::string line;
	while (std::getline(in, line))
	{
		if (line.find_first_not_of(" \t\r") == std::string::npos)
		{
			continue;
		}

		Poco::JSON::Object::Ptr response;
		try
		{
			Poco::JSON::Parser parser;
			Poco::Dynamic::Var parsed = parser.parse(line);
			if (parsed.type() == typeid(Poco::JSON::Object::Ptr))
			{
				response = handleMessage(parsed.extract<Poco::JSON::Object::Ptr>());
			}
			else
			{
				// A top-level array is a JSON-RPC batch, which the 2025-06-18 MCP
				// revision removed; anything else is not a valid message either.
				response = Response::error(Id(), ErrorCode::InvalidRequest, "Invalid Request");
			}
		}
		catch (Poco::Exception&)
		{
			response = Response::error(Id(), ErrorCode::ParseError, "Parse error");
		}

		if (response)
		{
			writeMessage(out, response);
		}
	}
}


} } } // namespace Poco::AI::MCP
