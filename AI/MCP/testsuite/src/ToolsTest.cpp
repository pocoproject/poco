//
// ToolsTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ToolsTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	void buildServer(Server& server)
	{
		Object::Ptr echoSchema = new Object;
		echoSchema->set("type", "object");
		server.registerTool("echo", "Echo back the message.", echoSchema,
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

		// Drive the handshake so tools/* are accepted.
		Object::Ptr params = new Object;
		params->set("protocolVersion", Poco::AI::MCP::PROTOCOL_VERSION);
		params->set("capabilities", Object::Ptr(new Object));
		server.handleMessage(Request::make(Id(1), "initialize", params));
	}

	Object::Ptr callTool(Server& server, int id, const std::string& name, Object::Ptr arguments)
	{
		Object::Ptr params = new Object;
		params->set("name", name);
		if (arguments)
		{
			params->set("arguments", arguments);
		}
		return server.handleMessage(Request::make(Id(id), "tools/call", params));
	}
}


ToolsTest::ToolsTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ToolsTest::~ToolsTest()
{
}


void ToolsTest::testToolsList()
{
	Server server;
	buildServer(server);

	Object::Ptr resp = server.handleMessage(Request::make(Id(2), "tools/list", nullptr));
	assertTrue(resp->isObject("result"));

	Array::Ptr tools = resp->getObject("result")->getArray("tools");
	assertEqual(2, static_cast<int>(tools->size()));

	bool foundEcho = false;
	for (unsigned int i = 0; i < tools->size(); ++i)
	{
		Object::Ptr tool = tools->getObject(i);
		assertTrue(tool->has("inputSchema"));
		if (tool->getValue<std::string>("name") == "echo")
		{
			foundEcho = true;
		}
	}
	assertTrue(foundEcho);
}


void ToolsTest::testToolsCall()
{
	Server server;
	buildServer(server);

	Object::Ptr args = new Object;
	args->set("message", "hi");
	Object::Ptr resp = callTool(server, 2, "echo", args);

	assertTrue(resp->isObject("result"));
	Object::Ptr result = resp->getObject("result");
	assertTrue(!result->getValue<bool>("isError"));

	Array::Ptr content = result->getArray("content");
	assertEqual(1, static_cast<int>(content->size()));
	Object::Ptr block = content->getObject(0);
	assertEqual(std::string("text"), block->getValue<std::string>("type"));
	assertEqual(std::string("hi"), block->getValue<std::string>("text"));
}


void ToolsTest::testUnknownTool()
{
	Server server;
	buildServer(server);

	Object::Ptr resp = callTool(server, 2, "does-not-exist", new Object);
	assertTrue(resp->isObject("error"));
	assertEqual(-32602, resp->getObject("error")->getValue<int>("code"));
}


void ToolsTest::testBadArguments()
{
	Server server;
	buildServer(server);

	// arguments present but not an object -> InvalidParams.
	Object::Ptr params = new Object;
	params->set("name", "echo");
	params->set("arguments", "not-an-object");
	Object::Ptr resp = server.handleMessage(Request::make(Id(2), "tools/call", params));

	assertTrue(resp->isObject("error"));
	assertEqual(-32602, resp->getObject("error")->getValue<int>("code"));
}


void ToolsTest::testHandlerThrowIsToolError()
{
	Server server;
	buildServer(server);

	Object::Ptr resp = callTool(server, 2, "boom", new Object);

	// A throwing handler is reported as a tool result with isError, NOT a
	// JSON-RPC error.
	assertTrue(resp->isObject("result"));
	assertTrue(!resp->has("error"));
	assertTrue(resp->getObject("result")->getValue<bool>("isError"));
}


void ToolsTest::setUp()
{
}


void ToolsTest::tearDown()
{
}


CppUnit::Test* ToolsTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ToolsTest");

	CppUnit_addTest(pSuite, ToolsTest, testToolsList);
	CppUnit_addTest(pSuite, ToolsTest, testToolsCall);
	CppUnit_addTest(pSuite, ToolsTest, testUnknownTool);
	CppUnit_addTest(pSuite, ToolsTest, testBadArguments);
	CppUnit_addTest(pSuite, ToolsTest, testHandlerThrowIsToolError);

	return pSuite;
}
