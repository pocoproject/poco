//
// AgentLoopTest.cpp
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AgentLoopTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/AgentLoop.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/Exception.h"
#include <stdexcept>


using Poco::AI::AgentLoop;
using Poco::AI::AgentEvent;
using Poco::AI::ContentEvent;
using Poco::AI::ContentEventCallback;
using Poco::AI::LLMProvider;
using Poco::AI::ToolRegistry;
using Poco::JSON::Object;
using Poco::JSON::Array;


/// MockProvider emits scripted responses per round.
/// Each call to chat() pops the front of the scripted sequence.
class MockProvider: public LLMProvider
{
public:
	using Script = std::vector<std::vector<ContentEvent>>;

	MockProvider(Script script):
		_script(std::move(script)),
		_round(0)
	{
	}

	void setModel(const std::string&) override {}

	void chat(
		const Array& /*messages*/,
		const std::string& /*systemPrompt*/,
		const Array& /*tools*/,
		ContentEventCallback onEvent) override
	{
		if (_round < _script.size())
		{
			for (const auto& evt : _script[_round])
			{
				onEvent(evt);
			}
			++_round;
		}
	}

	void appendAssistantMessage(
		Array& messages,
		const std::string& text,
		const std::vector<ContentEvent>& toolCalls) override
	{
		Array content;
		if (!text.empty())
		{
			Object textBlock;
			textBlock.set("type", "text");
			textBlock.set("text", text);
			content.add(textBlock);
		}
		for (const auto& tc : toolCalls)
		{
			Object toolUseBlock;
			toolUseBlock.set("type", "tool_use");
			toolUseBlock.set("id", tc.id);
			toolUseBlock.set("name", tc.name);
			toolUseBlock.set("input", tc.input);
			content.add(toolUseBlock);
		}
		Object msg;
		msg.set("role", "assistant");
		msg.set("content", content);
		messages.add(msg);
	}

	void appendToolResult(
		Array& messages,
		const std::string& toolCallId,
		const std::string& result) override
	{
		Array content;
		Object toolResult;
		toolResult.set("type", "tool_result");
		toolResult.set("tool_use_id", toolCallId);
		toolResult.set("content", result);
		content.add(toolResult);
		Object msg;
		msg.set("role", "user");
		msg.set("content", content);
		messages.add(msg);
	}

private:
	Script _script;
	std::size_t _round;
};


/// ThrowingProvider throws on every chat() call.
class ThrowingProvider: public LLMProvider
{
public:
	enum ExceptionType { POCO_IO, POCO_NULL, STD_RUNTIME };

	ThrowingProvider(ExceptionType type): _type(type) {}

	void setModel(const std::string&) override {}

	void chat(const Array&, const std::string&, const Array&, ContentEventCallback) override
	{
		switch (_type)
		{
		case POCO_IO:
			throw Poco::IOException("connection failed");
		case POCO_NULL:
			throw Poco::NullPointerException();
		case STD_RUNTIME:
			throw std::runtime_error("std error");
		}
	}

	void appendAssistantMessage(Array&, const std::string&, const std::vector<ContentEvent>&) override {}
	void appendToolResult(Array&, const std::string&, const std::string&) override {}

private:
	ExceptionType _type;
};


AgentLoopTest::AgentLoopTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


void AgentLoopTest::testToolResultCap()
{
	// A tool result longer than the cap is cut before it is reported and
	// before it goes back to the model.
	ContentEvent call;
	call.type = ContentEvent::TYPE_TOOL_USE;
	call.id = "call-1";
	call.name = "big";
	ContentEvent text;
	text.type = ContentEvent::TYPE_TEXT;
	text.text = "done";
	auto pProvider = std::make_shared<MockProvider>(MockProvider::Script{{call}, {text}});

	ToolRegistry registry;
	Object def;
	def.set("name", "big");
	def.set("description", "returns 100 characters");
	def.set("parameters", Object::Ptr(new Object));
	registry.registerTool(def, [](const Object&) { return std::string(100, 'x'); });

	AgentLoop loop(pProvider, registry, 3);
	loop.setMaxToolResultSize(10);
	assertEqual(10, static_cast<int>(loop.maxToolResultSize()));

	std::string toolResult;
	loop.run(Array(), "", [&toolResult](const AgentEvent& evt)
	{
		if (evt.type == AgentEvent::TYPE_TOOL_RESULT) toolResult = evt.content;
	});
	assertEqual(std::string(10, 'x'), toolResult);
}


void AgentLoopTest::setUp()
{
}


void AgentLoopTest::tearDown()
{
}


void AgentLoopTest::testTextOnly()
{
	ContentEvent textEvt;
	textEvt.type = ContentEvent::TYPE_TEXT;
	textEvt.text = "Hello world";

	auto pProvider = std::make_shared<MockProvider>(
		MockProvider::Script{{textEvt}});

	ToolRegistry registry;
	AgentLoop loop(pProvider, registry, 10);

	std::vector<AgentEvent> events;
	Array messages;
	loop.run(messages, "system prompt", [&](const AgentEvent& evt)
	{
		events.push_back(evt);
	});

	// Expect: TEXT, DONE
	assertTrue(events.size() >= 2);
	assertEqual(static_cast<int>(AgentEvent::TYPE_TEXT), static_cast<int>(events[0].type));
	assertEqual(std::string("Hello world"), events[0].content);
	assertEqual(static_cast<int>(AgentEvent::TYPE_DONE), static_cast<int>(events.back().type));
}


void AgentLoopTest::testToolCallAndResult()
{
	// Round 1: provider emits a tool_use
	ContentEvent toolEvt;
	toolEvt.type = ContentEvent::TYPE_TOOL_USE;
	toolEvt.id = "call_123";
	toolEvt.name = "echo";
	toolEvt.input.set("text", "ping");

	// Round 2: provider emits text (no more tool calls)
	ContentEvent textEvt;
	textEvt.type = ContentEvent::TYPE_TEXT;
	textEvt.text = "Done";

	auto pProvider = std::make_shared<MockProvider>(
		MockProvider::Script{{toolEvt}, {textEvt}});

	ToolRegistry registry;
	Object def;
	def.set("name", "echo");
	def.set("description", "Echoes text");
	registry.registerTool(def, [](const Object& input) -> std::string
	{
		return input.getValue<std::string>("text");
	});

	AgentLoop loop(pProvider, registry, 10);

	std::vector<AgentEvent> events;
	Array messages;
	loop.run(messages, "", [&](const AgentEvent& evt)
	{
		events.push_back(evt);
	});

	// Expect: TOOL_CALL, TOOL_RESULT, TEXT, DONE
	assertTrue(events.size() >= 4);
	assertEqual(static_cast<int>(AgentEvent::TYPE_TOOL_CALL), static_cast<int>(events[0].type));
	assertEqual(std::string("echo"), events[0].toolName);
	assertEqual(static_cast<int>(AgentEvent::TYPE_TOOL_RESULT), static_cast<int>(events[1].type));
	assertEqual(std::string("echo"), events[1].toolName);
	assertEqual(std::string("ping"), events[1].content);
	assertEqual(static_cast<int>(AgentEvent::TYPE_TEXT), static_cast<int>(events[2].type));
	assertEqual(static_cast<int>(AgentEvent::TYPE_DONE), static_cast<int>(events.back().type));
}


void AgentLoopTest::testMaxRounds()
{
	// Provider always emits a tool_use — loop should stop after maxRounds
	ContentEvent toolEvt;
	toolEvt.type = ContentEvent::TYPE_TOOL_USE;
	toolEvt.id = "call_inf";
	toolEvt.name = "noop";

	auto pProvider = std::make_shared<MockProvider>(
		MockProvider::Script{{toolEvt}, {toolEvt}, {toolEvt}, {toolEvt}, {toolEvt}});

	ToolRegistry registry;
	Object def;
	def.set("name", "noop");
	def.set("description", "No-op");
	registry.registerTool(def, [](const Object&) { return "ok"; });

	AgentLoop loop(pProvider, registry, 2); // maxRounds=2

	std::vector<AgentEvent> events;
	Array messages;
	loop.run(messages, "", [&](const AgentEvent& evt)
	{
		events.push_back(evt);
	});

	// Should have TOOL_CALL+TOOL_RESULT for round 1, TOOL_CALL+TOOL_RESULT for round 2, then DONE
	int toolCallCount = 0;
	for (const auto& e : events)
	{
		if (e.type == AgentEvent::TYPE_TOOL_CALL) ++toolCallCount;
	}
	assertEqual(2, toolCallCount);
	assertEqual(static_cast<int>(AgentEvent::TYPE_DONE), static_cast<int>(events.back().type));
}


void AgentLoopTest::testProviderThrowsPoco()
{
	auto pProvider = std::make_shared<ThrowingProvider>(ThrowingProvider::POCO_IO);
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry, 10);

	std::vector<AgentEvent> events;
	Array messages;
	loop.run(messages, "", [&](const AgentEvent& evt)
	{
		events.push_back(evt);
	});

	assertTrue(events.size() >= 1);
	assertEqual(static_cast<int>(AgentEvent::TYPE_ERROR), static_cast<int>(events[0].type));
	assertTrue(events[0].content.find("LLM error:") != std::string::npos);
	assertTrue(events[0].content.find("connection failed") != std::string::npos);
}


void AgentLoopTest::testProviderThrowsNullPointer()
{
	auto pProvider = std::make_shared<ThrowingProvider>(ThrowingProvider::POCO_NULL);
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry, 10);

	std::vector<AgentEvent> events;
	Array messages;
	loop.run(messages, "", [&](const AgentEvent& evt)
	{
		events.push_back(evt);
	});

	// This reproduces the bug scenario
	assertTrue(events.size() >= 1);
	assertEqual(static_cast<int>(AgentEvent::TYPE_ERROR), static_cast<int>(events[0].type));
	assertTrue(events[0].content.find("LLM error:") != std::string::npos);
	assertTrue(events[0].content.find("Null pointer") != std::string::npos);
}


void AgentLoopTest::testProviderThrowsStd()
{
	auto pProvider = std::make_shared<ThrowingProvider>(ThrowingProvider::STD_RUNTIME);
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry, 10);

	std::vector<AgentEvent> events;
	Array messages;
	loop.run(messages, "", [&](const AgentEvent& evt)
	{
		events.push_back(evt);
	});

	assertTrue(events.size() >= 1);
	assertEqual(static_cast<int>(AgentEvent::TYPE_ERROR), static_cast<int>(events[0].type));
	assertTrue(events[0].content.find("LLM error:") != std::string::npos);
	assertTrue(events[0].content.find("std error") != std::string::npos);
}


CppUnit::Test* AgentLoopTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AgentLoopTest");

	CppUnit_addTest(pSuite, AgentLoopTest, testTextOnly);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolCallAndResult);
	CppUnit_addTest(pSuite, AgentLoopTest, testMaxRounds);
	CppUnit_addTest(pSuite, AgentLoopTest, testProviderThrowsPoco);
	CppUnit_addTest(pSuite, AgentLoopTest, testProviderThrowsNullPointer);
	CppUnit_addTest(pSuite, AgentLoopTest, testProviderThrowsStd);

	CppUnit_addTest(pSuite, AgentLoopTest, testToolResultCap);
	return pSuite;
}
