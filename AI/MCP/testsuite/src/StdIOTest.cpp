//
// StdIOTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StdIOTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/StdIOServer.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include <sstream>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::StdIOServer;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;


StdIOTest::StdIOTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


StdIOTest::~StdIOTest()
{
}


void StdIOTest::testStdoutIsJsonOnly()
{
	Server server;
	server.registerTool("echo", "Echo.", new Object,
		[](Object::Ptr args)
		{
			ToolResult result;
			result.addText(args && args->has("message") ? args->getValue<std::string>("message") : std::string());
			return result;
		});

	Object::Ptr initParams = new Object;
	initParams->set("protocolVersion", Poco::AI::MCP::PROTOCOL_VERSION);
	initParams->set("capabilities", Object::Ptr(new Object));

	Object::Ptr callParams = new Object;
	callParams->set("name", "echo");
	Object::Ptr args = new Object;
	args->set("message", "hi");
	callParams->set("arguments", args);

	std::ostringstream in;
	in << Poco::AI::MCP::serialize(Request::make(Id(1), "initialize", initParams)) << "\n";
	in << Poco::AI::MCP::serialize(Request::make(Id(2), "tools/call", callParams)) << "\n";

	std::istringstream input(in.str());
	std::ostringstream output;

	StdIOServer transport(server);
	transport.run(input, output);

	// Every non-blank line on stdout must be a valid JSON object - nothing else.
	std::istringstream rs(output.str());
	std::string line;
	int messages = 0;
	while (std::getline(rs, line))
	{
		if (line.empty())
		{
			continue;
		}
		Poco::JSON::Parser parser;
		Object::Ptr obj = parser.parse(line).extract<Object::Ptr>();
		assertEqual(std::string("2.0"), obj->getValue<std::string>("jsonrpc"));
		++messages;
	}
	assertEqual(2, messages);
}


void StdIOTest::setUp()
{
}


void StdIOTest::tearDown()
{
}


CppUnit::Test* StdIOTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StdIOTest");

	CppUnit_addTest(pSuite, StdIOTest, testStdoutIsJsonOnly);

	return pSuite;
}
