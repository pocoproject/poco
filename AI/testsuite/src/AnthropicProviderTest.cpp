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
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Event.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Format.h"
#include "Poco/Timestamp.h"
#include <atomic>
#include <fstream>
#include <memory>
#include <sstream>
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


#if defined(__linux__)
	bool connecting(Poco::UInt16 port)
		/// Returns true if the kernel lists a connection to the loopback port
		/// as SYN_SENT: a connect under way that nobody answers.
	{
		std::ifstream table("/proc/net/tcp");
		const std::string remote = Poco::format(":%04X", static_cast<unsigned>(port));
		std::string line;
		while (std::getline(table, line))
		{
			std::istringstream fields(line);
			std::string slot, local, peer, state;
			fields >> slot >> local >> peer >> state;
			if (state == "02" && peer.size() > remote.size() && peer.compare(peer.size() - remote.size(), remote.size(), remote) == 0) return true;
		}
		return false;
	}
#endif


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
		Poco::Event textArrived;
			/// Set once a piece of text has arrived.

		ContentEventCallback callback()
		{
			return [this](const ContentEvent& event)
			{
				events.push_back(event);
				if (event.type == ContentEvent::Type::TEXT) textArrived.set();
			};
		}

		int errors() const
		{
			int n = 0;
			for (const ContentEvent& event: events) if (event.type == ContentEvent::Type::FAILURE) ++n;
			return n;
		}

		const ContentEvent* firstError() const
		{
			for (const ContentEvent& event: events) if (event.type == ContentEvent::Type::FAILURE) return &event;
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
	// The last block of the last message carries a cache marker too; the
	// caller's message is not changed for it.
	Object::Ptr pLastBlock = pBody->getArray("messages")->getObject(0)->getArray("content")->getObject(0);
	assertEqual(std::string("Hi"), pLastBlock->getValue<std::string>("text"));
	assertEqual(std::string("ephemeral"), pLastBlock->getObject("cache_control")->getValue<std::string>("type"));
	assertEqual(std::string("Hi"), request.messages.getObject(0)->getValue<std::string>("content"));
	assertEqual(16000, AnthropicProvider::Params().maxTokens);
}


void AnthropicProviderTest::testCacheMarkers()
{
	CannedServer server(SSE, textStream());
	AnthropicProvider provider(params(server.baseUrl()));
	Cancellation cancellation;

	// On a tool result that ends the conversation.
	ChatRequest request = userRequest("Weather?");
	Object::Ptr pTurn = parse("{\"role\":\"assistant\",\"content\":[{\"type\":\"tool_use\",\"id\":\"t1\",\"name\":\"weather\",\"input\":{}}]}");
	request.messages.add(pTurn);
	Poco::AI::MCP::ToolResult toolResult;
	toolResult.addText("Sunny");
	provider.appendToolResult(request.messages, "t1", toolResult);
	(void) provider.chat(request, ContentEventCallback(), cancellation);
	Array::Ptr pMessages = parse(server.request())->getArray("messages");
	Object::Ptr pResult = pMessages->getObject(2)->getArray("content")->getObject(0);
	assertEqual(std::string("tool_result"), pResult->getValue<std::string>("type"));
	assertTrue(pResult->has("cache_control"));
	assertTrue(!pMessages->getObject(1)->getArray("content")->getObject(0)->has("cache_control"));
	assertTrue(!request.messages.getObject(2)->getArray("content")->getObject(0)->has("cache_control"));

	// Not on a thinking block.
	ChatRequest thinking = userRequest("Hm");
	thinking.messages.add(parse("{\"role\":\"assistant\",\"content\":[{\"type\":\"thinking\",\"thinking\":\"x\",\"signature\":\"s\"}]}"));
	(void) provider.chat(thinking, ContentEventCallback(), cancellation);
	Object::Ptr pThinking = parse(server.request())->getArray("messages")->getObject(1)->getArray("content")->getObject(0);
	assertTrue(!pThinking->has("cache_control"));
}


void AnthropicProviderTest::testRequestMembersAndHeaders()
{
	CannedServer server(SSE, textStream());
	AnthropicProvider::Params settings = params(server.baseUrl());
	settings.requestMembers = parse("{\"thinking\":{\"type\":\"adaptive\"},\"max_tokens\":32000}");
	settings.headers.set("anthropic-beta", "test-beta");
	AnthropicProvider provider(settings);
	Cancellation cancellation;
	(void) provider.chat(userRequest("Hi"), ContentEventCallback(), cancellation);
	Object::Ptr pBody = parse(server.request());
	assertEqual(std::string("adaptive"), pBody->getObject("thinking")->getValue<std::string>("type"));
	assertEqual(32000, pBody->getValue<int>("max_tokens"));
	assertEqual(std::string("test-beta"), server.header("anthropic-beta"));
	// The settings' object is not the one that went out.
	assertTrue(!settings.requestMembers->has("model"));
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

	assertTrue(result.stopReason == StopReason::FAILURE);
	assertTrue(result.toolCalls.empty());
	assertTrue(result.assistantMessage.isNull());
	assertEqual(1, events.errors());
	assertTrue(events.firstError()->code == FailureCode::STREAM_INTERRUPTED);
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
	assertTrue(result.stopReason == StopReason::FAILURE);
	assertTrue(events.firstError()->code == FailureCode::OVERLOADED);
	assertTrue(events.firstError()->text.find("Overloaded") != std::string::npos);
}


void AnthropicProviderTest::testHttpErrors()
{
	struct Case
	{
		int status;
		FailureCode code;
	};
	const Case cases[] = {
		{401, FailureCode::UNAUTHENTICATED},
		{403, FailureCode::UNAUTHENTICATED},
		{402, FailureCode::QUOTA_EXCEEDED},
		{429, FailureCode::RATE_LIMITED},
		{503, FailureCode::OVERLOADED},
		{500, FailureCode::SERVER_ERROR},
		{400, FailureCode::REQUEST_REJECTED}
	};
	for (const Case& c: cases)
	{
		// The excerpt of the body is cut at a character boundary.
		CannedServer::Response response = CannedServer::makeResponse(c.status, "application/json", std::string(499, 'x') + "\xC3\xA9" + "rest");
		response.headers.emplace_back("retry-after", "7");
		response.headers.emplace_back("request-id", "req_123");
		CannedServer server(response);
		AnthropicProvider provider(params(server.baseUrl()));
		Events events;
		Cancellation cancellation;
		ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);
		assertTrue(result.stopReason == StopReason::FAILURE);
		assertTrue(events.firstError()->code == c.code);
		const std::string text = events.firstError()->text;
		assertTrue(text.find(std::to_string(c.status)) != std::string::npos);
		assertTrue(text.size() >= 502);
		assertTrue(text.compare(text.size() - 502, 502, std::string(499, 'x') + "...") == 0);
		assertEqual(7, events.firstError()->retryAfter.totalSeconds());
		assertEqual(std::string("req_123"), events.firstError()->requestId);
	}

	// What a server asks to wait is taken up to a day, in seconds and as an
	// HTTP date.
	const auto retryAfter = [](const std::string& value)
	{
		CannedServer::Response response = CannedServer::makeResponse(429, "application/json", "{}");
		response.headers.emplace_back("retry-after", value);
		CannedServer server(response);
		AnthropicProvider provider(params(server.baseUrl()));
		Events events;
		Cancellation cancellation;
		(void) provider.chat(userRequest("Hi"), events.callback(), cancellation);
		return events.firstError()->retryAfter;
	};
	const Poco::Timespan day(1, 0, 0, 0, 0);
	assertTrue(retryAfter("99999999999999999999") == Poco::Timespan());
	assertTrue(retryAfter("9223372036854775807") == day);
	assertTrue(retryAfter(Poco::DateTimeFormatter::format(Poco::Timestamp() + 2*Poco::Timespan::HOURS, Poco::DateTimeFormat::HTTP_FORMAT)) > Poco::Timespan(0, 1, 59, 0, 0));
	assertTrue(retryAfter(Poco::DateTimeFormatter::format(Poco::DateTime(2999, 1, 1), Poco::DateTimeFormat::HTTP_FORMAT)) == day);
	assertTrue(retryAfter(Poco::DateTimeFormatter::format(Poco::DateTime(2000, 1, 1), Poco::DateTimeFormat::HTTP_FORMAT)) == Poco::Timespan());

	// The spend cap of the usage tier is a 429 of the type of a rate limit,
	// without retry-after, that fails until the next month: no rate limit,
	// as waiting does not help.
	CannedServer quota(CannedServer::makeResponse(429, "application/json", "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"You have reached your API usage limits.\",\"details\":{\"error_code\":\"enforced_spend_limit_reached\"}},\"request_id\":\"req_1\"}"));
	AnthropicProvider quotaProvider(params(quota.baseUrl()));
	Events quotaEvents;
	Cancellation cancellation;
	(void) quotaProvider.chat(userRequest("Hi"), quotaEvents.callback(), cancellation);
	assertTrue(quotaEvents.firstError()->code == FailureCode::QUOTA_EXCEEDED);
	assertTrue(quotaEvents.firstError()->retryAfter == Poco::Timespan());

	// The same type without the details is a rate limit.
	CannedServer rateLimit(CannedServer::makeResponse(429, "application/json", "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"Number of requests has exceeded your rate limit.\"}}"));
	AnthropicProvider rateLimitProvider(params(rateLimit.baseUrl()));
	Events rateLimitEvents;
	Cancellation rateLimitCancellation;
	(void) rateLimitProvider.chat(userRequest("Hi"), rateLimitEvents.callback(), rateLimitCancellation);
	assertTrue(rateLimitEvents.firstError()->code == FailureCode::RATE_LIMITED);

	// A spend limit the organization set itself is a 400, a rejected
	// request whose text says why.
	CannedServer ownLimit(CannedServer::makeResponse(400, "application/json", "{\"type\":\"error\",\"error\":{\"type\":\"invalid_request_error\",\"message\":\"You have reached your specified API usage limits.\"}}"));
	AnthropicProvider ownLimitProvider(params(ownLimit.baseUrl()));
	Events ownLimitEvents;
	Cancellation ownLimitCancellation;
	(void) ownLimitProvider.chat(userRequest("Hi"), ownLimitEvents.callback(), ownLimitCancellation);
	assertTrue(ownLimitEvents.firstError()->code == FailureCode::REQUEST_REJECTED);
	assertTrue(ownLimitEvents.firstError()->text.find("You have reached your specified API usage limits.") != std::string::npos);
}


void AnthropicProviderTest::testMalformedToolInput()
{
	CannedServer server(SSE, toolStream("{\\\"city\\\": ", "["));
	AnthropicProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);
	assertTrue(result.toolCalls.empty());
	assertTrue(events.firstError()->code == FailureCode::MALFORMED_TOOL_INPUT);
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
	catch (const Poco::Net::HTTPException& exc)
	{
		assertEqual(401, exc.code());
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
	// The stream stops after its text; the cancel comes while the call waits
	// for the rest.
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.pauseAt = response.body.find("event: content_block_stop");
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	ChatThread chat(provider, userRequest("Hi"));
	assertTrue(chat.events.textArrived.tryWait(10000));
	chat.cancellation.cancel();
	chat.join();
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
	assertEqual(0, chat.events.errors());
	// What arrived before the cancel stays reported: the first piece at
	// least, the second too if it came before the cancel.
	const std::string text = chat.events.text();
	assertTrue(text == "Hel" || text == "Hello");
}


void AnthropicProviderTest::testCancelDuringUpload()
{
	// The server reads nothing of a large request: the cancel ends the call
	// while it is still sending.
	CannedServer::Response response;
	response.stallBeforeBody = true;
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	ChatThread chat(provider, userRequest(std::string(16*1024*1024, 'x')));
	assertTrue(server.waitForRequest(10000));
	const Poco::Timestamp cancelled;
	chat.cancellation.cancel();
	chat.join();
	assertTrue(cancelled.elapsed() < 5 * Poco::Timestamp::resolution());
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
}


void AnthropicProviderTest::testCancelDuringConnect()
{
#if defined(__linux__)
	// A listener whose queue is full drops further connection requests, so
	// a connect to it hangs: the cancel ends it, not the timeout.
	Poco::Net::ServerSocket listener(Poco::Net::SocketAddress("127.0.0.1", 0), 0);
	Poco::Net::StreamSocket queued;
	queued.connect(listener.address());
	const Poco::UInt16 port = listener.address().port();
	AnthropicProvider::Params settings = params("http://127.0.0.1:" + std::to_string(port) + "/v1");
	settings.timeout = Poco::Timespan(20, 0);
	AnthropicProvider provider(settings);
	ChatThread chat(provider, userRequest("Hi"));
	const Poco::Timestamp started;
	bool hanging = false;
	while (!(hanging = connecting(port)) && started.elapsed() < 10 * Poco::Timestamp::resolution())
	{
		Poco::Thread::sleep(1);
	}
	const Poco::Timestamp cancelled;
	chat.cancellation.cancel();
	chat.join();
	// Where the kernel takes the connection all the same, no connect hangs.
	if (!hanging) return;
	assertTrue(cancelled.elapsed() < 5 * Poco::Timestamp::resolution());
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
#endif
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
	// phase it hits. The iterations take turns: at once, before the request;
	// once the server has the request, while the call waits for the head;
	// after a little spinning from there, which lands anywhere around the
	// head and the first events; and after the first text, while the call
	// reads the stream.
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.pauseAt = response.body.find("event: content_block_stop");
	CannedServer server(response);
	AnthropicProvider provider(params(server.baseUrl()));
	int afterText = 0;
	for (int i = 0; i < 100; ++i)
	{
		const int requestsBefore = server.requests();
		ChatThread chat(provider, userRequest("Hi"));
		switch (i % 4)
		{
		case 1:
			assertTrue(server.waitForRequests(requestsBefore + 1, 10000));
			break;
		case 2:
			{
				assertTrue(server.waitForRequests(requestsBefore + 1, 10000));
				volatile unsigned spin = 0;
				for (unsigned j = 0; j < static_cast<unsigned>((i * 7919) % 200000); ++j) spin = spin + 1;
			}
			break;
		case 3:
			assertTrue(chat.events.textArrived.tryWait(10000));
			++afterText;
			break;
		default:
			break;
		}
		chat.cancellation.cancel();
		chat.join();
		assertTrue(chat.result.stopReason == StopReason::CANCELLED);
		assertTrue(chat.error.empty());
		assertEqual(0, chat.events.errors());
	}
	// Three iterations in four reached the server, a quarter read text first.
	assertTrue(server.requests() >= 75);
	assertEqual(25, afterText);
}


void AnthropicProviderTest::testCutToolCall()
{
	// The token limit cut a tool call before any of its input: the call is
	// returned with no input, and the stop reason says the answer is cut.
	const std::string stream = sse("message_start", "{\"type\":\"message_start\",\"message\":{\"usage\":{\"input_tokens\":3}}}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"tool_use\",\"id\":\"toolu_cut\",\"name\":\"weather\",\"input\":{}}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
		+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"max_tokens\"},\"usage\":{\"output_tokens\":100}}")
		+ sse("message_stop", "{\"type\":\"message_stop\"}");
	CannedServer server(SSE, stream);
	AnthropicProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), ContentEventCallback(), cancellation);
	assertTrue(result.stopReason == StopReason::MAX_TOKENS);
	assertEqual(1u, static_cast<unsigned>(result.toolCalls.size()));
	assertEqual(0u, static_cast<unsigned>(result.toolCalls[0].input->size()));

	// A context window that ran out cuts the answer as well.
	std::string window = stream;
	window.replace(window.find("max_tokens"), 10, "model_context_window_exceeded");
	CannedServer windowServer(SSE, window);
	AnthropicProvider windowProvider(params(windowServer.baseUrl()));
	assertTrue(windowProvider.chat(userRequest("Weather?"), ContentEventCallback(), cancellation).stopReason == StopReason::MAX_TOKENS);
}


void AnthropicProviderTest::testThinkingSignatures()
{
	// A thinking block shown with its display omitted has a signature and no
	// text: it is kept. One without a signature is not.
	const std::string stream = sse("message_start", "{\"type\":\"message_start\",\"message\":{\"usage\":{\"input_tokens\":3}}}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"\"}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"signature_delta\",\"signature\":\"sig-only\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":1,\"content_block\":{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"\"}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":1,\"delta\":{\"type\":\"thinking_delta\",\"thinking\":\"unsigned\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":1}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":2,\"content_block\":{\"type\":\"text\",\"text\":\"\"}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":2,\"delta\":{\"type\":\"text_delta\",\"text\":\"Done.\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":2}")
		+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"end_turn\"},\"usage\":{\"output_tokens\":9}}")
		+ sse("message_stop", "{\"type\":\"message_stop\"}");
	CannedServer server(SSE, stream);
	AnthropicProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Think"), ContentEventCallback(), cancellation);
	Array::Ptr pContent = result.assistantMessage->getArray("content");
	assertEqual(2u, static_cast<unsigned>(pContent->size()));
	assertEqual(std::string("thinking"), pContent->getObject(0)->getValue<std::string>("type"));
	assertEqual(std::string(""), pContent->getObject(0)->getValue<std::string>("thinking"));
	assertEqual(std::string("sig-only"), pContent->getObject(0)->getValue<std::string>("signature"));
	assertEqual(std::string("text"), pContent->getObject(1)->getValue<std::string>("type"));
}


void AnthropicProviderTest::testBlocksAsTheyCame()
{
	// Citations, a server tool and its result, and the members of a tool
	// input in their order: the turn holds them as they came. A text block
	// with nothing but whitespace is left out, as the API refuses it back.
	const std::string stream = sse("message_start", "{\"type\":\"message_start\",\"message\":{\"usage\":{\"input_tokens\":3}}}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"server_tool_use\",\"id\":\"srvtoolu_1\",\"name\":\"web_search\",\"input\":{}}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"input_json_delta\",\"partial_json\":\"{\\\"query\\\":\\\"poco\\\"}\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":1,\"content_block\":{\"type\":\"web_search_tool_result\",\"tool_use_id\":\"srvtoolu_1\",\"content\":[{\"type\":\"web_search_result\",\"url\":\"https://pocoproject.org\"}]}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":1}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":2,\"content_block\":{\"type\":\"text\",\"text\":\"\",\"citations\":[]}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":2,\"delta\":{\"type\":\"citations_delta\",\"citation\":{\"type\":\"web_search_result_location\",\"url\":\"https://pocoproject.org\"}}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":2,\"delta\":{\"type\":\"text_delta\",\"text\":\"POCO\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":2}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":3,\"content_block\":{\"type\":\"text\",\"text\":\"\"}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":3,\"delta\":{\"type\":\"text_delta\",\"text\":\" \\n\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":3}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":4,\"content_block\":{\"type\":\"tool_use\",\"id\":\"toolu_2\",\"name\":\"weather\",\"input\":{}}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":4,\"delta\":{\"type\":\"input_json_delta\",\"partial_json\":\"{\\\"zeta\\\":1,\\\"alpha\\\":2}\"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":4}")
		+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"tool_use\"},\"usage\":{\"output_tokens\":9}}")
		+ sse("message_stop", "{\"type\":\"message_stop\"}");
	CannedServer server(SSE, stream);
	AnthropicProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Search"), ContentEventCallback(), cancellation);

	Array::Ptr pContent = result.assistantMessage->getArray("content");
	assertEqual(4u, static_cast<unsigned>(pContent->size()));
	assertEqual(std::string("server_tool_use"), pContent->getObject(0)->getValue<std::string>("type"));
	assertEqual(std::string("poco"), pContent->getObject(0)->getObject("input")->getValue<std::string>("query"));
	assertEqual(std::string("web_search_tool_result"), pContent->getObject(1)->getValue<std::string>("type"));
	assertEqual(std::string("https://pocoproject.org"), pContent->getObject(1)->getArray("content")->getObject(0)->getValue<std::string>("url"));
	assertEqual(std::string("POCO"), pContent->getObject(2)->getValue<std::string>("text"));
	assertEqual(1u, static_cast<unsigned>(pContent->getObject(2)->getArray("citations")->size()));
	// Only the tool of the client is a call to run.
	assertEqual(1u, static_cast<unsigned>(result.toolCalls.size()));
	assertEqual(std::string("toolu_2"), result.toolCalls[0].id);
	std::ostringstream sent;
	pContent->getObject(3)->getObject("input")->stringify(sent);
	assertEqual(std::string("{\"zeta\":1,\"alpha\":2}"), sent.str());
}


void AnthropicProviderTest::testRefusal()
{
	// A refused answer is not kept, and the explanation comes with the result.
	const std::string stream = sse("message_start", "{\"type\":\"message_start\",\"message\":{\"usage\":{\"input_tokens\":3}}}")
		+ sse("content_block_start", "{\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"text\",\"text\":\"\"}}")
		+ sse("content_block_delta", "{\"type\":\"content_block_delta\",\"index\":0,\"delta\":{\"type\":\"text_delta\",\"text\":\"Here is how to \"}}")
		+ sse("content_block_stop", "{\"type\":\"content_block_stop\",\"index\":0}")
		+ sse("message_delta", "{\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"refusal\",\"stop_details\":{\"type\":\"refusal\",\"explanation\":\"Declined by policy.\"}},\"usage\":{\"output_tokens\":4}}")
		+ sse("message_stop", "{\"type\":\"message_stop\"}");
	CannedServer server(SSE, stream);
	AnthropicProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Something"), ContentEventCallback(), cancellation);
	assertTrue(result.stopReason == StopReason::REFUSAL);
	assertTrue(result.assistantMessage.isNull());
	assertTrue(result.toolCalls.empty());
	assertEqual(std::string("Declined by policy."), result.explanation);
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
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCacheMarkers);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testRequestMembersAndHeaders);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCancelDuringUpload);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCancelDuringConnect);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testCutToolCall);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testThinkingSignatures);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testBlocksAsTheyCame);
	CppUnit_addTest(pSuite, AnthropicProviderTest, testRefusal);

	return pSuite;
}
