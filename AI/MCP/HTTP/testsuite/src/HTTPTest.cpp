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
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <memory>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::HTTP::HTTPClient;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	void registerEcho(Server& server)
	{
		server.registerTool("echo", "Echo back the message.", new Object,
			[](Object::Ptr args)
			{
				ToolResult result;
				result.addText(args && args->has("message") ? args->getValue<std::string>("message") : std::string());
				return result;
			});
	}

	Poco::URI endpoint(Poco::UInt16 port)
	{
		Poco::URI uri;
		uri.setScheme("http");
		uri.setHost("127.0.0.1");
		uri.setPort(port);
		uri.setPath("/mcp");
		return uri;
	}

	std::string firstText(const ToolResult& result)
	{
		Array::Ptr content = result.content();
		if (content && content->size() > 0 && content->getObject(0)->has("text"))
		{
			return content->getObject(0)->getValue<std::string>("text");
		}
		return std::string();
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
	server.setServerInfo("http-server", "1.0.0");
	registerEcho(server);

	TestHTTPHost http(server);
	http.start();

	HTTPClient client(endpoint(http.port()));
	client.initialize("test-client", "1.0.0");
	assertTrue(!client.sessionId().empty());

	Array::Ptr tools = client.listTools();
	assertEqual(1, static_cast<int>(tools->size()));

	Object::Ptr args = new Object;
	args->set("message", "hi");
	assertEqual(std::string("hi"), firstText(client.callTool("echo", args)));

	client.ping();

	http.stop();
}


void HTTPTest::testSessionRequired()
{
	Server server;
	server.setServerInfo("http-server", "1.0.0");
	registerEcho(server);

	TestHTTPHost http(server);
	http.start();

	// A non-initialize request without a session id is rejected; the client sees
	// the JSON-RPC error and throws.
	HTTPClient client(endpoint(http.port()));
	bool threw = false;
	try
	{
		client.listTools();
	}
	catch (Poco::Exception&)
	{
		threw = true;
	}
	assertTrue(threw);

	http.stop();
}


void HTTPTest::testSessionFactory()
{
	Server server;
	server.setServerInfo("http-server", "1.0.0");
	registerEcho(server);

	TestHTTPHost http(server);
	http.start();

	// A caller-supplied factory creates every session. This one counts the
	// requests and records the URI it is asked for.
	int created = 0;
	std::string scheme;
	HTTPClient client(endpoint(http.port()),
		[&created, &scheme](const Poco::URI& uri)
		{
			++created;
			scheme = uri.getScheme();
			return std::make_unique<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
		});
	client.initialize("test-client", "1.0.0");
	client.ping();

	// initialize, notifications/initialized, ping: one session each.
	assertEqual(3, created);
	assertEqual(std::string("http"), scheme);

	http.stop();
}


void HTTPTest::testHttpsNeedsInstantiator()
{
	// The default factory serves http itself; any other scheme goes to
	// Poco::Net::HTTPSessionFactory, which needs a registered instantiator
	// (for https, the one from NetSSL). Without one the client reports the
	// scheme as unknown instead of silently talking plain HTTP.
	HTTPClient client(Poco::URI("https://127.0.0.1:1/mcp"));
	try
	{
		client.ping();
		fail("https without a registered instantiator must throw");
	}
	catch (Poco::UnknownURISchemeException&)
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
	CppUnit_addTest(pSuite, HTTPTest, testSessionRequired);
	CppUnit_addTest(pSuite, HTTPTest, testSessionFactory);
	CppUnit_addTest(pSuite, HTTPTest, testHttpsNeedsInstantiator);

	return pSuite;
}
