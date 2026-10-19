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
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <memory>
#include <sstream>


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


	std::string firstText(const ToolResult& result)
	{
		return result.content()->getObject(0)->getValue<std::string>("text");
	}


	std::string decoded(const std::string& received)
		/// Decodes the base64 form of a header value the host received; fails
		/// on a plain one. Poco::Net::MessageHeader drops the "=?" of the form
		/// when it reads it.
	{
		const std::string prefix("=?base64?");
		const std::string value = received.compare(0, prefix.size() - 2, prefix, 2, std::string::npos) == 0 ? "=?" + received : received;
		if (value.compare(0, prefix.size(), prefix) != 0 || value.size() < prefix.size() + 2 || value.compare(value.size() - 2, 2, "?=") != 0)
		{
			return "(not encoded) " + value;
		}
		std::istringstream encoded(value.substr(prefix.size(), value.size() - prefix.size() - 2));
		Poco::Base64Decoder decoder(encoded);
		std::string text;
		Poco::StreamCopier::copyToString(decoder, text);
		return text;
	}


	Object::Ptr args(const std::string& json)
	{
		return parse(json);
	}


	std::vector<std::string> events(const std::string& stream, bool& cut)
		/// The data of the events in stream.
	{
		std::istringstream in(stream);
		EventStreamReader reader(in, 64);
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

	(void) client.callTool("m\xC3\xA9t\xC3\xA9o", args("{\"text\":\"plain value\"}"));
	assertEqual(std::string("plain value"), host.lastHeaders().get("Mcp-Param-Text"));
}


void HTTPTest::testInvalidAnnotationsDropped()
{
	Server server;
	buildServer(server);
	registerTool(server, "good", "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"A\"}}}");
	registerTool(server, "number", "{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"number\",\"x-mcp-header\":\"N\"}}}");
	registerTool(server, "in-items", "{\"type\":\"object\",\"properties\":{\"list\":{\"type\":\"array\",\"items\":{\"type\":\"string\",\"x-mcp-header\":\"Item\"}}}}");
	registerTool(server, "in-oneof", "{\"type\":\"object\",\"oneOf\":[{\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"A\"}}}]}");
	registerTool(server, "twice", "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"Name\"},\"b\":{\"type\":\"string\",\"x-mcp-header\":\"name\"}}}");
	registerTool(server, "bad-name", "{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"Bad Name\"}}}");
	registerTool(server, "at-root", "{\"type\":\"object\",\"x-mcp-header\":\"Root\"}");
	TestHTTPHost host(server);
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
	catch (const Poco::IOException&)
	{
	}
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
	// The host promises to keep the connection and closes it: the second
	// request fails on the kept session and is sent once more on a new one.
	Server server;
	buildServer(server);
	TestHTTPHost host(server);
	host.setFault(TestHTTPHost::FAULT_STALE_KEEP_ALIVE);
	host.start();
	HTTPClient client{Poco::URI(host.url())};
	assertEqual(std::string("one"), firstText(client.callTool("echo", args("{\"message\":\"one\"}"))));
	assertEqual(std::string("two"), firstText(client.callTool("echo", args("{\"message\":\"two\"}"))));
	assertEqual(2, host.requests());
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

	// Complete lines without the closing blank line still make an event.
	result = events("data: last\n", cut);
	assertEqual(1u, static_cast<unsigned>(result.size()));
	assertTrue(cut);

	// A line cut off at the end is dropped.
	result = events("data: full\n\ndata: cut off", cut);
	assertEqual(1u, static_cast<unsigned>(result.size()));
	assertEqual(std::string("|full"), result[0]);
	assertTrue(cut);

	// A line beyond the limit throws.
	try
	{
		(void) events("data: " + std::string(100, 'x') + "\n\n", cut);
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
	CppUnit_addTest(pSuite, HTTPTest, testTimeout);
	CppUnit_addTest(pSuite, HTTPTest, testEventStreamDeadline);
	CppUnit_addTest(pSuite, HTTPTest, testStaleKeepAlive);
	CppUnit_addTest(pSuite, HTTPTest, testRequestHook);
	CppUnit_addTest(pSuite, HTTPTest, testSessionFactory);
	CppUnit_addTest(pSuite, HTTPTest, testHostlessSession);
	CppUnit_addTest(pSuite, HTTPTest, testNullSession);
	CppUnit_addTest(pSuite, HTTPTest, testHttpsNeedsInstantiator);
	CppUnit_addTest(pSuite, HTTPTest, testEventStreamReader);

	return pSuite;
}
