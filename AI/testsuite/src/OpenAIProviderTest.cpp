//
// OpenAIProviderTest.cpp
//
// Tests for OpenAIProvider's pure-JSON helper methods
// (no network calls).
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
#include "Poco/JSON/Stringifier.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>
#include <vector>


using Poco::AI::OpenAIProvider;
using Poco::AI::ContentEvent;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace {


// Serves a canned /embeddings response; captures the request for assertions.
// Entries are returned in reversed index order to prove the client restores
// input order from the index field.
struct EmbeddingsMock
{
	std::string lastPath;
	std::string lastBody;
	bool respondError = false;
};


class EmbeddingsMockHandler: public Poco::Net::HTTPRequestHandler
{
public:
	explicit EmbeddingsMockHandler(EmbeddingsMock& state): _state(state) {}

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
	{
		_state.lastPath = request.getURI();
		Poco::StreamCopier::copyToString(request.stream(), _state.lastBody);
		if (_state.respondError)
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
			response.send() << "boom";
			return;
		}
		response.setContentType("application/json");
		response.send() <<
			"{\"object\":\"list\",\"model\":\"mock\",\"data\":["
			"{\"object\":\"embedding\",\"index\":1,\"embedding\":[0.5,0.6,0.7,0.8]},"
			"{\"object\":\"embedding\",\"index\":0,\"embedding\":[0.1,0.2,0.3,0.4]}"
			"]}";
	}

private:
	EmbeddingsMock& _state;
};


class EmbeddingsMockFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
	explicit EmbeddingsMockFactory(EmbeddingsMock& state): _state(state) {}

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
	{
		return new EmbeddingsMockHandler(_state);
	}

private:
	EmbeddingsMock& _state;
};


// Serves a canned /models response; captures the request for assertions.
struct ModelsMock
{
	std::string lastPath;
	bool respondError = false;
	bool respondMalformed = false;
};


class ModelsMockHandler: public Poco::Net::HTTPRequestHandler
{
public:
	explicit ModelsMockHandler(ModelsMock& state): _state(state) {}

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
	{
		_state.lastPath = request.getURI();
		if (_state.respondError)
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
			response.send() << "boom";
			return;
		}
		response.setContentType("application/json");
		if (_state.respondMalformed)
		{
			response.send() << "{\"data\": not json";
			return;
		}
		// Unsorted on purpose, with two embedding models mixed in.
		response.send() <<
			"{\"object\":\"list\",\"data\":["
			"{\"object\":\"model\",\"id\":\"zeta:7b\"},"
			"{\"object\":\"model\",\"id\":\"nomic-embed-text:latest\"},"
			"{\"object\":\"model\",\"id\":\"alpha:26b\"},"
			"{\"object\":\"model\",\"id\":\"text-embedding-3-small\"},"
			"{\"object\":\"model\",\"id\":\"mid:13b\"}"
			"]}";
	}

private:
	ModelsMock& _state;
};


class ModelsMockFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
	explicit ModelsMockFactory(ModelsMock& state): _state(state) {}

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
	{
		return new ModelsMockHandler(_state);
	}

private:
	ModelsMock& _state;
};


// One user message and one tool, for the chat tests.
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


// The stream of a Chat Completions response that calls the lookup tool with
// the given arguments, sent in the given fragments.
std::string toolCallStream(const std::vector<std::string>& argumentFragments)
{
	std::string stream = R"(data: {"choices":[{"delta":{"content":"Let me check."}}]})" "\n\n";
	bool first = true;
	for (const auto& fragment: argumentFragments)
	{
		Object::Ptr pFunction = new Object;
		if (first) pFunction->set("name", "lookup");
		pFunction->set("arguments", fragment);
		Object::Ptr pToolCall = new Object;
		pToolCall->set("index", 0);
		if (first) pToolCall->set("id", "call_1");
		pToolCall->set("function", pFunction);
		Array::Ptr pToolCalls = new Array;
		pToolCalls->add(pToolCall);
		Object::Ptr pDelta = new Object;
		pDelta->set("tool_calls", pToolCalls);
		Object::Ptr pChoice = new Object;
		pChoice->set("delta", pDelta);
		Array::Ptr pChoices = new Array;
		pChoices->add(pChoice);
		Object chunk;
		chunk.set("choices", pChoices);
		std::ostringstream text;
		chunk.stringify(text);
		stream += "data: " + text.str() + "\n\n";
		first = false;
	}
	stream += "data: [DONE]\n\n";
	return stream;
}


std::vector<ContentEvent> chat(OpenAIProvider& provider)
{
	std::vector<ContentEvent> events;
	provider.chat(userMessage(), "You are a test.", lookupTool(),
		[&events](const ContentEvent& event) { events.push_back(event); });
	return events;
}


} // namespace


OpenAIProviderTest::OpenAIProviderTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


void OpenAIProviderTest::setUp()
{
}


void OpenAIProviderTest::tearDown()
{
}


void OpenAIProviderTest::testAppendAssistantTextOnly()
{
	OpenAIProvider provider("http://localhost:1/v1", "", "test-model", 1024, 30);

	Array messages;
	std::vector<ContentEvent> noToolCalls;

	provider.appendAssistantMessage(messages, "Hello", noToolCalls);

	assertEqual(1, static_cast<int>(messages.size()));

	auto pMsg = messages.getObject(0);
	assertNotNull(pMsg.get());
	assertEqual(std::string("assistant"), pMsg->getValue<std::string>("role"));
	assertEqual(std::string("Hello"), pMsg->getValue<std::string>("content"));

	// No tool_calls key when there are no tool calls
	assertTrue(!pMsg->has("tool_calls"));
}


void OpenAIProviderTest::testAppendAssistantWithToolCalls()
{
	OpenAIProvider provider("http://localhost:1/v1", "", "test-model", 1024, 30);

	Array messages;

	ContentEvent tc;
	tc.type = ContentEvent::TYPE_TOOL_USE;
	tc.id = "call_abc";
	tc.name = "get_info";
	tc.input.set("query", "status");

	std::vector<ContentEvent> toolCalls = {tc};
	provider.appendAssistantMessage(messages, "Let me check.", toolCalls);

	auto pMsg = messages.getObject(0);
	assertEqual(std::string("assistant"), pMsg->getValue<std::string>("role"));
	assertEqual(std::string("Let me check."), pMsg->getValue<std::string>("content"));

	// Check tool_calls array
	assertTrue(pMsg->has("tool_calls"));
	auto pToolCalls = pMsg->getArray("tool_calls");
	assertNotNull(pToolCalls.get());
	assertEqual(1, static_cast<int>(pToolCalls->size()));

	auto pToolCall = pToolCalls->getObject(0);
	assertEqual(std::string("call_abc"), pToolCall->getValue<std::string>("id"));
	assertEqual(std::string("function"), pToolCall->getValue<std::string>("type"));

	auto pFunction = pToolCall->getObject("function");
	assertNotNull(pFunction.get());
	assertEqual(std::string("get_info"), pFunction->getValue<std::string>("name"));

	// Arguments should be a JSON string
	std::string args = pFunction->getValue<std::string>("arguments");
	assertTrue(args.find("query") != std::string::npos);
	assertTrue(args.find("status") != std::string::npos);
}


void OpenAIProviderTest::testAppendToolResult()
{
	OpenAIProvider provider("http://localhost:1/v1", "", "test-model", 1024, 30);

	Array messages;
	provider.appendToolResult(messages, "call_xyz", "result data");

	assertEqual(1, static_cast<int>(messages.size()));

	auto pMsg = messages.getObject(0);
	assertEqual(std::string("tool"), pMsg->getValue<std::string>("role"));
	assertEqual(std::string("call_xyz"), pMsg->getValue<std::string>("tool_call_id"));
	assertEqual(std::string("result data"), pMsg->getValue<std::string>("content"));
}


void OpenAIProviderTest::testAppendToolResultSeparateMessages()
{
	OpenAIProvider provider("http://localhost:1/v1", "", "test-model", 1024, 30);

	Array messages;

	// Two tool results should create TWO separate messages (not merge like Anthropic)
	provider.appendToolResult(messages, "call_1", "result 1");
	provider.appendToolResult(messages, "call_2", "result 2");

	assertEqual(2, static_cast<int>(messages.size()));

	auto pMsg1 = messages.getObject(0);
	assertEqual(std::string("tool"), pMsg1->getValue<std::string>("role"));
	assertEqual(std::string("call_1"), pMsg1->getValue<std::string>("tool_call_id"));
	assertEqual(std::string("result 1"), pMsg1->getValue<std::string>("content"));

	auto pMsg2 = messages.getObject(1);
	assertEqual(std::string("tool"), pMsg2->getValue<std::string>("role"));
	assertEqual(std::string("call_2"), pMsg2->getValue<std::string>("tool_call_id"));
	assertEqual(std::string("result 2"), pMsg2->getValue<std::string>("content"));
}


void OpenAIProviderTest::testToOpenAITools()
{
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

	Array openaiTools = OpenAIProvider::toOpenAITools(tools);

	assertEqual(1, static_cast<int>(openaiTools.size()));

	auto pTool = openaiTools.getObject(0);
	assertEqual(std::string("function"), pTool->getValue<std::string>("type"));

	auto pFunction = pTool->getObject("function");
	assertNotNull(pFunction.get());
	assertEqual(std::string("my_tool"), pFunction->getValue<std::string>("name"));
	assertEqual(std::string("A test tool"), pFunction->getValue<std::string>("description"));

	// "parameters" should be present inside "function" (not renamed)
	assertTrue(pFunction->has("parameters"));
}


void OpenAIProviderTest::testEmbed()
{
	EmbeddingsMock state;
	Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
	Poco::Net::HTTPServer server(new EmbeddingsMockFactory(state), socket, new Poco::Net::HTTPServerParams);
	server.start();

	OpenAIProvider provider(
		"http://127.0.0.1:" + std::to_string(socket.address().port()) + "/v1",
		"", "chat-model", 1024, 30);

	std::vector<std::vector<float>> vecs = provider.embed({"first text", "second text"}, "embed-model", 4);

	server.stop();

	assertEqual(std::string("/v1/embeddings"), state.lastPath);
	assertTrue(state.lastBody.find("\"model\":\"embed-model\"") != std::string::npos);
	assertTrue(state.lastBody.find("\"dimensions\":4") != std::string::npos);
	assertTrue(state.lastBody.find("first text") != std::string::npos);

	assertEqual(2, static_cast<int>(vecs.size()));
	assertEqual(4, static_cast<int>(vecs[0].size()));
	assertEqual(4, static_cast<int>(vecs[1].size()));
	// index field restores input order despite the reversed response
	assertEqualDelta(0.1, vecs[0][0], 0.0001);
	assertEqualDelta(0.5, vecs[1][0], 0.0001);
}


void OpenAIProviderTest::testEmbedRepeatedIndex()
{
	// Two entries for two inputs, but both claim index 0: the response does
	// not hold one vector per input, whatever its length says.
	CannedServer server("application/json",
		R"({"object":"list","data":[)"
		R"({"object":"embedding","index":0,"embedding":[0.1,0.2]},)"
		R"({"object":"embedding","index":0,"embedding":[0.3,0.4]})"
		R"(]})");
	OpenAIProvider provider(server.baseUrl(), "", "chat-model", 1024, 30);

	try
	{
		provider.embed({"first text", "second text"}, "embed-model");
		fail("a repeated index must throw");
	}
	catch (Poco::DataFormatException&)
	{
	}
}


void OpenAIProviderTest::testChatToolCall()
{
	// The arguments arrive in two fragments and are parsed when the stream ends.
	CannedServer server("text/event-stream", toolCallStream({R"({"city":)", R"("Paris"})"}));
	OpenAIProvider provider(server.baseUrl(), "secret", "chat-model", 1024, 30);

	const std::vector<ContentEvent> events = chat(provider);

	assertEqual(std::string("/v1/chat/completions"), server.path());
	assertEqual(std::string("Bearer secret"), server.header("Authorization"));
	assertTrue(server.request().find("\"stream\":true") != std::string::npos);
	assertTrue(server.request().find("\"lookup\"") != std::string::npos);

	assertEqual(2, static_cast<int>(events.size()));
	assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
	assertEqual(std::string("Let me check."), events[0].text);
	assertTrue(events[1].type == ContentEvent::TYPE_TOOL_USE);
	assertEqual(std::string("call_1"), events[1].id);
	assertEqual(std::string("lookup"), events[1].name);
	assertEqual(std::string("Paris"), events[1].input.getValue<std::string>("city"));
}


void OpenAIProviderTest::testChatMalformedToolArguments()
{
	// Arguments that are cut short, or are not an object, are an error: the
	// tool call is not delivered, so the tool cannot run without them.
	const std::vector<std::vector<std::string>> malformed{
		{R"({"city":)"},
		{R"({"city":"Par)"},
		{R"(["Paris"])"},
		{R"("Paris")"},
		{"not json"}};
	for (const auto& fragments: malformed)
	{
		CannedServer server("text/event-stream", toolCallStream(fragments));
		OpenAIProvider provider(server.baseUrl(), "", "chat-model", 1024, 30);

		const std::vector<ContentEvent> events = chat(provider);

		assertEqual(2, static_cast<int>(events.size()));
		assertTrue(events[0].type == ContentEvent::TYPE_TEXT);
		assertTrue(events[1].type == ContentEvent::TYPE_ERROR);
		assertEqual(std::string("malformed_tool_input"), events[1].code);
		assertTrue(events[1].text.find("lookup") != std::string::npos);
	}
}


void OpenAIProviderTest::testChatToolCallWithoutArguments()
{
	// No arguments, an empty object and null all stand for a call without
	// arguments.
	const std::vector<std::vector<std::string>> none{{""}, {"{}"}, {"null"}, {"  "}};
	for (const auto& fragments: none)
	{
		CannedServer server("text/event-stream", toolCallStream(fragments));
		OpenAIProvider provider(server.baseUrl(), "", "chat-model", 1024, 30);

		const std::vector<ContentEvent> events = chat(provider);

		assertEqual(2, static_cast<int>(events.size()));
		assertTrue(events[1].type == ContentEvent::TYPE_TOOL_USE);
		assertEqual(std::string("lookup"), events[1].name);
		assertEqual(0, static_cast<int>(events[1].input.size()));
	}
}


void OpenAIProviderTest::testListModels()
{
	ModelsMock state;
	Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
	Poco::Net::HTTPServer server(new ModelsMockFactory(state), socket, new Poco::Net::HTTPServerParams);
	server.start();

	OpenAIProvider provider(
		"http://127.0.0.1:" + std::to_string(socket.address().port()) + "/v1",
		"", "chat-model", 1024, 30);

	std::vector<std::string> models = provider.listModels();

	server.stop();

	assertEqual(std::string("/v1/models"), state.lastPath);
	// Sorted, with the embedding models filtered out.
	assertEqual(3, static_cast<int>(models.size()));
	assertEqual(std::string("alpha:26b"), models[0]);
	assertEqual(std::string("mid:13b"), models[1]);
	assertEqual(std::string("zeta:7b"), models[2]);
}


void OpenAIProviderTest::testListModelsError()
{
	// Discovery is an optimization: a failing server yields an empty list,
	// never an exception.
	ModelsMock state;
	state.respondError = true;
	Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
	Poco::Net::HTTPServer server(new ModelsMockFactory(state), socket, new Poco::Net::HTTPServerParams);
	server.start();

	OpenAIProvider provider(
		"http://127.0.0.1:" + std::to_string(socket.address().port()) + "/v1",
		"", "chat-model", 1024, 30);

	assertTrue(provider.listModels().empty());
	server.stop();

	// Nothing listening at all behaves the same.
	OpenAIProvider unreachable("http://127.0.0.1:1/v1", "", "chat-model", 1024, 30);
	assertTrue(unreachable.listModels().empty());
}


void OpenAIProviderTest::testListModelsMalformed()
{
	ModelsMock state;
	state.respondMalformed = true;
	Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
	Poco::Net::HTTPServer server(new ModelsMockFactory(state), socket, new Poco::Net::HTTPServerParams);
	server.start();

	OpenAIProvider provider(
		"http://127.0.0.1:" + std::to_string(socket.address().port()) + "/v1",
		"", "chat-model", 1024, 30);

	assertTrue(provider.listModels().empty());
	server.stop();
}


void OpenAIProviderTest::testEmbedError()
{
	EmbeddingsMock state;
	state.respondError = true;
	Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
	Poco::Net::HTTPServer server(new EmbeddingsMockFactory(state), socket, new Poco::Net::HTTPServerParams);
	server.start();

	OpenAIProvider provider(
		"http://127.0.0.1:" + std::to_string(socket.address().port()) + "/v1",
		"", "chat-model", 1024, 30);

	try
	{
		provider.embed({"text"}, "embed-model");
		server.stop();
		fail ("must throw on a non-OK response");
	}
	catch (Poco::RuntimeException& ex)
	{
		server.stop();
		assertTrue(ex.displayText().find("boom") != std::string::npos);
	}
}


CppUnit::Test* OpenAIProviderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OpenAIProviderTest");

	CppUnit_addTest(pSuite, OpenAIProviderTest, testAppendAssistantTextOnly);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testAppendAssistantWithToolCalls);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testAppendToolResult);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testAppendToolResultSeparateMessages);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testToOpenAITools);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testEmbed);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testEmbedError);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testEmbedRepeatedIndex);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testChatToolCall);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testChatMalformedToolArguments);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testChatToolCallWithoutArguments);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testListModels);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testListModelsError);
	CppUnit_addTest(pSuite, OpenAIProviderTest, testListModelsMalformed);

	return pSuite;
}
