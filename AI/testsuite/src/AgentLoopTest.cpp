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
#include "Poco/Net/NetException.h"
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
		std::string moreText;
			/// A second piece of the text, in an event of its own.
		std::vector<std::string> toolNames;
		StopReason stopReason = StopReason::END_TURN;
		bool throwException = false;
			/// The provider cannot be reached.
		bool throwTimeout = false;
			/// The server does not answer in time.
		FailureCode errorCode = FailureCode::NONE;
		Usage usage;
		std::string stopDetail;
		std::string explanation;
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
			if (step.throwException) throw Poco::Net::ConnectionRefusedException("connection refused");
			if (step.throwTimeout) throw Poco::TimeoutException("no answer in time");
			if (step.errorCode != FailureCode::NONE)
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
			if (!step.moreText.empty())
			{
				ContentEvent event;
				event.text = step.moreText;
				onEvent(event);
				result.text += step.moreText;
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
			result.usage = step.usage;
			result.stopDetail = step.stopDetail;
			result.explanation = step.explanation;
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
			results.push_back(result);
		}

		std::vector<ChatRequest> requests;
		std::vector<Poco::AI::MCP::ToolResult> results;
			/// The tool results as the model got them.

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


	std::string typeOf(const Poco::AI::MCP::ToolResult& result, unsigned index)
	{
		return result.content()->getObject(index)->getValue<std::string>("type");
	}


	std::string textOf(const Poco::AI::MCP::ToolResult& result, unsigned index)
	{
		return result.content()->getObject(index)->getValue<std::string>("text");
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
	assertTrue(pError->code == FailureCode::PROVIDER_FAILED);
	assertTrue(pError->content.find("connection refused") != std::string::npos);
	assertEqual(1, recorder.count(AgentEvent::Type::DONE));
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
}


void AgentLoopTest::testNoAnswer()
{
	// A call that throws something else than a network failure got no answer,
	// e.g. in time: the provider was reached.
	Step timeout;
	timeout.throwTimeout = true;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{timeout});
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());
	const AgentEvent* pError = recorder.first(AgentEvent::Type::FAILURE);
	assertTrue(pError != nullptr);
	assertTrue(pError->code == FailureCode::NO_ANSWER);
	assertTrue(pError->content.find("no answer in time") != std::string::npos);
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
	assertTrue(recorder.events.back().stopReason == StopReason::FAILURE);
}


void AgentLoopTest::testProviderError()
{
	Step error;
	error.errorCode = FailureCode::RATE_LIMITED;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{error, text("never")});
	ToolRegistry registry;
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	assertTrue(recorder.first(AgentEvent::Type::FAILURE)->code == FailureCode::RATE_LIMITED);
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
	// Every call is reported, with its result.
	assertEqual(2, recorder.count(AgentEvent::Type::TOOL_CALL));
	assertEqual(2, recorder.count(AgentEvent::Type::TOOL_RESULT));
	assertEqual(1, recorder.count(AgentEvent::Type::DONE));
	assertTrue(recorder.events.back().stopReason == StopReason::CANCELLED);
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
	assertTrue(pError != nullptr && pError->code == FailureCode::ROUND_LIMIT);
	assertEqual(3, recorder.count(AgentEvent::Type::TOOL_CALL));
	assertEqual(3, recorder.count(AgentEvent::Type::TOOL_RESULT));
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

	// A tool may ask cancelled() itself; besides the run's Cancellation it
	// asks what the given context asks.
	auto pAgain = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"asks"}), text("ok")});
	bool askedCancelled = false;
	Poco::AI::MCP::Tool asks;
	asks.name = "asks";
	asks.handler = [&askedCancelled](const Object::Ptr&, const Poco::AI::MCP::RequestContext& context)
	{
		askedCancelled = context.cancelled();
		return Poco::AI::MCP::ToolResult();
	};
	ToolRegistry asking;
	asking.registerTool(asks);
	AgentLoop askingLoop(pAgain, asking);
	Poco::AI::MCP::RequestContext cancelling;
	cancelling.cancelled = []() { return true; };
	askingLoop.setRequestContext(cancelling);
	Array again = userTurn("go");
	askingLoop.run(again, "", AgentEventCallback());
	assertTrue(askedCancelled);
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


void AgentLoopTest::testCutByTokenLimit()
{
	// The token limit cut the turn: its tool call may have lost input, so it
	// does not run, gets an error result, and the run ends.
	Step cut = tools({"echo"});
	cut.stopReason = StopReason::MAX_TOKENS;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{cut, text("never")});
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());

	assertEqual(0, runs);
	assertEqual(1u, static_cast<unsigned>(pProvider->requests.size()));
	assertEqual(3u, static_cast<unsigned>(conversation.size()));
	assertTrue(conversation.getObject(2)->getValue<bool>("error"));
	assertTrue(conversation.getObject(2)->getValue<std::string>("text").find("token limit") != std::string::npos);
	assertEqual(1, recorder.count(AgentEvent::Type::TOOL_CALL));
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
	assertTrue(recorder.events.back().stopReason == StopReason::MAX_TOKENS);
}


void AgentLoopTest::testToolAddedDuringRun()
{
	// A tool registered while the run is on is not one of the run's tools:
	// a call of it is an unknown tool, and its handler does not run, so
	// that an approver that refuses it cannot be passed by.
	ToolRegistry registry;
	int lateRuns = 0;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"adder"}), tools({"late"}), text("ok")});
	registry.registerTool("adder", "", nullptr, [&registry, &lateRuns](const Object::Ptr&)
	{
		registerEcho(registry, "late", &lateRuns);
		return std::string("added");
	});
	AgentLoop loop(pProvider, registry);
	std::vector<std::string> asked;
	loop.setApprover([&asked](const ToolCall& call, const Poco::AI::MCP::Tool&)
	{
		asked.push_back(call.name);
		return call.name != "late";
	});
	Array conversation = userTurn("go");
	loop.run(conversation, "", AgentEventCallback());
	assertEqual(0, lateRuns);
	assertTrue(pProvider->results[1].isError());
	assertEqual(std::string("Unknown tool: late"), textOf(pProvider->results[1], 0));
	assertEqual(1u, static_cast<unsigned>(asked.size()));
	assertEqual(std::string("adder"), asked[0]);
}


void AgentLoopTest::testToolReplacedDuringRun()
{
	// A tool registered anew while the run is on keeps, for the run, the
	// handler the run took at its start, the one the approver approves.
	ToolRegistry registry;
	int oldRuns = 0;
	int newRuns = 0;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"replacer"}), tools({"second"}), text("ok")});
	registerEcho(registry, "second", &oldRuns);
	registry.registerTool("replacer", "", nullptr, [&registry, &newRuns](const Object::Ptr&)
	{
		registerEcho(registry, "second", &newRuns);
		return std::string("replaced");
	});
	AgentLoop loop(pProvider, registry);
	std::vector<std::string> asked;
	loop.setApprover([&asked](const ToolCall& call, const Poco::AI::MCP::Tool&)
	{
		asked.push_back(call.name);
		return true;
	});
	Array conversation = userTurn("go");
	loop.run(conversation, "", AgentEventCallback());
	assertEqual(1, oldRuns);
	assertEqual(0, newRuns);
	assertTrue(asked == std::vector<std::string>({"replacer", "second"}));
}


void AgentLoopTest::testApproverThrows()
{
	// An approver that throws refuses the call.
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"echo"}), text("ok")});
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry);
	loop.setApprover([](const ToolCall&, const Poco::AI::MCP::Tool&) -> bool
	{
		throw Poco::RuntimeException("no decision");
	});
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());
	assertEqual(0, runs);
	assertEqual(std::string("The tool call was refused."), textOf(pProvider->results[0], 0));
	assertTrue(recorder.events.back().type == AgentEvent::Type::DONE);
}


void AgentLoopTest::testCallbackThrows()
{
	// The callback throws on the first of two results: run() passes the
	// exception on, yet both calls have their result in the conversation,
	// each before its event, and DONE was delivered.
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"echo", "echo"}), text("never")});
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	std::vector<AgentEvent::Type> seen;
	std::size_t sizeAtResult = 0;
	try
	{
		loop.run(conversation, "", [&](const AgentEvent& event)
		{
			seen.push_back(event.type);
			if (event.type == AgentEvent::Type::TOOL_RESULT)
			{
				sizeAtResult = conversation.size();
				throw Poco::IllegalStateException("callback failed");
			}
		});
		fail("the exception of the callback must reach the caller");
	}
	catch (const Poco::IllegalStateException&)
	{
	}
	assertEqual(1, runs);
	assertEqual(std::size_t(3), sizeAtResult);
	assertEqual(4u, static_cast<unsigned>(conversation.size()));
	assertTrue(!conversation.getObject(2)->getValue<bool>("error"));
	assertTrue(conversation.getObject(3)->getValue<bool>("error"));
	assertEqual(1u, static_cast<unsigned>(pProvider->requests.size()));
	assertTrue(seen.back() == AgentEvent::Type::DONE);
}


void AgentLoopTest::testCallbackThrowsDuringCall()
{
	// The callback throws on the first piece of text: no event of the call
	// follows, the tool of the answer does not run and its turn is not kept,
	// and run() passes the exception on after DONE, which tells of the call.
	Step step = tools({"echo"});
	step.text = "first";
	step.moreText = "second";
	step.usage.outputTokens = 5;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{step, text("never")});
	ToolRegistry registry;
	int runs = 0;
	registerEcho(registry, "echo", &runs);
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	std::vector<AgentEvent> seen;
	try
	{
		loop.run(conversation, "", [&](const AgentEvent& event)
		{
			seen.push_back(event);
			if (event.type == AgentEvent::Type::TEXT) throw Poco::IllegalStateException("callback failed");
		});
		fail("the exception of the callback must reach the caller");
	}
	catch (const Poco::IllegalStateException&)
	{
	}
	assertEqual(2u, static_cast<unsigned>(seen.size()));
	assertEqual(std::string("first"), seen[0].content);
	assertTrue(seen[1].type == AgentEvent::Type::DONE);
	assertTrue(seen[1].stopReason == StopReason::TOOL_USE);
	assertEqual(Poco::UInt64(5), seen[1].usage.outputTokens);
	assertEqual(0, runs);
	assertEqual(1u, static_cast<unsigned>(conversation.size()));
	assertEqual(1u, static_cast<unsigned>(pProvider->requests.size()));
}


void AgentLoopTest::testResultLimits()
{
	// Each kind of block has its limit: images of the types the APIs take
	// stay whole up to the image limit, other media and binary resources are
	// named, text is cut, structured content stays while it fits.
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"media", "big"}), text("ok")});
	ToolRegistry registry;
	Poco::AI::MCP::Tool media;
	media.name = "media";
	media.handler = [](const Object::Ptr&, const Poco::AI::MCP::RequestContext&)
	{
		Poco::AI::MCP::ResourceContents notes;
		notes.uri = "file:///notes.txt";
		notes.text = "notes";
		Poco::AI::MCP::ResourceContents blob;
		blob.uri = "file:///data.bin";
		blob.mimeType = "application/octet-stream";
		blob.blob = "AAAA";
		Poco::AI::MCP::ToolResult result;
		result.addImage("aW1n", "image/png")
			.addImage(std::string(100, 'A'), "image/png")
			.addImage("Qk0=", "image/bmp")
			.addAudio("YXU=", "audio/wav")
			.addResource(notes)
			.addResource(blob)
			.addResourceLink("file:///report.pdf", "report");
		result.setStructuredContent(Poco::Dynamic::Var(std::string("fits")));
		return result;
	};
	registry.registerTool(media);
	Poco::AI::MCP::Tool big;
	big.name = "big";
	big.handler = [](const Object::Ptr&, const Poco::AI::MCP::RequestContext&)
	{
		Poco::AI::MCP::ToolResult result;
		result.addText(std::string(300, 'x'));
		result.setStructuredContent(Poco::Dynamic::Var(std::string(300, 'y')));
		return result;
	};
	registry.registerTool(big);
	AgentLoop loop(pProvider, registry);
	loop.setMaxImageSize(50);
	assertEqual(std::size_t(50), loop.getMaxImageSize());
	loop.setMaxToolResultSize(250);
	Array conversation = userTurn("go");
	loop.run(conversation, "", AgentEventCallback());

	const Poco::AI::MCP::ToolResult& first = pProvider->results[0];
	assertEqual(7u, static_cast<unsigned>(first.content()->size()));
	assertEqual(std::string("image"), typeOf(first, 0));
	assertEqual(std::string("[image image/png of 100 bytes left out]"), textOf(first, 1));
	assertEqual(std::string("[image image/bmp of 4 bytes left out]"), textOf(first, 2));
	assertEqual(std::string("[audio audio/wav of 4 bytes left out]"), textOf(first, 3));
	assertEqual(std::string("notes"), textOf(first, 4));
	assertEqual(std::string("[resource file:///data.bin application/octet-stream of 4 bytes left out]"), textOf(first, 5));
	assertEqual(std::string("resource_link"), typeOf(first, 6));
	assertEqual(std::string("fits"), first.structuredContent().convert<std::string>());

	// The text is cut to the limit; the structured content no longer fits.
	const Poco::AI::MCP::ToolResult& second = pProvider->results[1];
	assertEqual(2u, static_cast<unsigned>(second.content()->size()));
	assertEqual(std::string(250, 'x'), textOf(second, 0));
	assertEqual(std::string("...(truncated)"), textOf(second, 1));
	assertTrue(second.structuredContent().isEmpty());
}


void AgentLoopTest::testDoneCarriesTheOutcome()
{
	// DONE tells how the run ended: the stop reason of the last call, its
	// detail and explanation, and the tokens of all calls.
	Step first = tools({"echo"});
	first.usage.inputTokens = 10;
	first.usage.outputTokens = 2;
	first.usage.cachedInputTokens = 1;
	Step second = text("Done");
	second.usage.inputTokens = 20;
	second.usage.outputTokens = 3;
	second.usage.cachedInputTokens = 15;
	second.stopDetail = "end_turn";
	second.explanation = "finished";
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{first, second});
	ToolRegistry registry;
	registerEcho(registry, "echo");
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback());
	const AgentEvent& done = recorder.events.back();
	assertTrue(done.type == AgentEvent::Type::DONE);
	assertTrue(done.stopReason == StopReason::END_TURN);
	assertEqual(std::string("end_turn"), done.stopDetail);
	assertEqual(std::string("finished"), done.explanation);
	assertEqual(Poco::UInt64(30), done.usage.inputTokens);
	assertEqual(Poco::UInt64(5), done.usage.outputTokens);
	assertEqual(Poco::UInt64(16), done.usage.cachedInputTokens);
}


void AgentLoopTest::testToolSeesTheCancel()
{
	// A tool learns from its context that the run was cancelled, e.g. to end
	// what it is doing.
	Cancellation cancellation;
	auto pProvider = std::make_shared<ScriptedProvider>(std::vector<Step>{tools({"watch"}), text("never")});
	ToolRegistry registry;
	bool before = true;
	bool after = false;
	Poco::AI::MCP::Tool watch;
	watch.name = "watch";
	watch.handler = [&](const Object::Ptr&, const Poco::AI::MCP::RequestContext& context)
	{
		before = context.isCancelled();
		cancellation.cancel();
		after = context.isCancelled();
		return Poco::AI::MCP::ToolResult();
	};
	registry.registerTool(watch);
	AgentLoop loop(pProvider, registry);
	Array conversation = userTurn("go");
	Recorder recorder;
	loop.run(conversation, "", recorder.callback(), cancellation);
	assertTrue(!before);
	assertTrue(after);
	assertEqual(1u, static_cast<unsigned>(pProvider->requests.size()));
	assertTrue(recorder.events.back().stopReason == StopReason::CANCELLED);
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
	CppUnit_addTest(pSuite, AgentLoopTest, testNoAnswer);
	CppUnit_addTest(pSuite, AgentLoopTest, testCancelled);
	CppUnit_addTest(pSuite, AgentLoopTest, testCancelDuringTools);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolResultCutAtCharacter);
	CppUnit_addTest(pSuite, AgentLoopTest, testFinalRound);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolsUnchangedAcrossRounds);
	CppUnit_addTest(pSuite, AgentLoopTest, testApprover);
	CppUnit_addTest(pSuite, AgentLoopTest, testRequestContext);
	CppUnit_addTest(pSuite, AgentLoopTest, testUnknownTool);
	CppUnit_addTest(pSuite, AgentLoopTest, testConstruction);
	CppUnit_addTest(pSuite, AgentLoopTest, testCutByTokenLimit);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolAddedDuringRun);
	CppUnit_addTest(pSuite, AgentLoopTest, testApproverThrows);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolReplacedDuringRun);
	CppUnit_addTest(pSuite, AgentLoopTest, testCallbackThrows);
	CppUnit_addTest(pSuite, AgentLoopTest, testCallbackThrowsDuringCall);
	CppUnit_addTest(pSuite, AgentLoopTest, testResultLimits);
	CppUnit_addTest(pSuite, AgentLoopTest, testDoneCarriesTheOutcome);
	CppUnit_addTest(pSuite, AgentLoopTest, testToolSeesTheCancel);

	return pSuite;
}
