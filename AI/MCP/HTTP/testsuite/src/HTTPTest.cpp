//
// HTTPTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPTest.h"
#include "TestHTTPHost.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/AI/MCP/HTTP/Body.h"
#include "Poco/AI/MCP/HTTP/Connection.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"
#include "Poco/Channel.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include <limits>
#include <memory>
#include <sstream>
#include <streambuf>
#include <algorithm>
#include <typeinfo>
#if !defined(POCO_OS_FAMILY_WINDOWS)
#include <fcntl.h>
#endif


using Poco::AI::MCP::Server;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Tool;
using Poco::AI::MCP::MCPException;
using Poco::AI::MCP::ErrorCode;
using Poco::AI::MCP::RequestContext;
using Poco::AI::MCP::HTTP::HTTPClient;
using Poco::AI::MCP::HTTP::EventStreamReader;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
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
		info.name = "http-server";
		info.version = "1.0.0";
		server.setServerInfo(info);
		server.registerTool("echo", "Echo back the message.", nullptr, echo);
	}


	void registerTool(Server& server, const std::string& name, const std::string& schema)
	{
		server.registerTool(name, "", parse(schema), echo);
	}


	std::string toolEntry(const std::string& name, const std::string& schema)
		/// A tools/list entry.
	{
		Object::Ptr pTool = new Object;
		pTool->set("name", name);
		pTool->set("inputSchema", parse(schema));
		std::ostringstream text;
		pTool->stringify(text);
		return text.str();
	}


	std::string firstText(const ToolResult& result)
	{
		return result.content()->getObject(0)->getValue<std::string>("text");
	}


	std::string decoded(const std::string& received)
		/// Decodes the base64 form of a header value the host received; fails
		/// on a plain one.
	{
		const std::string prefix("=?base64?");
		if (received.compare(0, prefix.size(), prefix) != 0 || received.size() < prefix.size() + 2 || received.compare(received.size() - 2, 2, "?=") != 0)
		{
			return "(not encoded) " + received;
		}
		std::istringstream encoded(received.substr(prefix.size(), received.size() - prefix.size() - 2));
		Poco::Base64Decoder decoder(encoded);
		std::string text;
		Poco::StreamCopier::copyToString(decoder, text);
		return text;
	}


	Object::Ptr args(const std::string& json)
	{
		return parse(json);
	}


	class CapturingChannel: public Poco::Channel
		/// Keeps the text of the messages logged through it.
	{
	public:
		void log(const Poco::Message& message) override
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_text += message.getText() + "\n";
		}

		std::string text() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _text;
		}

	private:
		mutable Poco::FastMutex _mutex;
		std::string _text;
	};


	class TrickleBuffer: public std::streambuf
		/// Hands out its text a few bytes at a time: what it has buffered, and
		/// so in_avail(), is never more.
	{
	public:
		TrickleBuffer(const std::string& text, std::size_t piece):
			_text(text),
			_piece(piece)
		{
		}

	protected:
		int_type underflow() override
		{
			if (gptr() != nullptr && gptr() < egptr()) return traits_type::to_int_type(*gptr());
			if (_offset >= _text.size()) return traits_type::eof();
			const std::size_t n = std::min(_piece, _text.size() - _offset);
			char* pBegin = &_text[_offset];
			setg(pBegin, pBegin, pBegin + n);
			_offset += n;
			return traits_type::to_int_type(*gptr());
		}

	private:
		std::string _text;
		std::size_t _piece;
		std::size_t _offset = 0;
	};


	std::vector<std::string> events(const std::string& stream, bool& cut, std::size_t piece = 0)
		/// The data of the events in stream, read as a whole, or in pieces of
		/// that many bytes.
	{
		std::istringstream whole(stream);
		TrickleBuffer trickle(stream, piece > 0 ? piece : 1);
		std::istream pieces(&trickle);
		EventStreamReader reader(piece > 0 ? pieces : static_cast<std::istream&>(whole), 64);
		std::vector<std::string> result;
		EventStreamReader::Event event;
		while (reader.next(event)) result.push_back(event.type + "|" + event.data);
		cut = reader.cut();
		return result;
	}
}


HTTPTest::HTTPTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


HTTPTest::~HTTPTest()
{
}


void HTTPTest::testRoundTrip()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.start();

	HTTPClient client{Poco::URI(host.url())};
	assertEqual(std::string("http-server"), client.discover().serverInfo.name);
	std::vector<Tool> tools = client.listTools();
	assertEqual(1u, static_cast<unsigned>(tools.size()));
	assertEqual(std::string("hello"), firstText(client.callTool("echo", args("{\"message\":\"hello\"}"))));
	assertEqual(3, host.requests());
}


void HTTPTest::testStandardHeaders()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.start();
	HTTPClient client{Poco::URI(host.url())};

	(void) client.listTools();
	Poco::Net::NameValueCollection headers = host.lastHeaders();
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), headers.get("MCP-Protocol-Version"));
	assertEqual(std::string("tools/list"), headers.get("Mcp-Method"));
	assertTrue(!headers.has("Mcp-Name"));
	assertTrue(!headers.has("Mcp-Session-Id"));
	assertTrue(headers.get("Accept").find("application/json") != std::string::npos);
	assertTrue(headers.get("Accept").find("text/event-stream") != std::string::npos);

	(void) client.callTool("echo", args("{\"message\":\"x\"}"));
	headers = host.lastHeaders();
	assertEqual(std::string("tools/call"), headers.get("Mcp-Method"));
	assertEqual(std::string("echo"), headers.get("Mcp-Name"));
}


void HTTPTest::testParamHeaders()
{
	Server server;
	buildServer(server);
	registerTool(server, "route",
		"{\"type\":\"object\",\"properties\":{"
		"\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"},"
		"\"count\":{\"type\":\"integer\",\"x-mcp-header\":\"Count\"},"
		"\"verbose\":{\"type\":\"boolean\",\"x-mcp-header\":\"Verbose\"},"
		"\"note\":{\"type\":\"string\",\"x-mcp-header\":\"Note\"},"
		"\"absent\":{\"type\":\"string\",\"x-mcp-header\":\"Absent\"},"
		"\"nested\":{\"type\":\"object\",\"properties\":{\"zone\":{\"type\":\"string\",\"x-mcp-header\":\"Zone\"}}}}}");
	TestHTTPHost host(server);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	assertEqual(2u, static_cast<unsigned>(client.listTools().size()));

	(void) client.callTool("route", args("{\"region\":\"us-west1\",\"count\":42,\"verbose\":true,\"note\":null,\"nested\":{\"zone\":\"b\"}}"));
	Poco::Net::NameValueCollection headers = host.lastHeaders();
	assertEqual(std::string("us-west1"), headers.get("Mcp-Param-Region"));
	assertEqual(std::string("42"), headers.get("Mcp-Param-Count"));
	assertEqual(std::string("true"), headers.get("Mcp-Param-Verbose"));
	assertEqual(std::string("b"), headers.get("Mcp-Param-Zone"));
	// A null or missing value sends no header.
	assertTrue(!headers.has("Mcp-Param-Note"));
	assertTrue(!headers.has("Mcp-Param-Absent"));
}


void HTTPTest::testUnsafeValuesEncoded()
{
	Server server;
	buildServer(server);
	registerTool(server, "m\xC3\xA9t\xC3\xA9o", "{\"type\":\"object\",\"properties\":{\"text\":{\"type\":\"string\",\"x-mcp-header\":\"Text\"}}}");
	TestHTTPHost host(server);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	(void) client.listTools();

	// A name that is not ASCII travels in the base64 form; the host checks it against the body.
	assertEqual(std::string("ok"), firstText(client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"message\":\"ok\",\"text\":\" padded \"}"))));
	Poco::Net::NameValueCollection headers = host.lastHeaders();
	assertEqual(std::string("m\xC3\xA9t\xC3\xA9o"), decoded(headers.get("Mcp-Name")));
	assertEqual(std::string(" padded "), decoded(headers.get("Mcp-Param-Text")));

	(void) client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"text\":\"line1\\nline2\"}"));
	assertEqual(std::string("line1\nline2"), decoded(host.lastHeaders().get("Mcp-Param-Text")));

	(void) client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"text\":\"=?base64?literal?=\"}"));
	assertEqual(std::string("=?base64?literal?="), decoded(host.lastHeaders().get("Mcp-Param-Text")));

	// The shortest value that starts and ends like the base64 form, where the
	// two overlap, is encoded too.
	(void) client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"text\":\"=?base64?=\"}"));
	assertEqual(std::string("=?base64?="), decoded(host.lastHeaders().get("Mcp-Param-Text")));

	(void) client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"text\":\"plain value\"}"));
	assertEqual(std::string("plain value"), host.lastHeaders().get("Mcp-Param-Text"));

	// "=?" anywhere, where an RFC 2047 encoded word would start, and a value
	// that looks like the base64 form without its "=?", travel encoded and
	// arrive as they were.
	for (const std::string& text: {std::string("a=?b"), std::string("=?UTF-8?B?ZWNobw==?="), std::string("base64?aGVsbG8=?=")})
	{
		assertEqual(std::string("ok"), firstText(client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"message\":\"ok\",\"text\":\"" + text + "\"}"))));
		assertEqual(text, decoded(host.lastHeaders().get("Mcp-Param-Text")));
	}

	// A resource URI with "=?" in it.
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
	const std::vector<Poco::AI::MCP::ResourceContents> contents = client.readResource("db://t?filter=?x");
	assertEqual(std::size_t(1), contents.size());
	assertEqual(std::string("db://t?filter=?x"), decoded(host.lastHeaders().get("Mcp-Name")));
}


void HTTPTest::testInvalidAnnotationsDropped()
{
	Server server;
	buildServer(server);
	registerTool(server, "good", "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"A\"}}}");
	// Tools another server may list, which this one would not take.
	TestHTTPHost host(server);
	host.addListedTool(toolEntry("number", "{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"number\",\"x-mcp-header\":\"N\"}}}"));
	host.addListedTool(toolEntry("in-items", "{\"type\":\"object\",\"properties\":{\"list\":{\"type\":\"array\",\"items\":{\"type\":\"string\",\"x-mcp-header\":\"Item\"}}}}"));
	host.addListedTool(toolEntry("in-oneof", "{\"type\":\"object\",\"oneOf\":[{\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"A\"}}}]}"));
	host.addListedTool(toolEntry("twice", "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"Name\"},\"b\":{\"type\":\"string\",\"x-mcp-header\":\"name\"}}}"));
	host.addListedTool(toolEntry("bad-name", "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"Bad Name\"}}}"));
	host.addListedTool(toolEntry("at-root", "{\"type\":\"object\",\"x-mcp-header\":\"Root\"}"));
	host.start();
	HTTPClient client{Poco::URI(host.url())};

	std::vector<Tool> tools = client.listTools();
	assertEqual(2u, static_cast<unsigned>(tools.size()));
	assertEqual(std::string("echo"), tools[0].name);
	assertEqual(std::string("good"), tools[1].name);
}


void HTTPTest::testErrorReplies()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.start();
	HTTPClient client{Poco::URI(host.url())};

	// An unknown tool: a JSON-RPC error in a 200.
	try
	{
		(void) client.callTool("missing");
		fail("an unknown tool must throw");
	}
	catch (const MCPException& exc)
	{
		assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), exc.code());
	}

	// An unknown method: a JSON-RPC error in a 404.
	try
	{
		(void) client.call("subscriptions/listen");
		fail("an unknown method must throw");
	}
	catch (const MCPException& exc)
	{
		assertEqual(static_cast<int>(ErrorCode::METHOD_NOT_FOUND), exc.code());
	}

	// The client still works.
	assertEqual(std::string("still"), firstText(client.callTool("echo", args("{\"message\":\"still\"}"))));
}


void HTTPTest::testHttpStatus()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_HTML_502);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	try
	{
		(void) client.listTools();
		fail("an HTML 502 must throw");
	}
	catch (const Poco::Net::HTTPException& exc)
	{
		assertTrue(exc.displayText().find("502") != std::string::npos);
		assertTrue(exc.displayText().find("Bad Gateway") != std::string::npos);
	}
}


void HTTPTest::testEventStream()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server, TestHTTPHost::REPLY_EVENT_STREAM);
	host.start();
	HTTPClient client{Poco::URI(host.url())};

	// The stream carries a comment, a notification, another id and an event of
	// another type before the reply, whose data spans two lines.
	assertEqual(std::string("http-server"), client.discover().serverInfo.name);
	ToolResult result = client.callTool("echo", args("{\"message\":\"streamed\"}"));
	assertEqual(std::string("streamed"), firstText(result));
	assertEqual(1u, static_cast<unsigned>(client.listTools().size()));
}


void HTTPTest::testCutEventStream()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_CUT_STREAM);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	try
	{
		(void) client.listTools();
		fail("a stream without the reply must throw");
	}
	catch (const Poco::IOException& exc)
	{
		// The end of the stream, not a failure of the connection or of HTTP.
		assertTrue(typeid(exc) == typeid(Poco::IOException));
		assertTrue(exc.message().find("ended before the reply to tools/list") != std::string::npos);
	}
	assertEqual(1, host.requests());
}


void HTTPTest::testResponseLimit()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_OVERSIZED);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	client.setMaxResponseSize(16384);
	try
	{
		(void) client.listTools();
		fail("a reply beyond the limit must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}

	TestHTTPHost streamHost(server, TestHTTPHost::REPLY_EVENT_STREAM);
	streamHost.setFault(TestHTTPHost::FAULT_OVERSIZED);
	streamHost.start();
	HTTPClient streamClient{Poco::URI(streamHost.url())};
	streamClient.setMaxResponseSize(16384);
	try
	{
		(void) streamClient.listTools();
		fail("an event beyond the limit must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}

	// Within the limit all is well.
	HTTPClient bigClient{Poco::URI(host.url())};
	assertEqual(1u, static_cast<unsigned>(bigClient.listTools().size()));
}


void HTTPTest::testEventAtResponseLimit()
{
	// A reply on one data line of an event stream is read when it is exactly
	// the limit, with the field name in the line, and refused when it is one
	// byte more. Each client sends the same request, which gets the same reply.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_ONE_LINE);
	host.start();
	HTTPClient sizer{Poco::URI(host.url())};
	(void) sizer.listTools();
	const std::size_t size = host.replies().back().size();

	HTTPClient exact{Poco::URI(host.url())};
	exact.setMaxResponseSize(size);
	assertEqual(1u, static_cast<unsigned>(exact.listTools().size()));

	HTTPClient under{Poco::URI(host.url())};
	under.setMaxResponseSize(size - 1);
	try
	{
		(void) under.listTools();
		fail("an event beyond the limit must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}
	const std::vector<std::string> replies = host.replies();
	assertEqual(3u, static_cast<unsigned>(replies.size()));
	assertTrue(replies[1] == replies[0] && replies[2] == replies[0]);
}


void HTTPTest::testTimeout()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_SLOW);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	client.setTimeout(Poco::Timespan(0, 300000));
	try
	{
		(void) client.listTools();
		fail("a reply later than the timeout must throw");
	}
	catch (const Poco::TimeoutException&)
	{
	}
}


void HTTPTest::testEventStreamDeadline()
{
	// Comments that keep coming do not keep a request alive past its timeout.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_TRICKLE);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	client.setTimeout(Poco::Timespan(0, 500000));
	try
	{
		(void) client.listTools();
		fail("a stream without the reply must end at the deadline");
	}
	catch (const Poco::TimeoutException&)
	{
	}
}


void HTTPTest::testStaleKeepAlive()
{
	// The host promises to keep the connection and closes it: the client does
	// not use the closed connection again.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_STALE_KEEP_ALIVE);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	assertEqual(std::string("one"), firstText(client.callTool("echo", args("{\"message\":\"one\"}"))));
	assertEqual(1u, static_cast<unsigned>(client.listTools().size()));
	assertEqual(2, host.requests());
}


void HTTPTest::testLostAnswerNotRepeated()
{
	// The host reads the second request and closes the connection without
	// answering. A tool call may have run: it is not sent again.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_DROP_SECOND);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	assertEqual(std::string("one"), firstText(client.callTool("echo", args("{\"message\":\"one\"}"))));
	try
	{
		(void) client.callTool("echo", args("{\"message\":\"two\"}"));
		fail("a tool call whose answer was lost must throw");
	}
	catch (const Poco::IOException&)
	{
	}
	assertEqual(2, host.requests());

	// A list changes nothing: it is sent again on a new connection.
	Server listServer;
	buildServer(listServer);
	TestHTTPHost listHost(listServer);
	listHost.setFault(TestHTTPHost::FAULT_DROP_SECOND);
	listHost.start();
	HTTPClient listClient{Poco::URI(listHost.url())};
	assertEqual(1u, static_cast<unsigned>(listClient.listTools().size()));
	assertEqual(1u, static_cast<unsigned>(listClient.listTools().size()));
	assertEqual(3, listHost.requests());
	// It goes again as a new request, with an id of its own.
	const std::vector<std::string> bodies = listHost.bodies();
	assertEqual(3u, static_cast<unsigned>(bodies.size()));
	const Object::Ptr pLost = parse(bodies[1]);
	const Object::Ptr pAgain = parse(bodies[2]);
	assertEqual(std::string("tools/list"), pAgain->getValue<std::string>("method"));
	assertTrue(pLost->get("id") != pAgain->get("id"));
}


void HTTPTest::testAnswersRefused()
{
	// A 202 to a request, the reply to another id, a body that is not JSON,
	// and a 400 with the error of another request.
	const TestHTTPHost::Fault faults[] = {
		TestHTTPHost::FAULT_ACCEPTED, TestHTTPHost::FAULT_OTHER_ID, TestHTTPHost::FAULT_NOT_JSON, TestHTTPHost::FAULT_ERROR_OTHER_ID
	};
	for (const TestHTTPHost::Fault fault: faults)
	{
		Server server;
		buildServer(server);
		TestHTTPHost host(server);
		host.setFault(fault);
		host.start();
		HTTPClient client{Poco::URI(host.url())};
		try
		{
			(void) client.callTool("echo", args("{\"message\":\"x\"}"));
			fail("an answer that is not the reply must throw");
		}
		catch (const Poco::ProtocolException& exc)
		{
			if (fault == TestHTTPHost::FAULT_NOT_JSON)
			{
				assertTrue(exc.displayText().find("Service temporarily moved") != std::string::npos);
			}
		}
		assertEqual(1, host.requests());
	}
}


void HTTPTest::testBodyBreaksOff()
{
	// A body shorter than its promised length broke off: an I/O failure, not
	// an answer that is not JSON.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_CUT_BODY);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	try
	{
		(void) client.callTool("echo", args("{\"message\":\"x\"}"));
		fail("a body that breaks off must throw");
	}
	catch (const Poco::IOException& exc)
	{
		assertTrue(dynamic_cast<const Poco::ProtocolException*>(&exc) == nullptr);
	}
	assertEqual(1, host.requests());
}


void HTTPTest::testHeaderMismatch()
{
	Server server;
	buildServer(server);
	registerTool(server, "regional", "{\"type\":\"object\",\"properties\":{\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"}}}");
	TestHTTPHost host(server);
	host.start();

	// A tool called before it was listed has no Mcp-Param headers: the host
	// refuses the call with HEADER_MISMATCH, and callTool() lists the tools
	// and calls once more, now with the header.
	HTTPClient client{Poco::URI(host.url())};
	assertEqual(std::string("eu"), firstText(client.callTool("regional", args("{\"message\":\"eu\",\"region\":\"eu-west1\"}"))));
	assertEqual(3, host.requests());
	assertEqual(std::string("eu-west1"), host.lastHeaders().get("Mcp-Param-Region"));

	// call() does not: the 400 with the error is an MCPException.
	HTTPClient bare{Poco::URI(host.url())};
	Object::Ptr pParams = new Object;
	pParams->set("name", "regional");
	pParams->set("arguments", args("{\"region\":\"eu-west1\"}"));
	try
	{
		(void) bare.call("tools/call", pParams);
		fail("a header mismatch must throw");
	}
	catch (const MCPException& exc)
	{
		assertEqual(static_cast<int>(ErrorCode::HEADER_MISMATCH), exc.code());
	}

	// An integer outside the safe range has no header: the client sends
	// none, and the server expects none.
	registerTool(server, "counted", "{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"integer\",\"x-mcp-header\":\"N\"}}}");
	(void) client.listTools();
	(void) client.callTool("counted", args("{\"n\":9007199254740993}"));
	assertTrue(!host.lastHeaders().has("Mcp-Param-N"));
	(void) client.callTool("counted", args("{\"n\":9007199254740991}"));
	assertEqual(std::string("9007199254740991"), host.lastHeaders().get("Mcp-Param-N"));
}


void HTTPTest::testReadsBoundedByTheTimeout()
{
	// A body that stops half way, and a body and an event line that trickle
	// in, a byte every 20 ms: each read gets what is left of the timeout, so
	// the request ends near it, not a socket timeout or more later.
	const TestHTTPHost::Fault faults[] = {TestHTTPHost::FAULT_STALL_BODY, TestHTTPHost::FAULT_TRICKLE_BODY, TestHTTPHost::FAULT_TRICKLE_LINE};
	for (const TestHTTPHost::Fault fault: faults)
	{
		Server server;
		buildServer(server);
		TestHTTPHost host(server);
		host.setFault(fault);
		host.start();
		HTTPClient client{Poco::URI(host.url())};
		client.setTimeout(Poco::Timespan(0, 600000));
		const Poco::Timestamp start;
		try
		{
			(void) client.listTools();
			fail("a reply that takes longer than the timeout must throw");
		}
		catch (const Poco::TimeoutException&)
		{
		}
		assertTrue(start.elapsed() < 3 * Poco::Timestamp::resolution());
	}
}


void HTTPTest::testHeadBoundedByTheTimeout()
{
	// The head of an answer that trickles in, a byte every 20 ms, and one
	// 100 Continue after another: the session reads the head by itself, and
	// the request still ends near the timeout.
	const TestHTTPHost::Fault faults[] = {TestHTTPHost::FAULT_TRICKLE_HEAD, TestHTTPHost::FAULT_CONTINUE_FLOOD};
	for (const TestHTTPHost::Fault fault: faults)
	{
		Server server;
		buildServer(server);
		TestHTTPHost host(server);
		host.setFault(fault);
		host.start();
		HTTPClient client{Poco::URI(host.url())};
		client.setTimeout(Poco::Timespan(0, 600000));
		const Poco::Timestamp start;
		try
		{
			(void) client.listTools();
			fail("an answer whose head takes longer than the timeout must throw");
		}
		catch (const Poco::TimeoutException&)
		{
		}
		assertTrue(start.elapsed() < 3 * Poco::Timestamp::resolution());
	}
}


void HTTPTest::testDuplicateNotInherited()
{
	// The descriptor Impl::duplicate() makes of a connection is not inherited
	// by child processes, as the sockets of Poco::Net are not.
	Poco::Net::ServerSocket listener(Poco::Net::SocketAddress("127.0.0.1", 0));
	const poco_socket_t copy = Poco::AI::MCP::HTTP::Impl::duplicate(listener.impl()->sockfd());
	assertTrue(copy != POCO_INVALID_SOCKET);
#if defined(POCO_OS_FAMILY_WINDOWS)
	DWORD flags = 0;
	const bool known = ::GetHandleInformation(reinterpret_cast<HANDLE>(copy), &flags) != 0;
	poco_closesocket(copy);
	assertTrue(known);
	assertTrue((flags & HANDLE_FLAG_INHERIT) == 0);
#else
	const int flags = ::fcntl(copy, F_GETFD);
	poco_closesocket(copy);
	assertTrue(flags != -1);
	assertTrue((flags & FD_CLOEXEC) != 0);
#endif
	assertTrue(Poco::AI::MCP::HTTP::Impl::duplicate(POCO_INVALID_SOCKET) == POCO_INVALID_SOCKET);
}


void HTTPTest::testEventStreamFraming()
{
	// A byte order mark at the start of the stream is skipped.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_BOM);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	assertEqual(1u, static_cast<unsigned>(client.listTools().size()));

	// A reply whose blank line never comes is not delivered.
	Server cutServer;
	buildServer(cutServer);
	TestHTTPHost cutHost(cutServer);
	cutHost.setFault(TestHTTPHost::FAULT_NO_BLANK_LINE);
	cutHost.start();
	HTTPClient cutClient{Poco::URI(cutHost.url())};
	try
	{
		(void) cutClient.listTools();
		fail("an event without its blank line must not be delivered");
	}
	catch (const Poco::IOException& exc)
	{
		assertTrue(exc.message().find("ended before the reply") != std::string::npos);
	}
}


void HTTPTest::testLogShowsNoSecrets()
{
	// A tool left out is logged with its name made harmless, and with the
	// endpoint without user info and query.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.addListedTool(toolEntry(std::string("forged\nINFO all is well") + std::string(300, 'x'), "{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"number\",\"x-mcp-header\":\"N\"}}}"));
	host.start();
	Poco::AutoPtr<CapturingChannel> pChannel = new CapturingChannel;
	Poco::Logger& logger = Poco::Logger::get("AI.MCP.HTTPClient");
	Poco::Channel::Ptr pOldChannel = logger.getChannel();
	const int oldLevel = logger.getLevel();
	logger.setChannel(pChannel);
	logger.setLevel(Poco::Message::PRIO_WARNING);
	HTTPClient client{Poco::URI("http://user:secret@127.0.0.1:" + std::to_string(host.port()) + "/mcp?token=abc")};
	(void) client.listTools();
	logger.setChannel(pOldChannel);
	logger.setLevel(oldLevel);

	const std::string text = pChannel->text();
	assertTrue(text.find("forged\\x0aINFO") != std::string::npos);
	assertTrue(text.find("forged\nINFO") == std::string::npos);
	assertTrue(text.find(std::string(200, 'x')) == std::string::npos);
	assertTrue(text.find("secret") == std::string::npos);
	assertTrue(text.find("token") == std::string::npos);
	assertTrue(text.find("127.0.0.1") != std::string::npos);
}


void HTTPTest::testRequestHook()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	client.setRequestHook([](Poco::Net::HTTPRequest& request)
	{
		request.set("Authorization", "Bearer secret-token");
	});
	(void) client.listTools();
	assertEqual(std::string("Bearer secret-token"), host.lastHeaders().get("Authorization"));
}


void HTTPTest::testSessionFactory()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.start();

	// A session with a host is kept for later requests.
	int created = 0;
	std::string scheme;
	HTTPClient client(Poco::URI(host.url()),
		[&created, &scheme](const Poco::URI& uri)
		{
			++created;
			scheme = uri.getScheme();
			return std::make_unique<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
		});
	(void) client.discover();
	(void) client.listTools();
	(void) client.callTool("echo");
	assertEqual(1, created);
	assertEqual(std::string("http"), scheme);
	assertEqual(3, host.requests());
}


void HTTPTest::testHostlessSession()
{
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.start();

	// A session on a socket the factory connected has no host to reconnect
	// to: it serves one request.
	int created = 0;
	HTTPClient client(Poco::URI(host.url()),
		[&created](const Poco::URI& uri)
		{
			++created;
			Poco::Net::StreamSocket socket(Poco::Net::SocketAddress(uri.getHost(), uri.getPort()));
			return std::make_unique<Poco::Net::HTTPClientSession>(socket);
		});
	(void) client.discover();
	(void) client.listTools();
	assertEqual(2, created);
}


void HTTPTest::testNullSession()
{
	HTTPClient client(Poco::URI("http://127.0.0.1:1/mcp"),
		[](const Poco::URI&)
		{
			return std::unique_ptr<Poco::Net::HTTPClientSession>();
		});
	try
	{
		(void) client.listTools();
		fail("a factory that returns no session must throw");
	}
	catch (const Poco::NullPointerException&)
	{
	}
}


void HTTPTest::testHttpsNeedsInstantiator()
{
	// The default factory serves http itself; any other scheme goes to
	// Poco::Net::HTTPSessionFactory, which needs a registered instantiator
	// (for https, the one from NetSSL). Without one the client reports the
	// scheme as unknown instead of silently talking plain HTTP.
	HTTPClient client{Poco::URI("https://127.0.0.1:1/mcp")};
	try
	{
		(void) client.discover();
		fail("https without a registered instantiator must throw");
	}
	catch (const Poco::UnknownURISchemeException&)
	{
	}
}


void HTTPTest::testReadAtMostStopsAtTheLimit()
{
	// Once the prefix is complete, nothing more is asked of the source: a
	// guard that would throw, as for a deadline that has passed, is not
	// called, and the prefix is returned.
	int guardCalls = 0;
	const Poco::AI::MCP::HTTP::ReadGuard guard = [&guardCalls]()
	{
		++guardCalls;
		throw Poco::TimeoutException("no more input wanted");
	};
	std::istringstream exact("0123456789");
	assertEqual(std::string("0123456789"), Poco::AI::MCP::HTTP::Impl::readAtMost(exact, 10, guard));
	std::istringstream longer("0123456789abcdef");
	assertEqual(std::string("0123456789"), Poco::AI::MCP::HTTP::Impl::readAtMost(longer, 10, guard));
	assertEqual(0, guardCalls);

	// A body that is not complete yet still asks the guard before it waits.
	std::istringstream shorter("01234");
	try
	{
		(void) Poco::AI::MCP::HTTP::Impl::readAtMost(shorter, 10, guard);
		fail("a prefix that is not complete must wait for more input");
	}
	catch (const Poco::TimeoutException&)
	{
	}
	assertEqual(1, guardCalls);

	// A whole body larger than the limit is refused, whatever message it is.
	std::istringstream large("0123456789abcdef");
	try
	{
		(void) Poco::AI::MCP::HTTP::Impl::readBody(large, 10);
		fail("a body larger than the limit must be refused");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertMessageEqual("The body exceeds the size limit", exc.message());
	}
}


void HTTPTest::testEventStreamReader()
{
	bool cut = false;
	// CR LF, LF and CR line ends; comments; one space after the colon dropped;
	// data lines joined; a field without a colon; events without data skipped.
	std::vector<std::string> result = events(": hello\r\ndata: a\r\ndata:b\r\n\r\nevent: x\ndata:  two spaces\n\nevent: y\n\ndata\rdata: c\r\r", cut);
	assertEqual(3u, static_cast<unsigned>(result.size()));
	assertEqual(std::string("|a\nb"), result[0]);
	assertEqual(std::string("x| two spaces"), result[1]);
	assertEqual(std::string("|\nc"), result[2]);
	assertTrue(!cut);

	// An event whose blank line has not come at the end is dropped.
	result = events("data: last\n", cut);
	assertEqual(0u, static_cast<unsigned>(result.size()));
	assertTrue(cut);

	// One byte order mark at the start is skipped; bytes that only begin like
	// one are text.
	result = events("\xEF\xBB\xBF" "data: marked\n\n", cut);
	assertEqual(1u, static_cast<unsigned>(result.size()));
	assertEqual(std::string("|marked"), result[0]);
	result = events("\xEF\xBB" "data: x\n\ndata: y\n\n", cut);
	assertEqual(1u, static_cast<unsigned>(result.size()));
	assertEqual(std::string("|y"), result[0]);

	// Data of exactly the limit fits, with its field name in the line.
	result = events("data: " + std::string(64, 'd') + "\n\n", cut);
	assertEqual(1u, static_cast<unsigned>(result.size()));
	assertEqual(std::string("|") + std::string(64, 'd'), result[0]);

	// A limit near the largest size leaves lines of any length.
	{
		std::istringstream in("data: " + std::string(200, 'x') + "\n\n");
		EventStreamReader reader(in, std::numeric_limits<std::size_t>::max());
		EventStreamReader::Event event;
		assertTrue(reader.next(event));
		assertEqual(std::string(200, 'x'), event.data);
	}

	// A line cut off at the end is dropped.
	result = events("data: full\n\ndata: cut off", cut);
	assertEqual(1u, static_cast<unsigned>(result.size()));
	assertEqual(std::string("|full"), result[0]);
	assertTrue(cut);

	// A line beyond the limit throws.
	try
	{
		(void) events("data: " + std::string(200, 'x') + "\n\n", cut);
		fail("a line beyond the limit must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}
	try
	{
		(void) events("data: " + std::string(40, 'x') + "\ndata: " + std::string(40, 'x') + "\n\n", cut);
		fail("an event beyond the limit must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}

	// Read in pieces of a few bytes, as a network may deliver them, the
	// streams give the same events: a CR and its LF, a byte order mark and a
	// line may be split between two reads.
	const std::string streams[] = {
		": hello\r\ndata: a\r\ndata:b\r\n\r\nevent: x\ndata:  two spaces\n\nevent: y\n\ndata\rdata: c\r\r",
		"\xEF\xBB\xBF" "data: marked\n\n",
		"\xEF\xBB" "data: x\n\ndata: y\n\n",
		"data: " + std::string(64, 'd') + "\n\n",
		"data: full\n\ndata: cut off",
		"data: last\n"
	};
	for (const std::string& text: streams)
	{
		bool wholeCut = false;
		const std::vector<std::string> whole = events(text, wholeCut);
		for (const std::size_t piece: {std::size_t(1), std::size_t(2), std::size_t(3), std::size_t(5)})
		{
			bool pieceCut = false;
			assertTrue(events(text, pieceCut, piece) == whole);
			assertEqual(wholeCut, pieceCut);
		}
	}
	try
	{
		(void) events("data: " + std::string(200, 'x') + "\n\n", cut, 3);
		fail("a line beyond the limit must throw, also in pieces");
	}
	catch (const Poco::DataFormatException&)
	{
	}

	// A line longer than what the reader takes from the stream at once.
	std::istringstream longStream("data: " + std::string(20000, 'L') + "\r\n\r\ndata: next\n\n");
	EventStreamReader longReader(longStream, 30000);
	EventStreamReader::Event longEvent;
	assertTrue(longReader.next(longEvent));
	assertEqual(std::string(20000, 'L'), longEvent.data);
	assertTrue(longReader.next(longEvent));
	assertEqual(std::string("next"), longEvent.data);
	assertTrue(!longReader.next(longEvent));
	assertTrue(!longReader.cut());
}


void HTTPTest::setUp()
{
}


void HTTPTest::tearDown()
{
}


CppUnit::Test* HTTPTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPTest");

	CppUnit_addTest(pSuite, HTTPTest, testRoundTrip);
	CppUnit_addTest(pSuite, HTTPTest, testStandardHeaders);
	CppUnit_addTest(pSuite, HTTPTest, testParamHeaders);
	CppUnit_addTest(pSuite, HTTPTest, testUnsafeValuesEncoded);
	CppUnit_addTest(pSuite, HTTPTest, testInvalidAnnotationsDropped);
	CppUnit_addTest(pSuite, HTTPTest, testErrorReplies);
	CppUnit_addTest(pSuite, HTTPTest, testHttpStatus);
	CppUnit_addTest(pSuite, HTTPTest, testEventStream);
	CppUnit_addTest(pSuite, HTTPTest, testCutEventStream);
	CppUnit_addTest(pSuite, HTTPTest, testResponseLimit);
	CppUnit_addTest(pSuite, HTTPTest, testEventAtResponseLimit);
	CppUnit_addTest(pSuite, HTTPTest, testTimeout);
	CppUnit_addTest(pSuite, HTTPTest, testEventStreamDeadline);
	CppUnit_addTest(pSuite, HTTPTest, testStaleKeepAlive);
	CppUnit_addTest(pSuite, HTTPTest, testLostAnswerNotRepeated);
	CppUnit_addTest(pSuite, HTTPTest, testAnswersRefused);
	CppUnit_addTest(pSuite, HTTPTest, testBodyBreaksOff);
	CppUnit_addTest(pSuite, HTTPTest, testHeaderMismatch);
	CppUnit_addTest(pSuite, HTTPTest, testReadsBoundedByTheTimeout);
	CppUnit_addTest(pSuite, HTTPTest, testHeadBoundedByTheTimeout);
	CppUnit_addTest(pSuite, HTTPTest, testDuplicateNotInherited);
	CppUnit_addTest(pSuite, HTTPTest, testEventStreamFraming);
	CppUnit_addTest(pSuite, HTTPTest, testLogShowsNoSecrets);
	CppUnit_addTest(pSuite, HTTPTest, testRequestHook);
	CppUnit_addTest(pSuite, HTTPTest, testSessionFactory);
	CppUnit_addTest(pSuite, HTTPTest, testHostlessSession);
	CppUnit_addTest(pSuite, HTTPTest, testNullSession);
	CppUnit_addTest(pSuite, HTTPTest, testHttpsNeedsInstantiator);
	CppUnit_addTest(pSuite, HTTPTest, testEventStreamReader);
	CppUnit_addTest(pSuite, HTTPTest, testReadAtMostStopsAtTheLimit);

	return pSuite;
}
