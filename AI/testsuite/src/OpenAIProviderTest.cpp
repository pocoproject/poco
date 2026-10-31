//
// OpenAIProviderTest.cpp
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OpenAIProviderTest.h"
#include "CannedServer.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/OpenAIProvider.h"
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Event.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include <atomic>
#include <sstream>
#include <string>
#include <vector>


using namespace Poco::AI;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	const std::string SSE("text/event-stream");


	std::string data(const std::string& json)
	{
		return "data: " + json + "\n\n";
	}


	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	OpenAIProvider::Params params(const std::string& baseUrl)
	{
		OpenAIProvider::Params params;
		params.baseURI = Poco::URI(baseUrl);
		params.apiKey = "sk-test";
		params.model = "test-model";
		params.maxTokens = 50;
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


	std::string textStream(bool done = true, bool finish = true)
	{
		// OpenAI sends a null refusal with the first piece of an answer.
		std::string stream = data("{\"choices\":[{\"index\":0,\"delta\":{\"role\":\"assistant\",\"content\":\"Hel\",\"refusal\":null},\"finish_reason\":null}]}")
			+ data("{\"choices\":[{\"index\":0,\"delta\":{\"content\":\"lo\"},\"finish_reason\":null}]}");
		if (finish)
		{
			// With include_usage, the usage comes in a chunk of its own, with
			// no choices, after the one with the finish reason.
			stream += data("{\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"stop\"}],\"usage\":null}");
			stream += data("{\"choices\":[],\"usage\":{\"prompt_tokens\":11,\"completion_tokens\":2,\"prompt_tokens_details\":{\"cached_tokens\":3}}}");
		}
		if (done) stream += "data: [DONE]\n\n";
		return stream;
	}


	std::string toolStream(const std::string& arguments = "\\\"Paris\\\"}")
	{
		return data("{\"choices\":[{\"index\":0,\"delta\":{\"role\":\"assistant\",\"content\":null,\"tool_calls\":[{\"index\":0,\"id\":\"call_1\",\"type\":\"function\",\"function\":{\"name\":\"weather\",\"arguments\":\"\"}}]},\"finish_reason\":null}]}")
			+ data("{\"choices\":[{\"index\":0,\"delta\":{\"tool_calls\":[{\"index\":0,\"id\":null,\"function\":{\"name\":null,\"arguments\":\"{\\\"city\\\":\"}}]},\"finish_reason\":null}]}")
			+ data("{\"choices\":[{\"index\":0,\"delta\":{\"tool_calls\":[{\"index\":0,\"id\":\"\",\"function\":{\"arguments\":\"" + arguments + "\"}}]},\"finish_reason\":null}]}")
			+ data("{\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"tool_calls\"}]}")
			+ "data: [DONE]\n\n";
	}


	struct ChatThread
		/// Runs one chat call on a thread of its own.
	{
		ChatThread(OpenAIProvider& provider, ChatRequest request):
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

	private:
		OpenAIProvider& _provider;
		ChatRequest _request;
		Poco::Thread _thread;
	};
}


OpenAIProviderTest::OpenAIProviderTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


OpenAIProviderTest::~OpenAIProviderTest()
{
}


void OpenAIProviderTest::testRequestBody()
{
	CannedServer server(SSE, textStream());
	OpenAIProvider::Params settings = params(server.baseUrl());
	OpenAIProvider provider(settings);
	ChatRequest request = userRequest("Hi", "Be brief.");
	request.tools.push_back(weatherTool());
	request.toolChoice = ToolChoice::NONE;
	Cancellation cancellation;
	(void) provider.chat(request, ContentEventCallback(), cancellation);

	assertEqual(std::string("/v1/chat/completions"), server.path());
	assertEqual(std::string("Bearer sk-test"), server.header("Authorization"));
	Object::Ptr pBody = parse(server.request());
	assertEqual(50, pBody->getValue<int>("max_completion_tokens"));
	assertTrue(!pBody->has("max_tokens"));
	assertTrue(pBody->getObject("stream_options")->getValue<bool>("include_usage"));
	Array::Ptr pMessages = pBody->getArray("messages");
	assertEqual(std::string("system"), pMessages->getObject(0)->getValue<std::string>("role"));
	assertEqual(std::string("Be brief."), pMessages->getObject(0)->getValue<std::string>("content"));
	assertEqual(std::string("user"), pMessages->getObject(1)->getValue<std::string>("role"));
	Object::Ptr pTool = pBody->getArray("tools")->getObject(0);
	assertEqual(std::string("function"), pTool->getValue<std::string>("type"));
	assertEqual(std::string("weather"), pTool->getObject("function")->getValue<std::string>("name"));
	assertTrue(pTool->getObject("function")->getObject("parameters")->isObject("properties"));
	assertEqual(std::string("none"), pBody->getValue<std::string>("tool_choice"));

	// The deprecated field for a server that knows only it, and no usage
	// option for one that refuses it.
	settings.maxTokensField = "max_tokens";
	settings.streamUsage = false;
	OpenAIProvider older(settings);
	(void) older.chat(userRequest("Hi"), ContentEventCallback(), cancellation);
	Object::Ptr pOlder = parse(server.request());
	assertEqual(50, pOlder->getValue<int>("max_tokens"));
	assertTrue(!pOlder->has("max_completion_tokens"));
	assertTrue(!pOlder->has("stream_options"));
}


void OpenAIProviderTest::testRequestMembersAndHeaders()
{
	CannedServer server(SSE, textStream());
	OpenAIProvider::Params settings = params(server.baseUrl());
	settings.requestMembers = parse("{\"reasoning_effort\":\"low\",\"parallel_tool_calls\":false}");
	settings.headers.set("OpenAI-Organization", "org-test");
	OpenAIProvider provider(settings);
	Cancellation cancellation;
	(void) provider.chat(userRequest("Hi"), ContentEventCallback(), cancellation);
	Object::Ptr pBody = parse(server.request());
	assertEqual(std::string("low"), pBody->getValue<std::string>("reasoning_effort"));
	assertTrue(!pBody->getValue<bool>("parallel_tool_calls"));
	assertTrue(pBody->getValue<bool>("stream"));
	assertEqual(std::string("org-test"), server.header("OpenAI-Organization"));
	assertTrue(!settings.requestMembers->has("model"));

	// The members the provider builds itself are refused.
	const std::vector<std::string> built = {"model", "messages", "max_tokens", "max_completion_tokens", "stream", "stream_options", "tools", "tool_choice"};
	for (const std::string& name: built)
	{
		OpenAIProvider::Params reserved = params(server.baseUrl());
		reserved.requestMembers = new Object;
		reserved.requestMembers->set(name, 1);
		try
		{
			OpenAIProvider refused(reserved);
			fail("a member the provider builds must be refused: " + name);
		}
		catch (const Poco::InvalidArgumentException& exc)
		{
			assertMessageEqual("The provider builds this request member itself: " + name, exc.message());
		}
	}
}


void OpenAIProviderTest::testNoTools()
{
	CannedServer server(SSE, textStream());
	OpenAIProvider::Params settings = params(server.baseUrl());
	settings.supportsTools = false;
	settings.apiKey.clear();
	settings.maxTokens = 0;
	OpenAIProvider provider(settings);
	ChatRequest request = userRequest("Hi");
	request.tools.push_back(weatherTool());
	Cancellation cancellation;
	(void) provider.chat(request, ContentEventCallback(), cancellation);
	Object::Ptr pBody = parse(server.request());
	assertTrue(!pBody->has("tools"));
	assertTrue(!pBody->has("tool_choice"));
	assertTrue(!pBody->has("max_tokens"));
	assertEqual(1u, static_cast<unsigned>(pBody->getArray("messages")->size()));
	assertTrue(server.header("Authorization").empty());
}


void OpenAIProviderTest::testStreamedText()
{
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.chunked = true;
	CannedServer server(response);
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);
	assertEqual(std::string("Hello"), events.text());
	assertTrue(events.firstError() == nullptr);
	assertTrue(result.stopReason == StopReason::END_TURN);
	assertEqual(std::string("stop"), result.stopDetail);
	assertEqual(Poco::UInt64(11), result.usage.inputTokens);
	assertEqual(Poco::UInt64(2), result.usage.outputTokens);
	assertEqual(Poco::UInt64(3), result.usage.cachedInputTokens);
	assertEqual(std::string("Hello"), result.assistantMessage->getValue<std::string>("content"));
	assertTrue(!result.assistantMessage->has("tool_calls"));
}


void OpenAIProviderTest::testStreamedToolCalls()
{
	CannedServer server(SSE, toolStream());
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);
	// Null and empty ids and names of later chunks keep those of the first.
	assertEqual(1u, static_cast<unsigned>(result.toolCalls.size()));
	assertEqual(std::string("call_1"), result.toolCalls[0].id);
	assertEqual(std::string("weather"), result.toolCalls[0].name);
	assertEqual(std::string("Paris"), result.toolCalls[0].input->getValue<std::string>("city"));
	assertTrue(result.stopReason == StopReason::TOOL_USE);
	assertTrue(events.firstError() == nullptr);
	assertTrue(result.assistantMessage->isNull("content"));
	Object::Ptr pCall = result.assistantMessage->getArray("tool_calls")->getObject(0);
	assertEqual(std::string("call_1"), pCall->getValue<std::string>("id"));
	assertEqual(std::string("{\"city\":\"Paris\"}"), pCall->getObject("function")->getValue<std::string>("arguments"));
}


void OpenAIProviderTest::testFinishWithoutDone()
{
	// A finish reason ends the answer, even without [DONE].
	CannedServer server(SSE, textStream(false, true));
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);
	assertTrue(result.stopReason == StopReason::END_TURN);
	assertTrue(events.firstError() == nullptr);
}


void OpenAIProviderTest::testCutStream()
{
	// Neither a finish reason nor [DONE]: the answer is incomplete.
	std::string stream = toolStream();
	stream = stream.substr(0, stream.find("data: {\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"tool_calls\""));
	CannedServer server(SSE, stream);
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);
	assertTrue(result.stopReason == StopReason::FAILURE);
	assertTrue(result.toolCalls.empty());
	assertTrue(result.assistantMessage.isNull());
	assertTrue(events.firstError()->code == FailureCode::STREAM_INTERRUPTED);
	assertEqual(1u, static_cast<unsigned>(events.events.size()));

	// A call cut right after the header of a tool call does not run the tool with no arguments.
	const std::string header = data("{\"choices\":[{\"index\":0,\"delta\":{\"tool_calls\":[{\"index\":0,\"id\":\"call_9\",\"function\":{\"name\":\"weather\",\"arguments\":\"\"}}]},\"finish_reason\":null}]}");
	CannedServer cutServer(SSE, header);
	OpenAIProvider cutProvider(params(cutServer.baseUrl()));
	ChatResult cut = cutProvider.chat(userRequest("Weather?"), ContentEventCallback(), cancellation);
	assertTrue(cut.toolCalls.empty());
	assertTrue(cut.stopReason == StopReason::FAILURE);
}


void OpenAIProviderTest::testErrorInStream()
{
	const std::string stream = data("{\"choices\":[{\"index\":0,\"delta\":{\"content\":\"Par\"}}]}")
		+ data("{\"error\":{\"message\":\"model is overloaded\",\"type\":\"server_error\"}}");
	CannedServer server(SSE, stream);
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Hi"), events.callback(), cancellation);
	assertTrue(result.stopReason == StopReason::FAILURE);
	assertTrue(events.firstError()->code == FailureCode::STREAM_ERROR);
	assertTrue(events.firstError()->text.find("model is overloaded") != std::string::npos);

	// A null error, which a gateway sends with every chunk, is none.
	const std::string nullErrors = data("{\"choices\":[{\"index\":0,\"delta\":{\"content\":\"Hi\"},\"finish_reason\":null}],\"error\":null}")
		+ data("{\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"stop\"}],\"error\":null}")
		+ data("[DONE]");
	CannedServer nullServer(SSE, nullErrors);
	OpenAIProvider nullProvider(params(nullServer.baseUrl()));
	Events answered;
	ChatResult answer = nullProvider.chat(userRequest("Hi"), answered.callback(), cancellation);
	assertTrue(answer.stopReason == StopReason::END_TURN);
	assertEqual(std::string("Hi"), answer.text);
	assertTrue(answered.firstError() == nullptr);
}


void OpenAIProviderTest::testHttpErrors()
{
	struct Case
	{
		int status;
		FailureCode code;
	};
	const Case cases[] = {
		{401, FailureCode::UNAUTHENTICATED},
		{429, FailureCode::RATE_LIMITED},
		{503, FailureCode::OVERLOADED},
		{502, FailureCode::SERVER_ERROR},
		{404, FailureCode::REQUEST_REJECTED}
	};
	for (const Case& c: cases)
	{
		// The old retry without tools on one server's wording is gone.
		CannedServer::Response response = CannedServer::makeResponse(c.status, "application/json", "{\"error\":\"Unsupported param: tools\"}");
		response.headers.emplace_back("retry-after-ms", "1500");
		response.headers.emplace_back("x-request-id", "req_abc");
		CannedServer server(response);
		OpenAIProvider provider(params(server.baseUrl()));
		ChatRequest request = userRequest("Hi");
		request.tools.push_back(weatherTool());
		Events events;
		Cancellation cancellation;
		ChatResult result = provider.chat(request, events.callback(), cancellation);
		assertTrue(result.stopReason == StopReason::FAILURE);
		assertTrue(events.firstError()->code == c.code);
		assertEqual(1, server.requests());
		assertEqual(Poco::Timespan::TimeDiff(1500), events.firstError()->retryAfter.totalMilliseconds());
		assertEqual(std::string("req_abc"), events.firstError()->requestId);
	}

	// What a server asks to wait in milliseconds is taken up to a day.
	CannedServer::Response longWait = CannedServer::makeResponse(429, "application/json", "{}");
	longWait.headers.emplace_back("retry-after-ms", "9223372036854775807");
	CannedServer longWaitServer(longWait);
	OpenAIProvider longWaitProvider(params(longWaitServer.baseUrl()));
	Events longWaitEvents;
	Cancellation longWaitCancellation;
	(void) longWaitProvider.chat(userRequest("Hi"), longWaitEvents.callback(), longWaitCancellation);
	assertTrue(longWaitEvents.firstError()->retryAfter == Poco::Timespan(1, 0, 0, 0, 0));

	// A 429 for a used-up quota is no rate limit: waiting does not help.
	CannedServer quota(CannedServer::makeResponse(429, "application/json", "{\"error\":{\"message\":\"quota\",\"type\":\"insufficient_quota\",\"code\":\"insufficient_quota\"}}"));
	OpenAIProvider quotaProvider(params(quota.baseUrl()));
	Events quotaEvents;
	Cancellation cancellation;
	(void) quotaProvider.chat(userRequest("Hi"), quotaEvents.callback(), cancellation);
	assertTrue(quotaEvents.firstError()->code == FailureCode::QUOTA_EXCEEDED);
}


void OpenAIProviderTest::testCutByLength()
{
	// The token limit cut a tool call before its arguments: the call comes
	// with no input and the stop reason says the answer is cut.
	const std::string stream = data("{\"choices\":[{\"index\":0,\"delta\":{\"role\":\"assistant\",\"tool_calls\":[{\"index\":0,\"id\":\"call_cut\",\"type\":\"function\",\"function\":{\"name\":\"weather\",\"arguments\":\"\"}}]},\"finish_reason\":null}]}")
		+ data("{\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"length\"}]}")
		+ "data: [DONE]\n\n";
	CannedServer server(SSE, stream);
	OpenAIProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), ContentEventCallback(), cancellation);
	assertTrue(result.stopReason == StopReason::MAX_TOKENS);
	assertEqual(std::string("length"), result.stopDetail);
	assertEqual(1u, static_cast<unsigned>(result.toolCalls.size()));
	assertEqual(0u, static_cast<unsigned>(result.toolCalls[0].input->size()));

	// Text cut by the limit is kept as far as it came.
	std::string text = textStream();
	text.replace(text.find("\"stop\""), 6, "\"length\"");
	CannedServer textServer(SSE, text);
	OpenAIProvider textProvider(params(textServer.baseUrl()));
	ChatResult cut = textProvider.chat(userRequest("Hi"), ContentEventCallback(), cancellation);
	assertTrue(cut.stopReason == StopReason::MAX_TOKENS);
	assertEqual(std::string("Hello"), cut.assistantMessage->getValue<std::string>("content"));
}


void OpenAIProviderTest::testArgumentsAsTheyCame()
{
	// The arguments go back in the turn as the model wrote them; the input
	// keeps the order of their members.
	CannedServer server(SSE, toolStream("\\\"Paris\\\", \\\"alpha\\\": 1}"));
	OpenAIProvider provider(params(server.baseUrl()));
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), ContentEventCallback(), cancellation);
	Object::Ptr pFunction = result.assistantMessage->getArray("tool_calls")->getObject(0)->getObject("function");
	assertEqual(std::string("{\"city\":\"Paris\", \"alpha\": 1}"), pFunction->getValue<std::string>("arguments"));
	std::ostringstream input;
	result.toolCalls[0].input->stringify(input);
	assertEqual(std::string("{\"city\":\"Paris\",\"alpha\":1}"), input.str());
}


void OpenAIProviderTest::testMalformedArguments()
{
	CannedServer server(SSE, toolStream("[1,"));
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Weather?"), events.callback(), cancellation);
	assertTrue(result.toolCalls.empty());
	assertTrue(events.firstError()->code == FailureCode::MALFORMED_TOOL_INPUT);
	assertTrue(!result.assistantMessage->has("tool_calls"));
}


void OpenAIProviderTest::testAppendToolResult()
{
	OpenAIProvider provider(params("http://127.0.0.1:1/v1"));
	Array messages;
	Poco::AI::MCP::ToolResult result;
	result.addText("line one").addText("line two");
	provider.appendToolResult(messages, "call_1", result);
	provider.appendToolResult(messages, "call_2", Poco::AI::MCP::ToolResult::error("failed"));
	assertEqual(2u, static_cast<unsigned>(messages.size()));
	assertEqual(std::string("tool"), messages.getObject(0)->getValue<std::string>("role"));
	assertEqual(std::string("call_1"), messages.getObject(0)->getValue<std::string>("tool_call_id"));
	assertEqual(std::string("line one\nline two"), messages.getObject(0)->getValue<std::string>("content"));
	assertEqual(std::string("failed"), messages.getObject(1)->getValue<std::string>("content"));
}


void OpenAIProviderTest::testListModels()
{
	CannedServer server("application/json", "{\"object\":\"list\",\"data\":[{\"id\":\"zeta\"},{\"id\":\"text-embedding-3\"},{\"id\":\"alpha\"},{\"id\":null}]}");
	OpenAIProvider provider(params(server.baseUrl()));
	std::vector<std::string> models = provider.listModels();
	assertEqual(2u, static_cast<unsigned>(models.size()));
	assertEqual(std::string("alpha"), models[0]);
	assertEqual(std::string("zeta"), models[1]);
	assertEqual(std::string("/v1/models"), server.path());

	CannedServer failing(CannedServer::makeResponse(500, "text/plain", "down"));
	OpenAIProvider down(params(failing.baseUrl()));
	try
	{
		(void) down.listModels();
		fail("a failed listing must throw");
	}
	catch (const Poco::Net::HTTPException& exc)
	{
		assertEqual(500, exc.code());
	}

	CannedServer malformed("application/json", "[1,2]");
	OpenAIProvider broken(params(malformed.baseUrl()));
	try
	{
		(void) broken.listModels();
		fail("a malformed listing must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void OpenAIProviderTest::testEmbeddings()
{
	CannedServer server("application/json",
		"{\"data\":[{\"index\":1,\"embedding\":[0.5,0.25]},{\"index\":0,\"embedding\":[1,2]}]}");
	OpenAIProvider provider(params(server.baseUrl()));
	std::vector<std::vector<float>> vectors = provider.embed({"first", "second"}, "embed-model", 2);
	assertEqual(2u, static_cast<unsigned>(vectors.size()));
	assertEqual(1.0f, vectors[0][0]);
	assertEqual(2.0f, vectors[0][1]);
	assertEqual(0.5f, vectors[1][0]);
	Object::Ptr pBody = parse(server.request());
	assertEqual(std::string("embed-model"), pBody->getValue<std::string>("model"));
	assertEqual(2, pBody->getValue<int>("dimensions"));
	assertEqual(2u, static_cast<unsigned>(pBody->getArray("input")->size()));
	assertTrue(provider.embed({}, "embed-model").empty());
}


void OpenAIProviderTest::testMalformedEmbeddings()
{
	const char* bodies[] = {
		"{\"data\":[{\"index\":0,\"embedding\":[1]}]}",
		"{\"data\":[{\"index\":0,\"embedding\":[1]},{\"index\":0,\"embedding\":[2]}]}",
		"{\"data\":[{\"index\":0,\"embedding\":[1e300]},{\"index\":1,\"embedding\":[2]}]}",
		"{\"data\":[{\"index\":0,\"embedding\":[1,2]},{\"index\":1,\"embedding\":[3]}]}",
		"{\"data\":[{\"index\":5,\"embedding\":[1]},{\"index\":1,\"embedding\":[2]}]}",
		"{\"data\":[{\"index\":0,\"embedding\":[]},{\"index\":1,\"embedding\":[2]}]}"
	};
	for (const char* body: bodies)
	{
		CannedServer server("application/json", body);
		OpenAIProvider provider(params(server.baseUrl()));
		try
		{
			(void) provider.embed({"a", "b"}, "m");
			fail(std::string("must be refused: ") + body);
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}
	CannedServer failing(CannedServer::makeResponse(400, "application/json", "{\"error\":\"bad\"}"));
	OpenAIProvider provider(params(failing.baseUrl()));
	try
	{
		(void) provider.embed({"a"}, "m");
		fail("an error status must throw");
	}
	catch (const Poco::Net::HTTPException&)
	{
	}
}


void OpenAIProviderTest::testRefusal()
{
	// A refusal streams in delta.refusal with a null content: the answer is
	// not kept, and the refusal comes as the explanation.
	const std::string stream = data("{\"choices\":[{\"index\":0,\"delta\":{\"role\":\"assistant\",\"content\":null,\"refusal\":\"\"},\"finish_reason\":null}]}")
		+ data("{\"choices\":[{\"index\":0,\"delta\":{\"refusal\":\"I can't help \"},\"finish_reason\":null}]}")
		+ data("{\"choices\":[{\"index\":0,\"delta\":{\"refusal\":\"with that.\"},\"finish_reason\":null}]}")
		+ data("{\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"stop\"}]}")
		+ "data: [DONE]\n\n";
	CannedServer server(SSE, stream);
	OpenAIProvider provider(params(server.baseUrl()));
	Events events;
	Cancellation cancellation;
	ChatResult result = provider.chat(userRequest("Something"), events.callback(), cancellation);
	assertTrue(result.stopReason == StopReason::REFUSAL);
	assertEqual(std::string("stop"), result.stopDetail);
	assertEqual(std::string("I can't help with that."), result.explanation);
	assertTrue(result.assistantMessage.isNull());
	assertTrue(result.toolCalls.empty());
	assertTrue(events.text().empty());
	assertTrue(events.firstError() == nullptr);
}


void OpenAIProviderTest::testCancelBeforeHeaders()
{
	CannedServer::Response response;
	response.waitBeforeHeaders = true;
	CannedServer server(response);
	OpenAIProvider provider(params(server.baseUrl()));
	ChatThread chat(provider, userRequest("Hi"));
	assertTrue(server.waitForRequest(10000));
	chat.cancellation.cancel();
	chat.join();
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
	assertTrue(chat.events.firstError() == nullptr);
}


void OpenAIProviderTest::testCancelDuringStream()
{
	// The stream stops after its first text; the cancel comes while the call
	// waits for the rest.
	CannedServer::Response response = CannedServer::makeResponse(200, SSE, textStream());
	response.pauseAt = response.body.find("data: {\"choices\":[{\"index\":0,\"delta\":{\"content\":\"lo\"}");
	CannedServer server(response);
	OpenAIProvider provider(params(server.baseUrl()));
	ChatThread chat(provider, userRequest("Hi"));
	assertTrue(chat.events.textArrived.tryWait(10000));
	chat.cancellation.cancel();
	chat.join();
	assertTrue(chat.result.stopReason == StopReason::CANCELLED);
	assertTrue(chat.error.empty());
	assertTrue(chat.events.firstError() == nullptr);
	assertEqual(std::string("Hel"), chat.events.text());
}


void OpenAIProviderTest::setUp()
{
}


void OpenAIProviderTest::tearDown()
{
}


CppUnit::Test* OpenAIProviderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OpenAIProviderTest");

	CppUnit_addTest(pSuite, OpenAIProviderTest, testRequestBody);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testNoTools);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testStreamedText);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testStreamedToolCalls);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testFinishWithoutDone);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testCutStream);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testErrorInStream);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testHttpErrors);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testMalformedArguments);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testAppendToolResult);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testListModels);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testEmbeddings);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testMalformedEmbeddings);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testCancelBeforeHeaders);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testCancelDuringStream);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testRefusal);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testRequestMembersAndHeaders);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testCutByLength);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testArgumentsAsTheyCame);

	return pSuite;
}
