//
// StreamClientTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StreamClientTest.h"
#include "MCPTestHelpers.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/StreamClient.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>
#include <vector>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::StreamClient;
using Poco::AI::MCP::MCPException;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Tool;
using Poco::AI::MCP::Implementation;
using Poco::AI::MCP::ErrorCode;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	std::vector<Object::Ptr> sent(const std::ostringstream& out)
		/// The requests the client wrote.
	{
		std::vector<Object::Ptr> requests;
		std::istringstream lines(out.str());
		std::string line;
		while (std::getline(lines, line)) requests.push_back(parse(line));
		return requests;
	}


	std::string reply(int id, const std::string& result)
	{
		return "{\"jsonrpc\":\"2.0\",\"id\":" + std::to_string(id) + ",\"result\":" + result + "}\n";
	}


	const std::string TOOLS_PAGE = "{\"resultType\":\"complete\",\"tools\":[{\"name\":\"a\",\"inputSchema\":{\"type\":\"object\"}}]}";
}


StreamClientTest::StreamClientTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


StreamClientTest::~StreamClientTest()
{
}


void StreamClientTest::testRoundTrip()
{
	Server server;
	Implementation info;
	info.name = "pipe-server";
	info.version = "1";
	server.setServerInfo(info);
	server.registerTool("echo", "Echo.", nullptr, [](const Object::Ptr& pArgs)
	{
		ToolResult toolResult;
		toolResult.addText(pArgs->optValue<std::string>("message", ""));
		return toolResult;
	});

	Poco::Pipe toServer;
	Poco::Pipe toClient;
	Poco::Thread thread;
	thread.startFunc([&server, toServer, toClient]()
	{
		Poco::PipeInputStream in(toServer);
		Poco::PipeOutputStream out(toClient);
		try
		{
			server.serve(in, out);
		}
		catch (const Poco::Exception&)
		{
		}
	});

	{
		Poco::PipeOutputStream clientOut(toServer);
		Poco::PipeInputStream clientIn(toClient);
		StreamClient client(clientIn, clientOut);
		assertEqual(std::string("pipe-server"), client.discover().serverInfo.name);
		std::vector<Tool> tools = client.listTools();
		assertEqual(1u, static_cast<unsigned>(tools.size()));
		assertEqual(std::string("echo"), tools[0].name);
		Object::Ptr pArgs = new Object;
		pArgs->set("message", "over a pipe");
		ToolResult toolResult = client.callTool("echo", pArgs);
		assertEqual(std::string("over a pipe"), toolResult.content()->getObject(0)->getValue<std::string>("text"));

		// An error reply becomes an MCPException with the error's code.
		try
		{
			(void) client.callTool("missing");
			fail("an unknown tool must throw");
		}
		catch (const MCPException& exc)
		{
			assertEqual(static_cast<int>(ErrorCode::INVALID_PARAMS), exc.code());
		}
	}
	toServer.close(Poco::Pipe::CLOSE_WRITE);
	thread.join();
}


void StreamClientTest::testRequestCarriesMeta()
{
	std::istringstream in(reply(1, TOOLS_PAGE));
	std::ostringstream out;
	StreamClient client(in, out);
	Implementation info;
	info.name = "meta-client";
	info.version = "9";
	client.setClientInfo(info);
	(void) client.listTools();

	std::vector<Object::Ptr> requests = sent(out);
	assertEqual(1u, static_cast<unsigned>(requests.size()));
	assertEqual(std::string("tools/list"), requests[0]->getValue<std::string>("method"));
	Object::Ptr pMeta = requests[0]->getObject("params")->getObject("_meta");
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), pMeta->getValue<std::string>(Poco::AI::MCP::META_PROTOCOL_VERSION));
	assertTrue(pMeta->isObject(Poco::AI::MCP::META_CLIENT_CAPABILITIES));
	assertEqual(std::string("meta-client"), pMeta->getObject(Poco::AI::MCP::META_CLIENT_INFO)->getValue<std::string>("name"));
	assertTrue(!requests[0]->getObject("params")->has("cursor"));
}


void StreamClientTest::testPassesOverOtherMessages()
{
	// Before its reply come a notification, a request, the reply to another
	// id, a line that is not JSON and a JSON line that is not an object.
	std::istringstream in(
		std::string("{\"jsonrpc\":\"2.0\",\"method\":\"notifications/message\",\"params\":{\"level\":\"info\",\"data\":\"x\"}}\n")
		+ "{\"jsonrpc\":\"2.0\",\"id\":77,\"method\":\"ping\"}\n"
		+ reply(99, "{\"resultType\":\"complete\",\"tools\":[]}")
		+ "log output that went to stdout\n"
		+ "[1,2,3]\n"
		+ reply(1, TOOLS_PAGE));
	std::ostringstream out;
	StreamClient client(in, out);
	std::vector<Tool> tools = client.listTools();
	assertEqual(1u, static_cast<unsigned>(tools.size()));
	assertEqual(std::string("a"), tools[0].name);
	// The client answered nothing: it wrote only its request.
	assertEqual(1u, static_cast<unsigned>(sent(out).size()));
}


void StreamClientTest::testErrorReply()
{
	std::istringstream in("{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":{\"code\":-32602,\"message\":\"Unknown tool: x\",\"data\":{\"name\":\"x\"}}}\n");
	std::ostringstream out;
	StreamClient client(in, out);
	try
	{
		(void) client.callTool("x");
		fail("an error reply must throw");
	}
	catch (const MCPException& exc)
	{
		assertEqual(-32602, exc.code());
		assertTrue(exc.message().find("Unknown tool: x") != std::string::npos);
	}
}


void StreamClientTest::testMalformedReplies()
{
	const char* replies[] = {
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":\"boom\"}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":{\"code\":\"x\",\"message\":\"m\"}}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":5}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":null}\n"
	};
	for (const char* text: replies)
	{
		std::istringstream in(text);
		std::ostringstream out;
		StreamClient client(in, out);
		try
		{
			(void) client.listTools();
			fail(std::string("a malformed reply must throw: ") + text);
		}
		catch (const Poco::ProtocolException&)
		{
		}
	}

	// A tool result without content.
	std::istringstream in(reply(1, "{\"resultType\":\"complete\"}"));
	std::ostringstream out;
	StreamClient client(in, out);
	try
	{
		(void) client.callTool("t");
		fail("a tool result without content must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void StreamClientTest::testResultTypeRefused()
{
	std::istringstream in(reply(1, "{\"resultType\":\"input_required\",\"inputRequests\":{}}"));
	std::ostringstream out;
	StreamClient client(in, out);
	try
	{
		(void) client.callTool("t");
		fail("a result type other than complete must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}

	// A result without a result type counts as complete.
	std::istringstream legacy(reply(1, "{\"content\":[]}"));
	std::ostringstream legacyOut;
	StreamClient legacyClient(legacy, legacyOut);
	assertTrue(!legacyClient.callTool("t").isError());
}


void StreamClientTest::testEndOfStream()
{
	std::istringstream in("{\"jsonrpc\":\"2.0\",\"method\":\"notifications/progress\"}\n");
	std::ostringstream out;
	StreamClient client(in, out);
	try
	{
		(void) client.listTools();
		fail("the end of the stream before the reply must throw");
	}
	catch (const Poco::IOException&)
	{
	}
}


void StreamClientTest::testPagination()
{
	// The empty string is a cursor too; only a missing nextCursor ends the list.
	std::istringstream in(
		reply(1, "{\"tools\":[{\"name\":\"a\",\"inputSchema\":{}}],\"nextCursor\":\"\"}")
		+ reply(2, "{\"tools\":[{\"name\":\"b\",\"inputSchema\":{}},{\"name\":\"\",\"inputSchema\":{}},{\"name\":\"c\"}],\"nextCursor\":\"page3\"}")
		+ reply(3, "{\"tools\":[{\"name\":\"d\",\"inputSchema\":{\"type\":\"object\"}}]}"));
	std::ostringstream out;
	StreamClient client(in, out);
	std::vector<Tool> tools = client.listTools();
	// Entries without a name or an input schema are left out.
	assertEqual(3u, static_cast<unsigned>(tools.size()));
	assertEqual(std::string("a"), tools[0].name);
	assertEqual(std::string("b"), tools[1].name);
	assertEqual(std::string("d"), tools[2].name);

	std::vector<Object::Ptr> requests = sent(out);
	assertEqual(3u, static_cast<unsigned>(requests.size()));
	assertTrue(!requests[0]->getObject("params")->has("cursor"));
	assertEqual(std::string(""), requests[1]->getObject("params")->getValue<std::string>("cursor"));
	assertEqual(std::string("page3"), requests[2]->getObject("params")->getValue<std::string>("cursor"));
}


void StreamClientTest::testPageLimit()
{
	std::string replies;
	for (int id = 1; id <= 3; ++id) replies += reply(id, "{\"tools\":[],\"nextCursor\":\"again\"}");
	std::istringstream in(replies);
	std::ostringstream out;
	StreamClient client(in, out);
	client.setMaxPages(2);
	try
	{
		(void) client.listTools();
		fail("a list beyond the page limit must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}
	assertEqual(2u, static_cast<unsigned>(sent(out).size()));
}


void StreamClientTest::testDiscoverChecksVersion()
{
	std::istringstream in(reply(1, "{\"resultType\":\"complete\",\"supportedVersions\":[\"2025-11-25\"],\"capabilities\":{\"tools\":{}},\"ttlMs\":0,\"cacheScope\":\"public\"}"));
	std::ostringstream out;
	StreamClient client(in, out);
	try
	{
		(void) client.discover();
		fail("a server without this client's revision must throw");
	}
	catch (const MCPException& exc)
	{
		assertEqual(static_cast<int>(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), exc.code());
		assertTrue(exc.displayText().find("2025-11-25") != std::string::npos);
	}
}


void StreamClientTest::setUp()
{
}


void StreamClientTest::tearDown()
{
}


CppUnit::Test* StreamClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StreamClientTest");

	CppUnit_addTest(pSuite, StreamClientTest, testRoundTrip);
	CppUnit_addTest(pSuite, StreamClientTest, testRequestCarriesMeta);
	CppUnit_addTest(pSuite, StreamClientTest, testPassesOverOtherMessages);
	CppUnit_addTest(pSuite, StreamClientTest, testErrorReply);
	CppUnit_addTest(pSuite, StreamClientTest, testMalformedReplies);
	CppUnit_addTest(pSuite, StreamClientTest, testResultTypeRefused);
	CppUnit_addTest(pSuite, StreamClientTest, testEndOfStream);
	CppUnit_addTest(pSuite, StreamClientTest, testPagination);
	CppUnit_addTest(pSuite, StreamClientTest, testPageLimit);
	CppUnit_addTest(pSuite, StreamClientTest, testDiscoverChecksVersion);

	return pSuite;
}
