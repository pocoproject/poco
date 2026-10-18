//
// AnthropicProviderTest.cpp
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
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Event.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include <atomic>
#include <memory>
#include <string>
#include <vector>


using namespace Poco::AI;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	const std::string SSE("text/event-stream");


	std::string sse(const std::string& type, const std::string& json)
	{
		return "event: " + type + "\ndata: " + json + "\n\n";
	}


	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	AnthropicProvider::Params params(const std::string& baseUrl)
	{
		AnthropicProvider::Params params;
		params.baseURI = Poco::URI(baseUrl);
		params.apiKey = "test-key";
		params.model = "test-model";
		params.maxTokens = 100;
		params.timeout = Poco::Timespan(30, 0);
		return params;
	}


	ChatRequest userRequest(const std::string& text, const std::string& systemPrompt = std::string())
	{
		ChatRequest request;
		request.systemPrompt = systemPrompt;
		Object::Ptr pMessage = new Object;
		pMessage->set("role", "user");
		pMessage->set("content", text);
		request.messages.add(pMessage);
		return request;
	}


	Poco::AI::MCP::Tool weatherTool()
	{
		Poco::AI::MCP::Tool tool;
		tool.name = "weather";
		tool.description = "The weather of a city.";
		tool.inputSchema = parse("{\"type\":\"object\",\"properties\":{\"city\":{\"type\":\"string\"}}}");
		return tool;
	}


	struct Events
	{
		std::vector<ContentEvent> events;

		ContentEventCallback callback()
		{
			return [this](const ContentEvent& event) { events.push_back(event); };
		}

		int errors() const
		{
			int n = 0;
			for (const ContentEvent& event: events) if (event.type == ContentEvent::Type::ERROR) ++n;
			return n;
		}

		const ContentEvent* firstError() const
		{
			for (const ContentEvent& event: events) if (event.type == ContentEvent::Type::ERROR) return &event;
			return nullptr;
		}

		std::string text() const
		{
			std::string all;
			for (const ContentEvent& event: events) if (event.type == ContentEvent::Type::TEXT) all += event.text;
			return all;
		}
	};


	std::string textStream()
	{
		return sse("message_start", "{\"type\":\"message_start\",\"message\":{\"id\":\"m1\",\"usage\":{\"input_tokens\":10,\"cache_read_input_tokens\":4,\"cache_creation_input_tokens\":0}}}")
			+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"text\",\"text\":\"\"}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"text_delta\",\"text\":\"Hel\"}}")
			+ sse("ping", "{\"type\":\"ping\"}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"text_delta\",\"text\":\"lo\"}}")
			+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
			+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"end_turn\"},\"usage\":{\"output_tokens\":5}}")
			+ sse("message_stop", "{\"type\":\"message_stop\"}");
	}


	std::string toolStream(const std::string& arguments1 = "{\\\"city\\\":", const std::string& arguments2 = "\\\"Paris\\\"}")
	{
		return sse("message_start", "{\"type\":\"message_start\",\"message\":{\"usage\":{\"input_tokens\":3}}}")
			+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"tool_use\",\"id\":\"toolu_1\",\"name\":\"weather\",\"input\":{}}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"input_json_delta\",\"partial_json\":\"" + arguments1 + "\"}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"input_json_delta\",\"partial_json\":\"" + arguments2 + "\"}}")
			+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
			+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"tool_use\"},\"usage\":{\"output_tokens\":7}}")
			+ sse("message_stop", "{\"type\":\"message_stop\"}");
	}


	std::string thinkingStream()
	{
		return sse("message_start", "{\"type\":\"message_start\",\"message\":{\"usage\":{\"input_tokens\":3}}}")
			+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"\"}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"thinking_delta\",\"thinking\":\"Let me \"}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"thinking_delta\",\"thinking\":\"think.\"}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"signature_delta\",\"signature\":\"sig-abc\"}}")
			+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
			+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":1,\"content_block\":{\"type\":\"redacted_thinking\",\"data\":\"opaque==\"}}")
			+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":1}")
			+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":2,\"content_block\":{\"type\":\"text\",\"text\":\"\"}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":2,\"delta\":{\"type\":\"text_delta\",\"text\":\"I will check.\"}}")
			+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":2}")
			+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":3,\"content_block\":{\"type\":\"tool_use\",\"id\":\"toolu_9\",\"name\":\"weather\",\"input\":{}}}")
			+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":3,\"delta\":{\"type\":\"input_json_delta\",\"partial_json\":\"{\\\"city\\\":\\\"Rome\\\"}\"}}")
			+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":3}")
			+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"tool_use\"},\"usage\":{\"output_tokens\":9}}")
			+ sse("message_stop", "{\"type\":\"message_stop\"}");
	}


	struct ChatThread
		/// Runs one chat call on a thread of its own.
	{
		ChatThread(AnthropicProvider& provider, ChatRequest request):
			_provider(provider),
			_request(std::move(request))
		{
			_thread.startFunc([this]()
			{
				try
				{
					result = _provider.chat(_request, events.callback(), cancellation);
				}
				catch (const Poco::Exception& exc)
				{
					error = exc.displayText();
				}
				finished = true;
			});
		}

		~ChatThread()
		{
			cancellation.cancel();
			_thread.join();
		}

		void join()
		{
			_thread.join();
		}

		Cancellation cancellation;
		Events events;
		ChatResult result;
		std::string error;
		std::atomic<bool> finished{false};

	private:
		AnthropicProvider& _provider;
		ChatRequest _request;
		Poco::Thread _thread;
	};
}


AnthropicProviderTest::AnthropicProviderTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


AnthropicProviderTest::~AnthropicProviderTest()
{
}


void AnthropicProviderTest::testRequestBody()
{
	CannedServer server(SSE, textStream());
	AnthropicProvider provider(params(server.baseUrl()));
	ChatRequest request = userRequest("Hi", "Be brief.");
	request.tools.push_back(weatherTool());
	Poco::AI::MCP::Tool second = weatherTool();
	second.name = "clock";
	second.description.clear();
	request.tools.push_back(second);
	request.toolChoice = ToolChoice::NONE;
	Cancellation cancellation;
	(void) provider.chat(request, ContentEventCallback(), cancellation);

	assertEqual(std::string("/v1/messages"), server.path());
	assertEqual(std::string("test-key"), server.header("x-api-key"));
	assertEqual(std::string(AnthropicProvider::DEFAULT_API_VERSION), server.header("anthropic-version"));
	Object::Ptr pBody = parse(server.request());
	assertEqual(std::string("test-model"), pBody->getValue<std::string>("model"));
	assertEqual(100, pBody->getValue<int>("max_tokens"));
	assertTrue(pBody->getValue<bool>("stream"));
	Object::Ptr pSystem = pBody->getArray("system")->getObject(0);
	assertEqual(std::string("Be brief."), pSystem->getValue<std::string>("text"));
	assertEqual(std::string("ephemeral"), pSystem->getObject("cache_control")->getValue<std::string>("type"));
	Array::Ptr pTools = pBody->getArray("tools");
	assertEqual(2u, static_cast<unsigned>(pTools->size()));
	assertEqual(std::string("weather"), pTools->getObject(0)->getValue<std::string>("name"));
	assertTrue(pTools->getObject(0)->getObject("input_schema")->isObject("properties"));
	assertTrue(!pTools->getObject(0)->has("cache_control"));
	// The description is optional; the cache marker sits on the last tool.
	assertTrue(!pTools->getObject(1)->has("description"));
	assertTrue(pTools->getObject(1)->has("cache_control"));
	assertEqual(std::string("none"), pBody->getObject("tool_choice")->getValue<std::string>("type"));
	assertEqual(std::string("Hi"), pBody->getArray("messages")->getObject(0)->getValue<std::string>("content"));
}


void AnthropicProviderTest::testEmptySystemPrompt()
{
	CannedServer server(SSE, textStream());
	AnthropicProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	(void) provider.chat(userRequest("Hi"), ContentEventCallback(), cancellation);
	Object::Ptr pBody = parse(server.request());
	assertTrue(!pBody->has("system"));
	assertTrue(!pBody->has("tools"));
	assertTrue(!pBody->has("tool_choice"));
}


void AnthropicProviderTest::testURIJoin()
{
	CannedServer server(SSE, textStream());
	AnthropicProvider provider(params(server.baseUrl() + "/?beta=yes"));
	Cancellation cancellation;
	(void) provider.chat(userRequest("Hi"), ContentEventCallback(), cancellation);
	assertEqual(std::string("/v1/messages?beta=yes"), server.path());
}


void AnthropicProviderTest::testStreamedText()
{
	CannedServer server(SSE, textStream());
	AnthropicProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);

	assertEqual(std::string("Hello"), events.text());
	assertEqual(0, events.errors());
	assertEqual(std::string("Hello"), result.text);
	assertTrue(result.stopReason == StopReason::END_TURN);
	assertEqual(std::string("end_turn"), result.stopDetail);
	assertEqual(Poco::UInt64(14), result.usage.inputTokens);
	assertEqual(Poco::UInt64(4), result.usage.cachedInputTokens);
	assertEqual(Poco::UInt64(5), result.usage.outputTokens);
	assertTrue(result.toolCalls.empty());
	Array::Ptr pContent = result.assistantMessage->getArray("content");
	assertEqual(std::string("assistant"), result.assistantMessage->getValue<std::string>("role"));
	assertEqual(1u, static_cast<unsigned>(pContent->size()));
	assertEqual(std::string("Hello"), pContent->getObject(0)->getValue<std::string>("text"));
}


void AnthropicProviderTest::testStreamedToolUse()
{
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, toolStream());
	response.chunked = true;
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);

	assertTrue(result.stopReason == StopReason::TOOL_USE);
	assertEqual(1u, static_cast<unsigned>(result.toolCalls.size()));
	assertEqual(std::string("toolu_1"), result.toolCalls[0].id);
	assertEqual(std::string("weather"), result.toolCalls[0].name);
	assertEqual(std::string("Paris"), result.toolCalls[0].input->getValue<std::string>("city"));
	assertEqual(1u, static_cast<unsigned>(events.events.size()));
	assertTrue(events.events[0].type == ContentEvent::Type::TOOL_USE);
	Object::Ptr pBlock = result.assistantMessage->getArray("content")->getObject(0);
	assertEqual(std::string("tool_use"), pBlock->getValue<std::string>("type"));
	assertEqual(std::string("Paris"), pBlock->getObject("input")->getValue<std::string>("city"));
}


void AnthropicProviderTest::testThinkingRoundTrip()
{
	CannedServer server(SSE, thinkingStream());
	AnthropicProvider provider(params(server.baseUrl()));
	ChatRequest request = userRequest("Weather in Rome?");
	request.tools.push_back(weatherTool());
	Cancellation cancellation;
	ChatResult result = provider.chat(request, ContentEventCallback(), cancellation);

	// The turn holds the blocks in their order, thinking unchanged with its signature.
	Array::Ptr pContent = result.assistantMessage->getArray("content");
	assertEqual(4u, static_cast<unsigned>(pContent->size()));
	assertEqual(std::string("thinking"), pContent->getObject(0)->getValue<std::string>("type"));
	assertEqual(std::string("Let me think."), pContent->getObject(0)->getValue<std::string>("thinking"));
	assertEqual(std::string("sig-abc"), pContent->getObject(0)->getValue<std::string>("signature"));
	assertEqual(std::string("redacted_thinking"), pContent->getObject(1)->getValue<std::string>("type"));
	assertEqual(std::string("opaque=="), pContent->getObject(1)->getValue<std::string>("data"));
	assertEqual(std::string("text"), pContent->getObject(2)->getValue<std::string>("type"));
	assertEqual(std::string("tool_use"), pContent->getObject(3)->getValue<std::string>("type"));

	// The next call sends the turn back as it was, followed by the tool result.
	request.messages.add(result.assistantMessage);
	Poco::AI::MCP::ToolResult toolResult;
	toolResult.addText("Sunny");
	provider.appendToolResult(request.messages, "toolu_9", toolResult);
	(void) provider.chat(request, ContentEventCallback(), cancellation);
	Array::Ptr pMessages = parse(server.request(1))->getArray("messages");
	assertEqual(3u, static_cast<unsigned>(pMessages->size()));
	Array::Ptr pSent = pMessages->getObject(1)->getArray("content");
	assertEqual(std::string("thinking"), pSent->getObject(0)->getValue<std::string>("type"));
	assertEqual(std::string("sig-abc"), pSent->getObject(0)->getValue<std::string>("signature"));
	assertEqual(std::string("opaque=="), pSent->getObject(1)->getValue<std::string>("data"));
	Object::Ptr pToolResult = pMessages->getObject(2)->getArray("content")->getObject(0);
	assertEqual(std::string("tool_result"), pToolResult->getValue<std::string>("type"));
	assertEqual(std::string("toolu_9"), pToolResult->getValue<std::string>("tool_use_id"));
}


void AnthropicProviderTest::testCutStream()
{
	// The server stops after a tool call is complete, before the end of the message.
	std::string stream = toolStream();
	stream = stream.substr(0, stream.find("event: message_delta"));
	CannedServer server(SSE, stream);
	AnthropicProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);

	assertTrue(result.stopReason == StopReason::ERROR);
	assertTrue(result.toolCalls.empty());
	assertTrue(result.assistantMessage.isNull());
	assertEqual(1, events.errors());
	assertTrue(events.firstError()->code == ErrorCode::STREAM_INTERRUPTED);
	// No tool call was reported.
	assertEqual(1u, static_cast<unsigned>(events.events.size()));
}


void AnthropicProviderTest::testErrorEvent()
{
	const std::string stream = sse("message_start", "{\"type\":\"message_start\",\"message\":{}}")
		+ sse("error", "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}");
	CannedServer server(SSE, stream);
	AnthropicProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);
	assertTrue(result.stopReason == StopReason::ERROR);
	assertTrue(events.firstError()->code == ErrorCode::OVERLOADED);
	assertTrue(events.firstError()->text.find("Overloaded") != std::string::npos);
}


void AnthropicProviderTest::testHttpErrors()
{
	struct Case
	{
		int status;
		ErrorCode code;
	};
	const Case cases[] = {
		{401, ErrorCode::UNAUTHENTICATED},
		{403, ErrorCode::UNAUTHENTICATED},
		{429, ErrorCode::RATE_LIMITED},
		{503, ErrorCode::OVERLOADED},
		{500, ErrorCode::SERVER_ERROR},
		{400, ErrorCode::REQUEST_REJECTED}
	};
	for (const Case& c: cases)
	{
		// The excerpt of the body is cut at a character boundary.
		CannedServer server(CannedServer::makeResponse(c.status, "application/json", std::string(499, 'x') + "\xC3\xA9" + "rest"));
		AnthropicProvider provider(params(server.baseUrl()));
		Events events;
		Cancellation cancellation;
		ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);
		assertTrue(result.stopReason == StopReason::ERROR);
		assertTrue(events.firstError()->code == c.code);
		const std::string text = events.firstError()->text;
		assertTrue(text.find(std::to_string(c.status)) != std::string::npos);
		assertTrue(text.size() >= 499);
		assertTrue(text.compare(text.size() - 499, 499, std::string(499, 'x')) == 0);
	}
}


void AnthropicProviderTest::testMalformedToolInput()
{
	CannedServer server(SSE, toolStream("{\\\"city\\\": ", "["));
	AnthropicProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);
	assertTrue(result.toolCalls.empty());
	assertTrue(events.firstError()->code == ErrorCode::MALFORMED_TOOL_INPUT);
	// The turn leaves the call out, so that no call is left without a result.
	assertTrue(result.assistantMessage.isNull());
}


void AnthropicProviderTest::testAppendToolResult()
{
	AnthropicProvider provider(params("http://127.0.0.1:1/v1"));
	Array messages;
	Poco::AI::MCP::ToolResult first;
	first.addText("text result").addImage("aW1n", "image/png");
	provider.appendToolResult(messages, "id-1", first);
	provider.appendToolResult(messages, "id-2", Poco::AI::MCP::ToolResult::error("failed"));
	Poco::AI::MCP::ToolResult structured;
	structured.setStructuredContent(Poco::Dynamic::Var(parse("{\"n\":1}")));
	provider.appendToolResult(messages, "id-3", structured);

	// The results of one turn share one user message.
	assertEqual(1u, static_cast<unsigned>(messages.size()));
	Array::Ptr pContent = messages.getObject(0)->getArray("content");
	assertEqual(3u, static_cast<unsigned>(pContent->size()));
	Array::Ptr pFirst = pContent->getObject(0)->getArray("content");
	assertEqual(std::string("text"), pFirst->getObject(0)->getValue<std::string>("type"));
	assertEqual(std::string("image"), pFirst->getObject(1)->getValue<std::string>("type"));
	assertEqual(std::string("image/png"), pFirst->getObject(1)->getObject("source")->getValue<std::string>("media_type"));
	assertTrue(!pContent->getObject(0)->has("is_error"));
	assertTrue(pContent->getObject(1)->getValue<bool>("is_error"));
	assertEqual(std::string("{\"n\":1}"), pContent->getObject(2)->getArray("content")->getObject(0)->getValue<std::string>("text"));
}


void AnthropicProviderTest::testListModels()
{
	CannedServer server("application/json", "{\"data\":[{\"id\":\"model-b\"},{\"id\":\"model-a\"},{\"type\":\"model\"}],\"has_more\":false}");
	AnthropicProvider provider(params(server.baseUrl()));
	std::vector<std::string> models = provider.listModels();
	assertEqual(2u, static_cast<unsigned>(models.size()));
	assertEqual(std::string("model-b"), models[0]);
	assertEqual(std::string("/v1/models?limit=1000"), server.path());

	CannedServer failing(CannedServer::makeResponse(401, "application/json", "{\"error\":{}}"));
	AnthropicProvider unauthorized(params(failing.baseUrl()));
	try
	{
		(void) unauthorized.listModels();
		fail("a failed listing must throw");
	}
	catch (const Poco::Net::HTTPException&)
	{
	}
}


void AnthropicProviderTest::testCancelBeforeHeaders()
{
	CannedServer::Response response;
	response.waitBeforeHeaders = true;
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	ChatThread chat(provider, userRequest("Hi"));
	assertTrue(server.waitForRequest(10000));
	chat.cancellation.cancel();
	chat.join();
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
	assertEqual(0, chat.events.errors());
}


void AnthropicProviderTest::testCancelDuringStream()
{
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.pauseAt = response.body.find("event: content_block_stop");
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	ChatThread chat(provider, userRequest("Hi"));
	assertTrue(server.waitForRequest(10000));
	chat.cancellation.cancel();
	chat.join();
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
	assertEqual(0, chat.events.errors());
}


void AnthropicProviderTest::testConcurrentCalls()
{
	// Two calls share the provider; cancelling one leaves the other running.
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.pauseAt = response.body.find("event: content_block_stop");
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	ChatThread first(provider, userRequest("one"));
	ChatThread second(provider, userRequest("two"));
	assertTrue(server.waitForRequests(2, 10000));
	first.cancellation.cancel();
	first.join();
	assertTrue(first.result.stopReason == StopReason::CANCELLED);
	assertTrue(!second.finished.load());
	second.cancellation.cancel();
	second.join();
	assertTrue(second.result.stopReason == StopReason::CANCELLED);
}


void AnthropicProviderTest::testCancelStress()
{
	// A cancel that comes at any moment ends the call quietly, whatever
	// phase it hits: before the request, while connecting and sending, while
	// waiting for the headers or while reading the stream.
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.pauseAt = response.body.find("event: content_block_stop");
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	for (int i = 0; i < 100; ++i)
	{
		ChatThread chat(provider, userRequest("Hi"));
		chat.cancellation.cancel();
		chat.join();
		assertTrue(chat.result.stopReason == StopReason::CANCELLED);
		assertTrue(chat.error.empty());
		assertEqual(0, chat.events.errors());
	}
}


void AnthropicProviderTest::setUp()
{
}


void AnthropicProviderTest::tearDown()
{
}


CppUnit::Test* AnthropicProviderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AnthropicProviderTest");

	CppUnit_addTest(pSuite, AnthropicProviderTest, testRequestBody);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testEmptySystemPrompt);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testURIJoin);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testStreamedText);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testStreamedToolUse);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testThinkingRoundTrip);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCutStream);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testErrorEvent);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testHttpErrors);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testMalformedToolInput);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testAppendToolResult);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testListModels);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCancelBeforeHeaders);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCancelDuringStream);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testConcurrentCalls);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCancelStress);

	return pSuite;
}
