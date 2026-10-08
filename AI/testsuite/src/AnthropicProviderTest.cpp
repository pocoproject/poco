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
#include "CannedServer.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/AnthropicProvider.h"
#include <sstream>
#include <string>
#include <vector>


using Poco::AI::AnthropicProvider;
using Poco::AI::ContentEvent;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{


Array userMessage()
{
	Object::Ptr pMessage = new Object;
	pMessage->set("role", "user");
	pMessage->set("content", "What is the weather in Paris?");
	Array messages;
	messages.add(pMessage);
	return messages;
}


Array lookupTool()
{
	Object::Ptr pTool = new Object;
	pTool->set("name", "lookup");
	pTool->set("description", "Looks up the weather of a city.");
	Object::Ptr pParameters = new Object;
	pParameters->set("type", "object");
	pTool->set("parameters", pParameters);
	Array tools;
	tools.add(pTool);
	return tools;
}


std::string event(const Object& data)
	/// Returns one event of a Messages API stream.
{
	std::ostringstream text;
	data.stringify(text);
	return "event: " + data.getValue<std::string>("type") + "\ndata: " + text.str() + "\n\n";
}


std::string toolUseStream(const std::vector<std::string>& inputFragments)
	/// Returns the stream of a Messages API response with a text block and
	/// a tool use block for the lookup tool, whose input arrives in the
	/// given fragments.
{
	std::string stream;

	Object::Ptr pTextDelta = new Object;
	pTextDelta->set("type", "text_delta");
	pTextDelta->set("text", "Let me check.");
	Object textDelta;
	textDelta.set("type", "content_block_delta");
	textDelta.set("index", 0);
	textDelta.set("delta", pTextDelta);
	stream += event(textDelta);

	Object::Ptr pBlock = new Object;
	pBlock->set("type", "tool_use");
	pBlock->set("id", "toolu_1");
	pBlock->set("name", "lookup");
	Object blockStart;
	blockStart.set("type", "content_block_start");
	blockStart.set("index", 1);
	blockStart.set("content_block", pBlock);
	stream += event(blockStart);

	for (const auto& fragment: inputFragments)
	{
		Object::Ptr pInputDelta = new Object;
		pInputDelta->set("type", "input_json_delta");
		pInputDelta->set("partial_json", fragment);
		Object inputDelta;
		inputDelta.set("type", "content_block_delta");
		inputDelta.set("index", 1);
		inputDelta.set("delta", pInputDelta);
		stream += event(inputDelta);
	}

	Object blockStop;
	blockStop.set("type", "content_block_stop");
	blockStop.set("index", 1);
	stream += event(blockStop);

	Object messageStop;
	messageStop.set("type", "message_stop");
	stream += event(messageStop);
	return stream;
}


std::vector<ContentEvent> chat(AnthropicProvider& provider)
{
	std::vector<ContentEvent> events;
	provider.chat(userMessage(), "You are a test.", lookupTool(),
		[&events](const ContentEvent& event) { events.push_back(event); });
	return events;
}


} // namespace


AnthropicProviderTest::AnthropicProviderTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


void AnthropicProviderTest::testChatToolUse()
{
	// The input arrives in two fragments and is parsed when its block ends.
	CannedServer server("text/event-stream", toolUseStream({R"({"city":)", R"("Paris"})"}));
	AnthropicProvider provider(server.baseUrl(), "secret", "test-model", 1024, 30);

	const std::vector<ContentEvent> events = chat(provider);

	assertEqual(std::string("/v1/messages"), server.path());
	assertEqual(std::string("secret"), server.header("x-api-key"));
	assertEqual(std::string(AnthropicProvider::DEFAULT_API_VERSION), server.header("anthropic-version"));
	assertTrue(server.request().find("\"stream\":true") != std::string::npos);
	assertTrue(server.request().find("\"input_schema\"") != std::string::npos);

	assertEqual(2, static_cast<int>(events.size()));
	assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
	assertEqual(std::string("Let me check."), events[0].text);
	assertTrue(events[1].type == ContentEvent::TYPE_TOOL_USE);
	assertEqual(std::string("toolu_1"), events[1].id);
	assertEqual(std::string("lookup"), events[1].name);
	assertEqual(std::string("Paris"), events[1].input.getValue<std::string>("city"));
}


void AnthropicProviderTest::testChatMalformedToolInput()
{
	// Input that is cut short, or is not an object, is an error: the tool
	// call is not delivered, so the tool cannot run without it.
	const std::vector<std::vector<std::string>> malformed{
		{R"({"city":)"},
		{R"({"city":"Par)"},
		{R"(["Paris"])"},
		{"not json"}};
	for (const auto& fragments: malformed)
	{
		CannedServer server("text/event-stream", toolUseStream(fragments));
		AnthropicProvider provider(server.baseUrl(), "secret", "test-model", 1024, 30);

		const std::vector<ContentEvent> events = chat(provider);

		assertEqual(2, static_cast<int>(events.size()));
		assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
		assertTrue(events[1].type == ContentEvent::TYPE_ERROR);
		assertEqual(std::string("malformed_tool_input"), events[1].code);
		assertTrue(events[1].text.find("lookup") != std::string::npos);
	}
}


void AnthropicProviderTest::testChatToolUseWithoutInput()
{
	// A tool without parameters is called with no input at all or with an
	// empty object.
	const std::vector<std::vector<std::string>> none{{}, {""}, {"{}"}};
	for (const auto& fragments: none)
	{
		CannedServer server("text/event-stream", toolUseStream(fragments));
		AnthropicProvider provider(server.baseUrl(), "secret", "test-model", 1024, 30);

		const std::vector<ContentEvent> events = chat(provider);

		assertEqual(2, static_cast<int>(events.size()));
		assertTrue(events[1].type == ContentEvent::TYPE_TOOL_USE);
		assertEqual(std::string("lookup"), events[1].name);
		assertEqual(0, static_cast<int>(events[1].input.size()));
	}
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
	CppUnit_addTest(pSuite, AnthropicProviderTest, testChatToolUse);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testChatMalformedToolInput);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testChatToolUseWithoutInput);

	return pSuite;
}
