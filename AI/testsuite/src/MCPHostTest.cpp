//
// MCPHostTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MCPHostTest.h"
#include "TestHTTPHost.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCPHost.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <vector>


using Poco::AI::MCPHost;
using Poco::AI::ToolRegistry;
using Poco::JSON::Object;
using Poco::JSON::Array;
using Poco::AI::MCP::ToolResult;


namespace
{
	void buildServer(Poco::AI::MCP::Server& server)
	{
		server.setServerInfo("test-mcp", "1.0");

		Object::Ptr messageProp = new Object;
		messageProp->set("type", "string");
		Object::Ptr props = new Object;
		props->set("message", messageProp);
		Object::Ptr schema = new Object;
		schema->set("type", "object");
		schema->set("properties", props);

		server.registerTool("echo", "Echo back the message.", schema,
			[](Object::Ptr args)
			{
				ToolResult result;
				result.addText(args && args->has("message") ? args->getValue<std::string>("message") : std::string());
				return result;
			});

		server.registerTool("boom", "Always throws.", new Object,
			[](Object::Ptr) -> ToolResult
			{
				throw Poco::RuntimeException("kaboom");
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
}


MCPHostTest::MCPHostTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


MCPHostTest::~MCPHostTest()
{
}


void MCPHostTest::testRegisterAndCall()
{
	Poco::AI::MCP::Server server;
	buildServer(server);
	TestHTTPHost http(server);
	http.start();

	ToolRegistry registry;
	MCPHost host;
	host.addServer("test", endpoint(http.port()), "test-client", "1.0");
	assertEqual(1, static_cast<int>(host.serverCount()));

	std::size_t registered = host.registerInto(registry);
	assertEqual(2, static_cast<int>(registered));
	assertTrue(registry.hasTool("echo"));
	assertTrue(registry.hasTool("boom"));

	// The MCP inputSchema becomes the registry "parameters" JSON Schema.
	Array defs = registry.getToolDefinitions();
	bool foundEcho = false;
	for (std::size_t i = 0; i < defs.size(); ++i)
	{
		Object::Ptr def = defs.getObject(i);
		if (def->getValue<std::string>("name") == "echo")
		{
			foundEcho = true;
			assertTrue(def->isObject("parameters"));
			assertTrue(def->getObject("parameters")->isObject("properties"));
		}
	}
	assertTrue(foundEcho);

	// Full bridge: registry -> executor -> HTTPClient -> server -> back.
	Object input;
	input.set("message", "hi");
	assertEqual(std::string("hi"), registry.executeTool("echo", input));

	assertEqual(std::string("test"), host.toolOwners().at("echo"));

	http.stop();
}


void MCPHostTest::testToolErrorBecomesErrorString()
{
	Poco::AI::MCP::Server server;
	buildServer(server);
	TestHTTPHost http(server);
	http.start();

	ToolRegistry registry;
	MCPHost host;
	host.addServer("test", endpoint(http.port()), "test-client", "1.0");
	host.registerInto(registry);

	// A throwing remote tool comes back as an isError result, flattened to a
	// JSON error string, not as a thrown exception.
	Object empty;
	const std::string result = registry.executeTool("boom", empty);
	assertTrue(result.find("error") != std::string::npos);

	http.stop();
}


void MCPHostTest::testCollisionSkipped()
{
	Poco::AI::MCP::Server server;
	buildServer(server);
	TestHTTPHost http(server);
	http.start();

	ToolRegistry registry;
	// A local tool already owns the name "echo".
	Object localDef;
	localDef.set("name", "echo");
	localDef.set("description", "local echo");
	registry.registerTool(localDef, [](const Object&) -> std::string { return "local"; });

	MCPHost host;
	host.addServer("test", endpoint(http.port()), "test-client", "1.0");
	std::vector<std::string> shadowed;
	std::size_t registered = host.registerInto(registry, &shadowed);

	// echo is shadowed and reported; only boom is added.
	assertEqual(1, static_cast<int>(registered));
	assertEqual(1, static_cast<int>(shadowed.size()));
	assertEqual(std::string("echo"), shadowed[0]);
	assertTrue(registry.hasTool("boom"));
	assertEqual(std::string("local"), registry.executeTool("echo", Object()));
	assertTrue(host.toolOwners().find("echo") == host.toolOwners().end());
	assertEqual(std::string("test"), host.toolOwners().at("boom"));

	http.stop();
}


void MCPHostTest::testDeadServerThrows()
{
	// Grab a free port, then nothing listens on it.
	Poco::Net::ServerSocket probe(0);
	Poco::UInt16 deadPort = probe.address().port();
	probe.close();

	MCPHost host;
	try
	{
		host.addServer("dead", endpoint(deadPort), "test-client", "1.0");
		fail("a server that cannot be reached must throw");
	}
	catch (Poco::Exception&)
	{
	}
	// The failed server left no trace.
	assertEqual(0, static_cast<int>(host.serverCount()));
}


void MCPHostTest::setUp()
{
}


void MCPHostTest::tearDown()
{
}


CppUnit::Test* MCPHostTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MCPHostTest");

	CppUnit_addTest(pSuite, MCPHostTest, testRegisterAndCall);
	CppUnit_addTest(pSuite, MCPHostTest, testToolErrorBecomesErrorString);
	CppUnit_addTest(pSuite, MCPHostTest, testCollisionSkipped);
	CppUnit_addTest(pSuite, MCPHostTest, testDeadServerThrows);

	return pSuite;
}
