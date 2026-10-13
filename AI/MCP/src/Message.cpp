//
// Message.cpp
//
// Library: AIMCP
// Package: Core
// Module:  Message
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include <istream>
#include <ostream>
#include <sstream>


namespace Poco {
namespace AI {
namespace MCP {


Poco::JSON::Object::Ptr Request::make(const Id& id, const std::string& method, Poco::JSON::Object::Ptr params)
{
	Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
	o->set("jsonrpc", JSONRPC_VERSION);
	o->set("id", id);
	o->set("method", method);
	if (params)
	{
		o->set("params", params);
	}
	return o;
}


Poco::JSON::Object::Ptr Request::makeNotification(const std::string& method, Poco::JSON::Object::Ptr params)
{
	Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
	o->set("jsonrpc", JSONRPC_VERSION);
	o->set("method", method);
	if (params)
	{
		o->set("params", params);
	}
	return o;
}


Poco::JSON::Object::Ptr Response::result(const Id& id, const Poco::Dynamic::Var& result)
{
	Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
	o->set("jsonrpc", JSONRPC_VERSION);
	o->set("id", id);
	o->set("result", result);
	return o;
}


Poco::JSON::Object::Ptr Response::error(const Id& id, int code, const std::string& message, Poco::JSON::Object::Ptr data)
{
	Poco::JSON::Object::Ptr err = new Poco::JSON::Object;
	err->set("code", code);
	err->set("message", message);
	if (data)
	{
		err->set("data", data);
	}

	Poco::JSON::Object::Ptr o = new Poco::JSON::Object;
	o->set("jsonrpc", JSONRPC_VERSION);
	o->set("id", id);
	o->set("error", err);
	return o;
}


std::string serialize(const Poco::JSON::Object::Ptr& message)
{
	std::ostringstream os;
	// Default indent 0 produces a single compact line with no embedded newlines.
	Poco::JSON::Stringifier::stringify(message, os);
	return os.str();
}


void writeMessage(std::ostream& out, const Poco::JSON::Object::Ptr& message)
{
	Poco::JSON::Stringifier::stringify(message, out);
	out << '\n';
	out.flush();
}


Poco::JSON::Object::Ptr readMessage(std::istream& in)
{
	std::string line;
	while (std::getline(in, line))
	{
		if (line.find_first_not_of(" \t\r") == std::string::npos)
		{
			continue;
		}
		Poco::JSON::Parser parser;
		Poco::Dynamic::Var parsed = parser.parse(line);
		return parsed.extract<Poco::JSON::Object::Ptr>();
	}
	return Poco::JSON::Object::Ptr();
}


} } } // namespace Poco::AI::MCP
