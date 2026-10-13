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
#include "Poco/Net/MediaType.h"
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


namespace
{
	bool isResponseTo(const Poco::JSON::Object::Ptr& pMessage, Poco::Int64 id)
		/// Returns true if the message is the response to the request with
		/// the given id, which this client always makes a number.
	{
		if (!pMessage || !(pMessage->has("result") || pMessage->has("error"))) return false;
		const Poco::Dynamic::Var value = pMessage->get("id");
		if (value.isEmpty() || !value.isInteger()) return false;
		try
		{
			return value.convert<Poco::Int64>() == id;
		}
		catch (const Poco::Exception&)
		{
			return false;
		}
	}


	Poco::JSON::Object::Ptr parseEvent(const std::string& data)
		/// Parses the data of one event; an event that carries no JSON object
		/// is not a message.
	{
		try
		{
			Poco::JSON::Parser parser;
			const Poco::Dynamic::Var parsed = parser.parse(data);
			if (parsed.type() == typeid(Poco::JSON::Object::Ptr))
				return parsed.extract<Poco::JSON::Object::Ptr>();
		}
		catch (const Poco::Exception&)
		{
		}
		return Poco::JSON::Object::Ptr();
	}


	Poco::JSON::Object::Ptr readEventStream(std::istream& stream, Poco::Int64 id)
		/// Reads server-sent events until the response to the request with
		/// the given id arrives. Notifications and requests the server sends
		/// ahead of it are passed over. Returns null when the stream ends
		/// without the response.
	{
		std::string data;
		bool hasData = false;
		std::string line;
		for (;;)
		{
			const bool more = static_cast<bool>(std::getline(stream, line));
			if (more && !line.empty() && line.back() == '\r') line.pop_back();
			if (!more || line.empty())
			{
				// A blank line ends an event; so does the end of the stream.
				if (hasData)
				{
					Poco::JSON::Object::Ptr pMessage = parseEvent(data);
					if (isResponseTo(pMessage, id)) return pMessage;
				}
				data.clear();
				hasData = false;
				if (!more) return Poco::JSON::Object::Ptr();
				continue;
			}
			if (line.compare(0, 5, "data:") == 0)
			{
				// The value of a data field starts after one optional space;
				// the lines of one event are joined with a newline.
				const std::size_t begin = (line.size() > 5 && line[5] == ' ') ? 6 : 5;
				if (hasData) data += '\n';
				data.append(line, begin, std::string::npos);
				hasData = true;
			}
			// Comments and the event, id and retry fields carry no message.
		}
	}
}


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
	if (!_protocolVersion.empty())
	{
		request.set("MCP-Protocol-Version", _protocolVersion);
	}
	request.setContentLength(static_cast<std::streamsize>(body.size()));

	std::ostream& os = pSession->sendRequest(request);
	os << body;

	Poco::Net::HTTPResponse response;
	std::istream& rs = pSession->receiveResponse(response);

	if (response.has("Mcp-Session-Id"))
	{
		_sessionId = response.get("Mcp-Session-Id");
	}

	if (expectReply && response.has(Poco::Net::HTTPMessage::CONTENT_TYPE)
		&& Poco::Net::MediaType(response.getContentType()).matches("text", "event-stream"))
	{
		// The server answers with an event stream. The session ends with this
		// call, which closes the stream once the response has been read.
		Poco::JSON::Object::Ptr pReply = readEventStream(rs, message->getValue<Poco::Int64>("id"));
		if (!pReply)
		{
			throw Poco::IOException(Poco::format("MCP event stream ended without a response (HTTP %d)", static_cast<int>(response.getStatus())));
		}
		return pReply;
	}

	std::string responseBody;
	Poco::StreamCopier::copyToString(rs, responseBody);

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
	// Every request after this one names the version the server answered
	// with, in the MCP-Protocol-Version header.
	_protocolVersion = result ? result->optValue<std::string>("protocolVersion", PROTOCOL_VERSION) : std::string(PROTOCOL_VERSION);
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
