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
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/UTF8String.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/TextIterator.h"
#include "Poco/Exception.h"
#include <memory>
#include <string>
#include <vector>


using namespace Poco::AI;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	struct Step
		/// What the scripted provider answers in one call.
	{
		std::string text;
		std::vector<std::string> toolNames;
		StopReason stopReason = StopReason::END_TURN;
		bool throwException = false;
		ErrorCode errorCode = ErrorCode::NONE;
	};


	class ScriptedProvider: public LLMProvider
		/// Answers each call with the next step of its script and keeps the
		/// requests it got. Its message format is a simple one of its own.
	{
	public:
		explicit ScriptedProvider(std::vector<Step> script, Cancellation* pCancelAfterFirst = nullptr):
			_script(std::move(script)),
			_pCancelAfterFirst(pCancelAfterFirst)
		{
		}

		void setModel(const std::string& model) override
		{
			_model = model;
		}

		std::string getModel() const override
		{
			return _model;
		}

		ChatResult chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation) override
		{
			requests.push_back(request);
			ChatResult result;
			if (cancellation.cancelled())
			{
				result.stopReason = StopReason::CANCELLED;
				return result;
			}
			if (_round >= _script.size())
			{
				result.stopReason = StopReason::END_TURN;
				return result;
			}
			const Step step = _script[_round++];
			if (step.throwException) throw Poco::IOException("connection refused");
			if (step.errorCode != ErrorCode::NONE)
			{
				ContentEvent event;
				event.type = ContentEvent::Type::FAILURE;
				event.code = step.errorCode;
				event.text = "scripted error";
				onEvent(event);
				result.stopReason = StopReason::FAILURE;
				return result;
			}
			if (!step.text.empty())
			{
				ContentEvent event;
				event.text = step.text;
				onEvent(event);
				result.text = step.text;
			}
			Array::Ptr pCalls = new Array;
			for (const std::string& name: step.toolNames)
			{
				ToolCall call;
				call.id = "call-" + std::to_string(++_nextId);
				call.name = name;
				call.input = new Object;
				call.input->set("arg", name);
				result.toolCalls.push_back(call);
				pCalls->add(call.id);
			}
			result.stopReason = step.stopReason;
			result.assistantMessage = new Object;
			result.assistantMessage->set("role", "assistant");
			result.assistantMessage->set("text", step.text);
			result.assistantMessage->set("calls", pCalls);
			if (_pCancelAfterFirst != nullptr) _pCancelAfterFirst->cancel();
			return result;
		}

		void appendToolResult(Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult& result) override
		{
			Object::Ptr pMessage = new Object;
			pMessage->set("role", "tool");
			pMessage->set("id", toolCallId);
			pMessage->set("text", toolResultText(result));
			pMessage->set("error", result.isError());
			messages.add(pMessage);
		}

		std::vector<ChatRequest> requests;

	private:
		std::vector<Step> _script;
		Cancellation* _pCancelAfterFirst;
		std::size_t _round = 0;
		int _nextId = 0;
		std::string _model;
	};


	struct Recorder
	{
		std::vector<AgentEvent> events;

		AgentEventCallback callback()
		{
			return [this](const AgentEvent& event) { events.push_back(event); };
		}

		int count(AgentEvent::Type type) const
		{
			int n = 0;
			for (const AgentEvent& event: events)
			{
				if (event.type == type) ++n;
			}
			return n;
		}

		const AgentEvent* first(AgentEvent::Type type) const
		{
			for (const AgentEvent& event: events)
			{
				if (event.type == type) return &event;
			}
			return nullptr;
		}
	};


	Step text(const std::string& value)
	{
		Step step;
		step.text = value;
		return step;
	}


	Step tools(const std::vector<std::string>& names)
	{
		Step step;
		step.toolNames = names;
		step.stopReason = StopReason::TOOL_USE;
		return step;
	}


	Array userTurn(const std::string& message)
	{
		Array conversation;
		Object::Ptr pMessage = new Object;
		pMessage->set("role", "user");
		pMessage->set("text", message);
		conversation.add(pMessage);
		return conversation;
	}


	void registerEcho(ToolRegistry& registry, const std::string& name, int* pRuns = nullptr)
	{
		registry.registerTool(name, "Echoes its input.", nullptr, [name, pRuns](const Object::Ptr& pInput)
		{
			if (pRuns != nullptr) ++*pRuns;
			return name + ":" + pInput->optValue<std::string>("arg", "");
		});
	}


	bool isValidUTF8(const std::string& text)
	{
		Poco::UTF8Encoding utf8;
		Poco::TextIterator it(text, utf8);
		Poco::TextIterator end(text);
		for (; it != end; ++it)
		{
			if (*it < 0) return false;
		}
		return true;
	}
}


AgentLoopTest::AgentLoopTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


AgentLoopTest::~AgentLoopTest()
{
}


void AgentLoopTest::testTextOnly()
{
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{text("Hello")});
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("hi");
	Recorder recorder;
	loop.run(conversation, "system", recorder.callback());

	assertEqual(1, recorder.count(AgentEvent::Type::TEXT));
	assertEqual(std::string("Hello"), recorder.first(AgentEvent::Type::TEXT)->content);
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
	// The model's turn is appended to the conversation.
	assertEqual(2u, static_cast<unsigned>(conversation.size()));
	assertEqual(std::string("assistant"), conversation.getObject(1)->getValue<std::string>("role"));
	assertEqual(std::string("system"), pProvider->requests[0].systemPrompt);
}


void AgentLoopTest::testToolRound()
{
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"echo"}), text("Done")});
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	assertEqual(1, runs);
	assertEqual(1, recorder.count(AgentEvent::Type::TOOL_CALL));
	assertEqual(std::string("{\"arg\":\"echo\"}"), recorder.first(AgentEvent::Type::TOOL_CALL)->toolInput);
	const AgentEvent* pResult = recorder.first(AgentEvent::Type::TOOL_RESULT);
	assertEqual(std::string("echo:echo"), pResult->content);
	assertTrue(!pResult->toolError);
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
	// user, assistant with the call, tool result, assistant with the answer
	assertEqual(4u, static_cast<unsigned>(conversation.size()));
	assertEqual(std::string("tool"), conversation.getObject(2)->getValue<std::string>("role"));
	assertEqual(std::string("Done"), conversation.getObject(3)->getValue<std::string>("text"));
	// The second call sees the tool result.
	assertEqual(3u, static_cast<unsigned>(pProvider->requests[1].messages.size()));
}


void AgentLoopTest::testDoneOnEveryExit()
{
	Step failing;
	failing.throwException = true;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"echo"}), failing});
	ToolRegistry registry;
	registerEcho(registry, "echo");
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	const AgentEvent* pError = recorder.first(AgentEvent::Type::FAILURE);
	assertTrue(pError != nullptr);
	assertTrue(pError->code == ErrorCode::PROVIDER_FAILED);
	assertTrue(pError->content.find("connection refused") != std::string::npos);
	assertEqual(1, recorder.count(AgentEvent::Type::DONE));
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
}


void AgentLoopTest::testProviderError()
{
	Step error;
	error.errorCode = ErrorCode::RATE_LIMITED;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{error, text("never")});
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	assertTrue(recorder.first(AgentEvent::Type::FAILURE)->code == ErrorCode::RATE_LIMITED);
	assertEqual(1u, static_cast<unsigned>(pProvider->requests.size()));
	assertEqual(1u, static_cast<unsigned>(conversation.size()));
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
}


void AgentLoopTest::testCancelled()
{
	// A token cancelled before the run: no call, and DONE.
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{text("never")});
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	Cancellation cancellation;
	cancellation.cancel();
	loop.run(conversation, "", recorder.callback(), cancellation);
	assertEqual(0u, static_cast<unsigned>(pProvider->requests.size()));
	assertEqual(0, recorder.count(AgentEvent::Type::FAILURE));
	assertEqual(1, recorder.count(AgentEvent::Type::DONE));
}


void AgentLoopTest::testCancelDuringTools()
{
	// The run is cancelled once the model has asked for two tools: neither
	// runs, both get a result, so that the conversation stays valid.
	Cancellation cancellation;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"echo", "echo"}), text("never")}, &cancellation);
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback(), cancellation);

	assertEqual(0, runs);
	assertEqual(1u, static_cast<unsigned>(pProvider->requests.size()));
	assertEqual(4u, static_cast<unsigned>(conversation.size()));
	assertTrue(conversation.getObject(2)->getValue<bool>("error"));
	assertTrue(conversation.getObject(3)->getValue<bool>("error"));
	assertEqual(1, recorder.count(AgentEvent::Type::DONE));
}


void AgentLoopTest::testToolResultCutAtCharacter()
{
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"long"}), text("ok")});
	ToolRegistry registry;
	// 19999 ASCII bytes and then a two-byte character across the limit.
	registry.registerTool("long", "", nullptr, [](const Object::Ptr&)
	{
		return std::string(19999, 'a') + "\xC3\xA9" + "tail";
	});
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	const std::string reported = recorder.first(AgentEvent::Type::TOOL_RESULT)->content;
	assertTrue(isValidUTF8(reported));
	assertEqual(std::string(19999, 'a') + "\n...(truncated)", reported);
	const std::string passed = conversation.getObject(2)->getValue<std::string>("text");
	assertEqual(reported, passed);

	// Within the limit nothing is cut.
	auto pShort = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"long"}), text("ok")});
	AgentLoop wide(pShort, registry);
	wide.setMaxToolResultSize(30000);
	assertEqual(std::size_t(30000), wide.getMaxToolResultSize());
	Array other = userTurn("go");
	Recorder wideRecorder;
	wide.run(other, "", wideRecorder.callback());
	assertEqual(std::string(19999, 'a') + "\xC3\xA9" + "tail", wideRecorder.first(AgentEvent::Type::TOOL_RESULT)->content);
}


void AgentLoopTest::testFinalRound()
{
	// The model keeps calling tools: after two rounds one more call, with tool
	// choice none, asks for text; a tool call in it is not run.
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"echo"}), tools({"echo"}), tools({"echo"})});
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry, 2);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	assertEqual(2, runs);
	assertEqual(3u, static_cast<unsigned>(pProvider->requests.size()));
	assertTrue(pProvider->requests[0].toolChoice == ToolChoice::AUTO);
	assertTrue(pProvider->requests[1].toolChoice == ToolChoice::AUTO);
	assertTrue(pProvider->requests[2].toolChoice == ToolChoice::NONE);
	// The final call sends the same tools.
	assertEqual(1u, static_cast<unsigned>(pProvider->requests[2].tools.size()));
	const AgentEvent* pError = recorder.first(AgentEvent::Type::FAILURE);
	assertTrue(pError != nullptr && pError->code == ErrorCode::ROUND_LIMIT);
	// The call of the final round has a result too.
	const Object::Ptr pLast = conversation.getObject(static_cast<unsigned>(conversation.size() - 1));
	assertEqual(std::string("tool"), pLast->getValue<std::string>("role"));
	assertTrue(pLast->getValue<bool>("error"));
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
}


void AgentLoopTest::testToolsUnchangedAcrossRounds()
{
	ToolRegistry registry;
	registerEcho(registry, "echo");
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"adder"}), text("ok")});
	// The tool registers another tool while the run is on.
	registry.registerTool("adder", "", nullptr, [&registry](const Object::Ptr&)
	{
		registerEcho(registry, "late");
		return std::string("added");
	});
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	loop.run(conversation, "", AgentEventCallback());
	assertEqual(2u, static_cast<unsigned>(pProvider->requests[0].tools.size()));
	assertEqual(2u, static_cast<unsigned>(pProvider->requests[1].tools.size()));
	assertEqual(3u, static_cast<unsigned>(registry.size()));
}


void AgentLoopTest::testApprover()
{
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"safe", "risky"}), text("ok")});
	ToolRegistry registry;
	int safeRuns = 0;
	int riskyRuns = 0;
	registerEcho(registry, "safe", &safeRuns);
	Poco::AI::MCP::Tool risky;
	risky.name = "risky";
	risky.annotations = new Object;
	risky.annotations->set("destructiveHint", true);
	risky.handler = [&riskyRuns](const Object::Ptr&, const Poco::AI::MCP::RequestContext&)
	{
		++riskyRuns;
		return Poco::AI::MCP::ToolResult();
	};
	registry.registerTool(risky);
	AgentLoop loop(pProvider, registry);
	loop.setApprover([](const ToolCall&, const Poco::AI::MCP::Tool& tool)
	{
		return tool.annotations.isNull() || !tool.annotations->optValue<bool>("destructiveHint", false);
	});
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	assertEqual(1, safeRuns);
	assertEqual(0, riskyRuns);
	assertEqual(std::string("The tool call was refused."), conversation.getObject(3)->getValue<std::string>("text"));
	assertTrue(conversation.getObject(3)->getValue<bool>("error"));
}


void AgentLoopTest::testRequestContext()
{
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"who"}), text("ok")});
	ToolRegistry registry;
	std::string seen;
	Poco::AI::MCP::Tool who;
	who.name = "who";
	who.handler = [&seen](const Object::Ptr&, const Poco::AI::MCP::RequestContext& context)
	{
		seen = context.identity.principal;
		return Poco::AI::MCP::ToolResult();
	};
	registry.registerTool(who);
	AgentLoop loop(pProvider, registry);
	Poco::AI::MCP::RequestContext context;
	context.identity.principal = "alice";
	loop.setRequestContext(context);
	Array conversation = userTurn("go");
	loop.run(conversation, "", AgentEventCallback());
	assertEqual(std::string("alice"), seen);
}


void AgentLoopTest::testUnknownTool()
{
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"nope"}), text("ok")});
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());
	const AgentEvent* pResult = recorder.first(AgentEvent::Type::TOOL_RESULT);
	assertTrue(pResult->toolError);
	assertTrue(pResult->content.find("Unknown tool: nope") != std::string::npos);
}


void AgentLoopTest::testConstruction()
{
	ToolRegistry registry;
	try
	{
		AgentLoop loop(std::shared_ptr<LLMProvider>(), registry);
		fail("a loop without a provider must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	try
	{
		AgentLoop loop(std::make_shared<ScriptedProvider>(std::vector<Step>()), registry, 0);
		fail("a loop without rounds must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
}


void AgentLoopTest::setUp()
{
}


void AgentLoopTest::tearDown()
{
}


CppUnit::Test* AgentLoopTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AgentLoopTest");

	CppUnit_addTest(pSuite, AgentLoopTest, testTextOnly);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolRound);
	CppUnit_addTest(pSuite, AgentLoopTest, testDoneOnEveryExit);
	CppUnit_addTest(pSuite, AgentLoopTest, testProviderError);
	CppUnit_addTest(pSuite, AgentLoopTest, testCancelled);
	CppUnit_addTest(pSuite, AgentLoopTest, testCancelDuringTools);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolResultCutAtCharacter);
	CppUnit_addTest(pSuite, AgentLoopTest, testFinalRound);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolsUnchangedAcrossRounds);
	CppUnit_addTest(pSuite, AgentLoopTest, testApprover);
	CppUnit_addTest(pSuite, AgentLoopTest, testRequestContext);
	CppUnit_addTest(pSuite, AgentLoopTest, testUnknownTool);
	CppUnit_addTest(pSuite, AgentLoopTest, testConstruction);

	return pSuite;
}
