//
// ToolDispatchProviderTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ToolDispatchProviderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/ToolDispatchProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include <memory>
#include <string>
#include <vector>


using Poco::AI::ContentEvent;
using Poco::AI::ContentEventCallback;
using Poco::AI::LLMProvider;
using Poco::AI::ToolDispatchProvider;
using Poco::AI::ToolRegistry;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{


class RecordingProvider: public LLMProvider
	/// Answers every call with the same text and keeps what it was asked.
{
public:
	void setModel(const std::string&) override
	{
	}

	void chat(
		const Array& messages,
		const std::string& /*systemPrompt*/,
		const Array& tools,
		ContentEventCallback onEvent) override
	{
		++calls;
		lastMessages = messages;
		lastToolCount = tools.size();
		ContentEvent event;
		event.type = ContentEvent::TYPE_TEXT;
		event.text = "answer";
		onEvent(event);
	}

	void appendAssistantMessage(Array&, const std::string&, const std::vector<ContentEvent>&) override
	{
	}

	void appendToolResult(Array&, const std::string&, const std::string&) override
	{
	}

	int calls = 0;
	Array lastMessages;
	std::size_t lastToolCount = 0;
};


Object::Ptr message(const std::string& role, const std::string& content)
{
	Object::Ptr pMessage = new Object;
	pMessage->set("role", role);
	pMessage->set("content", content);
	return pMessage;
}


struct Fixture
	/// A registry with one tool, a dispatch provider with one rule for it
	/// over a recording provider, and the events of the last call.
{
	Fixture():
		pRecorder(std::make_shared<RecordingProvider>()),
		dispatch(pRecorder, registry)
	{
		Object definition;
		definition.set("name", "status");
		definition.set("description", "Reports the status.");
		definition.set("parameters", Object::Ptr(new Object));
		registry.registerTool(definition, [](const Object&) { return std::string("all good"); });

		ToolDispatchProvider::DispatchRule rule;
		rule.keywordGroups = {{"status"}};
		rule.toolName = "status";
		dispatch.addRule(rule);
	}

	std::vector<ContentEvent> chat(const Array& messages, bool withTools = true)
	{
		std::vector<ContentEvent> events;
		const Array tools = withTools ? registry.getToolDefinitions() : Array();
		dispatch.chat(messages, "You are a test.", tools,
			[&events](const ContentEvent& event) { events.push_back(event); });
		return events;
	}

	ToolRegistry registry;
	std::shared_ptr<RecordingProvider> pRecorder;
	ToolDispatchProvider dispatch;
};


std::string content(const Array& messages, unsigned int index)
{
	return messages.getObject(index)->getValue<std::string>("content");
}


} // namespace


ToolDispatchProviderTest::ToolDispatchProviderTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


void ToolDispatchProviderTest::setUp()
{
}


void ToolDispatchProviderTest::tearDown()
{
}


void ToolDispatchProviderTest::testRuleMatch()
{
	Fixture fixture;
	Array messages;
	messages.add(message("user", "What is the STATUS of the plant?"));

	// The rule matches, whatever the case: the tool call comes from the
	// rule and the wrapped provider is not asked.
	const std::vector<ContentEvent> events = fixture.chat(messages);
	assertEqual(1, static_cast<int>(events.size()));
	assertTrue(events[0].type == ContentEvent::TYPE_TOOL_USE);
	assertEqual(std::string("status"), events[0].name);
	assertTrue(!events[0].id.empty());
	assertEqual(0, fixture.pRecorder->calls);
}


void ToolDispatchProviderTest::testNoMatchFallsThrough()
{
	Fixture fixture;
	Array messages;
	messages.add(message("user", "Good morning."));
	messages.add(message("assistant", "Good morning to you."));
	messages.add(message("user", "Tell me a joke."));

	// No rule matches: the wrapped provider gets the conversation as it is,
	// with the tools.
	const std::vector<ContentEvent> events = fixture.chat(messages);
	assertEqual(1, static_cast<int>(events.size()));
	assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
	assertEqual(1, fixture.pRecorder->calls);
	assertEqual(3, static_cast<int>(fixture.pRecorder->lastMessages.size()));
	assertEqual(std::string("Tell me a joke."), content(fixture.pRecorder->lastMessages, 2));
	assertEqual(1, static_cast<int>(fixture.pRecorder->lastToolCount));
}


void ToolDispatchProviderTest::testAnswerFromResults()
{
	Fixture fixture;
	Array messages;
	messages.add(message("user", "What is the status?"));
	std::vector<ContentEvent> toolCalls = fixture.chat(messages);
	assertEqual(1, static_cast<int>(toolCalls.size()));

	// The agent loop appends the call and its result, then asks again.
	fixture.dispatch.appendAssistantMessage(messages, "", toolCalls);
	fixture.dispatch.appendToolResult(messages, toolCalls[0].id, "all good");

	// The wrapped provider answers from the result, without tools.
	const std::vector<ContentEvent> events = fixture.chat(messages);
	assertEqual(1, static_cast<int>(events.size()));
	assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
	assertEqual(1, fixture.pRecorder->calls);
	assertEqual(1, static_cast<int>(fixture.pRecorder->lastMessages.size()));
	const std::string context = content(fixture.pRecorder->lastMessages, 0);
	assertTrue(context.find("all good") != std::string::npos);
	assertTrue(context.find("What is the status?") != std::string::npos);
	assertEqual(0, static_cast<int>(fixture.pRecorder->lastToolCount));
}


void ToolDispatchProviderTest::testEarlierResultsDoNotCount()
{
	Fixture fixture;

	// An earlier turn of the history, with its tool call and result.
	Array messages;
	messages.add(message("user", "What is the status?"));
	std::vector<ContentEvent> earlier = fixture.chat(messages);
	fixture.dispatch.appendAssistantMessage(messages, "", earlier);
	fixture.dispatch.appendToolResult(messages, earlier[0].id, "stale reading");
	messages.add(message("assistant", "It was fine."));

	// A new turn: the rule matches again, although the history holds a
	// tool result, and the wrapped provider is not asked.
	messages.add(message("user", "And what is the status now?"));
	std::vector<ContentEvent> toolCalls = fixture.chat(messages);
	assertEqual(1, static_cast<int>(toolCalls.size()));
	assertTrue(toolCalls[0].type == ContentEvent::TYPE_TOOL_USE);
	assertEqual(0, fixture.pRecorder->calls);

	// The answer comes from the result of this turn alone.
	fixture.dispatch.appendAssistantMessage(messages, "", toolCalls);
	fixture.dispatch.appendToolResult(messages, toolCalls[0].id, "fresh reading");
	fixture.chat(messages);
	assertEqual(1, fixture.pRecorder->calls);
	const std::string context = content(fixture.pRecorder->lastMessages, 0);
	assertTrue(context.find("fresh reading") != std::string::npos);
	assertTrue(context.find("stale reading") == std::string::npos);
	assertTrue(context.find("And what is the status now?") != std::string::npos);
}


void ToolDispatchProviderTest::testNoToolsKeepsHistory()
{
	Fixture fixture;
	Array messages;
	messages.add(message("user", "What is the status?"));
	messages.add(message("assistant", "I cannot look that up."));
	messages.add(message("user", "Why not?"));

	// Without tools nothing is dispatched and nothing is summarized: the
	// wrapped provider gets the whole conversation.
	const std::vector<ContentEvent> events = fixture.chat(messages, false);
	assertEqual(1, static_cast<int>(events.size()));
	assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
	assertEqual(1, fixture.pRecorder->calls);
	assertEqual(3, static_cast<int>(fixture.pRecorder->lastMessages.size()));
	assertEqual(std::string("What is the status?"), content(fixture.pRecorder->lastMessages, 0));
	assertEqual(std::string("Why not?"), content(fixture.pRecorder->lastMessages, 2));
	assertEqual(0, static_cast<int>(fixture.pRecorder->lastToolCount));
}


CppUnit::Test* ToolDispatchProviderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ToolDispatchProviderTest");

	CppUnit_addTest(pSuite, ToolDispatchProviderTest, testRuleMatch);
	CppUnit_addTest(pSuite, ToolDispatchProviderTest, testNoMatchFallsThrough);
	CppUnit_addTest(pSuite, ToolDispatchProviderTest, testAnswerFromResults);
	CppUnit_addTest(pSuite, ToolDispatchProviderTest, testEarlierResultsDoNotCount);
	CppUnit_addTest(pSuite, ToolDispatchProviderTest, testNoToolsKeepsHistory);

	return pSuite;
}
