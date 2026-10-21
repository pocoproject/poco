//
// ServerTransportTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ServerTransportTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/AI/MCP/HTTP/ServerTransport.h"
#include "Poco/AI/MCP/HTTP/ServerRequestHandler.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"
#include "Poco/Channel.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/Mutex.h"
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <vector>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Tool;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::RequestContext;
using Poco::AI::MCP::ErrorCode;
using Poco::AI::MCP::Id;
using Poco::AI::MCP::HTTP::ServerTransport;
using Poco::AI::MCP::HTTP::ServerRequestHandlerFactory;
using Poco::AI::MCP::HTTP::HTTPClient;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::NameValueCollection;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	class MessageCollector: public Poco::Channel
		/// Keeps the texts of the messages logged to it.
	{
	public:
		void log(const Poco::Message& msg) override
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_texts.push_back(msg.getText());
		}

		std::vector<std::string> texts() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _texts;
		}

	private:
		mutable Poco::FastMutex _mutex;
		std::vector<std::string> _texts;
	};


	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	ToolResult echo(const Object::Ptr& pArgs)
	{
		ToolResult result;
		result.addText(pArgs->optValue<std::string>("message", ""));
		return result;
	}


	void buildServer(Server& server)
	{
		Poco::AI::MCP::Implementation info;
		info.name = "transport-server";
		info.version = "1.0.0";
		server.setServerInfo(info);
		server.registerTool("echo", "Echo back the message.", nullptr, echo);
	}


	Object::Ptr meta(const std::string& revision = Poco::AI::MCP::PROTOCOL_VERSION)
	{
		Object::Ptr pMeta = new Object;
		pMeta->set(Poco::AI::MCP::META_PROTOCOL_VERSION, revision);
		pMeta->set(Poco::AI::MCP::META_CLIENT_CAPABILITIES, Object::Ptr(new Object));
		return pMeta;
	}


	std::string request(const Id& id, const std::string& method, const std::string& params = "{}")
		/// A request of this revision: the params with the _meta object.
	{
		Object::Ptr pParams = parse(params);
		pParams->set("_meta", meta());
		return Poco::AI::MCP::serialize(Poco::AI::MCP::Request::make(id, method, pParams));
	}


	NameValueCollection headersOf(const std::string& method, const std::string& name = std::string())
		/// The headers a client of this revision sends with a request.
	{
		NameValueCollection headers;
		headers.set("Content-Type", "application/json");
		headers.set("Accept", "application/json, text/event-stream");
		headers.set("MCP-Protocol-Version", Poco::AI::MCP::PROTOCOL_VERSION);
		headers.set("Mcp-Method", method);
		if (!name.empty()) headers.set("Mcp-Name", name);
		return headers;
	}


	struct Answer
		/// The answer to a request, as it came.
	{
		int status = 0;
		NameValueCollection headers;
		std::string body;

		Object::Ptr message() const
		{
			return parse(body);
		}

		int code() const
			/// The code of the JSON-RPC error in the body, or 0.
		{
			if (body.empty()) return 0;
			const Object::Ptr pMessage = message();
			return pMessage->isObject("error") ? pMessage->getObject("error")->getValue<int>("code") : 0;
		}
	};


	class FunctionHandler: public Poco::Net::HTTPRequestHandler
	{
	public:
		using Function = std::function<void (Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&)>;

		explicit FunctionHandler(const Function& function):
			_function(function)
		{
		}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
		{
			_function(request, response);
		}

	private:
		Function _function;
	};


	class FunctionFactory: public Poco::Net::HTTPRequestHandlerFactory
		/// Serves every request with a function: a host of its own.
	{
	public:
		explicit FunctionFactory(const FunctionHandler::Function& function):
			_function(function)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
		{
			return new FunctionHandler(_function);
		}

	private:
		FunctionHandler::Function _function;
	};


	class Endpoint
		/// An HTTP server on an ephemeral 127.0.0.1 port: by default the
		/// request handler factory of the library, serving /mcp.
	{
	public:
		explicit Endpoint(Server& server, const ServerTransport::Params& params = ServerTransport::Params(), bool autoDecodeHeaders = false)
		{
			start(new ServerRequestHandlerFactory(server, "/mcp", params), autoDecodeHeaders);
		}

		explicit Endpoint(const FunctionHandler::Function& function, bool autoDecodeHeaders = false)
		{
			start(new FunctionFactory(function), autoDecodeHeaders);
		}

		~Endpoint()
		{
			_pServer->stopAll(true);
		}

		std::string url() const
		{
			return "http://127.0.0.1:" + std::to_string(_port) + "/mcp";
		}

		Answer send(const std::string& method, const NameValueCollection& headers, const std::string& body, const std::string& path = "/mcp", bool chunked = false) const
		{
			Poco::Net::HTTPClientSession session("127.0.0.1", _port);
			session.setTimeout(Poco::Timespan(10, 0));
			HTTPRequest httpRequest(method, path, Poco::Net::HTTPMessage::HTTP_1_1);
			for (const auto& header: headers) httpRequest.set(header.first, header.second);
			if (chunked) httpRequest.setChunkedTransferEncoding(true);
			else httpRequest.setContentLength(static_cast<std::streamsize>(body.size()));
			std::ostream& out = session.sendRequest(httpRequest);
			out << body;
			out.flush();
			HTTPResponse response;
			std::istream& in = session.receiveResponse(response);
			Answer answer;
			answer.status = static_cast<int>(response.getStatus());
			for (const auto& header: response) answer.headers.add(header.first, header.second);
			Poco::StreamCopier::copyToString(in, answer.body);
			return answer;
		}

		Answer post(const std::string& body, const NameValueCollection& headers) const
		{
			return send(HTTPRequest::HTTP_POST, headers, body);
		}

	private:
		void start(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory, bool autoDecodeHeaders = false)
		{
			Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
			_port = socket.address().port();
			Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams;
			pParams->setAutoDecodeHeaders(autoDecodeHeaders);
			_pServer = std::make_unique<Poco::Net::HTTPServer>(pFactory, socket, pParams);
			_pServer->start();
		}

		Poco::UInt16 _port = 0;
		std::unique_ptr<Poco::Net::HTTPServer> _pServer;
	};


	std::string firstText(const Object::Ptr& pReply)
	{
		return pReply->getObject("result")->getArray("content")->getObject(0)->getValue<std::string>("text");
	}
}


ServerTransportTest::ServerTransportTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ServerTransportTest::~ServerTransportTest()
{
}


void ServerTransportTest::testMethods()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);

	// The transport is stateless: no stream to GET, no session to DELETE.
	for (const std::string& method: {HTTPRequest::HTTP_GET, HTTPRequest::HTTP_DELETE, HTTPRequest::HTTP_PUT, HTTPRequest::HTTP_OPTIONS})
	{
		const Answer answer = endpoint.send(method, NameValueCollection(), "");
		assertEqual(405, answer.status);
		assertEqual(std::string("POST"), answer.headers.get("Allow"));
		assertTrue(answer.body.empty());
	}
	assertEqual(200, endpoint.post(request(Id(1), "tools/list"), headersOf("tools/list")).status);
}


void ServerTransportTest::testOrigin()
{
	Server server;
	buildServer(server);
	ServerTransport::Params params;
	params.allowedOrigins.push_back("http://localhost:3000");
	Endpoint endpoint(server, params);
	const std::string body = request(Id(1), "tools/list");
	NameValueCollection headers = headersOf("tools/list");

	// A client that is no browser sends no Origin.
	assertEqual(200, endpoint.post(body, headers).status);
	headers.set("Origin", "http://localhost:3000");
	assertEqual(200, endpoint.post(body, headers).status);
	headers.set("Origin", "HTTP://LOCALHOST:3000");
	assertEqual(200, endpoint.post(body, headers).status);

	const std::string refused[] = {"http://evil.example", "http://localhost:3001", "null"};
	for (const std::string& origin: refused)
	{
		headers.set("Origin", origin);
		const Answer answer = endpoint.post(body, headers);
		assertEqual(403, answer.status);
		assertTrue(answer.body.empty());
		assertEqual(std::string("close"), answer.headers.get("Connection"));
	}

	// "*" allows every origin; no allowed origin refuses every browser.
	ServerTransport::Params any;
	any.allowedOrigins.push_back("*");
	Endpoint anyEndpoint(server, any);
	headers.set("Origin", "http://evil.example");
	assertEqual(200, anyEndpoint.post(body, headers).status);
	Endpoint noneEndpoint(server);
	headers.set("Origin", "http://localhost:3000");
	assertEqual(403, noneEndpoint.post(body, headers).status);

	assertTrue(ServerTransport(params).allowsOrigin("http://localhost:3000"));
	assertTrue(!ServerTransport().allowsOrigin("http://localhost:3000"));
}


void ServerTransportTest::testContentType()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	const std::string body = request(Id(1), "tools/list");
	NameValueCollection headers = headersOf("tools/list");

	headers.erase("Content-Type");
	assertEqual(415, endpoint.post(body, headers).status);
	const std::string refused[] = {"text/plain", "application/jsonp", "application/x-www-form-urlencoded"};
	for (const std::string& type: refused)
	{
		headers.set("Content-Type", type);
		assertEqual(415, endpoint.post(body, headers).status);
	}
	const std::string accepted[] = {"application/json; charset=utf-8", "Application/JSON"};
	for (const std::string& type: accepted)
	{
		headers.set("Content-Type", type);
		assertEqual(200, endpoint.post(body, headers).status);
	}
}


void ServerTransportTest::testAccept()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	const std::string body = request(Id(1), "tools/list");
	NameValueCollection headers = headersOf("tools/list");

	const std::string refused[] = {"text/html", "text/event-stream", "application/json;q=0", "application/json;q=0.0, text/event-stream"};
	for (const std::string& accept: refused)
	{
		headers.set("Accept", accept);
		assertEqual(406, endpoint.post(body, headers).status);
	}
	const std::string accepted[] = {"*/*", "application/*", "text/event-stream, application/json", "application/json;q=0.5", "text/html;q=0.9, */*;q=0.1"};
	for (const std::string& accept: accepted)
	{
		headers.set("Accept", accept);
		assertEqual(200, endpoint.post(body, headers).status);
	}
	headers.erase("Accept");
	assertEqual(200, endpoint.post(body, headers).status);
}


void ServerTransportTest::testBodyLimit()
{
	Server server;
	buildServer(server);
	ServerTransport::Params params;
	params.maxMessageSize = 300;
	Endpoint endpoint(server, params);
	const NameValueCollection headers = headersOf("tools/list");

	// A body of exactly the limit passes; one byte more does not, whether
	// its length is announced or it comes in chunks.
	const std::string base = request(Id(1), "tools/list", "{\"padding\":\"\"}");
	assertTrue(base.size() < params.maxMessageSize);
	const std::string padding(params.maxMessageSize - base.size(), 'x');
	const std::string atLimit = request(Id(1), "tools/list", "{\"padding\":\"" + padding + "\"}");
	assertEqual(params.maxMessageSize, atLimit.size());
	assertEqual(200, endpoint.post(atLimit, headers).status);
	assertEqual(200, endpoint.send(HTTPRequest::HTTP_POST, headers, atLimit, "/mcp", true).status);

	const std::string overLimit = request(Id(1), "tools/list", "{\"padding\":\"" + padding + "x\"}");
	Answer answer = endpoint.post(overLimit, headers);
	assertEqual(413, answer.status);
	assertTrue(answer.body.empty());
	assertEqual(std::string("close"), answer.headers.get("Connection"));
	answer = endpoint.send(HTTPRequest::HTTP_POST, headers, overLimit, "/mcp", true);
	assertEqual(413, answer.status);
	assertEqual(std::string("close"), answer.headers.get("Connection"));

	// By default a request may have 1 MiB, less than a message of the stdio
	// transport. A longer one is refused once its length is announced, before
	// its body, which this client therefore does not send.
	assertEqual(ServerTransport::DEFAULT_MAX_REQUEST_SIZE, ServerTransport::Params().maxMessageSize);
	assertTrue(ServerTransport::DEFAULT_MAX_REQUEST_SIZE < Poco::AI::MCP::DEFAULT_MAX_MESSAGE_SIZE);
	Endpoint defaults(server);
	Poco::Net::HTTPClientSession session("127.0.0.1", Poco::URI(defaults.url()).getPort());
	session.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest large(HTTPRequest::HTTP_POST, "/mcp", Poco::Net::HTTPMessage::HTTP_1_1);
	for (const auto& header: headers) large.set(header.first, header.second);
	large.setContentLength(static_cast<std::streamsize>(ServerTransport::DEFAULT_MAX_REQUEST_SIZE + 1));
	session.sendRequest(large);
	HTTPResponse response;
	session.receiveResponse(response);
	assertEqual(413, static_cast<int>(response.getStatus()));
}


void ServerTransportTest::testMalformedBodies()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	const NameValueCollection headers = headersOf("tools/list");

	const std::string notJSON[] = {"", "not json", "{\"jsonrpc\":"};
	for (const std::string& body: notJSON)
	{
		const Answer answer = endpoint.post(body, headers);
		assertEqual(400, answer.status);
		assertEqual(static_cast<int>(ErrorCode::PARSE_ERROR), answer.code());
		assertTrue(!answer.message()->has("id"));
	}

	// One message per request: no batch, no value that is no message.
	const std::string notMessage[] = {"[" + request(Id(1), "tools/list") + "]", "42", "\"text\"", "null"};
	for (const std::string& body: notMessage)
	{
		const Answer answer = endpoint.post(body, headers);
		assertEqual(400, answer.status);
		assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), answer.code());
	}

	// What is no request, the server refuses: an id that is null, no
	// jsonrpc member, a response.
	const std::string invalid[] = {
		"{\"jsonrpc\":\"2.0\",\"id\":null,\"method\":\"tools/list\"}",
		"{\"id\":1,\"method\":\"tools/list\"}",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{}}"
	};
	for (const std::string& body: invalid)
	{
		const Answer answer = endpoint.post(body, headers);
		assertEqual(400, answer.status);
		assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), answer.code());
	}
}


void ServerTransportTest::testNotifications()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	const std::string body = Poco::AI::MCP::serialize(Poco::AI::MCP::Request::makeNotification("notifications/cancelled", parse("{\"requestId\":1}")));

	NameValueCollection headers = headersOf("notifications/cancelled");
	Answer answer = endpoint.post(body, headers);
	assertEqual(202, answer.status);
	assertTrue(answer.body.empty());

	// The headers a notification carries match it too.
	headers.set("Mcp-Method", "tools/list");
	answer = endpoint.post(body, headers);
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());
	assertTrue(!answer.message()->has("id"));

	// A notification needs none of them.
	headers.erase("Mcp-Method");
	headers.erase("MCP-Protocol-Version");
	assertEqual(202, endpoint.post(body, headers).status);
}


void ServerTransportTest::testStandardHeaders()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	const std::string body = request(Id(7), "tools/list");

	Answer answer = endpoint.post(body, headersOf("tools/list"));
	assertEqual(200, answer.status);
	assertEqual(std::string("application/json"), answer.headers.get("Content-Type"));
	assertTrue(answer.message()->isObject("result"));

	const auto refused = [&endpoint, &body](NameValueCollection headers)
	{
		const Answer reply = endpoint.post(body, headers);
		return reply.status == 400 && reply.code() == static_cast<int>(ErrorCode::HEADER_MISMATCH) && reply.message()->getValue<int>("id") == 7;
	};
	NameValueCollection headers = headersOf("tools/list");
	headers.erase("MCP-Protocol-Version");
	assertTrue(refused(headers));
	headers = headersOf("tools/list");
	headers.set("MCP-Protocol-Version", "2025-06-18");
	assertTrue(refused(headers));
	headers = headersOf("tools/list");
	headers.erase("Mcp-Method");
	assertTrue(refused(headers));
	headers = headersOf("tools/list");
	headers.set("Mcp-Method", "tools/call");
	assertTrue(refused(headers));

	// A string id comes back with the refusal.
	answer = endpoint.post(request(Id(std::string("abc")), "tools/list"), headersOf("tools/call"));
	assertEqual(400, answer.status);
	assertEqual(std::string("abc"), answer.message()->getValue<std::string>("id"));
}


void ServerTransportTest::testNameHeader()
{
	Server server;
	buildServer(server);
	Poco::AI::MCP::Resource resource;
	resource.uri = "mem://notes/today";
	resource.name = "today";
	server.registerResource(resource, [](const std::string& uri, const RequestContext&)
		{
			Poco::AI::MCP::ResourceContents contents;
			contents.uri = uri;
			contents.text = "notes";
			return contents;
		});
	Poco::AI::MCP::Prompt prompt;
	prompt.name = "greet";
	server.registerPrompt(prompt, [](const Object::Ptr&, const RequestContext&)
		{
			return Array::Ptr(new Array);
		});
	Endpoint endpoint(server);

	const std::string call = request(Id(1), "tools/call", "{\"name\":\"echo\",\"arguments\":{\"message\":\"hi\"}}");
	Answer answer = endpoint.post(call, headersOf("tools/call", "echo"));
	assertEqual(200, answer.status);
	assertEqual(std::string("hi"), firstText(answer.message()));
	// The base64 form of the name, ZWNobw== for "echo".
	assertEqual(200, endpoint.post(call, headersOf("tools/call", "=?base64?ZWNobw==?=")).status);

	const std::string wrong[] = {"", "other", "=?base64?ZWNobw?=", "=?base64?ZW Nobw==?=", "=?base64?/w==?="};
	for (const std::string& name: wrong)
	{
		NameValueCollection headers = headersOf("tools/call", name);
		if (name.empty()) headers.erase("Mcp-Name");
		answer = endpoint.post(call, headers);
		assertEqual(400, answer.status);
		assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());
	}

	const std::string read = request(Id(2), "resources/read", "{\"uri\":\"mem://notes/today\"}");
	assertEqual(200, endpoint.post(read, headersOf("resources/read", "mem://notes/today")).status);
	assertEqual(400, endpoint.post(read, headersOf("resources/read", "today")).status);
	const std::string get = request(Id(3), "prompts/get", "{\"name\":\"greet\"}");
	assertEqual(200, endpoint.post(get, headersOf("prompts/get", "greet")).status);
	assertEqual(400, endpoint.post(get, headersOf("prompts/get", "echo")).status);

	// A name that is no string needs no header: the server answers.
	answer = endpoint.post(request(Id(4), "tools/call", "{\"name\":42}"), headersOf("tools/call"));
	assertEqual(200, answer.status);
	assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), answer.code());
}


void ServerTransportTest::testParamHeaders()
{
	Server server;
	buildServer(server);
	server.registerTool("route", "", parse(
		"{\"type\":\"object\",\"properties\":{"
		"\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"},"
		"\"count\":{\"type\":\"integer\",\"x-mcp-header\":\"Count\"},"
		"\"verbose\":{\"type\":\"boolean\",\"x-mcp-header\":\"Verbose\"},"
		"\"nested\":{\"type\":\"object\",\"properties\":{\"zone\":{\"type\":\"string\",\"x-mcp-header\":\"Zone\"}}}}}"), echo);
	server.registerTool("invalid", "", parse("{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"number\",\"x-mcp-header\":\"N\"}}}"), echo);
	Endpoint endpoint(server);

	const std::string call = request(Id(1), "tools/call", "{\"name\":\"route\",\"arguments\":{\"message\":\"ok\",\"region\":\"eu\",\"count\":42,\"verbose\":true,\"nested\":{\"zone\":\"b\"}}}");
	const auto withParams = []()
	{
		NameValueCollection headers = headersOf("tools/call", "route");
		headers.set("Mcp-Param-Region", "eu");
		headers.set("Mcp-Param-Count", "42");
		headers.set("Mcp-Param-Verbose", "true");
		headers.set("Mcp-Param-Zone", "b");
		return headers;
	};
	Answer answer = endpoint.post(call, withParams());
	assertEqual(200, answer.status);
	assertEqual(std::string("ok"), firstText(answer.message()));

	// An integer is compared as a number; the base64 form is decoded.
	const std::pair<std::string, std::string> accepted[] = {{"Mcp-Param-Count", "42.0"}, {"Mcp-Param-Count", "042"}, {"Mcp-Param-Region", "=?base64?ZXU=?="}};
	for (const auto& header: accepted)
	{
		NameValueCollection headers = withParams();
		headers.set(header.first, header.second);
		assertEqual(200, endpoint.post(call, headers).status);
	}
	const std::pair<std::string, std::string> refused[] = {
		{"Mcp-Param-Region", ""}, {"Mcp-Param-Zone", ""}, {"Mcp-Param-Region", "us"}, {"Mcp-Param-Count", "43"},
		{"Mcp-Param-Count", "4.2e1"}, {"Mcp-Param-Verbose", "TRUE"}, {"Mcp-Param-Region", "=?base64?ZXU?="}
	};
	for (const auto& header: refused)
	{
		NameValueCollection headers = withParams();
		if (header.second.empty()) headers.erase(header.first);
		else headers.set(header.first, header.second);
		answer = endpoint.post(call, headers);
		assertEqual(400, answer.status);
		assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());
	}

	// A parameter without a value, absent or null, needs no header, and a
	// header for it is ignored.
	const std::string sparse = request(Id(2), "tools/call", "{\"name\":\"route\",\"arguments\":{\"message\":\"ok\",\"count\":null}}");
	assertEqual(200, endpoint.post(sparse, headersOf("tools/call", "route")).status);
	NameValueCollection headers = headersOf("tools/call", "route");
	headers.set("Mcp-Param-Region", "anything");
	assertEqual(200, endpoint.post(sparse, headers).status);

	// A tool whose annotations are not valid has no header to check.
	assertEqual(200, endpoint.post(request(Id(3), "tools/call", "{\"name\":\"invalid\",\"arguments\":{\"n\":1.5}}"), headersOf("tools/call", "invalid")).status);
}


void ServerTransportTest::testEncodedWords()
{
	Server server;
	buildServer(server);
	server.registerTool("route", "", parse("{\"type\":\"object\",\"properties\":{\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"}}}"), echo);
	Poco::AI::MCP::Resource resource;
	resource.uri = "db://t?filter=?x";
	resource.name = "filtered";
	server.registerResource(resource, [](const std::string& uri, const RequestContext&)
		{
			Poco::AI::MCP::ResourceContents contents;
			contents.uri = uri;
			contents.text = "rows";
			return contents;
		});
	Endpoint endpoint(server);

	// An RFC 2047 encoded word, or "=?" anywhere, is checked as it was sent:
	// =?UTF-8?B?ZWNobw==?= does not name "echo".
	const std::string call = request(Id(1), "tools/call", "{\"name\":\"echo\",\"arguments\":{\"message\":\"hi\"}}");
	for (const std::string& name: {std::string("=?UTF-8?B?ZWNobw==?="), std::string("ec=?ho")})
	{
		const Answer answer = endpoint.post(call, headersOf("tools/call", name));
		assertEqual(400, answer.status);
		assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());
	}
	NameValueCollection headers = headersOf("tools/call", "echo");
	headers.set("MCP-Protocol-Version", "2026-=?07-28");
	Answer answer = endpoint.post(call, headers);
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());

	const std::string route = request(Id(2), "tools/call", "{\"name\":\"route\",\"arguments\":{\"message\":\"ok\",\"region\":\"eu\"}}");
	headers = headersOf("tools/call", "route");
	headers.set("Mcp-Param-Region", "=?UTF-8?B?ZXU=?=");
	answer = endpoint.post(route, headers);
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());

	// A value with "=?" in it matches as it is, and in the base64 form.
	const std::string read = request(Id(3), "resources/read", "{\"uri\":\"db://t?filter=?x\"}");
	assertEqual(200, endpoint.post(read, headersOf("resources/read", "db://t?filter=?x")).status);
	assertEqual(200, endpoint.post(read, headersOf("resources/read", "=?base64?ZGI6Ly90P2ZpbHRlcj0/eA==?=")).status);

	// Without its "=?", a value that looks like the base64 form is plain text.
	const std::string lookalike = request(Id(4), "tools/call", "{\"name\":\"route\",\"arguments\":{\"message\":\"ok\",\"region\":\"base64?aGVsbG8=?=\"}}");
	headers = headersOf("tools/call", "route");
	headers.set("Mcp-Param-Region", "base64?aGVsbG8=?=");
	assertEqual(200, endpoint.post(lookalike, headers).status);
}


void ServerTransportTest::testDecodedHeadersRefused()
{
	Server server;
	buildServer(server);
	ServerTransport::Params params;
	params.loggerName = "ServerTransportTest.DecodedHeaders";
	Poco::Logger& logger = Poco::Logger::get(params.loggerName);
	const Poco::Channel::Ptr pChannel = logger.getChannel();
	Poco::AutoPtr<MessageCollector> pCollector = new MessageCollector;
	logger.setChannel(pCollector);
	logger.setLevel(Poco::Message::PRIO_ERROR);

	// A server that decodes encoded words in its request headers gets 500
	// for every request, and the log says how to turn the decoding off.
	{
		Endpoint endpoint(server, params, true);
		const Answer answer = endpoint.post(request(Id(1), "tools/list"), headersOf("tools/list"));
		assertEqual(500, answer.status);
		assertEqual(static_cast<int>(ErrorCode::INTERNAL_ERROR), answer.code());
		assertEqual(std::string("close"), answer.headers.get("Connection", ""));
	}

	// Also when the host has read the body itself.
	const ServerTransport transport(params);
	{
		Endpoint endpoint([&server, &transport](Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
			{
				std::string body;
				Poco::StreamCopier::copyToString(request.stream(), body);
				transport.send(transport.answer(server, request, body), response);
			}, true);
		const Answer answer = endpoint.post(request(Id(2), "tools/list"), headersOf("tools/list"));
		assertEqual(500, answer.status);
		assertEqual(static_cast<int>(ErrorCode::INTERNAL_ERROR), answer.code());
	}

	const std::vector<std::string> texts = pCollector->texts();
	logger.setChannel(pChannel);
	assertEqual(std::size_t(2), texts.size());
	assertTrue(texts[0].find("setAutoDecodeHeaders(false)") != std::string::npos);
}


void ServerTransportTest::testOtherRevisions()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	NameValueCollection plain;
	plain.set("Content-Type", "application/json");

	// A request without the _meta object gets the answer of the server,
	// with or without the headers.
	const std::string bare = Poco::AI::MCP::serialize(Poco::AI::MCP::Request::make(Id(1), "tools/list"));
	Answer answer = endpoint.post(bare, plain);
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), answer.code());
	answer = endpoint.post(bare, headersOf("tools/list"));
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), answer.code());

	// A request of another revision, whose header agrees, learns which one
	// the server implements; a header that disagrees is a mismatch.
	Object::Ptr pParams = new Object;
	pParams->set("_meta", meta("2025-06-18"));
	const std::string older = Poco::AI::MCP::serialize(Poco::AI::MCP::Request::make(Id(2), "tools/list", pParams));
	NameValueCollection headers = headersOf("tools/list");
	headers.set("MCP-Protocol-Version", "2025-06-18");
	answer = endpoint.post(older, headers);
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), answer.code());
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), answer.message()->getObject("error")->getObject("data")->getArray("supported")->getElement<std::string>(0));
	answer = endpoint.post(older, headersOf("tools/list"));
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), answer.code());

	// The initialize request of an earlier revision as well.
	const std::string initialize = Poco::AI::MCP::serialize(Poco::AI::MCP::Request::make(Id(3), "initialize",
		parse("{\"protocolVersion\":\"2025-06-18\",\"capabilities\":{},\"clientInfo\":{\"name\":\"old\",\"version\":\"1\"}}")));
	plain.set("Accept", "application/json, text/event-stream");
	answer = endpoint.post(initialize, plain);
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), answer.code());
	assertEqual(3, answer.message()->getValue<int>("id"));
}


void ServerTransportTest::testReplyStatus()
{
	Server server;
	buildServer(server);
	server.registerTool("broken", "", nullptr, [](const Object::Ptr&) -> ToolResult
		{
			throw Poco::IllegalStateException("out of order");
		});
	Endpoint endpoint(server);

	// A method the server does not know: 404.
	Answer answer = endpoint.post(request(Id(1), "subscriptions/listen"), headersOf("subscriptions/listen"));
	assertEqual(404, answer.status);
	assertEqual(static_cast<int>(ErrorCode::METHOD_NOT_FOUND), answer.code());

	// The errors of a method are its answer: 200.
	answer = endpoint.post(request(Id(2), "tools/call", "{\"name\":\"missing\"}"), headersOf("tools/call", "missing"));
	assertEqual(200, answer.status);
	assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), answer.code());
	answer = endpoint.post(request(Id(3), "tools/list", "{\"cursor\":\"next\"}"), headersOf("tools/list"));
	assertEqual(200, answer.status);
	assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), answer.code());
	answer = endpoint.post(request(Id(4), "tools/call", "{\"name\":\"broken\"}"), headersOf("tools/call", "broken"));
	assertEqual(200, answer.status);
	assertTrue(answer.message()->getObject("result")->getValue<bool>("isError"));

	// An incomplete _meta object refuses the request: 400.
	Object::Ptr pMeta = new Object;
	pMeta->set(Poco::AI::MCP::META_PROTOCOL_VERSION, Poco::AI::MCP::PROTOCOL_VERSION);
	Object::Ptr pParams = new Object;
	pParams->set("_meta", pMeta);
	answer = endpoint.post(Poco::AI::MCP::serialize(Poco::AI::MCP::Request::make(Id(5), "tools/list", pParams)), headersOf("tools/list"));
	assertEqual(400, answer.status);
	assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), answer.code());
}


void ServerTransportTest::testContext()
{
	Server server;
	buildServer(server);
	Tool whoami;
	whoami.name = "whoami";
	whoami.handler = [](const Object::Ptr&, const RequestContext& context)
	{
		ToolResult result;
		result.addText(context.identity.principal + "/" + context.clientInfo.name);
		return result;
	};
	server.registerTool(whoami);
	const ServerTransport transport;
	// A host that signs its callers in passes their identity.
	Endpoint endpoint([&server, &transport](Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
		{
			RequestContext context;
			context.identity.principal = request.get("X-User", "");
			context.identity.authenticated = true;
			transport.handle(server, request, response, context);
		});

	Object::Ptr pParams = parse("{\"name\":\"whoami\"}");
	Object::Ptr pMeta = meta();
	pMeta->set(Poco::AI::MCP::META_CLIENT_INFO, parse("{\"name\":\"tester\",\"version\":\"1\"}"));
	pParams->set("_meta", pMeta);
	NameValueCollection headers = headersOf("tools/call", "whoami");
	headers.set("X-User", "alice");
	const Answer answer = endpoint.post(Poco::AI::MCP::serialize(Poco::AI::MCP::Request::make(Id(1), "tools/call", pParams)), headers);
	assertEqual(200, answer.status);
	assertEqual(std::string("alice/tester"), firstText(answer.message()));
}


void ServerTransportTest::testBodyReadByHost()
{
	Server server;
	buildServer(server);
	ServerTransport::Params params;
	params.maxMessageSize = 200;
	const ServerTransport transport(params);
	Endpoint endpoint([&server, &transport](Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
		{
			std::string body;
			Poco::StreamCopier::copyToString(request.stream(), body);
			transport.send(transport.answer(server, request, body), response);
		});

	assertEqual(200, endpoint.post(request(Id(1), "tools/list"), headersOf("tools/list")).status);
	// The host has read the body: the connection can go on.
	const Answer answer = endpoint.post(request(Id(1), "tools/list", "{\"padding\":\"" + std::string(200, 'x') + "\"}"), headersOf("tools/list"));
	assertEqual(413, answer.status);
	assertTrue(!answer.headers.has("Connection") || answer.headers.get("Connection") != "close");
}


void ServerTransportTest::testUnavailable()
{
	const ServerTransport::Reply reply = ServerTransport::unavailable();
	assertEqual(static_cast<int>(HTTPResponse::HTTP_SERVICE_UNAVAILABLE), static_cast<int>(reply.status));
	assertEqual(static_cast<int>(ErrorCode::INTERNAL_ERROR), reply.message->getObject("error")->getValue<int>("code"));
	assertTrue(!reply.message->has("id"));

	const ServerTransport transport;
	Endpoint endpoint([&transport](Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response)
		{
			transport.send(ServerTransport::unavailable(), response);
		});
	const Answer answer = endpoint.post(request(Id(1), "tools/list"), headersOf("tools/list"));
	assertEqual(503, answer.status);
	assertEqual(std::string("application/json"), answer.headers.get("Content-Type"));
	assertEqual(static_cast<int>(ErrorCode::INTERNAL_ERROR), answer.code());
}


void ServerTransportTest::testEndpointPath()
{
	Server server;
	buildServer(server);
	Endpoint endpoint(server);
	const std::string body = request(Id(1), "tools/list");
	const NameValueCollection headers = headersOf("tools/list");

	assertEqual(200, endpoint.send(HTTPRequest::HTTP_POST, headers, body, "/mcp").status);
	assertEqual(200, endpoint.send(HTTPRequest::HTTP_POST, headers, body, "/mcp?session=none").status);
	assertEqual(404, endpoint.send(HTTPRequest::HTTP_POST, headers, body, "/mcp/").status);
	assertEqual(404, endpoint.send(HTTPRequest::HTTP_POST, headers, body, "/other").status);
	assertEqual(404, endpoint.send(HTTPRequest::HTTP_GET, NameValueCollection(), "", "/").status);
}


void ServerTransportTest::testClientRoundTrip()
{
	Server server;
	buildServer(server);
	server.registerTool("m\xC3\xA9t\xC3\xA9o", "", parse(
		"{\"type\":\"object\",\"properties\":{"
		"\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"},"
		"\"count\":{\"type\":\"integer\",\"x-mcp-header\":\"Count\"},"
		"\"nested\":{\"type\":\"object\",\"properties\":{\"zone\":{\"type\":\"string\",\"x-mcp-header\":\"Zone\"}}}}}"), echo);
	Endpoint endpoint(server);

	// The headers the client sends are the ones the transport checks: a name
	// and values in the base64 form among them.
	HTTPClient client{Poco::URI(endpoint.url())};
	assertEqual(std::string("transport-server"), client.discover().serverInfo.name);
	assertEqual(2u, static_cast<unsigned>(client.listTools().size()));
	const Object::Ptr pArgs = parse("{\"message\":\"sunny\",\"region\":\" padded \",\"count\":7,\"nested\":{\"zone\":\"z\\u00fcrich\\nnorth\"}}");
	const ToolResult result = client.callTool("m\xC3\xA9t\xC3\xA9o", pArgs);
	assertEqual(std::string("sunny"), result.content()->getObject(0)->getValue<std::string>("text"));
	try
	{
		(void) client.call("subscriptions/listen");
		fail("an unknown method must throw");
	}
	catch (const Poco::AI::MCP::MCPException& exc)
	{
		assertEqual(static_cast<int>(ErrorCode::METHOD_NOT_FOUND), exc.code());
	}
}


void ServerTransportTest::testConcurrentRequests()
{
	Server server;
	buildServer(server);
	server.registerTool("route", "", parse("{\"type\":\"object\",\"properties\":{\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"}}}"), echo);
	Endpoint endpoint(server);
	const std::string url = endpoint.url();

	class Caller: public Poco::Runnable
	{
	public:
		Caller(const std::string& url, int index, std::atomic<int>& failures):
			_url(url),
			_index(index),
			_failures(failures)
		{
		}

		void run() override
		{
			try
			{
				HTTPClient client{Poco::URI(_url)};
				(void) client.listTools();
				for (int i = 0; i < 20; ++i)
				{
					const std::string message = std::to_string(_index) + "-" + std::to_string(i);
					Object::Ptr pArgs = new Object;
					pArgs->set("message", message);
					pArgs->set("region", "r" + std::to_string(_index));
					const ToolResult result = client.callTool("route", pArgs);
					if (result.content()->getObject(0)->getValue<std::string>("text") != message) ++_failures;
				}
			}
			catch (const Poco::Exception&)
			{
				++_failures;
			}
		}

	private:
		std::string _url;
		int _index;
		std::atomic<int>& _failures;
	};

	std::atomic<int> failures{0};
	std::vector<std::unique_ptr<Caller>> callers;
	std::vector<std::unique_ptr<Poco::Thread>> threads;
	for (int i = 0; i < 8; ++i)
	{
		callers.push_back(std::make_unique<Caller>(url, i, failures));
		threads.push_back(std::make_unique<Poco::Thread>());
		threads.back()->start(*callers.back());
	}
	for (auto& pThread: threads) pThread->join();
	assertEqual(0, failures.load());
}


void ServerTransportTest::setUp()
{
}


void ServerTransportTest::tearDown()
{
}


CppUnit::Test* ServerTransportTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ServerTransportTest");

	CppUnit_addTest(pSuite, ServerTransportTest, testMethods);
	CppUnit_addTest(pSuite, ServerTransportTest, testOrigin);
	CppUnit_addTest(pSuite, ServerTransportTest, testContentType);
	CppUnit_addTest(pSuite, ServerTransportTest, testAccept);
	CppUnit_addTest(pSuite, ServerTransportTest, testBodyLimit);
	CppUnit_addTest(pSuite, ServerTransportTest, testMalformedBodies);
	CppUnit_addTest(pSuite, ServerTransportTest, testNotifications);
	CppUnit_addTest(pSuite, ServerTransportTest, testStandardHeaders);
	CppUnit_addTest(pSuite, ServerTransportTest, testNameHeader);
	CppUnit_addTest(pSuite, ServerTransportTest, testParamHeaders);
	CppUnit_addTest(pSuite, ServerTransportTest, testEncodedWords);
	CppUnit_addTest(pSuite, ServerTransportTest, testDecodedHeadersRefused);
	CppUnit_addTest(pSuite, ServerTransportTest, testOtherRevisions);
	CppUnit_addTest(pSuite, ServerTransportTest, testReplyStatus);
	CppUnit_addTest(pSuite, ServerTransportTest, testContext);
	CppUnit_addTest(pSuite, ServerTransportTest, testBodyReadByHost);
	CppUnit_addTest(pSuite, ServerTransportTest, testUnavailable);
	CppUnit_addTest(pSuite, ServerTransportTest, testEndpointPath);
	CppUnit_addTest(pSuite, ServerTransportTest, testClientRoundTrip);
	CppUnit_addTest(pSuite, ServerTransportTest, testConcurrentRequests);

	return pSuite;
}
