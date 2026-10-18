//
// TestHTTPHost.h
//
// A minimal in-test Streamable HTTP host of MCP protocol revision 2026-07-28
// for a Poco::AI::MCP::Server, the server-side peer of the tests of the HTTP
// client (shared by the AIMCPHTTP and AI test suites). It accepts POST only
// and checks the request headers against the body: MCP-Protocol-Version
// against the _meta object, Mcp-Method against the method and Mcp-Name
// against the name or URI, decoding the base64 form. It answers a missing
// _meta object or a header mismatch with 400, an unknown method with 404, a
// notification with 202, and a request with one JSON object or an event
// stream. Faults can be switched on to test the client's handling of them.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTPTest_TestHTTPHost_INCLUDED
#define AIMCPHTTPTest_TestHTTPHost_INCLUDED


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
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Condition.h"
#include "Poco/Mutex.h"
#include <atomic>
#include <ostream>
#include <sstream>
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

	enum Fault
	{
		FAULT_NONE,
		FAULT_CUT_STREAM,       /// An event stream that ends before the reply
		FAULT_HTML_502,         /// A 502 with an HTML page
		FAULT_OVERSIZED,        /// A reply with 64 KB of padding
		FAULT_SLOW,             /// The reply comes after 2 seconds
		FAULT_TRICKLE,          /// An event stream of comments, one every 50 ms, for 5 seconds
		FAULT_STALE_KEEP_ALIVE  /// Promises to keep the connection, and closes it
	};

	explicit TestHTTPHost(Poco::AI::MCP::Server& server, ReplyMode replyMode = REPLY_JSON):
		_state(server, replyMode)
	{
	}

	~TestHTTPHost()
	{
		stop();
	}

	void start()
	{
		if (!_pHttpServer.isNull()) return;
		Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams;
		if (_state.fault == FAULT_STALE_KEEP_ALIVE) pParams->setKeepAlive(false);
		Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
		_port = socket.address().port();
		_pHttpServer = new Poco::Net::HTTPServer(new Factory(_state), socket, pParams);
		_pHttpServer->start();
	}

	void stop()
		/// Stops the server and waits until no handler runs, so that none
		/// outlives the host and the server it serves.
	{
		if (!_pHttpServer.isNull())
		{
			_state.stopping.set();
			_pHttpServer->stopAll(true);
			Poco::Mutex::ScopedLock lock(_state.activeMutex);
			while (_state.active > 0)
			{
				if (!_state.idle.tryWait(_state.activeMutex, 10000)) break;
			}
			_pHttpServer = nullptr;
		}
	}

	void setFault(Fault fault)
		/// Sets the fault; call before start().
	{
		_state.fault = fault;
	}

	Poco::UInt16 port() const
	{
		return _port;
	}

	std::string url() const
	{
		return "http://127.0.0.1:" + std::to_string(_port) + "/mcp";
	}

	int requests() const
		/// The number of requests the host has read.
	{
		return _state.requests.load();
	}

	Poco::Net::NameValueCollection lastHeaders() const
		/// The headers of the last request.
	{
		Poco::FastMutex::ScopedLock lock(_state.mutex);
		return _state.lastHeaders;
	}

private:
	struct State
	{
		State(Poco::AI::MCP::Server& server, ReplyMode mode):
			server(server),
			replyMode(mode)
		{
		}

		Poco::AI::MCP::Server& server;
		ReplyMode replyMode;
		Fault fault = FAULT_NONE;
		std::atomic<int> requests{0};
		mutable Poco::FastMutex mutex;
		Poco::Net::NameValueCollection lastHeaders;
		Poco::Event stopping{Poco::Event::EVENT_MANUALRESET};
		Poco::Mutex activeMutex;
		Poco::Condition idle;
		int active = 0;
	};

	class ActiveGuard
		/// Counts a running handler.
	{
	public:
		explicit ActiveGuard(State& state):
			_state(state)
		{
			Poco::Mutex::ScopedLock lock(_state.activeMutex);
			++_state.active;
		}

		~ActiveGuard()
		{
			Poco::Mutex::ScopedLock lock(_state.activeMutex);
			--_state.active;
			_state.idle.broadcast();
		}

	private:
		State& _state;
	};

	class Handler: public Poco::Net::HTTPRequestHandler
	{
	public:
		explicit Handler(State& state):
			_state(state)
		{
		}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
		{
			ActiveGuard guard(_state);
			++_state.requests;
			{
				Poco::FastMutex::ScopedLock lock(_state.mutex);
				_state.lastHeaders.clear();
				for (const auto& header: request) _state.lastHeaders.add(header.first, header.second);
			}
			if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST)
			{
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
				response.setContentLength(0);
				response.send();
				return;
			}

			std::string body;
			Poco::StreamCopier::copyToString(request.stream(), body);

			switch (_state.fault)
			{
			case FAULT_HTML_502:
				sendText(response, Poco::Net::HTTPResponse::HTTP_BAD_GATEWAY, "text/html", "<html><body><h1>502 Bad Gateway</h1></body></html>");
				return;
			case FAULT_SLOW:
				if (_state.stopping.tryWait(2000)) return;
				break;
			case FAULT_TRICKLE:
				trickle(response, _state.stopping);
				return;
			case FAULT_STALE_KEEP_ALIVE:
				// The server parameters close every connection after its response.
				response.setKeepAlive(true);
				break;
			default:
				break;
			}

			Poco::JSON::Object::Ptr pMessage;
			try
			{
				Poco::JSON::Parser parser;
				pMessage = parser.parse(body).extract<Poco::JSON::Object::Ptr>();
			}
			catch (const Poco::Exception&)
			{
				sendJson(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
					Poco::AI::MCP::Response::error(Poco::AI::MCP::Id(), Poco::AI::MCP::ErrorCode::PARSE_ERROR, "Parse error"));
				return;
			}

			if (!pMessage->has("id"))
			{
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_ACCEPTED);
				response.setContentLength(0);
				response.send();
				return;
			}

			const Poco::AI::MCP::Id id = pMessage->get("id");
			const std::string mismatch = headerMismatch(request, pMessage);
			if (!mismatch.empty())
			{
				sendJson(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
					Poco::AI::MCP::Response::error(id, Poco::AI::MCP::ErrorCode::HEADER_MISMATCH, "Header mismatch: " + mismatch));
				return;
			}

			Poco::JSON::Object::Ptr pReply = _state.server.handle(pMessage);
			Poco::Net::HTTPResponse::HTTPStatus status = Poco::Net::HTTPResponse::HTTP_OK;
			if (pReply->isObject("error"))
			{
				const int code = pReply->getObject("error")->getValue<int>("code");
				if (code == static_cast<int>(Poco::AI::MCP::ErrorCode::METHOD_NOT_FOUND))
				{
					status = Poco::Net::HTTPResponse::HTTP_NOT_FOUND;
				}
				else if (code == static_cast<int>(Poco::AI::MCP::ErrorCode::UNSUPPORTED_PROTOCOL_VERSION) || !hasMeta(pMessage))
				{
					status = Poco::Net::HTTPResponse::HTTP_BAD_REQUEST;
				}
			}
			if (_state.fault == FAULT_OVERSIZED && pReply->isObject("result"))
			{
				pReply->getObject("result")->set("padding", std::string(65536, 'p'));
			}
			if (_state.fault == FAULT_CUT_STREAM)
			{
				cutStream(response);
				return;
			}
			if (_state.replyMode == REPLY_EVENT_STREAM && status == Poco::Net::HTTPResponse::HTTP_OK)
			{
				sendEventStream(response, pReply);
				return;
			}
			sendJson(response, status, pReply);
		}

	private:
		static bool hasMeta(const Poco::JSON::Object::Ptr& pMessage)
		{
			return pMessage->isObject("params") && pMessage->getObject("params")->isObject("_meta");
		}

		static std::string decoded(const std::string& received)
			/// Decodes the base64 form of a header value.
		{
			static const std::string PREFIX("=?base64?");
			static const std::string SUFFIX("?=");
			// Poco::Net::MessageHeader decodes RFC 2047 encoded words in every
			// header it reads and drops the "=?" of a value that only starts
			// like one, as the base64 form does.
			const std::string value = received.compare(0, PREFIX.size() - 2, PREFIX, 2, std::string::npos) == 0 ? "=?" + received : received;
			if (value.size() < PREFIX.size() + SUFFIX.size() || value.compare(0, PREFIX.size(), PREFIX) != 0
				|| value.compare(value.size() - SUFFIX.size(), SUFFIX.size(), SUFFIX) != 0)
			{
				return value;
			}
			std::istringstream encoded(value.substr(PREFIX.size(), value.size() - PREFIX.size() - SUFFIX.size()));
			Poco::Base64Decoder decoder(encoded);
			std::string text;
			Poco::StreamCopier::copyToString(decoder, text);
			return text;
		}

		static std::string headerMismatch(const Poco::Net::HTTPServerRequest& request, const Poco::JSON::Object::Ptr& pMessage)
			/// What does not match, or an empty string.
		{
			const std::string method = pMessage->optValue<std::string>("method", "");
			if (!request.has("MCP-Protocol-Version")) return "no MCP-Protocol-Version header";
			if (hasMeta(pMessage))
			{
				const std::string version = pMessage->getObject("params")->getObject("_meta")->optValue<std::string>(Poco::AI::MCP::META_PROTOCOL_VERSION, "");
				if (request.get("MCP-Protocol-Version") != version) return "MCP-Protocol-Version does not match the body";
			}
			if (request.get("Mcp-Method", "") != method) return "Mcp-Method does not match the body";
			if (method == "tools/call" || method == "prompts/get" || method == "resources/read")
			{
				if (!request.has("Mcp-Name")) return "no Mcp-Name header";
				const Poco::JSON::Object::Ptr pParams = pMessage->getObject("params");
				const std::string name = pParams->optValue<std::string>(method == "resources/read" ? "uri" : "name", "");
				if (decoded(request.get("Mcp-Name")) != name) return "Mcp-Name does not match the body";
			}
			return std::string();
		}

		static void sendText(Poco::Net::HTTPServerResponse& response, Poco::Net::HTTPResponse::HTTPStatus status, const std::string& type, const std::string& text)
		{
			response.setStatusAndReason(status);
			response.setContentType(type);
			response.setContentLength(static_cast<std::streamsize>(text.size()));
			response.send() << text;
		}

		static void sendJson(Poco::Net::HTTPServerResponse& response, Poco::Net::HTTPResponse::HTTPStatus status, const Poco::JSON::Object::Ptr& pBody)
		{
			sendText(response, status, "application/json", Poco::AI::MCP::serialize(pBody));
		}

		static void sendEventStream(Poco::Net::HTTPServerResponse& response, const Poco::JSON::Object::Ptr& pReply)
			/// Sends the reply the way a streaming server may: a comment, a
			/// notification, a response to some other request, an event of
			/// another type, and then the reply itself with its JSON spread
			/// over two data lines, the first ended by a CR alone.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType("text/event-stream");
			response.setChunkedTransferEncoding(true);
			std::ostream& out = response.send();

			out << ": stream open\r\n\r\n";
			out << "event: message\r\ndata: " << Poco::AI::MCP::serialize(Poco::AI::MCP::Request::makeNotification("notifications/progress")) << "\r\n\r\n";
			out << "data: " << Poco::AI::MCP::serialize(Poco::AI::MCP::Response::result(Poco::AI::MCP::Id(-1), Poco::JSON::Object::Ptr(new Poco::JSON::Object))) << "\n\n";
			// An event of another type is not a message, even with the right id.
			Poco::JSON::Object::Ptr pDecoy = new Poco::JSON::Object;
			pDecoy->set("decoy", true);
			out << "event: other\ndata: " << Poco::AI::MCP::serialize(Poco::AI::MCP::Response::result(pReply->get("id"), pDecoy)) << "\n\n";

			// A serialized message has no newline of its own; the break is put
			// after the first comma, where JSON takes whitespace.
			const std::string text = Poco::AI::MCP::serialize(pReply);
			const std::size_t comma = text.find(',');
			out << "event: message\nid: 1\n";
			if (comma == std::string::npos)
			{
				out << "data: " << text << "\n\n";
			}
			else
			{
				out << "data: " << text.substr(0, comma + 1) << "\r";
				out << "data:" << text.substr(comma + 1) << "\r\n\r\n";
			}
			out.flush();
		}

		static void cutStream(Poco::Net::HTTPServerResponse& response)
			/// An event stream that ends after a notification, without the reply.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType("text/event-stream");
			response.setChunkedTransferEncoding(true);
			std::ostream& out = response.send();
			out << "data: " << Poco::AI::MCP::serialize(Poco::AI::MCP::Request::makeNotification("notifications/progress")) << "\n\n";
			out.flush();
		}

		static void trickle(Poco::Net::HTTPServerResponse& response, Poco::Event& stopping)
			/// An event stream of keep-alive comments without any event.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType("text/event-stream");
			response.setChunkedTransferEncoding(true);
			std::ostream& out = response.send();
			for (int i = 0; i < 100 && out.good(); ++i)
			{
				out << ":\r\n";
				out.flush();
				if (stopping.tryWait(50)) break;
			}
		}

		State& _state;
	};

	class Factory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		explicit Factory(State& state):
			_state(state)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
		{
			return new Handler(_state);
		}

	private:
		State& _state;
	};

	State _state;
	Poco::UInt16 _port = 0;
	Poco::SharedPtr<Poco::Net::HTTPServer> _pHttpServer;
};


#endif // AIMCPHTTPTest_TestHTTPHost_INCLUDED
