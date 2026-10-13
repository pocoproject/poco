//
// Client.cpp
//
// Library: AIMCP
// Package: Core
// Module:  Client
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Client.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <istream>
#include <ostream>


namespace Poco {
namespace AI {
namespace MCP {


Client::Client(std::istream& in, std::ostream& out):
	_in(in),
	_out(out),
	_nextId(1)
{
}


Client::~Client()
{
}


Poco::JSON::Object::Ptr Client::call(const std::string& method, Poco::JSON::Object::Ptr params)
{
	Poco::JSON::Object::Ptr request = Request::make(Id(_nextId++), method, params);
	writeMessage(_out, request);

	Poco::JSON::Object::Ptr response = readMessage(_in);
	if (!response)
	{
		throw Poco::IOException("MCP connection closed while awaiting a response to " + method);
	}
	if (response->has("error"))
	{
		Poco::JSON::Object::Ptr error = response->getObject("error");
		const int code = error->optValue<int>("code", 0);
		const std::string text = error->optValue<std::string>("message", "");
		throw Poco::RuntimeException(Poco::format("MCP error %d: %s", code, text));
	}
	return response->getObject("result");
}


void Client::notify(const std::string& method, Poco::JSON::Object::Ptr params)
{
	writeMessage(_out, Request::makeNotification(method, params));
}


Poco::JSON::Object::Ptr Client::initialize(const std::string& clientName, const std::string& clientVersion)
{
	Poco::JSON::Object::Ptr clientInfo = new Poco::JSON::Object;
	clientInfo->set("name", clientName);
	clientInfo->set("version", clientVersion);

	Poco::JSON::Object::Ptr params = new Poco::JSON::Object;
	params->set("protocolVersion", PROTOCOL_VERSION);
	params->set("capabilities", Poco::JSON::Object::Ptr(new Poco::JSON::Object));
	params->set("clientInfo", clientInfo);

	Poco::JSON::Object::Ptr result = call("initialize", params);
	notify("notifications/initialized", nullptr);
	return result;
}


void Client::ping()
{
	call("ping", nullptr);
}


Poco::JSON::Array::Ptr Client::listTools()
{
	Poco::JSON::Object::Ptr result = call("tools/list", nullptr);
	return result->getArray("tools");
}


ToolResult Client::callTool(const std::string& name, Poco::JSON::Object::Ptr arguments)
{
	Poco::JSON::Object::Ptr params = new Poco::JSON::Object;
	params->set("name", name);
	params->set("arguments", arguments ? arguments : Poco::JSON::Object::Ptr(new Poco::JSON::Object));

	Poco::JSON::Object::Ptr result = call("tools/call", params);
	return ToolResult::fromObject(result);
}


} } } // namespace Poco::AI::MCP
