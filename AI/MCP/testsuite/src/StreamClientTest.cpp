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
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":{\"code\":9999999999,\"message\":\"m\"}}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":{\"code\":-32602}}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":5}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":null}\n",
		// The reply to the request, with neither a result nor an error: refused
		// at once, not passed over until the stream ends.
		"{\"jsonrpc\":\"2.0\",\"id\":1}\n{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{\"tools\":[]}}\n"
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
	const char* refused[] = {
		// A result type the client does not know.
		"{\"resultType\":\"elsewhere\",\"content\":[]}",
		// Input the client does not declare it can give.
		"{\"resultType\":\"input_required\",\"inputRequests\":{\"login\":{\"method\":\"elicitation/create\",\"params\":{}}}}",
		// A request state that is not a string.
		"{\"resultType\":\"input_required\",\"requestState\":7}"
	};
	for (const char* result: refused)
	{
		std::istringstream in(reply(1, result));
		std::ostringstream out;
		StreamClient client(in, out);
		try
		{
			(void) client.callTool("t");
			fail(std::string("a result that cannot be completed must throw: ") + result);
		}
		catch (const Poco::ProtocolException&)
		{
		}
		assertEqual(1u, static_cast<unsigned>(sent(out).size()));
	}

	// input_required is no answer to tools/list.
	std::istringstream list(reply(1, "{\"resultType\":\"input_required\",\"requestState\":\"s\"}"));
	std::ostringstream listOut;
	StreamClient listClient(list, listOut);
	try
	{
		(void) listClient.listTools();
		fail("input_required to tools/list must throw");
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


void StreamClientTest::testInputRequiredIsSentAgain()
{
	// A server under load answers input_required with only a state to echo,
	// then without one; the call is sent again each time, with a new id.
	std::istringstream in(
		reply(1, "{\"resultType\":\"input_required\",\"requestState\":\"opaque \\u00e9 state\"}")
		+ reply(2, "{\"resultType\":\"input_required\",\"inputRequests\":{}}")
		+ reply(3, "{\"resultType\":\"complete\",\"content\":[{\"type\":\"text\",\"text\":\"done\"}]}"));
	std::ostringstream out;
	StreamClient client(in, out);
	Object::Ptr pArgs = new Object;
	pArgs->set("city", "Zagreb");
	const ToolResult toolResult = client.callTool("weather", pArgs);
	assertEqual(std::string("done"), toolResult.content()->getObject(0)->getValue<std::string>("text"));

	std::vector<Object::Ptr> requests = sent(out);
	assertEqual(3u, static_cast<unsigned>(requests.size()));
	for (std::size_t i = 0; i < requests.size(); ++i)
	{
		assertEqual(static_cast<int>(i + 1), requests[i]->getValue<int>("id"));
		assertEqual(std::string("tools/call"), requests[i]->getValue<std::string>("method"));
		const Object::Ptr pParams = requests[i]->getObject("params");
		assertEqual(std::string("weather"), pParams->getValue<std::string>("name"));
		assertEqual(std::string("Zagreb"), pParams->getObject("arguments")->getValue<std::string>("city"));
		assertTrue(pParams->isObject("_meta"));
	}
	assertTrue(!requests[0]->getObject("params")->has("requestState"));
	assertEqual(std::string("opaque \xC3\xA9 state"), requests[1]->getObject("params")->getValue<std::string>("requestState"));
	// The second answer had no state, so the third request carries none.
	assertTrue(!requests[2]->getObject("params")->has("requestState"));
}


void StreamClientTest::testInputRequiredLimit()
{
	std::string replies;
	for (int id = 1; id <= 3; ++id) replies += reply(id, "{\"resultType\":\"input_required\",\"requestState\":\"again\"}");
	std::istringstream in(replies);
	std::ostringstream out;
	StreamClient client(in, out);
	client.setMaxInputRounds(2);
	try
	{
		(void) client.readResource("doc://busy");
		fail("input_required beyond the limit must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}
	assertEqual(2u, static_cast<unsigned>(sent(out).size()));
}


void StreamClientTest::testCallerMeta()
{
	// What the caller puts in _meta is sent along; the protocol members are
	// the client's.
	std::istringstream in(reply(1, "{\"content\":[]}"));
	std::ostringstream out;
	StreamClient client(in, out);
	Object::Ptr pMeta = new Object;
	pMeta->set("progressToken", 7);
	pMeta->set(Poco::AI::MCP::META_PROTOCOL_VERSION, "1999-01-01");
	Object::Ptr pParams = new Object;
	pParams->set("name", "t");
	pParams->set("_meta", pMeta);
	(void) client.call("tools/call", pParams);

	const Object::Ptr pSent = sent(out)[0]->getObject("params")->getObject("_meta");
	assertEqual(7, pSent->getValue<int>("progressToken"));
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), pSent->getValue<std::string>(Poco::AI::MCP::META_PROTOCOL_VERSION));
	assertTrue(pSent->isObject(Poco::AI::MCP::META_CLIENT_CAPABILITIES));
	// The caller's objects are not changed.
	assertEqual(std::string("1999-01-01"), pMeta->getValue<std::string>(Poco::AI::MCP::META_PROTOCOL_VERSION));
	assertTrue(!pMeta->has(Poco::AI::MCP::META_CLIENT_CAPABILITIES));

	pParams->set("_meta", "not an object");
	try
	{
		(void) client.call("tools/call", pParams);
		fail("a _meta that is not an object must throw");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
}


void StreamClientTest::testClientInfo()
{
	// By default the client reports this library; told not to, nothing.
	std::istringstream in(reply(1, TOOLS_PAGE) + reply(2, TOOLS_PAGE));
	std::ostringstream out;
	StreamClient client(in, out);
	(void) client.listTools();
	client.setClientInfo(Implementation());
	(void) client.listTools();

	std::vector<Object::Ptr> requests = sent(out);
	const Object::Ptr pFirst = requests[0]->getObject("params")->getObject("_meta");
	assertEqual(Implementation::library().name, pFirst->getObject(Poco::AI::MCP::META_CLIENT_INFO)->getValue<std::string>("name"));
	assertEqual(Implementation::library().version, pFirst->getObject(Poco::AI::MCP::META_CLIENT_INFO)->getValue<std::string>("version"));
	assertTrue(!requests[1]->getObject("params")->getObject("_meta")->has(Poco::AI::MCP::META_CLIENT_INFO));
}


void StreamClientTest::testResourcesAndPrompts()
{
	std::istringstream in(
		reply(1, "{\"resources\":[{\"uri\":\"doc://a\",\"name\":\"a\",\"mimeType\":\"text/plain\"},{\"name\":\"no uri\"}]}")
		+ reply(2, "{\"contents\":[{\"uri\":\"doc://a\",\"mimeType\":\"text/plain\",\"text\":\"hello\"},{\"uri\":\"doc://a#2\",\"blob\":\"AAEC\"}]}")
		+ reply(3, "{\"prompts\":[{\"name\":\"greet\",\"arguments\":[{\"name\":\"who\",\"required\":true}]}]}")
		+ reply(4, "{\"description\":\"Greets.\",\"messages\":[{\"role\":\"user\",\"content\":{\"type\":\"text\",\"text\":\"Hello, Ana\"}}]}"));
	std::ostringstream out;
	StreamClient client(in, out);

	const std::vector<Poco::AI::MCP::Resource> resources = client.listResources();
	assertEqual(1u, static_cast<unsigned>(resources.size()));
	assertEqual(std::string("doc://a"), resources[0].uri);
	assertEqual(std::string("text/plain"), resources[0].mimeType);

	const std::vector<Poco::AI::MCP::ResourceContents> contents = client.readResource("doc://a");
	assertEqual(2u, static_cast<unsigned>(contents.size()));
	assertEqual(std::string("hello"), contents[0].text);
	assertEqual(std::string("AAEC"), contents[1].blob);

	const std::vector<Poco::AI::MCP::Prompt> prompts = client.listPrompts();
	assertEqual(1u, static_cast<unsigned>(prompts.size()));
	assertEqual(std::string("greet"), prompts[0].name);
	assertEqual(1u, static_cast<unsigned>(prompts[0].arguments.size()));
	assertTrue(prompts[0].arguments[0].required);

	Object::Ptr pArgs = new Object;
	pArgs->set("who", "Ana");
	const Object::Ptr pPrompt = client.getPrompt("greet", pArgs);
	assertEqual(std::string("Hello, Ana"), pPrompt->getArray("messages")->getObject(0)->getObject("content")->getValue<std::string>("text"));

	std::vector<Object::Ptr> requests = sent(out);
	assertEqual(4u, static_cast<unsigned>(requests.size()));
	assertEqual(std::string("resources/list"), requests[0]->getValue<std::string>("method"));
	assertEqual(std::string("resources/read"), requests[1]->getValue<std::string>("method"));
	assertEqual(std::string("doc://a"), requests[1]->getObject("params")->getValue<std::string>("uri"));
	assertEqual(std::string("prompts/list"), requests[2]->getValue<std::string>("method"));
	assertEqual(std::string("prompts/get"), requests[3]->getValue<std::string>("method"));
	assertEqual(std::string("greet"), requests[3]->getObject("params")->getValue<std::string>("name"));
	assertEqual(std::string("Ana"), requests[3]->getObject("params")->getObject("arguments")->getValue<std::string>("who"));

	// Results without their arrays break the protocol.
	const char* broken[] = {"{\"resultType\":\"complete\"}", "{\"contents\":{}}", "{\"messages\":7}"};
	std::istringstream brokenIn(reply(1, broken[0]) + reply(2, broken[1]) + reply(3, broken[2]));
	std::ostringstream brokenOut;
	StreamClient brokenClient(brokenIn, brokenOut);
	try
	{
		(void) brokenClient.listResources();
		fail("a resources/list result without resources must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}
	try
	{
		(void) brokenClient.readResource("doc://a");
		fail("a resources/read result without contents must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}
	try
	{
		(void) brokenClient.getPrompt("greet");
		fail("a prompts/get result without messages must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}
}


void StreamClientTest::testErrorReplyWithoutId()
{
	// The error reply to a message the server could not read has no id, or
	// a null one; either is the reply to the request in flight.
	const char* replies[] = {
		"{\"jsonrpc\":\"2.0\",\"error\":{\"code\":-32700,\"message\":\"Parse error\"}}\n",
		"{\"jsonrpc\":\"2.0\",\"id\":null,\"error\":{\"code\":-32700,\"message\":\"Parse error\"}}\n"
	};
	for (const char* text: replies)
	{
		std::istringstream in(text);
		std::ostringstream out;
		StreamClient client(in, out);
		try
		{
			(void) client.listTools();
			fail(std::string("an error reply without an id must throw: ") + text);
		}
		catch (const MCPException& exc)
		{
			assertEqual(-32700, exc.code());
		}
	}
}


void StreamClientTest::testOversizedReply()
{
	// A message longer than the limit may be the reply: the request fails at
	// once instead of waiting for the end of the stream.
	std::istringstream in(reply(1, "{\"tools\":[],\"padding\":\"" + std::string(200, 'x') + "\"}") + reply(1, TOOLS_PAGE));
	std::ostringstream out;
	StreamClient client(in, out, 128);
	try
	{
		(void) client.listTools();
		fail("an oversized message must throw");
	}
	catch (const Poco::ProtocolException& exc)
	{
		assertTrue(exc.message().find("128") != std::string::npos);
	}
}


void StreamClientTest::testDiscoverMalformed()
{
	const char* results[] = {
		"{\"resultType\":\"complete\",\"capabilities\":{}}",
		"{\"resultType\":\"complete\",\"supportedVersions\":\"2026-07-28\",\"capabilities\":{}}",
		"{\"resultType\":\"complete\",\"supportedVersions\":[\"2026-07-28\",7],\"capabilities\":{}}"
	};
	for (const char* result: results)
	{
		std::istringstream in(reply(1, result));
		std::ostringstream out;
		StreamClient client(in, out);
		try
		{
			(void) client.discover();
			fail(std::string("a malformed discover result must throw: ") + result);
		}
		catch (const Poco::ProtocolException&)
		{
		}
	}
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
	CppUnit_addTest(pSuite, StreamClientTest, testInputRequiredIsSentAgain);
	CppUnit_addTest(pSuite, StreamClientTest, testInputRequiredLimit);
	CppUnit_addTest(pSuite, StreamClientTest, testCallerMeta);
	CppUnit_addTest(pSuite, StreamClientTest, testClientInfo);
	CppUnit_addTest(pSuite, StreamClientTest, testResourcesAndPrompts);
	CppUnit_addTest(pSuite, StreamClientTest, testErrorReplyWithoutId);
	CppUnit_addTest(pSuite, StreamClientTest, testOversizedReply);
	CppUnit_addTest(pSuite, StreamClientTest, testDiscoverMalformed);
	CppUnit_addTest(pSuite, StreamClientTest, testDiscoverChecksVersion);

	return pSuite;
}
