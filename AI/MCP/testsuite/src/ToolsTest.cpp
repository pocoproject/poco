//
// ToolsTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ToolsTest.h"
#include "MCPTestHelpers.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include <atomic>
#include <string>
#include <algorithm>
#include <vector>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::Tool;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::ResourceContents;
using Poco::AI::MCP::RequestContext;
using Poco::AI::MCP::ErrorCode;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;
using Poco::JSON::Array;
using MCPTest::request;
using MCPTest::errorCode;
using MCPTest::result;
using MCPTest::code;


namespace
{
	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	void buildServer(Server& server)
	{
		server.registerTool("echo", "Echo back the message.", parse("{\"type\":\"object\",\"properties\":{\"message\":{\"type\":\"string\"}}}"),
			[](const Object::Ptr& pArgs)
			{
				ToolResult toolResult;
				toolResult.addText(pArgs->optValue<std::string>("message", ""));
				return toolResult;
			});

		server.registerTool("boom", "Always throws.", nullptr,
			[](const Object::Ptr&) -> ToolResult
			{
				throw Poco::RuntimeException("kaboom");
			});
	}


	Object::Ptr callTool(Server& server, int id, const Poco::Dynamic::Var& name, const Poco::Dynamic::Var& arguments = Poco::Dynamic::Var())
	{
		Object::Ptr pParams = new Object;
		pParams->set("name", name);
		if (!arguments.isEmpty()) pParams->set("arguments", arguments);
		return server.handle(request(Id(id), "tools/call", pParams));
	}


	std::string firstText(const Object::Ptr& pResult)
	{
		return pResult->getArray("content")->getObject(0)->getValue<std::string>("text");
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
	Tool tool;
	tool.name = "annotated";
	tool.title = "Annotated tool";
	tool.description = "Has every optional member.";
	tool.inputSchema = parse("{\"type\":\"object\",\"additionalProperties\":false}");
	tool.outputSchema = parse("{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"integer\"}}}");
	tool.annotations = parse("{\"readOnlyHint\":true}");
	tool.handler = [](const Object::Ptr&, const RequestContext&) { return ToolResult(); };
	server.registerTool(tool);

	Array::Ptr pTools = result(server.handle(request(Id(1), "tools/list")))->getArray("tools");
	assertEqual(3u, static_cast<unsigned>(pTools->size()));
	// In name order.
	assertEqual(std::string("annotated"), pTools->getObject(0)->getValue<std::string>("name"));
	assertEqual(std::string("boom"), pTools->getObject(1)->getValue<std::string>("name"));
	assertEqual(std::string("echo"), pTools->getObject(2)->getValue<std::string>("name"));

	Object::Ptr pAnnotated = pTools->getObject(0);
	assertEqual(std::string("Annotated tool"), pAnnotated->getValue<std::string>("title"));
	assertTrue(pAnnotated->getObject("outputSchema")->isObject("properties"));
	assertTrue(pAnnotated->getObject("annotations")->getValue<bool>("readOnlyHint"));

	// A tool registered without a schema takes no particular arguments.
	Object::Ptr pBoomSchema = pTools->getObject(1)->getObject("inputSchema");
	assertEqual(std::string("object"), pBoomSchema->getValue<std::string>("type"));
	assertTrue(!pTools->getObject(1)->has("outputSchema"));
	assertTrue(!pTools->getObject(1)->has("title"));
}


void ToolsTest::testSchemasAreCopied()
{
	Server server;
	Object::Ptr pSchema = parse("{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\"}}}");
	server.registerTool("t", "", pSchema, [](const Object::Ptr&) { return ToolResult(); });

	Object::Ptr pListed = result(server.handle(request(Id(1), "tools/list")))->getArray("tools")->getObject(0)->getObject("inputSchema");
	assertTrue(pListed.get() != pSchema.get());
	assertTrue(pListed->getObject("properties").get() != pSchema->getObject("properties").get());
	assertEqual(std::string("string"), pListed->getObject("properties")->getObject("a")->getValue<std::string>("type"));
}


void ToolsTest::testConcurrentReplies()
{
	// Replies serialized on several threads at once share no JSON object, so
	// each serialization equals the one made alone. A shared schema object
	// rarely shows here as a wrong reply: the data race it causes is what
	// ThreadSanitizer reports, so the test catches it only in a TSan build.
	Server server;
	Object::Ptr pSchema = parse("{\"type\":\"object\",\"properties\":{\"text\":{\"type\":\"string\",\"description\":\"caf\\u00e9 \\u20ac \\u65e5\"}}}");
	server.registerTool("unicode", "Unicode in the schema.", pSchema, [](const Object::Ptr&) { return ToolResult(); });
	const std::string expected = Poco::AI::MCP::serialize(server.handle(request(Id(1), "tools/list")));

	std::atomic<int> mismatches(0);
	std::vector<std::unique_ptr<Poco::Thread>> threads;
	for (int t = 0; t < 4; ++t)
	{
		threads.push_back(std::make_unique<Poco::Thread>());
		threads.back()->startFunc([&server, &expected, &mismatches]()
		{
			for (int i = 0; i < 50; ++i)
			{
				if (Poco::AI::MCP::serialize(server.handle(request(Id(1), "tools/list"))) != expected) ++mismatches;
			}
		});
	}
	for (auto& pThread: threads) pThread->join();
	assertEqual(0, mismatches.load());
}


void ToolsTest::testToolsCall()
{
	Server server;
	buildServer(server);
	Object::Ptr pArgs = new Object;
	pArgs->set("message", "hello");
	Object::Ptr pReply = callTool(server, 2, std::string("echo"), pArgs);
	Object::Ptr pResult = result(pReply);
	assertEqual(2, pReply->getValue<int>("id"));
	assertTrue(!pResult->getValue<bool>("isError"));
	assertEqual(std::string("hello"), firstText(pResult));

	// Arguments may be left out.
	pResult = result(callTool(server, 3, std::string("echo")));
	assertEqual(std::string(""), firstText(pResult));
}


void ToolsTest::testCallErrors()
{
	Server server;
	buildServer(server);

	// Unknown tool.
	Object::Ptr pReply = callTool(server, 1, std::string("nope"));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(pReply));
	assertEqual(1, pReply->getValue<int>("id"));

	// A name that is not a string, null included.
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(callTool(server, 2, 42)));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(MCPTest::request(Id(3), "tools/call", parse("{\"name\":null}")))));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(MCPTest::request(Id(4), "tools/call"))));

	// Arguments that are not an object.
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(callTool(server, 5, std::string("echo"), std::string("not an object"))));
	pReply = server.handle(MCPTest::request(Id(6), "tools/call", parse("{\"name\":\"echo\",\"arguments\":[1,2]}")));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(pReply));
	assertEqual(6, pReply->getValue<int>("id"));
}


void ToolsTest::testHandlerThrowIsToolError()
{
	Server server;
	buildServer(server);
	Object::Ptr pResult = result(callTool(server, 1, std::string("boom")));
	assertTrue(!pResult.isNull());
	assertTrue(pResult->getValue<bool>("isError"));
	assertTrue(firstText(pResult).find("kaboom") != std::string::npos);
}


void ToolsTest::testRegistrationRefused()
{
	Server server;
	try
	{
		server.registerTool("", "", nullptr, [](const Object::Ptr&) { return ToolResult(); });
		fail("a tool without a name must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	try
	{
		server.registerTool("x", "", nullptr, Poco::AI::MCP::ToolHandler());
		fail("a tool without a handler must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	Tool tool;
	tool.name = "no-handler";
	try
	{
		server.registerTool(tool);
		fail("a tool without a handler must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}

	// An input schema must describe an object.
	const char* schemas[] = {"{}", "{\"type\":\"string\"}", "{\"type\":[\"object\",\"null\"]}"};
	for (const char* schema: schemas)
	{
		try
		{
			server.registerTool("bad-schema", "", parse(schema), [](const Object::Ptr&) { return ToolResult(); });
			fail(std::string("an input schema that is not of type object must be refused: ") + schema);
		}
		catch (const Poco::InvalidArgumentException&)
		{
		}
	}

	// So must its x-mcp-header annotations, which clients would leave out.
	const char* annotated[] = {
		"{\"type\":\"object\",\"x-mcp-header\":\"Root\"}",
		"{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"\"}}}",
		"{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"Not a token\"}}}",
		"{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"number\",\"x-mcp-header\":\"A\"}}}",
		"{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\",\"x-mcp-header\":\"Same\"},\"b\":{\"type\":\"string\",\"x-mcp-header\":\"SAME\"}}}",
		"{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"array\",\"items\":{\"type\":\"string\",\"x-mcp-header\":\"Item\"}}}}",
		"{\"type\":\"object\",\"properties\":{\"a\":{\"oneOf\":[{\"type\":\"string\",\"x-mcp-header\":\"One\"}]}}}"
	};
	for (const char* schema: annotated)
	{
		try
		{
			server.registerTool("bad-annotation", "", parse(schema), [](const Object::Ptr&) { return ToolResult(); });
			fail(std::string("an x-mcp-header annotation that is not valid must be refused: ") + schema);
		}
		catch (const Poco::InvalidArgumentException&)
		{
		}
	}
	assertEqual(0u, static_cast<unsigned>(result(server.handle(request(Id(1), "tools/list")))->getArray("tools")->size()));
}


void ToolsTest::testHeaderAnnotations()
{
	Tool tool;
	tool.name = "route";
	assertTrue(tool.headerAnnotations().empty());
	tool.inputSchema = parse(
		"{\"type\":\"object\",\"properties\":{"
		"\"region\":{\"type\":\"string\",\"x-mcp-header\":\"Region\"},"
		"\"plain\":{\"type\":\"string\"},"
		"\"nested\":{\"type\":\"object\",\"properties\":{\"count\":{\"type\":\"integer\",\"x-mcp-header\":\"Count\"}}}}}");
	const std::vector<Tool::HeaderAnnotation> annotations = tool.headerAnnotations();
	assertEqual(2u, static_cast<unsigned>(annotations.size()));
	const auto named = [&annotations](const std::string& name)
	{
		const auto it = std::find_if(annotations.begin(), annotations.end(), [&name](const Tool::HeaderAnnotation& annotation) { return annotation.name == name; });
		return it == annotations.end() ? Tool::HeaderAnnotation() : *it;
	};
	assertTrue(named("Region").path == std::vector<std::string>({"region"}));
	assertEqual(std::string("string"), named("Region").type);
	assertTrue(named("Count").path == std::vector<std::string>({"nested", "count"}));
	assertEqual(std::string("integer"), named("Count").type);

	tool.inputSchema = parse("{\"type\":\"object\",\"properties\":{\"n\":{\"type\":\"number\",\"x-mcp-header\":\"N\"}}}");
	try
	{
		(void) tool.headerAnnotations();
		fail("an annotation on a number is not valid");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void ToolsTest::testToolResultForms()
{
	ResourceContents contents;
	contents.uri = "test://doc";
	contents.mimeType = "text/plain";
	contents.text = "embedded";
	ToolResult toolResult;
	toolResult.addText("t").addImage("aW1n", "image/png").addAudio("YXVk", "audio/wav").addResourceLink("test://other", "other").addResource(contents);
	toolResult.setStructuredContent(Poco::Dynamic::Var(42));

	Object::Ptr pObject = toolResult.toObject();
	Array::Ptr pContent = pObject->getArray("content");
	assertEqual(5u, static_cast<unsigned>(pContent->size()));
	assertEqual(std::string("text"), pContent->getObject(0)->getValue<std::string>("type"));
	assertEqual(std::string("image"), pContent->getObject(1)->getValue<std::string>("type"));
	assertEqual(std::string("image/png"), pContent->getObject(1)->getValue<std::string>("mimeType"));
	assertEqual(std::string("audio"), pContent->getObject(2)->getValue<std::string>("type"));
	assertEqual(std::string("resource_link"), pContent->getObject(3)->getValue<std::string>("type"));
	assertEqual(std::string("resource"), pContent->getObject(4)->getValue<std::string>("type"));
	assertEqual(std::string("embedded"), pContent->getObject(4)->getObject("resource")->getValue<std::string>("text"));
	// Structured content may be any JSON value.
	assertEqual(42, pObject->getValue<int>("structuredContent"));
	assertTrue(!pObject->getValue<bool>("isError"));

	// A binary resource of zero bytes stays binary.
	ResourceContents empty = ResourceContents::fromObject(parse("{\"uri\":\"test://empty\",\"blob\":\"\"}"));
	assertTrue(empty.binary);
	Object::Ptr pEmpty = empty.toObject();
	assertEqual(std::string(""), pEmpty->getValue<std::string>("blob"));
	assertTrue(!pEmpty->has("text"));
	ResourceContents zeroBytes;
	zeroBytes.uri = "test://zero";
	zeroBytes.binary = true;
	assertTrue(zeroBytes.toObject()->has("blob"));
	ResourceContents textual = ResourceContents::fromObject(parse("{\"uri\":\"test://text\",\"text\":\"\"}"));
	assertTrue(!textual.binary);
	assertTrue(textual.toObject()->has("text"));

	ToolResult failed = ToolResult::error("went wrong");
	assertTrue(failed.isError());
	assertEqual(std::string("went wrong"), failed.content()->getObject(0)->getValue<std::string>("text"));

	try
	{
		toolResult.addContent(parse("{\"text\":\"no type\"}"));
		fail("a block without a type must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
}


void ToolsTest::testToolResultFromObject()
{
	ToolResult structuredOnly = ToolResult::fromObject(parse("{\"content\":[],\"structuredContent\":{\"n\":1}}"));
	assertTrue(!structuredOnly.isError());
	assertEqual(0u, static_cast<unsigned>(structuredOnly.content()->size()));
	assertEqual(1, structuredOnly.structuredContent().extract<Object::Ptr>()->getValue<int>("n"));

	ToolResult arrayContent = ToolResult::fromObject(parse("{\"content\":[{\"type\":\"text\",\"text\":\"a\"}],\"structuredContent\":[1,2],\"isError\":true}"));
	assertTrue(arrayContent.isError());
	assertEqual(2u, static_cast<unsigned>(arrayContent.structuredContent().extract<Array::Ptr>()->size()));

	// Null is structured content too, and stays; absence stays absence.
	ToolResult nullContent = ToolResult::fromObject(parse("{\"content\":[],\"structuredContent\":null}"));
	assertTrue(nullContent.hasStructuredContent());
	assertTrue(nullContent.structuredContent().isEmpty());
	Object::Ptr pNull = nullContent.toObject();
	assertTrue(pNull->has("structuredContent"));
	assertTrue(pNull->isNull("structuredContent"));
	ToolResult none = ToolResult::fromObject(parse("{\"content\":[]}"));
	assertTrue(!none.hasStructuredContent());
	assertTrue(!none.toObject()->has("structuredContent"));
	ToolResult built;
	built.setStructuredContent(Poco::Dynamic::Var());
	assertTrue(built.toObject()->isNull("structuredContent"));
	built.removeStructuredContent();
	assertTrue(!built.hasStructuredContent());
	assertTrue(!built.toObject()->has("structuredContent"));

	const char* malformed[] = {
		"{}",
		"{\"content\":\"text\"}",
		"{\"content\":[1]}",
		"{\"content\":[{\"text\":\"no type\"}]}",
		"{\"content\":[],\"isError\":\"yes\"}"
	};
	for (const char* text: malformed)
	{
		try
		{
			(void) ToolResult::fromObject(parse(text));
			fail(std::string("must be refused: ") + text);
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}
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
	CppUnit_addTest(pSuite, ToolsTest, testSchemasAreCopied);
	CppUnit_addTest(pSuite, ToolsTest, testConcurrentReplies);
	CppUnit_addTest(pSuite, ToolsTest, testToolsCall);
	CppUnit_addTest(pSuite, ToolsTest, testCallErrors);
	CppUnit_addTest(pSuite, ToolsTest, testHandlerThrowIsToolError);
	CppUnit_addTest(pSuite, ToolsTest, testRegistrationRefused);
	CppUnit_addTest(pSuite, ToolsTest, testHeaderAnnotations);
	CppUnit_addTest(pSuite, ToolsTest, testToolResultForms);
	CppUnit_addTest(pSuite, ToolsTest, testToolResultFromObject);

	return pSuite;
}
