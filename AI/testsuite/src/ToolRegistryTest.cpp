//
// ToolRegistryTest.cpp
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ToolRegistryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include <stdexcept>


using Poco::AI::ToolRegistry;
using Poco::AI::toolResultText;
using Poco::AI::MCP::Tool;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::ResourceContents;
using Poco::AI::MCP::RequestContext;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	std::string firstText(const ToolResult& result)
	{
		return result.content()->getObject(0)->getValue<std::string>("text");
	}
}


ToolRegistryTest::ToolRegistryTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ToolRegistryTest::~ToolRegistryTest()
{
}


void ToolRegistryTest::testRegisterAndExecute()
{
	ToolRegistry registry;
	registry.registerTool("greet", "Greets.", parse("{\"type\":\"object\"}"), [](const Object::Ptr& pInput)
	{
		return "Hello, " + pInput->optValue<std::string>("name", "you");
	});
	assertTrue(registry.hasTool("greet"));
	assertEqual(std::size_t(1), registry.size());
	ToolResult result = registry.executeTool("greet", parse("{\"name\":\"Ann\"}"));
	assertTrue(!result.isError());
	assertEqual(std::string("Hello, Ann"), firstText(result));
	// No input is an empty object.
	assertEqual(std::string("Hello, you"), firstText(registry.executeTool("greet", Object::Ptr())));
	registry.unregisterTool("greet");
	assertTrue(!registry.hasTool("greet"));
}


void ToolRegistryTest::testReplace()
{
	ToolRegistry registry;
	registry.registerTool("t", "first", nullptr, [](const Object::Ptr&) { return std::string("1"); });
	registry.registerTool("t", "second", nullptr, [](const Object::Ptr&) { return std::string("2"); });
	assertEqual(std::size_t(1), registry.size());
	assertEqual(std::string("second"), registry.tools()[0].description);
	assertEqual(std::string("2"), firstText(registry.executeTool("t", nullptr)));
}


void ToolRegistryTest::testRegisterIfAbsent()
{
	ToolRegistry registry;
	Tool tool;
	tool.name = "t";
	tool.handler = [](const Object::Ptr&, const RequestContext&) { return ToolResult::error("first"); };
	assertTrue(registry.registerToolIfAbsent(tool));
	tool.handler = [](const Object::Ptr&, const RequestContext&) { return ToolResult::error("second"); };
	assertTrue(!registry.registerToolIfAbsent(tool));
	assertEqual(std::string("first"), firstText(registry.executeTool("t", nullptr)));
}


void ToolRegistryTest::testUnknownTool()
{
	ToolRegistry registry;
	ToolResult result = registry.executeTool("nope \"quoted\"", nullptr);
	assertTrue(result.isError());
	assertEqual(std::string("Unknown tool: nope \"quoted\""), firstText(result));
}


void ToolRegistryTest::testExecutorThrows()
{
	ToolRegistry registry;
	registry.registerTool("poco", "", nullptr, [](const Object::Ptr&) -> std::string
	{
		throw Poco::FileNotFoundException("missing.txt");
	});
	registry.registerTool("std", "", nullptr, [](const Object::Ptr&) -> std::string
	{
		throw std::runtime_error("line one\nline two");
	});
	ToolResult result = registry.executeTool("poco", nullptr);
	assertTrue(result.isError());
	assertTrue(firstText(result).find("missing.txt") != std::string::npos);
	result = registry.executeTool("std", nullptr);
	assertTrue(result.isError());
	assertTrue(firstText(result).find("line one\nline two") != std::string::npos);
}


void ToolRegistryTest::testToolsInOrder()
{
	ToolRegistry registry;
	registry.registerTool("zeta", "", nullptr, [](const Object::Ptr&) { return std::string(); });
	registry.registerTool("alpha", "", parse("{\"type\":\"object\",\"properties\":{\"a\":{\"type\":\"string\"}}}"), [](const Object::Ptr&) { return std::string(); });
	std::vector<Tool> tools = registry.tools();
	assertEqual(2u, static_cast<unsigned>(tools.size()));
	assertEqual(std::string("alpha"), tools[0].name);
	assertEqual(std::string("zeta"), tools[1].name);
	// A description is optional; a schema too.
	assertTrue(tools[1].description.empty());
	assertTrue(tools[1].inputSchema.isNull());
	assertTrue(tools[0].inputSchema->isObject("properties"));
}


void ToolRegistryTest::testRegistrationRefused()
{
	ToolRegistry registry;
	try
	{
		registry.registerTool("", "", nullptr, [](const Object::Ptr&) { return std::string(); });
		fail("a tool without a name must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	try
	{
		registry.registerTool("t", "", nullptr, ToolRegistry::TextExecutor());
		fail("a tool without an executor must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	Tool tool;
	tool.name = "no-handler";
	try
	{
		registry.registerTool(tool);
		fail("a tool without a handler must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	assertEqual(std::size_t(0), registry.size());
}


void ToolRegistryTest::testToolResultText()
{
	// Media is described by its type and size, not pasted as base64.
	ToolResult texts;
	texts.addText("one").addImage("aW1n", "image/png").addText("two");
	assertEqual(std::string("one\n[image image/png, 4 bytes of base64]\ntwo"), toolResultText(texts));

	ToolResult structured;
	structured.setStructuredContent(Poco::Dynamic::Var(parse("{\"n\":1}")));
	assertEqual(std::string("{\"n\":1}"), toolResultText(structured));

	ToolResult image;
	image.addImage("aW1n", "image/png");
	assertEqual(std::string("[image image/png, 4 bytes of base64]"), toolResultText(image));

	// A resource link by its URI; an embedded resource by its text, or its
	// URI and size when it is binary.
	ResourceContents text;
	text.uri = "file:///notes.txt";
	text.text = "notes";
	ResourceContents binary;
	binary.uri = "file:///logo.png";
	binary.blob = "aW1nZQ==";
	ToolResult resources;
	resources.addResourceLink("file:///report.pdf", "report").addResource(text).addResource(binary).addAudio("YXU=", "audio/wav");
	assertEqual(std::string("[resource file:///report.pdf]\nnotes\n[resource file:///logo.png, 8 bytes of base64]\n[audio audio/wav, 4 bytes of base64]"), toolResultText(resources));

	assertEqual(std::string(), toolResultText(ToolResult()));
}


void ToolRegistryTest::testContextReachesTheHandler()
{
	ToolRegistry registry;
	Tool tool;
	tool.name = "who";
	tool.handler = [](const Object::Ptr&, const RequestContext& context)
	{
		ToolResult result;
		result.addText(context.identity.principal);
		return result;
	};
	registry.registerTool(tool);
	RequestContext context;
	context.identity.principal = "bob";
	assertEqual(std::string("bob"), firstText(registry.executeTool("who", nullptr, context)));
}


void ToolRegistryTest::setUp()
{
}


void ToolRegistryTest::tearDown()
{
}


CppUnit::Test* ToolRegistryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ToolRegistryTest");

	CppUnit_addTest(pSuite, ToolRegistryTest, testRegisterAndExecute);
	CppUnit_addTest(pSuite, ToolRegistryTest, testReplace);
	CppUnit_addTest(pSuite, ToolRegistryTest, testRegisterIfAbsent);
	CppUnit_addTest(pSuite, ToolRegistryTest, testUnknownTool);
	CppUnit_addTest(pSuite, ToolRegistryTest, testExecutorThrows);
	CppUnit_addTest(pSuite, ToolRegistryTest, testToolsInOrder);
	CppUnit_addTest(pSuite, ToolRegistryTest, testRegistrationRefused);
	CppUnit_addTest(pSuite, ToolRegistryTest, testToolResultText);
	CppUnit_addTest(pSuite, ToolRegistryTest, testContextReachesTheHandler);

	return pSuite;
}
