//
// TestHTTPHost.h
//
// A minimal in-test Streamable HTTP host for a Poco::AI::MCP::Server. The
// library ships only the HTTP client, so the tests use this fixture as the
// server-side peer to exercise it: POST carries a JSON-RPC message, an
// Mcp-Session-Id is minted on initialize and required thereafter together
// with the MCP-Protocol-Version header, and a notification is answered with
// an empty 202. A request is answered with one JSON object or, in event
// stream mode, with a stream that carries a notification ahead of the
// response.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AITest_TestHTTPHost_INCLUDED
#define AITest_TestHTTPHost_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/JSON/Parser.h"
#include "Poco/StreamCopier.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Mutex.h"
#include <ostream>
#include <set>
#include <string>


class TestHTTPHost
	/// Serves a Poco::AI::MCP::Server over HTTP on an ephemeral 127.0.0.1 port.
{
public:
	enum ReplyMode
	{
		REPLY_JSON,         /// One JSON object per request
		REPLY_EVENT_STREAM  /// An event stream per request
	};

	explicit TestHTTPHost(Poco::AI::MCP::Server& server, ReplyMode replyMode = REPLY_JSON):
		_server(server),
		_replyMode(replyMode)
	{
	}

	~TestHTTPHost()
	{
		stop();
	}

	void start()
	{
		if (_pHttpServer) return;
		Poco::Net::HTTPServerParams::Ptr params = new Poco::Net::HTTPServerParams;
		Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
		_port = socket.address().port();
		_pHttpServer = new Poco::Net::HTTPServer(new Factory(_server, _sessions, _replyMode), socket, params);
		_pHttpServer->start();
	}

	void stop()
	{
		if (_pHttpServer)
		{
			_pHttpServer->stopAll(true);
			_pHttpServer = nullptr;
		}
	}

	Poco::UInt16 port() const
	{
		return _port;
	}

private:
	class SessionStore
	{
	public:
		std::string create()
		{
			std::string id = Poco::UUIDGenerator::defaultGenerator().createRandom().toString();
			Poco::FastMutex::ScopedLock lock(_mutex);
			_ids.insert(id);
			return id;
		}

		bool has(const std::string& id) const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _ids.find(id) != _ids.end();
		}

	private:
		mutable Poco::FastMutex _mutex;
		std::set<std::string> _ids;
	};

	class Handler: public Poco::Net::HTTPRequestHandler
	{
	public:
		Handler(Poco::AI::MCP::Server& server, SessionStore& sessions, ReplyMode replyMode):
			_server(server),
			_sessions(sessions),
			_replyMode(replyMode)
		{
		}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
		{
			if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST)
			{
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
				response.setContentLength(0);
				response.send();
				return;
			}

			std::string body;
			Poco::StreamCopier::copyToString(request.stream(), body);

			Poco::JSON::Object::Ptr message;
			try
			{
				Poco::JSON::Parser parser;
				message = parser.parse(body).extract<Poco::JSON::Object::Ptr>();
			}
			catch (Poco::Exception&)
			{
				sendJson(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
					Poco::AI::MCP::Response::error(Poco::AI::MCP::Id(), Poco::AI::MCP::ErrorCode::ParseError, "Parse error"));
				return;
			}

			const bool isInitialize = message->optValue<std::string>("method", "") == "initialize";
			const std::string sessionId = request.get("Mcp-Session-Id", "");
			if (!isInitialize && (sessionId.empty() || !_sessions.has(sessionId)))
			{
				sendJson(response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND,
					Poco::AI::MCP::Response::error(Poco::AI::MCP::Id(), Poco::AI::MCP::ErrorCode::InvalidRequest, "Missing or unknown Mcp-Session-Id"));
				return;
			}
			// Every request after initialize names the negotiated protocol version.
			if (!isInitialize && request.get("MCP-Protocol-Version", "") != Poco::AI::MCP::PROTOCOL_VERSION)
			{
				sendJson(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
					Poco::AI::MCP::Response::error(Poco::AI::MCP::Id(), Poco::AI::MCP::ErrorCode::InvalidRequest, "Missing or unsupported MCP-Protocol-Version"));
				return;
			}

			Poco::JSON::Object::Ptr reply = _server.handleMessage(message);

			if (isInitialize)
			{
				response.set("Mcp-Session-Id", _sessions.create());
			}
			if (!reply)
			{
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_ACCEPTED);
				response.setContentLength(0);
				response.send();
				return;
			}
			if (_replyMode == REPLY_EVENT_STREAM)
			{
				sendEventStream(response, reply);
				return;
			}
			sendJson(response, Poco::Net::HTTPResponse::HTTP_OK, reply);
		}

	private:
		static void sendJson(Poco::Net::HTTPServerResponse& response, Poco::Net::HTTPResponse::HTTPStatus status, const Poco::JSON::Object::Ptr& body)
		{
			response.setStatusAndReason(status);
			response.setContentType("application/json");
			const std::string text = Poco::AI::MCP::serialize(body);
			response.setContentLength(static_cast<std::streamsize>(text.size()));
			response.send() << text;
		}

		static void sendEventStream(Poco::Net::HTTPServerResponse& response, const Poco::JSON::Object::Ptr& reply)
			/// Sends the reply the way a streaming server may: a comment, a
			/// notification, a response to some other request, and then the
			/// reply itself with its JSON spread over two data lines.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType("text/event-stream");
			response.setChunkedTransferEncoding(true);
			std::ostream& out = response.send();

			out << ": stream open\r\n\r\n";

			const std::string notification = Poco::AI::MCP::serialize(
				Poco::AI::MCP::Request::makeNotification("notifications/message", nullptr));
			out << "event: message\r\ndata: " << notification << "\r\n\r\n";

			const std::string other = Poco::AI::MCP::serialize(
				Poco::AI::MCP::Response::result(Poco::AI::MCP::Id(-1), Poco::JSON::Object::Ptr(new Poco::JSON::Object)));
			out << "event: message\ndata: " << other << "\n\n";

			// A serialized message has no newline of its own; the break is put
			// after the first comma, where JSON takes whitespace.
			const std::string text = Poco::AI::MCP::serialize(reply);
			const std::size_t comma = text.find(',');
			out << "event: message\nid: 1\n";
			if (comma == std::string::npos)
			{
				out << "data: " << text << "\n\n";
			}
			else
			{
				out << "data: " << text.substr(0, comma + 1) << "\n";
				out << "data:" << text.substr(comma + 1) << "\n\n";
			}
			out.flush();
		}

		Poco::AI::MCP::Server& _server;
		SessionStore& _sessions;
		ReplyMode _replyMode;
	};

	class Factory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		Factory(Poco::AI::MCP::Server& server, SessionStore& sessions, ReplyMode replyMode):
			_server(server),
			_sessions(sessions),
			_replyMode(replyMode)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
		{
			return new Handler(_server, _sessions, _replyMode);
		}

	private:
		Poco::AI::MCP::Server& _server;
		SessionStore& _sessions;
		ReplyMode _replyMode;
	};

	Poco::AI::MCP::Server& _server;
	ReplyMode _replyMode;
	SessionStore _sessions;
	Poco::UInt16 _port = 0;
	Poco::SharedPtr<Poco::Net::HTTPServer> _pHttpServer;
};


#endif // AITest_TestHTTPHost_INCLUDED
