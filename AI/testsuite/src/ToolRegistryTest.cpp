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
#include "Poco/JSON/Parser.h"


using Poco::AI::ToolRegistry;
using Poco::JSON::Object;
using Poco::JSON::Array;


ToolRegistryTest::ToolRegistryTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


void ToolRegistryTest::setUp()
{
}


void ToolRegistryTest::tearDown()
{
}


void ToolRegistryTest::testRegisterAndExecute()
{
	ToolRegistry registry;

	Object def;
	def.set("name", "echo");
	def.set("description", "Echoes input");

	registry.registerTool(def, [](const Object& input) -> std::string
	{
		return input.getValue<std::string>("text");
	});

	assertTrue(registry.hasTool("echo"));

	Object input;
	input.set("text", "hello");
	std::string result = registry.executeTool("echo", input);
	assertEqual("hello", result);
}


void ToolRegistryTest::testGetDefinitions()
{
	ToolRegistry registry;

	Object def1;
	def1.set("name", "tool_a");
	def1.set("description", "Tool A");
	registry.registerTool(def1, [](const Object&) { return "a"; });

	Object def2;
	def2.set("name", "tool_b");
	def2.set("description", "Tool B");
	registry.registerTool(def2, [](const Object&) { return "b"; });

	Array defs = registry.getToolDefinitions();
	assertEqual(2, static_cast<int>(defs.size()));

	// Definitions are sorted by name (std::map)
	auto p0 = defs.getObject(0);
	auto p1 = defs.getObject(1);
	assertEqual(std::string("tool_a"), p0->getValue<std::string>("name"));
	assertEqual(std::string("tool_b"), p1->getValue<std::string>("name"));
}


void ToolRegistryTest::testUnregister()
{
	ToolRegistry registry;

	Object def;
	def.set("name", "temp");
	def.set("description", "Temporary");
	registry.registerTool(def, [](const Object&) { return "ok"; });

	assertTrue(registry.hasTool("temp"));
	registry.unregisterTool("temp");
	assertTrue(!registry.hasTool("temp"));
}


void ToolRegistryTest::testUnknownTool()
{
	ToolRegistry registry;

	Object input;
	std::string result = registry.executeTool("nonexistent", input);

	// Should return JSON error, not throw
	Poco::JSON::Parser parser;
	auto parsed = parser.parse(result);
	auto pObj = parsed.extract<Object::Ptr>();
	assertTrue(pObj->has("error"));
	std::string error = pObj->getValue<std::string>("error");
	assertTrue(error.find("nonexistent") != std::string::npos);
}


void ToolRegistryTest::testExecutorException()
{
	ToolRegistry registry;

	Object def;
	def.set("name", "crasher");
	def.set("description", "Always throws");
	registry.registerTool(def, [](const Object&) -> std::string
	{
		throw Poco::RuntimeException("boom");
	});

	Object input;
	std::string result = registry.executeTool("crasher", input);

	// Should return JSON error, not propagate the exception
	Poco::JSON::Parser parser;
	auto parsed = parser.parse(result);
	auto pObj = parsed.extract<Object::Ptr>();
	assertTrue(pObj->has("error"));
	std::string error = pObj->getValue<std::string>("error");
	assertTrue(error.find("boom") != std::string::npos);
}


CppUnit::Test* ToolRegistryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ToolRegistryTest");

	CppUnit_addTest(pSuite, ToolRegistryTest, testRegisterAndExecute);
	CppUnit_addTest(pSuite, ToolRegistryTest, testGetDefinitions);
	CppUnit_addTest(pSuite, ToolRegistryTest, testUnregister);
	CppUnit_addTest(pSuite, ToolRegistryTest, testUnknownTool);
	CppUnit_addTest(pSuite, ToolRegistryTest, testExecutorException);

	return pSuite;
}
