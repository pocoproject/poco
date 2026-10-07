//
// HTTPClient.cpp
//
// Library: AIMCPHTTP
// Package: HTTP
// Module:  HTTPClient
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/JSON/Parser.h"
#include "Poco/StreamCopier.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include <memory>
#include <ostream>
#include <istream>
#include <utility>


namespace Poco {
namespace AI {
namespace MCP {
namespace HTTP {


HTTPClient::HTTPClient(const Poco::URI& endpoint, SessionFactory sessionFactory):
	_uri(endpoint),
	_sessionFactory(std::move(sessionFactory)),
	_nextId(1)
{
}


HTTPClient::~HTTPClient()
{
}


Poco::JSON::Object::Ptr HTTPClient::post(const Poco::JSON::Object::Ptr& message, bool expectReply)
{
	const std::string body = serialize(message);
	std::string path = _uri.getPathAndQuery();
	if (path.empty())
	{
		path = "/";
	}

	std::unique_ptr<Poco::Net::HTTPClientSession> pSession = _sessionFactory ? _sessionFactory(_uri) : createClientSession(_uri);

	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);
	// The Host header names the endpoint, whatever the session is connected to
	// (a factory may hand out a session through a proxy or a tunnel).
	request.setHost(_uri.getHost(), _uri.getPort());
	request.setContentType("application/json");
	request.set("Accept", "application/json, text/event-stream");
	if (!_sessionId.empty())
	{
		request.set("Mcp-Session-Id", _sessionId);
	}
	request.setContentLength(static_cast<std::streamsize>(body.size()));

	std::ostream& os = pSession->sendRequest(request);
	os << body;

	Poco::Net::HTTPResponse response;
	std::istream& rs = pSession->receiveResponse(response);
	std::string responseBody;
	Poco::StreamCopier::copyToString(rs, responseBody);

	if (response.has("Mcp-Session-Id"))
	{
		_sessionId = response.get("Mcp-Session-Id");
	}

	if (!expectReply)
	{
		return Poco::JSON::Object::Ptr();
	}

	if (responseBody.empty())
	{
		throw Poco::IOException(Poco::format("Empty MCP response (HTTP %d)", static_cast<int>(response.getStatus())));
	}

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parsed = parser.parse(responseBody);
	return parsed.extract<Poco::JSON::Object::Ptr>();
}


Poco::JSON::Object::Ptr HTTPClient::call(const std::string& method, Poco::JSON::Object::Ptr params)
{
	Poco::JSON::Object::Ptr request = Request::make(Id(_nextId++), method, params);
	Poco::JSON::Object::Ptr response = post(request, true);
	if (!response)
	{
		throw Poco::IOException("No MCP response to " + method);
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


void HTTPClient::notify(const std::string& method, Poco::JSON::Object::Ptr params)
{
	post(Request::makeNotification(method, params), false);
}


Poco::JSON::Object::Ptr HTTPClient::initialize(const std::string& clientName, const std::string& clientVersion)
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


void HTTPClient::ping()
{
	call("ping", nullptr);
}


Poco::JSON::Array::Ptr HTTPClient::listTools()
{
	Poco::JSON::Object::Ptr result = call("tools/list", nullptr);
	return result->getArray("tools");
}


ToolResult HTTPClient::callTool(const std::string& name, Poco::JSON::Object::Ptr arguments)
{
	Poco::JSON::Object::Ptr params = new Poco::JSON::Object;
	params->set("name", name);
	params->set("arguments", arguments ? arguments : Poco::JSON::Object::Ptr(new Poco::JSON::Object));

	Poco::JSON::Object::Ptr result = call("tools/call", params);
	return ToolResult::fromObject(result);
}


} } } } // namespace Poco::AI::MCP::HTTP
