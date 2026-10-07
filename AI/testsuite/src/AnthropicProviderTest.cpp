//
// AnthropicProviderTest.cpp
//
// Tests for AnthropicProvider's pure-JSON helper methods
// (no network calls).
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AnthropicProviderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/AnthropicProvider.h"


using Poco::AI::AnthropicProvider;
using Poco::AI::ContentEvent;
using Poco::JSON::Object;
using Poco::JSON::Array;


AnthropicProviderTest::AnthropicProviderTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


void AnthropicProviderTest::setUp()
{
}


void AnthropicProviderTest::tearDown()
{
}


void AnthropicProviderTest::testAppendAssistantTextOnly()
{
	// Use dummy credentials — we only test the message-building methods
	AnthropicProvider provider("http://localhost:1/v1", "fake-key", "fake-model", 1024, 30);

	Array messages;
	std::vector<ContentEvent> noToolCalls;

	provider.appendAssistantMessage(messages, "Hello", noToolCalls);

	assertEqual(1, static_cast<int>(messages.size()));

	auto pMsg = messages.getObject(0);
	assertNotNull(pMsg.get());
	assertEqual(std::string("assistant"), pMsg->getValue<std::string>("role"));

	auto pContent = pMsg->getArray("content");
	assertNotNull(pContent.get());
	assertEqual(1, static_cast<int>(pContent->size()));

	auto pBlock = pContent->getObject(0);
	assertEqual(std::string("text"), pBlock->getValue<std::string>("type"));
	assertEqual(std::string("Hello"), pBlock->getValue<std::string>("text"));
}


void AnthropicProviderTest::testAppendAssistantWithToolCalls()
{
	AnthropicProvider provider("http://localhost:1/v1", "fake-key", "fake-model", 1024, 30);

	Array messages;

	ContentEvent tc;
	tc.type = ContentEvent::TYPE_TOOL_USE;
	tc.id = "call_abc";
	tc.name = "get_info";
	tc.input.set("query", "status");

	std::vector<ContentEvent> toolCalls = {tc};
	provider.appendAssistantMessage(messages, "Let me check.", toolCalls);

	auto pMsg = messages.getObject(0);
	auto pContent = pMsg->getArray("content");
	assertEqual(2, static_cast<int>(pContent->size()));

	// First block: text
	auto pTextBlock = pContent->getObject(0);
	assertEqual(std::string("text"), pTextBlock->getValue<std::string>("type"));
	assertEqual(std::string("Let me check."), pTextBlock->getValue<std::string>("text"));

	// Second block: tool_use
	auto pToolBlock = pContent->getObject(1);
	assertEqual(std::string("tool_use"), pToolBlock->getValue<std::string>("type"));
	assertEqual(std::string("call_abc"), pToolBlock->getValue<std::string>("id"));
	assertEqual(std::string("get_info"), pToolBlock->getValue<std::string>("name"));
}


void AnthropicProviderTest::testAppendToolResult()
{
	AnthropicProvider provider("http://localhost:1/v1", "fake-key", "fake-model", 1024, 30);

	Array messages;
	provider.appendToolResult(messages, "call_xyz", "result data");

	assertEqual(1, static_cast<int>(messages.size()));

	auto pMsg = messages.getObject(0);
	assertEqual(std::string("user"), pMsg->getValue<std::string>("role"));

	auto pContent = pMsg->getArray("content");
	assertEqual(1, static_cast<int>(pContent->size()));

	auto pResult = pContent->getObject(0);
	assertEqual(std::string("tool_result"), pResult->getValue<std::string>("type"));
	assertEqual(std::string("call_xyz"), pResult->getValue<std::string>("tool_use_id"));
	assertEqual(std::string("result data"), pResult->getValue<std::string>("content"));
}


void AnthropicProviderTest::testAppendToolResultMerge()
{
	AnthropicProvider provider("http://localhost:1/v1", "fake-key", "fake-model", 1024, 30);

	Array messages;

	// First tool result creates a new user message
	provider.appendToolResult(messages, "call_1", "result 1");
	// Second should merge into the same user message
	provider.appendToolResult(messages, "call_2", "result 2");

	// Should still be 1 message (merged)
	assertEqual(1, static_cast<int>(messages.size()));

	auto pMsg = messages.getObject(0);
	auto pContent = pMsg->getArray("content");
	assertEqual(2, static_cast<int>(pContent->size()));

	auto pResult1 = pContent->getObject(0);
	assertEqual(std::string("call_1"), pResult1->getValue<std::string>("tool_use_id"));

	auto pResult2 = pContent->getObject(1);
	assertEqual(std::string("call_2"), pResult2->getValue<std::string>("tool_use_id"));
}


void AnthropicProviderTest::testToAnthropicTools()
{
	AnthropicProvider provider("http://localhost:1/v1", "fake-key", "fake-model", 1024, 30);

	Object params;
	params.set("type", "object");
	Object props;
	Object nameProp;
	nameProp.set("type", "string");
	props.set("name", nameProp);
	params.set("properties", props);

	Object::Ptr pToolDef = new Object;
	pToolDef->set("name", "my_tool");
	pToolDef->set("description", "A test tool");
	pToolDef->set("parameters", params);

	Array tools;
	tools.add(pToolDef);

	Array anthropicTools = provider.toAnthropicTools(tools);

	assertEqual(1, static_cast<int>(anthropicTools.size()));

	auto pTool = anthropicTools.getObject(0);
	assertEqual(std::string("my_tool"), pTool->getValue<std::string>("name"));
	assertEqual(std::string("A test tool"), pTool->getValue<std::string>("description"));

	// "parameters" should be renamed to "input_schema"
	assertTrue(!pTool->has("parameters"));
	assertTrue(pTool->has("input_schema"));
}


CppUnit::Test* AnthropicProviderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AnthropicProviderTest");

	CppUnit_addTest(pSuite, AnthropicProviderTest, testAppendAssistantTextOnly);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testAppendAssistantWithToolCalls);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testAppendToolResult);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testAppendToolResultMerge);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testToAnthropicTools);

	return pSuite;
}
