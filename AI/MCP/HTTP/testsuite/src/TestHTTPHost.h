//
// TestHTTPHost.h
//
// A minimal in-test Streamable HTTP host of MCP protocol revision 2026-07-28
// for a Poco::AI::MCP::Server, the server-side peer of the tests of the HTTP
// client (shared by the AIMCPHTTP and AI test suites). The transport of the
// library answers the requests; faults can be switched on to test the
// client's handling of them.
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
#include "Poco/AI/MCP/HTTP/ServerTransport.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/JSON/Parser.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Condition.h"
#include "Poco/Mutex.h"
#include <atomic>
#include <ostream>
#include <string>
#include <vector>


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
		FAULT_STALE_KEEP_ALIVE, /// Promises to keep the connection, and closes it
		FAULT_DROP_SECOND,      /// Reads the second request, and closes the connection without an answer
		FAULT_ACCEPTED,         /// Answers a request with 202 Accepted
		FAULT_OTHER_ID,         /// Answers with the reply to another id
		FAULT_ERROR_OTHER_ID,   /// Answers 400 with an error for another id
		FAULT_NOT_JSON,         /// Answers 200 with a body that is not JSON
		FAULT_STALL_BODY,       /// Sends the head and half of the body, then nothing
		FAULT_TRICKLE_BODY,     /// Sends the body one byte every 20 ms
		FAULT_TRICKLE_LINE,     /// Sends the event stream one byte every 20 ms
		FAULT_BOM,              /// Starts the event stream with a byte order mark
		FAULT_NO_BLANK_LINE,    /// Ends the event stream without the blank line after the reply
		FAULT_CUT_BODY          /// Sends half of the body, and closes the connection
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
		pParams->setAutoDecodeHeaders(false);
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

	void addListedTool(const std::string& entry)
		/// Lists the tools/list entry, JSON text, after the tools of the
		/// server, e.g. one the server would not take; call before start().
	{
		Poco::FastMutex::ScopedLock lock(_state.mutex);
		_state.listedTools.push_back(entry);
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
		Poco::AI::MCP::HTTP::ServerTransport transport;
		ReplyMode replyMode;
		Fault fault = FAULT_NONE;
		std::atomic<int> requests{0};
		mutable Poco::FastMutex mutex;
		Poco::Net::NameValueCollection lastHeaders;
		std::vector<std::string> listedTools;
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

			std::string body;
			Poco::StreamCopier::copyToString(request.stream(), body);

			if (_state.fault == FAULT_DROP_SECOND && _state.requests.load() == 2)
			{
				Poco::Net::StreamSocket socket = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).detachSocket();
				socket.close();
				return;
			}
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

			Poco::AI::MCP::HTTP::ServerTransport::Reply reply = _state.transport.answer(_state.server, request, body);
			if (!reply.message.isNull() && reply.message->isObject("result") && reply.message->getObject("result")->isArray("tools"))
			{
				// Each reply gets entries of its own.
				Poco::JSON::Array::Ptr pTools = reply.message->getObject("result")->getArray("tools");
				Poco::FastMutex::ScopedLock lock(_state.mutex);
				for (const std::string& entry: _state.listedTools)
				{
					Poco::JSON::Parser parser;
					pTools->add(parser.parse(entry));
				}
			}
			// The faults below change the reply to a request.
			if (reply.message.isNull() || !reply.message->has("id"))
			{
				_state.transport.send(reply, response);
				return;
			}
			switch (_state.fault)
			{
			case FAULT_ACCEPTED:
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_ACCEPTED);
				response.setContentLength(0);
				response.send();
				return;
			case FAULT_NOT_JSON:
				sendText(response, Poco::Net::HTTPResponse::HTTP_OK, "application/json", "Service temporarily moved: try again later");
				return;
			case FAULT_ERROR_OTHER_ID:
				sendJson(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST,
					Poco::AI::MCP::Response::error(Poco::AI::MCP::Id(std::string("someone-else")), Poco::AI::MCP::ErrorCode::INVALID_PARAMS, "Not yours"));
				return;
			case FAULT_OVERSIZED:
				if (reply.message->isObject("result"))
				{
					reply.message->getObject("result")->set("padding", std::string(65536, 'p'));
				}
				break;
			case FAULT_OTHER_ID:
				reply.message->set("id", std::string("someone-else"));
				break;
			case FAULT_CUT_STREAM:
				cutStream(response);
				return;
			default:
				break;
			}
			const std::string text = Poco::AI::MCP::serialize(reply.message);
			switch (_state.fault)
			{
			case FAULT_STALL_BODY:
				stallBody(response, text, _state.stopping);
				return;
			case FAULT_CUT_BODY:
				cutBody(request, response, text);
				return;
			case FAULT_TRICKLE_BODY:
				trickleBody(response, "application/json", text, _state.stopping);
				return;
			case FAULT_TRICKLE_LINE:
				trickleBody(response, "text/event-stream", "data: " + text + "\n\n", _state.stopping);
				return;
			case FAULT_BOM:
				sendText(response, Poco::Net::HTTPResponse::HTTP_OK, "text/event-stream", "\xEF\xBB\xBF" "data: " + text + "\n\n");
				return;
			case FAULT_NO_BLANK_LINE:
				sendText(response, Poco::Net::HTTPResponse::HTTP_OK, "text/event-stream", ": a notification first\n\ndata: " + text + "\n");
				return;
			default:
				break;
			}
			if (_state.replyMode == REPLY_EVENT_STREAM && reply.status == Poco::Net::HTTPResponse::HTTP_OK)
			{
				sendEventStream(response, reply.message);
				return;
			}
			_state.transport.send(reply, response);
		}

	private:
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

		static void stallBody(Poco::Net::HTTPServerResponse& response, const std::string& text, Poco::Event& stopping)
			/// Promises the whole body and sends half of it.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType("application/json");
			response.setContentLength(static_cast<std::streamsize>(text.size()));
			std::ostream& out = response.send();
			out << text.substr(0, text.size() / 2);
			out.flush();
			(void) stopping.tryWait(10000);
		}

		static void cutBody(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const std::string& text)
			/// Promises the whole body, sends half of it and closes the connection.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType("application/json");
			response.setContentLength(static_cast<std::streamsize>(text.size()));
			std::ostream& out = response.send();
			out << text.substr(0, text.size() / 2);
			out.flush();
			Poco::Net::StreamSocket socket = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).detachSocket();
			socket.close();
		}

		static void trickleBody(Poco::Net::HTTPServerResponse& response, const std::string& type, const std::string& text, Poco::Event& stopping)
			/// Sends the body one byte every 20 ms.
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			response.setContentType(type);
			response.setContentLength(static_cast<std::streamsize>(text.size()));
			std::ostream& out = response.send();
			for (const char c: text)
			{
				out.put(c);
				out.flush();
				if (!out.good() || stopping.tryWait(20)) break;
			}
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
