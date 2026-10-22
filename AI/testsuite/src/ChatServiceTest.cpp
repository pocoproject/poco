//
// ChatServiceTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ChatServiceTest.h"
#include "CannedServer.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/ChatService.h"
#include "Poco/AI/AgentLoop.h"
#include "Poco/AI/AnthropicProvider.h"
#include "Poco/AI/ChatEndpoint.h"
#include "Poco/AI/ChatSettings.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/StreamCopier.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
#include "Poco/Message.h"
#include "Poco/Logger.h"
#include "Poco/Channel.h"
#include "Poco/AutoPtr.h"
#include <atomic>
#include <functional>
#include <istream>
#include <memory>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


using Poco::AI::ChatService;
using Poco::AI::ChatEndpoint;
using Poco::AI::ChatSettings;
using Poco::AI::ProviderSettings;
using Poco::AI::ProviderStatus;
using Poco::AI::LLMProvider;
using Poco::AI::ChatRequest;
using Poco::AI::ChatResult;
using Poco::AI::ContentEvent;
using Poco::AI::ContentEventCallback;
using Poco::AI::Cancellation;
using Poco::AI::ToolCall;
using Poco::AI::ToolRegistry;
using Poco::AI::StopReason;
using Poco::AI::FailureCode;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	struct FakeState
		/// What the fake providers of a test share: their script, and what
		/// was asked of them.
	{
		Poco::FastMutex mutex;
		std::vector<std::string> models{"m1", "m2"};
		std::set<std::string> unreachable;
			/// The base URLs whose servers do not answer.
		std::vector<std::string> listings;
			/// The base URLs asked for models.
		std::vector<std::string> calls;
			/// "<base URL>|<model>" of every call.
		std::vector<std::string> keys;
			/// The keys the providers were created with.
		bool connectFails = false;
		bool noAnswer = false;
			/// A call times out before an answer comes.
		bool block = false;
			/// A call waits until it is cancelled.
		bool holdCall = false;
			/// A call waits for releaseCall, after it set started.
		Poco::Event releaseCall{Poco::Event::EVENT_MANUALRESET};
		std::vector<std::string> tools;
			/// The tools the model calls in the first round of a turn.
		std::vector<std::size_t> toolsGiven;
			/// How many tools each call carried.
		Poco::Event started{Poco::Event::EVENT_MANUALRESET};
		std::atomic<int> cancelled{0};
		bool holdListings = false;
			/// A listing of the models waits for releaseListings, after it set
			/// listingStarted.
		Poco::Event listingStarted{Poco::Event::EVENT_MANUALRESET};
		Poco::Event releaseListings{Poco::Event::EVENT_MANUALRESET};

		std::vector<std::string> listed()
		{
			Poco::FastMutex::ScopedLock lock(mutex);
			return listings;
		}

		std::vector<std::string> called()
		{
			Poco::FastMutex::ScopedLock lock(mutex);
			return calls;
		}
	};


	class FakeProvider: public LLMProvider
		/// A provider whose model answers "hello from <model>".
	{
	public:
		FakeProvider(std::shared_ptr<FakeState> pState, const std::string& baseUrl, const std::string& model):
			_pState(std::move(pState)),
			_baseUrl(baseUrl),
			_model(model)
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

		std::vector<std::string> listModels() override
		{
			bool hold = false;
			{
				Poco::FastMutex::ScopedLock lock(_pState->mutex);
				_pState->listings.push_back(_baseUrl);
				if (_pState->unreachable.count(_baseUrl) > 0)
				{
					// Named as a socket names the server it cannot reach.
					const Poco::URI uri(_baseUrl);
					throw Poco::Net::ConnectionRefusedException(uri.getHost() + ":" + std::to_string(uri.getPort()));
				}
				hold = _pState->holdListings;
			}
			if (hold)
			{
				_pState->listingStarted.set();
				(void) _pState->releaseListings.tryWait(20000);
			}
			Poco::FastMutex::ScopedLock lock(_pState->mutex);
			return _pState->models;
		}

		ChatResult chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation) override
		{
			bool connectFails = false;
			bool noAnswer = false;
			bool block = false;
			bool hold = false;
			std::vector<std::string> tools;
			{
				Poco::FastMutex::ScopedLock lock(_pState->mutex);
				_pState->calls.push_back(_baseUrl + "|" + _model);
				_pState->toolsGiven.push_back(request.tools.size());
				connectFails = _pState->connectFails;
				noAnswer = _pState->noAnswer;
				block = _pState->block;
				hold = _pState->holdCall;
				// The model calls only what it was given.
				for (const std::string& name: _pState->tools)
				{
					const bool given = std::any_of(request.tools.begin(), request.tools.end(),
						[&name](const Poco::AI::MCP::Tool& tool) { return tool.name == name; });
					if (_round == 0 && given) tools.push_back(name);
				}
			}
			++_round;
			_pState->started.set();
			if (hold) (void) _pState->releaseCall.tryWait(20000);
			if (noAnswer) throw Poco::TimeoutException("no answer in time");
			ChatResult result;
			if (connectFails)
			{
				ContentEvent failure;
				failure.type = ContentEvent::Type::FAILURE;
				failure.code = FailureCode::PROVIDER_FAILED;
				failure.text = "connection refused";
				onEvent(failure);
				result.stopReason = StopReason::FAILURE;
				return result;
			}
			if (block)
			{
				Poco::Event aborted;
				Cancellation::Request abort(cancellation, [&aborted]() { aborted.set(); });
				if (aborted.tryWait(10000)) ++_pState->cancelled;
				result.stopReason = StopReason::CANCELLED;
				return result;
			}
			result.assistantMessage = new Object;
			result.assistantMessage->set("role", "assistant");
			if (!tools.empty())
			{
				for (const std::string& name: tools)
				{
					ToolCall call;
					call.id = "call-" + name;
					call.name = name;
					call.input = new Object;
					result.toolCalls.push_back(call);
				}
				result.stopReason = StopReason::TOOL_USE;
				return result;
			}
			ContentEvent text;
			text.text = "hello from " + _model;
			onEvent(text);
			result.text = text.text;
			result.assistantMessage->set("content", text.text);
			result.stopReason = StopReason::END_TURN;
			return result;
		}

		void appendToolResult(Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult&) override
		{
			Object::Ptr pMessage = new Object;
			pMessage->set("role", "tool");
			pMessage->set("id", toolCallId);
			messages.add(pMessage);
		}

	private:
		std::shared_ptr<FakeState> _pState;
		std::string _baseUrl;
		std::string _model;
		int _round = 0;
	};


	ChatService::Params fakeParams(const std::shared_ptr<FakeState>& pState)
	{
		ChatService::Params params;
		params.systemPrompt = "You answer tests.";
		params.providerFactory = [pState](const ProviderSettings& settings, const std::string& baseUrl, const std::string& model, const Poco::Timespan&)
		{
			{
				Poco::FastMutex::ScopedLock lock(pState->mutex);
				pState->keys.push_back(settings.apiKey);
			}
			return std::make_shared<FakeProvider>(pState, baseUrl, model);
		};
		return params;
	}


	ProviderSettings entry(const std::string& name, bool enabled, const std::vector<std::string>& models = std::vector<std::string>())
	{
		ProviderSettings settings;
		settings.name = name;
		settings.enabled = enabled;
		settings.models = models;
		return settings;
	}


	ChatSettings settingsOf(const std::vector<ProviderSettings>& providers)
	{
		ChatSettings settings;
		settings.providers = providers;
		return settings;
	}


	Poco::AI::MCP::Identity who(const std::string& user)
	{
		Poco::AI::MCP::Identity identity;
		identity.principal = user;
		identity.scope = "watch";
		identity.authenticated = true;
		return identity;
	}


	class MessageCollector: public Poco::Channel
		/// Keeps the texts of the messages logged to it.
	{
	public:
		void log(const Poco::Message& msg) override
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_texts.push_back(msg.getText());
		}

		std::vector<std::string> texts() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _texts;
		}

	private:
		mutable Poco::FastMutex _mutex;
		std::vector<std::string> _texts;
	};


	class EnvironmentGuard
		/// Sets an environment variable while it lives, and then puts back what
		/// it was; a variable that was not set is left empty.
	{
	public:
		EnvironmentGuard(const std::string& name, const std::string& value):
			_name(name),
			_old(Poco::Environment::get(name, ""))
		{
			Poco::Environment::set(_name, value);
		}

		~EnvironmentGuard()
		{
			Poco::Environment::set(_name, _old);
		}

		EnvironmentGuard(const EnvironmentGuard&) = delete;
		EnvironmentGuard& operator = (const EnvironmentGuard&) = delete;

	private:
		std::string _name;
		std::string _old;
	};


	Array conversationOf(const std::string& text)
	{
		Array conversation;
		Object::Ptr pMessage = new Object;
		pMessage->set("role", "user");
		pMessage->set("content", text);
		conversation.add(pMessage);
		return conversation;
	}


	struct Events
		/// The events of a turn.
	{
		std::vector<Object::Ptr> list;

		ChatService::EventSink sink()
		{
			return [this](const Object::Ptr& pEvent) { list.push_back(pEvent); };
		}

		std::string types() const
		{
			std::string result;
			for (const Object::Ptr& pEvent: list)
			{
				if (!result.empty()) result += ",";
				result += pEvent->getValue<std::string>("type");
			}
			return result;
		}

		Object::Ptr last() const
		{
			return list.back();
		}
	};


	class Turn: public Poco::Runnable
		/// Runs a turn on a thread of its own.
	{
	public:
		Turn(ChatService& service, const std::string& user, const std::string& chatId, std::shared_ptr<Cancellation> pCancellation = nullptr):
			_service(service),
			_user(user),
			_chatId(chatId),
			_pCancellation(std::move(pCancellation)),
			_conversation(conversationOf("wait"))
		{
		}

		void run() override
		{
			_service.chat(_conversation, who(_user), _chatId, events.sink(), _pCancellation);
		}

		Events events;

	private:
		ChatService& _service;
		std::string _user;
		std::string _chatId;
		std::shared_ptr<Cancellation> _pCancellation;
		Array _conversation;
	};


	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	class EndpointHandler: public Poco::Net::HTTPRequestHandler
		/// Serves /api/ai/<route> with the endpoint; X-User names the caller,
		/// X-Manage: 1 lets it manage.
	{
	public:
		explicit EndpointHandler(const ChatEndpoint& endpoint):
			_endpoint(endpoint)
		{
		}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
		{
			ChatEndpoint::Caller caller;
			caller.user = request.get("X-User", "");
			caller.mayManage = request.get("X-Manage", "") == "1";
			const std::string path = Poco::URI(request.getURI()).getPath();
			_endpoint.handle(path.substr(std::string("/api/ai/").size()), request, response, caller);
		}

	private:
		const ChatEndpoint& _endpoint;
	};


	class EndpointFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		explicit EndpointFactory(const ChatEndpoint& endpoint):
			_endpoint(endpoint)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
		{
			return new EndpointHandler(_endpoint);
		}

	private:
		const ChatEndpoint& _endpoint;
	};


	struct Answer
	{
		int status = 0;
		std::string contentType;
		std::string body;

		Object::Ptr json() const
		{
			return parse(body);
		}
	};


	class Host
		/// An HTTP server with the endpoint of a service.
	{
	public:
		Host(ChatService& service, const ChatEndpoint::Params& params = ChatEndpoint::Params()):
			_endpoint(service, params),
			_socket(Poco::Net::SocketAddress("127.0.0.1", 0)),
			_server(new EndpointFactory(_endpoint), _socket, new Poco::Net::HTTPServerParams)
		{
			_server.start();
		}

		~Host()
		{
			_server.stopAll(true);
		}

		Poco::UInt16 port() const
		{
			return _socket.address().port();
		}

		Answer send(const std::string& method, const std::string& route, const std::string& body = std::string(), const std::string& user = "alice", bool manage = false, const Poco::Net::NameValueCollection& headers = Poco::Net::NameValueCollection()) const
			/// Sends a request; headers are added last and replace those of the
			/// same name, Content-Type application/json for a body among them.
		{
			Poco::Net::HTTPClientSession session("127.0.0.1", port());
			session.setTimeout(Poco::Timespan(20, 0));
			Poco::Net::HTTPRequest request(method, "/api/ai/" + route, Poco::Net::HTTPMessage::HTTP_1_1);
			request.set("X-User", user);
			if (manage) request.set("X-Manage", "1");
			if (!body.empty())
			{
				request.setContentType("application/json");
				request.setContentLength(static_cast<std::streamsize>(body.size()));
			}
			for (const auto& header: headers) request.set(header.first, header.second);
			std::ostream& out = session.sendRequest(request);
			out << body;
			Poco::Net::HTTPResponse response;
			std::istream& in = session.receiveResponse(response);
			Answer answer;
			answer.status = static_cast<int>(response.getStatus());
			answer.contentType = response.getContentType();
			Poco::StreamCopier::copyToString(in, answer.body);
			return answer;
		}

	private:
		ChatEndpoint _endpoint;
		Poco::Net::ServerSocket _socket;
		Poco::Net::HTTPServer _server;
	};


	class GoneResponse: public Poco::Net::HTTPServerResponse
		/// The response to a client that has gone away: every send throws,
		/// and the response counts as sent from the first try on, as that of
		/// the HTTP server does.
	{
	public:
		void sendContinue() override
		{
			++tries;
			throw Poco::Net::ConnectionResetException("the client has gone away");
		}

		std::ostream& send() override
		{
			++tries;
			throw Poco::Net::ConnectionResetException("the client has gone away");
		}

		void sendFile(const std::string&, const std::string&) override
		{
			++tries;
			throw Poco::Net::ConnectionResetException("the client has gone away");
		}

		void sendBuffer(const void*, std::size_t) override
		{
			++tries;
			throw Poco::Net::ConnectionResetException("the client has gone away");
		}

		void redirect(const std::string&, HTTPStatus) override
		{
			++tries;
			throw Poco::Net::ConnectionResetException("the client has gone away");
		}

		void requireAuthentication(const std::string&) override
		{
			++tries;
			throw Poco::Net::ConnectionResetException("the client has gone away");
		}

		bool sent() const override
		{
			return tries > 0;
		}

		int tries = 0;
	};


	class BareRequest: public Poco::Net::HTTPServerRequest
		/// A request with a method and a JSON body, and no connection behind.
	{
	public:
		BareRequest(const std::string& method, const std::string& body, Poco::Net::HTTPServerResponse& response):
			_body(body),
			_response(response),
			_pParams(new Poco::Net::HTTPServerParams)
		{
			setMethod(method);
			if (!body.empty())
			{
				setContentType("application/json");
				setContentLength(static_cast<std::streamsize>(body.size()));
			}
		}

		std::istream& stream() override
		{
			return _body;
		}

		const Poco::Net::SocketAddress& clientAddress() const override
		{
			return _address;
		}

		const Poco::Net::SocketAddress& serverAddress() const override
		{
			return _address;
		}

		const Poco::Net::HTTPServerParams& serverParams() const override
		{
			return *_pParams;
		}

		Poco::Net::HTTPServerResponse& response() const override
		{
			return _response;
		}

		bool secure() const override
		{
			return false;
		}

	private:
		std::istringstream _body;
		Poco::Net::SocketAddress _address;
		Poco::Net::HTTPServerResponse& _response;
		Poco::Net::HTTPServerParams::Ptr _pParams;
	};


	std::vector<Object::Ptr> streamEvents(const std::string& body)
		/// The events of the data lines of an event stream, but [DONE].
	{
		std::vector<Object::Ptr> result;
		std::istringstream in(body);
		std::string line;
		while (std::getline(in, line))
		{
			if (line.compare(0, 6, "data: ") != 0 || line == "data: [DONE]") continue;
			result.push_back(parse(line.substr(6)));
		}
		return result;
	}


	const std::string HELLO = "{\"messages\":[{\"role\":\"user\",\"content\":\"hi\"}],\"chat\":\"c1\"}";


	bool waitFor(const std::function<bool ()>& condition)
		/// Polls condition for ten seconds at most.
	{
		const Poco::Timestamp start;
		while (!condition())
		{
			if (start.isElapsed(10000000)) return false;
			Poco::Thread::sleep(10);
		}
		return true;
	}
}


ChatServiceTest::ChatServiceTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ChatServiceTest::~ChatServiceTest()
{
}


void ChatServiceTest::testTurn()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true)}), fakeParams(pState));

	Array conversation = conversationOf("hi");
	Events events;
	service.chat(conversation, who("alice"), "c1", events.sink());
	assertEqual(std::string("text,done"), events.types());
	assertEqual(std::string("hello from m1"), events.list[0]->getValue<std::string>("content"));
	assertEqual(std::string("end_turn"), events.last()->getValue<std::string>("stopReason"));
	// The turn is appended to the conversation.
	assertEqual(2u, static_cast<unsigned>(conversation.size()));
	// The default base URL of the kind, the first model it lists.
	assertEqual(std::string("http://127.0.0.1:11434/v1|m1"), pState->called().back());
	assertEqual(std::string("ollama"), service.provider());
	assertEqual(std::string("m1"), service.model());
}


void ChatServiceTest::testToolCall()
{
	auto pState = std::make_shared<FakeState>();
	pState->tools.push_back("probe");
	auto pTools = std::make_shared<ToolRegistry>();
	Poco::AI::MCP::Tool probe;
	probe.name = "probe";
	probe.description = "Probes.";
	probe.handler = [](const Object::Ptr&, const Poco::AI::MCP::RequestContext& context)
	{
		Poco::AI::MCP::ToolResult result;
		result.addText("probed for " + context.identity.principal + " with " + context.identity.scope);
		return result;
	};
	pTools->registerTool(probe);
	ChatService service(pTools, settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));

	Array conversation = conversationOf("probe it");
	Events events;
	service.chat(conversation, who("alice"), "c1", events.sink());
	assertEqual(std::string("tool_call,tool_result,text,done"), events.types());
	assertEqual(std::string("probe"), events.list[0]->getValue<std::string>("name"));
	// The tools run for the user of the turn.
	assertEqual(std::string("probed for alice with watch"), events.list[1]->getValue<std::string>("content"));
	assertTrue(!events.list[1]->has("error"));

	// A model that cannot call tools is given none.
	ProviderSettings without = entry("ollama", true, {"m1"});
	without.tools = false;
	service.configure({without});
	Array plain = conversationOf("probe it");
	Events answered;
	service.chat(plain, who("alice"), "c1", answered.sink());
	assertEqual(std::string("text,done"), answered.types());
	assertEqual(0u, static_cast<unsigned>(pState->toolsGiven.back()));

	// The approver of the host decides which calls of whose turns run; a
	// refused call does not run, and the model reads an error result.
	ChatService::Params params = fakeParams(pState);
	params.approver = [](const Poco::AI::MCP::Identity& user, const ToolCall& call, const Poco::AI::MCP::Tool& tool)
	{
		return user.principal == "alice" && call.name == "probe" && tool.name == "probe";
	};
	ChatService approving(pTools, settingsOf({entry("ollama", true, {"m1"})}), params);
	Array bobs = conversationOf("probe it");
	Events refused;
	approving.chat(bobs, who("bob"), "c2", refused.sink());
	assertEqual(std::string("tool_call,tool_result,text,done"), refused.types());
	assertTrue(refused.list[1]->optValue<bool>("error", false));
	assertTrue(refused.list[1]->getValue<std::string>("content").find("probed") == std::string::npos);
	Array alices = conversationOf("probe it");
	Events allowed;
	approving.chat(alices, who("alice"), "c3", allowed.sink());
	assertEqual(std::string("probed for alice with watch"), allowed.list[1]->getValue<std::string>("content"));
}


void ChatServiceTest::testNoProvider()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", false)}), fakeParams(pState));
	assertEqual(std::string(), service.provider());

	Array conversation = conversationOf("hi");
	Events events;
	service.chat(conversation, who("alice"), "c1", events.sink());
	assertEqual(std::string("error,done"), events.types());
	assertEqual(std::string("provider_unreachable"), events.list[0]->getValue<std::string>("code"));
	assertEqual(std::string("failure"), events.last()->getValue<std::string>("stopReason"));
	assertTrue(pState->called().empty());
}


void ChatServiceTest::testModels()
{
	// A list of models is offered as it is.
	auto pState = std::make_shared<FakeState>();
	{
		ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"a", "b"})}), fakeParams(pState));
		assertTrue(service.models() == std::vector<std::string>({"a", "b"}));
		assertEqual(std::string("a"), service.model());
		assertTrue(pState->listed().empty());
	}

	// Without one, the provider is asked, once while the answer is fresh.
	{
		ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true)}), fakeParams(pState));
		assertEqual(std::string(), service.model());
		assertTrue(service.models() == std::vector<std::string>({"m1", "m2"}));
		assertTrue(service.models() == std::vector<std::string>({"m1", "m2"}));
		assertEqual(1u, static_cast<unsigned>(pState->listed().size()));
		assertEqual(std::string("m1"), service.model());
		const std::vector<ProviderStatus> status = service.providerStatus();
		assertTrue(status[0].probed && status[0].reachable);
		assertEqual(2u, static_cast<unsigned>(status[0].modelCount));
	}
	ChatService::Params params = fakeParams(pState);
	params.modelsLifetime = Poco::Timespan(0);
	{
		ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true)}), params);
		(void) service.models();
		(void) service.models();
		assertEqual(3u, static_cast<unsigned>(pState->listed().size()));
	}

	// A provider that does not answer is not asked again at once, but by retry().
	pState->unreachable.insert("http://127.0.0.1:11434/v1");
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true)}), params);
	assertTrue(service.models().empty());
	assertTrue(service.models().empty());
	assertEqual(4u, static_cast<unsigned>(pState->listed().size()));
	const ProviderStatus status = service.retry("ollama");
	assertTrue(status.probed && !status.reachable);
	assertTrue(status.detail.find("127.0.0.1:11434") != std::string::npos);
	assertEqual(5u, static_cast<unsigned>(pState->listed().size()));
	try
	{
		(void) service.retry("anthropic");
		fail("a provider that is not enabled cannot be retried");
	}
	catch (const Poco::NotFoundException&)
	{
	}
}


void ChatServiceTest::testFailover()
{
	auto pState = std::make_shared<FakeState>();
	pState->unreachable.insert("http://a:11434/v1");
	ProviderSettings ollama = entry("ollama", true, {"m1"});
	ollama.baseUrl = "http://[server]:11434/v1";
	ollama.servers = {"a", "b"};
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({ollama}), fakeParams(pState));

	// The first server that answers serves the turn, and the next.
	Array conversation = conversationOf("hi");
	Events events;
	service.chat(conversation, who("alice"), "c1", events.sink());
	assertEqual(std::string("text,done"), events.types());
	assertEqual(std::string("http://b:11434/v1|m1"), pState->called().back());
	assertTrue(pState->listed() == std::vector<std::string>({"http://a:11434/v1", "http://b:11434/v1"}));
	assertEqual(std::string("http://b:11434/v1"), service.providerStatus()[0].detail);
	Events again;
	service.chat(conversation, who("alice"), "c1", again.sink());
	assertEqual(2u, static_cast<unsigned>(pState->listed().size()));

	// A call that cannot connect makes the next turn look for a server anew.
	pState->connectFails = true;
	Events failed;
	service.chat(conversation, who("alice"), "c1", failed.sink());
	assertEqual(std::string("error,done"), failed.types());
	assertEqual(std::string("provider_unreachable"), failed.list[0]->getValue<std::string>("code"));
	assertTrue(!service.providerStatus()[0].reachable);
	pState->connectFails = false;
	Events recovered;
	service.chat(conversation, who("alice"), "c1", recovered.sink());
	assertEqual(std::string("text,done"), recovered.types());
	assertEqual(4u, static_cast<unsigned>(pState->listed().size()));

	// A call that gets no answer in time does not make the provider look
	// unreachable, and its server is kept.
	pState->noAnswer = true;
	Events late;
	service.chat(conversation, who("alice"), "c1", late.sink());
	assertEqual(std::string("error,done"), late.types());
	assertEqual(std::string("no_answer"), late.list[0]->getValue<std::string>("code"));
	assertTrue(service.providerStatus()[0].reachable);
	pState->noAnswer = false;
	Events afterLate;
	service.chat(conversation, who("alice"), "c1", afterLate.sink());
	assertEqual(std::string("text,done"), afterLate.types());
	assertEqual(4u, static_cast<unsigned>(pState->listed().size()));

	// Settings that do not change keep the server; others look for one anew.
	pState->unreachable.insert("http://b:11434/v1");
	service.configure({ollama, entry("openai", false)});
	Events kept;
	service.chat(conversation, who("alice"), "c1", kept.sink());
	assertEqual(std::string("text,done"), kept.types());
	ollama.probeTimeout = 1;
	service.configure({ollama, entry("openai", false)});
	Events none;
	service.chat(conversation, who("alice"), "c1", none.sink());
	assertEqual(std::string("error,done"), none.types());
	assertTrue(none.list[0]->getValue<std::string>("content").find("No server") != std::string::npos);
}


void ChatServiceTest::testCancel()
{
	auto pState = std::make_shared<FakeState>();
	pState->block = true;
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));

	Turn turn(service, "alice", "c1");
	Poco::Thread thread;
	thread.start(turn);
	assertTrue(pState->started.tryWait(10000));
	// Only the user's turn in that chat.
	assertEqual(0, service.cancel("bob", "c1"));
	assertEqual(0, service.cancel("alice", "c2"));
	assertEqual(1, service.cancel("alice", "c1"));
	thread.join();
	assertEqual(1, pState->cancelled.load());
	assertEqual(std::string("cancelled"), turn.events.last()->getValue<std::string>("stopReason"));

	// cancelAll() cancels everybody's.
	pState->started.reset();
	Turn first(service, "alice", "c1");
	Turn second(service, "bob", "c9");
	Poco::Thread firstThread;
	Poco::Thread secondThread;
	firstThread.start(first);
	secondThread.start(second);
	assertTrue(waitFor([&pState]() { return pState->called().size() == 3; }));
	service.cancelAll();
	firstThread.join();
	secondThread.join();
	assertEqual(3, pState->cancelled.load());
	// A user's turns without a chat id.
	assertEqual(0, service.cancel("alice"));
}


void ChatServiceTest::testCancelOwnTurn()
{
	auto pState = std::make_shared<FakeState>();
	pState->block = true;
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));

	// Two turns of users who are not signed in, in chats of the same id: the
	// empty principal, everybody's who is not signed in, cancels neither, and
	// the cancellation a turn was started with stops that turn alone.
	auto pMine = std::make_shared<Cancellation>();
	Turn mine(service, "", "c1", pMine);
	Turn other(service, "", "c1");
	Poco::Thread mineThread;
	Poco::Thread otherThread;
	mineThread.start(mine);
	otherThread.start(other);
	assertTrue(waitFor([&pState]() { return pState->called().size() == 2; }));
	assertEqual(0, service.cancel("", "c1"));
	assertEqual(0, service.cancel(""));
	pMine->cancel();
	mineThread.join();
	assertEqual(std::string("cancelled"), mine.events.last()->getValue<std::string>("stopReason"));
	assertEqual(1, pState->cancelled.load());
	assertTrue(otherThread.isRunning());
	service.cancelAll();
	otherThread.join();
	assertEqual(2, pState->cancelled.load());
}


void ChatServiceTest::testSelect()
{
	auto pState = std::make_shared<FakeState>();
	std::vector<ChatSettings> stored;
	ChatService::Params params = fakeParams(pState);
	params.store = [&stored](const ChatSettings& settings) { stored.push_back(settings); };
	ProviderSettings lab = entry("lab", true, {"x1", "x2"});
	lab.baseUrl = "http://lab.example/v1";
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true), lab}), params);
	assertTrue(service.providers() == std::vector<std::string>({"ollama", "lab"}));

	try
	{
		service.selectProvider("openai");
		fail("a provider that is not enabled cannot be selected");
	}
	catch (const Poco::NotFoundException&)
	{
	}
	service.selectModel("m2");
	assertEqual(std::string("m2"), service.model());
	service.selectProvider("lab");
	assertEqual(std::string("lab"), service.provider());
	// A provider comes with its first model.
	assertEqual(std::string("x1"), service.model());
	// A provider with a list of models offers no other.
	try
	{
		service.selectModel("x3");
		fail("a model the provider does not list cannot be selected");
	}
	catch (const Poco::NotFoundException&)
	{
	}
	assertEqual(std::string("x1"), service.model());
	try
	{
		service.selectModel("");
		fail("a model needs a name");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	assertEqual(2u, static_cast<unsigned>(stored.size()));
	assertEqual(std::string("lab"), stored.back().provider);

	Array conversation = conversationOf("hi");
	Events events;
	service.chat(conversation, who("alice"), "c1", events.sink());
	assertEqual(std::string("http://lab.example/v1|x1"), pState->called().back());
}


void ChatServiceTest::testConfigure()
{
	auto pState = std::make_shared<FakeState>();
	std::vector<ChatSettings> stored;
	ChatService::Params params = fakeParams(pState);
	params.store = [&stored](const ChatSettings& settings) { stored.push_back(settings); };
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true)}), params);

	// Every kind is listed, those not configured disabled.
	ChatSettings settings = service.settings();
	assertEqual(3u, static_cast<unsigned>(settings.providers.size()));
	assertEqual(std::string("ollama"), settings.providers[0].name);
	assertTrue(!settings.providers[1].enabled && !settings.providers[2].enabled);

	const auto refused = [&service](const std::vector<ProviderSettings>& providers)
	{
		try
		{
			service.configure(providers);
			return false;
		}
		catch (const Poco::InvalidArgumentException&)
		{
			return true;
		}
	};
	assertTrue(refused({entry("ollama", true), entry("ollama", false)}));
	assertTrue(refused({entry("lab", true)}));
	assertTrue(refused({entry("ollama", false)}));
	ProviderSettings negative = entry("ollama", true);
	negative.timeout = -1;
	assertTrue(refused({negative}));
	assertTrue(stored.empty());

	// A key is kept when an entry comes without one.
	service.setKey("anthropic", "k1");
	assertTrue(service.hasKey("anthropic"));
	ProviderSettings anthropic = entry("anthropic", true, {"claude"});
	service.configure({anthropic, entry("ollama", false)});
	assertEqual(std::string("k1"), stored.back().providers[0].apiKey);
	// The provider in use is no longer enabled: the first that is.
	assertEqual(std::string("anthropic"), service.provider());
	assertEqual(std::string("claude"), service.model());
	Array conversation = conversationOf("hi");
	Events events;
	service.chat(conversation, who("alice"), "c1", events.sink());
	assertEqual(std::string("k1"), pState->keys.back());

	// A model no longer in the list of its provider gives way to the first.
	anthropic.models = {"claude", "old"};
	service.configure({anthropic});
	service.selectModel("old");
	assertEqual(std::string("old"), service.model());
	anthropic.models = {"new"};
	service.configure({anthropic});
	assertEqual(std::string("new"), service.model());

	service.setKey("anthropic", "");
	assertEqual(std::string(), service.settings().providers[0].apiKey);
	try
	{
		service.setKey("lab", "k");
		fail("a provider that is not configured has no key");
	}
	catch (const Poco::NotFoundException&)
	{
	}
}


void ChatServiceTest::testKeyStaysAtItsAddress()
{
	auto pState = std::make_shared<FakeState>();
	const auto lastKey = [&pState]()
	{
		Poco::FastMutex::ScopedLock lock(pState->mutex);
		return pState->keys.empty() ? std::string("(no provider created)") : pState->keys.back();
	};
	const auto turn = [](ChatService& service)
	{
		Array conversation = conversationOf("hi");
		Events events;
		service.chat(conversation, who("alice"), "c1", events.sink());
	};
	const std::string variable = ProviderSettings::keyVariable("anthropic");
	EnvironmentGuard noKey(variable, "");
	const ProviderSettings anthropic = entry("anthropic", true, {"claude"});
	ProviderSettings moved = anthropic;
	moved.baseUrl = "http://elsewhere.test";
	ProviderSettings withServers = anthropic;
	withServers.servers = {"backup"};

	// A key of the settings stays at the address it was given for, also when
	// the provider comes back.
	ProviderSettings keyed = anthropic;
	keyed.apiKey = "k1";
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({keyed}), fakeParams(pState));
	turn(service);
	assertEqual(std::string("k1"), lastKey());
	service.configure({moved});
	assertTrue(!service.hasKey("anthropic"));
	assertEqual(std::string(), service.settings().providers[0].apiKey);
	turn(service);
	assertEqual(std::string(), lastKey());
	service.configure({anthropic});
	assertTrue(!service.hasKey("anthropic"));
	// Other servers are another address too; the same address keeps the key.
	service.setKey("anthropic", "k2");
	service.configure({withServers});
	assertTrue(!service.hasKey("anthropic"));
	service.setKey("anthropic", "k3");
	service.configure({withServers});
	assertTrue(service.hasKey("anthropic"));

	// The key of the environment goes only to the address the service
	// started with.
	Poco::Environment::set(variable, "env-key");
	ChatService fromEnvironment(std::make_shared<ToolRegistry>(), settingsOf({anthropic}), fakeParams(pState));
	assertTrue(fromEnvironment.hasKey("anthropic"));
	turn(fromEnvironment);
	assertEqual(std::string("env-key"), lastKey());
	fromEnvironment.configure({moved});
	assertTrue(!fromEnvironment.hasKey("anthropic"));
	turn(fromEnvironment);
	assertEqual(std::string(), lastKey());
	fromEnvironment.configure({anthropic});
	assertTrue(fromEnvironment.hasKey("anthropic"));
	turn(fromEnvironment);
	assertEqual(std::string("env-key"), lastKey());
}


void ChatServiceTest::testStoreRefuses()
{
	auto pState = std::make_shared<FakeState>();
	ChatService::Params params = fakeParams(pState);
	params.store = [](const ChatSettings&) { throw Poco::WriteFileException("read-only"); };
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1", "m2"})}), params);
	try
	{
		service.selectModel("m2");
		fail("a change the store refuses must not take effect");
	}
	catch (const Poco::WriteFileException&)
	{
	}
	assertEqual(std::string("m1"), service.model());
}


void ChatServiceTest::testSettingsJSON()
{
	ChatSettings settings;
	settings.provider = "ollama";
	settings.model = "m2";
	ProviderSettings ollama = entry("ollama", true, {"m1", "m2"});
	ollama.baseUrl = "http://[server]:11434/v1";
	ollama.servers = {"a", "b"};
	ollama.maxTokens = 2048;
	ollama.timeout = 30;
	ollama.probeTimeout = 3;
	ollama.tools = false;
	ollama.apiKey = "secret";
	settings.providers.push_back(ollama);

	const ChatSettings read = ChatSettings::fromJSON(parse([&settings]()
		{
			std::ostringstream text;
			settings.toJSON()->stringify(text);
			return text.str();
		}()));
	assertEqual(std::string("m2"), read.model);
	assertEqual(1u, static_cast<unsigned>(read.providers.size()));
	const ProviderSettings& back = read.providers[0];
	assertTrue(back.enabled && back.servers == ollama.servers && back.models == ollama.models);
	assertEqual(2048, back.maxTokens);
	assertEqual(30, back.timeout);
	assertEqual(3, back.probeTimeout);
	assertTrue(!back.tools);
	assertEqual(std::string("secret"), back.apiKey);
	assertTrue(ChatSettings::fromJSON(parse("{\"providers\":[{\"name\":\"x\"}]}")).providers[0].tools);
	assertTrue(ChatSettings::fromJSON(parse("{}")).providers.empty());

	const std::string invalid[] = {"{\"providers\":\"x\"}", "{\"providers\":[{\"enabled\":\"yes\"}]}", "{\"providers\":[{\"models\":[1]}]}", "{\"model\":3}", "{\"providers\":[{\"tools\":1}]}", "{\"providers\":[{\"maxTokens\":9999999999}]}"};
	for (const std::string& json: invalid)
	{
		try
		{
			(void) ChatSettings::fromJSON(parse(json));
			fail("settings of the wrong types must be refused");
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}
}


void ChatServiceTest::testProviderDetail()
{
	auto pState = std::make_shared<FakeState>();
	pState->unreachable.insert("http://user:secret@a:11434/v1");
	ProviderSettings ollama = entry("ollama", true, {"m1"});
	ollama.baseUrl = "http://user:secret@[server]:11434/v1";
	ollama.servers = {"a", "b"};
	ChatService::Params params = fakeParams(pState);
	params.loggerName = "ChatServiceTest.ProviderDetail";
	Poco::Logger& logger = Poco::Logger::get(params.loggerName);
	const Poco::Channel::Ptr pChannel = logger.getChannel();
	Poco::AutoPtr<MessageCollector> pCollector = new MessageCollector;
	logger.setChannel(pCollector);
	logger.setLevel(Poco::Message::PRIO_WARNING);
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({ollama}), params);

	// The URL of the server that answered, and of the one that did not, go
	// into the status and the log without user name and password.
	const ProviderStatus status = service.retry("ollama");
	const std::vector<std::string> texts = pCollector->texts();
	logger.setChannel(pChannel);
	assertTrue(status.reachable);
	assertEqual(std::string("http://b:11434/v1"), status.detail);
	assertEqual(std::size_t(1), texts.size());
	assertTrue(texts[0].find("http://a:11434/v1") != std::string::npos);
	assertTrue(texts[0].find("secret") == std::string::npos);

	// Through the endpoint only a caller who may manage the chat reads the
	// detail; others get a fixed reason for a provider that does not answer.
	Host host(service);
	Object::Ptr pEntry = host.send("GET", "health").json()->getArray("providerStatus")->getObject(0);
	assertTrue(pEntry->getValue<bool>("reachable"));
	assertTrue(!pEntry->has("detail"));
	pEntry = host.send("GET", "health", "", "root", true).json()->getArray("providerStatus")->getObject(0);
	assertEqual(std::string("http://b:11434/v1"), pEntry->getValue<std::string>("detail"));

	{
		Poco::FastMutex::ScopedLock lock(pState->mutex);
		pState->unreachable.insert("http://user:secret@b:11434/v1");
	}
	Answer answer = host.send("POST", "providers", "{\"op\":\"retry\",\"provider\":\"ollama\"}");
	assertTrue(!answer.json()->getValue<bool>("reachable"));
	assertEqual(std::string("the provider does not answer"), answer.json()->getValue<std::string>("detail"));
	pEntry = host.send("GET", "providers").json()->getArray("providerStatus")->getObject(0);
	assertEqual(std::string("the provider does not answer"), pEntry->getValue<std::string>("detail"));
	answer = host.send("POST", "providers", "{\"op\":\"retry\",\"provider\":\"ollama\"}", "root", true);
	const std::string detail = answer.json()->getValue<std::string>("detail");
	assertTrue(detail.find("http://a:11434/v1") != std::string::npos);
	assertTrue(detail.find("http://b:11434/v1") != std::string::npos);
	assertTrue(detail.find("secret") == std::string::npos);
}


void ChatServiceTest::testOneProbeAtATime()
{
	class Asker: public Poco::Runnable
		/// Asks the service for the models of its provider in use, or asks
		/// the provider again.
	{
	public:
		Asker(ChatService& service, bool retry): _service(service), _retry(retry)
		{
		}

		void run() override
		{
			if (_retry) status = _service.retry("ollama");
			else models = _service.models();
		}

		std::vector<std::string> models;
		ProviderStatus status;

	private:
		ChatService& _service;
		bool _retry;
	};

	// While a caller asks a provider for its models, the others get what is
	// known, nothing yet here, without asking it again or waiting.
	auto pState = std::make_shared<FakeState>();
	pState->holdListings = true;
	ProviderSettings ollama = entry("ollama", true);
	ollama.baseUrl = "http://[server]:11434/v1";
	ollama.servers = {"a"};
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({ollama}), fakeParams(pState));
	Asker lister(service, false);
	Poco::Thread thread;
	thread.start(lister);
	assertTrue(pState->listingStarted.tryWait(10000));
	assertTrue(service.models().empty());
	assertTrue(!service.retry("ollama").probed);
	assertEqual(1u, static_cast<unsigned>(pState->listed().size()));
	pState->releaseListings.set();
	thread.join();
	assertTrue(lister.models == std::vector<std::string>({"m1", "m2"}));
	// What the one caller learnt serves the others.
	assertTrue(service.models() == std::vector<std::string>({"m1", "m2"}));
	assertEqual(1u, static_cast<unsigned>(pState->listed().size()));

	// A turn that needs the server of its provider, while another caller
	// looks for it, ends at once.
	auto pFixedState = std::make_shared<FakeState>();
	pFixedState->holdListings = true;
	ollama.models = {"m1"};
	ChatService fixed(std::make_shared<ToolRegistry>(), settingsOf({ollama}), fakeParams(pFixedState));
	Asker retrier(fixed, true);
	Poco::Thread retryThread;
	retryThread.start(retrier);
	assertTrue(pFixedState->listingStarted.tryWait(10000));
	Array conversation = conversationOf("hi");
	Events refused;
	fixed.chat(conversation, who("alice"), "c1", refused.sink());
	assertEqual(std::string("error,done"), refused.types());
	assertTrue(refused.list[0]->getValue<std::string>("content").find("being asked") != std::string::npos);
	pFixedState->releaseListings.set();
	retryThread.join();
	assertTrue(retrier.status.reachable);
	Events answered;
	fixed.chat(conversation, who("alice"), "c2", answered.sink());
	assertEqual(std::string("text,done"), answered.types());
	assertEqual(1u, static_cast<unsigned>(pFixedState->listed().size()));
}


void ChatServiceTest::testDefaultProvider()
{
	// The default factory: an Ollama server through the OpenAI API, found
	// among its servers, the first of which refuses connections.
	CannedServer server("application/json", "{\"object\":\"list\",\"data\":[{\"id\":\"llama3\",\"object\":\"model\"},{\"id\":\"qwen\",\"object\":\"model\"}]}");
	const Poco::URI uri(server.baseUrl());
	ProviderSettings ollama = entry("ollama", true);
	ollama.baseUrl = "http://[server]/v1";
	ollama.servers = {"127.0.0.1:1", uri.getHost() + ":" + std::to_string(uri.getPort())};
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({ollama}), ChatService::Params());

	const ProviderStatus status = service.retry("ollama");
	assertTrue(status.reachable);
	assertEqual(server.baseUrl(), status.detail);
	assertTrue(service.models() == std::vector<std::string>({"llama3", "qwen"}));
	assertEqual(std::string("/v1/models"), server.path());
	assertEqual(std::string("llama3"), service.model());
}


void ChatServiceTest::testProviderKind()
{
	// The kind, not the name, tells the API: a second Anthropic endpoint.
	CannedServer server("application/json", "{\"data\":[{\"id\":\"claude-x\",\"type\":\"model\"}]}");
	EnvironmentGuard noKey(ProviderSettings::keyVariable("anthropic"), "");
	ProviderSettings eu = entry("eu", true);
	eu.kind = "anthropic";
	eu.baseUrl = server.baseUrl();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({eu}), ChatService::Params());
	assertTrue(service.retry("eu").reachable);
	assertTrue(!server.header("anthropic-version").empty());
	assertTrue(service.models() == std::vector<std::string>({"claude-x"}));
	assertEqual(std::string("anthropic"), ChatSettings::fromJSON(service.settings().toJSON()).providers[0].kind);

	// Without a kind, the name gives it.
	assertEqual(std::string("anthropic"), entry("anthropic", true).effectiveKind());
	assertEqual(std::string("ollama"), entry("ollama", true).effectiveKind());
	assertEqual(std::string("openai-compatible"), entry("eu", true).effectiveKind());

	// A kind that is not known is refused, and one without a default base
	// URL needs one.
	ProviderSettings unknown = entry("x", true);
	unknown.kind = "gemini";
	unknown.baseUrl = "http://x.test/v1";
	ProviderSettings compatible = entry("y", true);
	compatible.kind = "openai-compatible";
	for (const ProviderSettings& refused: {unknown, compatible})
	{
		try
		{
			service.configure({refused});
			fail("settings that cannot work must be refused: " + refused.name);
		}
		catch (const Poco::InvalidArgumentException&)
		{
		}
	}
	ProviderSettings local = entry("local", true);
	local.kind = "ollama";
	service.configure({local});
	assertEqual(std::string("local"), service.provider());

	// The endpoint shows the kind and takes it.
	Host host(service);
	assertEqual(200, host.send("POST", "config", "{\"providers\":[{\"name\":\"local\",\"kind\":\"ollama\",\"enabled\":true},{\"name\":\"lab\",\"kind\":\"openai-compatible\",\"baseUrl\":\"http://lab.test/v1\"}]}", "root", true).status);
	const Array::Ptr pProviders = host.send("GET", "config", "", "root", true).json()->getArray("providers");
	assertEqual(std::string("ollama"), pProviders->getObject(0)->getValue<std::string>("kind"));
	assertEqual(std::string("openai-compatible"), pProviders->getObject(1)->getValue<std::string>("kind"));
	assertEqual(400, host.send("POST", "config", "{\"providers\":[{\"name\":\"local\",\"kind\":\"gemini\",\"enabled\":true}]}", "root", true).status);
}


void ChatServiceTest::testTokenLimit()
{
	// Without a limit in the settings or the parameters, the provider class
	// sends its own; else the one given.
	CannedServer server(CannedServer::makeResponse(500, "application/json", "{}"));
	EnvironmentGuard noKey(ProviderSettings::keyVariable("anthropic"), "");
	ProviderSettings anthropic = entry("anthropic", true, {"claude"});
	anthropic.baseUrl = server.baseUrl();
	const auto sentLimit = [&server, &anthropic](const ChatService::Params& params)
	{
		ChatService service(std::make_shared<ToolRegistry>(), settingsOf({anthropic}), params);
		Array conversation = conversationOf("hi");
		Events events;
		service.chat(conversation, who("alice"), "c1", events.sink());
		return parse(server.request())->getValue<int>("max_tokens");
	};
	assertEqual(Poco::AI::AnthropicProvider::Params().maxTokens, sentLimit(ChatService::Params()));
	ChatService::Params limited;
	limited.maxTokens = 500;
	assertEqual(500, sentLimit(limited));
	assertEqual(Poco::AI::AgentLoop::DEFAULT_MAX_ROUNDS, ChatService::Params().maxRounds);
}


void ChatServiceTest::testEndpointHealth()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1", "m2"})}), fakeParams(pState));
	Host host(service);

	Answer answer = host.send("GET", "health");
	assertEqual(200, answer.status);
	Object::Ptr pHealth = answer.json();
	assertEqual(std::string("ok"), pHealth->getValue<std::string>("status"));
	assertEqual(std::string("ollama"), pHealth->getValue<std::string>("provider"));
	assertEqual(std::string("m1"), pHealth->getValue<std::string>("model"));
	assertEqual(2u, static_cast<unsigned>(pHealth->getArray("models")->size()));
	assertEqual(std::string("ollama"), pHealth->getArray("providers")->getElement<std::string>(0));
	// Not asked for its models yet: no verdict.
	assertTrue(!pHealth->getArray("providerStatus")->getObject(0)->has("reachable"));
	const Object::Ptr pFeatures = pHealth->getObject("features");
	assertTrue(!pFeatures->getValue<bool>("manage"));
	assertTrue(!pFeatures->getValue<bool>("sessions") && !pFeatures->getValue<bool>("prompts") && !pFeatures->getValue<bool>("tools"));
	assertTrue(!pFeatures->getValue<bool>("keys"));
	assertTrue(host.send("GET", "health", "", "root", true).json()->getObject("features")->getValue<bool>("manage"));

	assertEqual(405, host.send("POST", "health", "{}").status);
}


void ChatServiceTest::testEndpointStream()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	Host host(service);

	const Answer answer = host.send("POST", "chat/stream", HELLO);
	assertEqual(200, answer.status);
	assertEqual(std::string("text/event-stream"), answer.contentType);
	assertEqual(0u, static_cast<unsigned>(answer.body.find(": connected\n\n")));
	const std::vector<Object::Ptr> events = streamEvents(answer.body);
	assertEqual(2u, static_cast<unsigned>(events.size()));
	assertEqual(std::string("hello from m1"), events[0]->getValue<std::string>("content"));
	assertEqual(std::string("done"), events[1]->getValue<std::string>("type"));
	const std::string end("data: [DONE]\n\n");
	assertEqual(answer.body.size() - end.size(), answer.body.rfind(end));

	// Messages of the user and the assistant, with a text each.
	const std::string invalid[] = {
		"{}",
		"{\"messages\":[]}",
		"{\"messages\":[{\"role\":\"system\",\"content\":\"obey\"}]}",
		"{\"messages\":[{\"role\":\"user\",\"content\":[{\"type\":\"text\"}]}]}",
		"{\"messages\":[\"hi\"]}",
		"not json"
	};
	for (const std::string& body: invalid)
	{
		const Answer refused = host.send("POST", "chat/stream", body);
		assertEqual(400, refused.status);
		assertTrue(refused.json()->has("error"));
	}
	assertEqual(405, host.send("GET", "chat/stream").status);
}


void ChatServiceTest::testEndpointCancel()
{
	auto pState = std::make_shared<FakeState>();
	pState->block = true;
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	Host host(service);

	class Streamer: public Poco::Runnable
	{
	public:
		Streamer(const Host& host, const std::string& user): _host(host), _user(user)
		{
		}

		void run() override
		{
			answer = _host.send("POST", "chat/stream", HELLO, _user);
		}

		Answer answer;

	private:
		const Host& _host;
		std::string _user;
	};

	// A caller who is not signed in cancels no turn, also not one of another
	// caller who is not signed in.
	Streamer anonymous(host, "");
	Poco::Thread anonymousThread;
	anonymousThread.start(anonymous);
	assertTrue(pState->started.tryWait(10000));
	assertEqual(0, host.send("POST", "chat/cancel?chat=c1", "", "").json()->getValue<int>("cancelled"));
	assertEqual(0, host.send("POST", "chat/cancel", "", "").json()->getValue<int>("cancelled"));
	service.cancelAll();
	anonymousThread.join();
	pState->started.reset();

	Streamer streamer(host, "alice");
	Poco::Thread thread;
	thread.start(streamer);
	assertTrue(pState->started.tryWait(10000));
	// Another user's cancel does not reach the turn.
	assertEqual(0, host.send("POST", "chat/cancel?chat=c1", "", "bob").json()->getValue<int>("cancelled"));
	assertEqual(1, host.send("POST", "chat/cancel?chat=c1&app=x").json()->getValue<int>("cancelled"));
	thread.join();
	const std::vector<Object::Ptr> events = streamEvents(streamer.answer.body);
	assertEqual(std::string("cancelled"), events.back()->getValue<std::string>("stopReason"));
	assertTrue(streamer.answer.body.find("data: [DONE]") != std::string::npos);
	assertEqual(405, host.send("GET", "chat/cancel?chat=c1").status);
}


void ChatServiceTest::testEndpointManage()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1", "m2"})}), fakeParams(pState));
	Host host(service);

	// Who may not manage the chat changes nothing of it.
	assertEqual(403, host.send("POST", "models", "{\"model\":\"m2\"}").status);
	assertEqual(403, host.send("POST", "providers", "{\"provider\":\"ollama\"}").status);
	assertEqual(403, host.send("GET", "config").status);
	assertEqual(403, host.send("POST", "config", "{\"providers\":[]}").status);
	assertEqual(403, host.send("POST", "token", "{\"provider\":\"anthropic\",\"token\":\"t\"}").status);
	assertEqual(std::string("m1"), service.model());
	// Asking a provider again changes nothing.
	Answer answer = host.send("POST", "providers", "{\"op\":\"retry\",\"provider\":\"ollama\"}");
	assertEqual(200, answer.status);
	assertTrue(answer.json()->getValue<bool>("reachable"));
	assertEqual(2u, static_cast<unsigned>(answer.json()->getArray("models")->size()));
	assertEqual(std::string("ollama"), host.send("GET", "providers").json()->getValue<std::string>("provider"));
	assertEqual(std::string("m1"), host.send("GET", "models").json()->getValue<std::string>("model"));

	answer = host.send("POST", "models", "{\"model\":\"m2\"}", "root", true);
	assertEqual(200, answer.status);
	assertEqual(std::string("m2"), answer.json()->getValue<std::string>("model"));
	assertEqual(404, host.send("POST", "models", "{\"model\":\"m3\"}", "root", true).status);
	assertEqual(std::string("m2"), service.model());
	assertEqual(404, host.send("POST", "providers", "{\"provider\":\"none\"}", "root", true).status);

	// By default the keys are the host's: none is set over the endpoint, and
	// the settings offer none to set.
	assertEqual(403, host.send("POST", "token", "{\"provider\":\"anthropic\",\"token\":\"s3cr3t\"}", "root", true).status);
	assertEqual(403, host.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"enabled\":true},{\"name\":\"openai\",\"apiKey\":\"k\"}]}", "root", true).status);
	assertTrue(!service.hasKey("anthropic") && !service.hasKey("openai"));
	Array::Ptr pDefaults = host.send("GET", "config", "", "root", true).json()->getArray("providers");
	for (std::size_t i = 0; i < pDefaults->size(); ++i)
	{
		assertTrue(!pDefaults->getObject(static_cast<unsigned>(i))->getValue<bool>("usesApiKey"));
		assertTrue(pDefaults->getObject(static_cast<unsigned>(i))->getValue<bool>("tools"));
	}

	// A host that lets them be set; the configuration never shows a key.
	ChatEndpoint::Params withKeys;
	withKeys.keysFromRequests = true;
	Host keyHost(service, withKeys);
	answer = keyHost.send("POST", "token", "{\"provider\":\"anthropic\",\"token\":\"s3cr3t\"}", "root", true);
	assertEqual(200, answer.status);
	answer = keyHost.send("GET", "config", "", "root", true);
	assertEqual(200, answer.status);
	assertTrue(answer.body.find("s3cr3t") == std::string::npos);
	const Array::Ptr pProviders = answer.json()->getArray("providers");
	assertEqual(3u, static_cast<unsigned>(pProviders->size()));
	for (std::size_t i = 0; i < pProviders->size(); ++i)
	{
		const Object::Ptr pEntry = pProviders->getObject(static_cast<unsigned>(i));
		assertTrue(!pEntry->has("apiKey"));
		const std::string name = pEntry->getValue<std::string>("name");
		assertEqual(name == "anthropic", pEntry->getValue<bool>("hasApiKey"));
		assertEqual(name != "ollama", pEntry->getValue<bool>("usesApiKey"));
		assertEqual(name == "ollama", pEntry->getValue<bool>("usesServers"));
		assertEqual(ProviderSettings::defaultBaseUrl(name), pEntry->getValue<std::string>("defaultBaseUrl"));
	}

	// Lists as lists or as comma-separated text; a new key per entry; the
	// tools on or off.
	answer = keyHost.send("POST", "config",
		"{\"providers\":[{\"name\":\"ollama\",\"enabled\":true,\"models\":\"m1, m3\",\"tools\":false},{\"name\":\"openai\",\"enabled\":true,\"models\":[\"gpt\"],\"apiKey\":\"k2\"}]}", "root", true);
	assertEqual(200, answer.status);
	assertEqual(std::string("m1"), answer.json()->getValue<std::string>("model"));
	assertEqual(2u, static_cast<unsigned>(answer.json()->getArray("models")->size()));
	assertEqual(2u, static_cast<unsigned>(answer.json()->getArray("providers")->size()));
	assertTrue(service.hasKey("openai"));
	assertTrue(!service.settings().providers[0].tools);
	assertEqual(std::string("s3cr3t"), service.settings().providers[2].apiKey);
	assertEqual(400, host.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"enabled\":false}]}", "root", true).status);
	assertEqual(400, host.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"timeout\":\"long\"}]}", "root", true).status);
	assertEqual(400, host.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"enabled\":true,\"timeout\":1e12}]}", "root", true).status);
}


void ChatServiceTest::testEndpointKeyAddress()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	const std::string defaultUrl = ProviderSettings::defaultBaseUrl("anthropic");
	const auto settingsOfProvider = [&service](const std::string& name)
	{
		for (const ProviderSettings& provider: service.settings().providers)
		{
			if (provider.name == name) return provider;
		}
		throw Poco::NotFoundException("No provider of that name", name);
	};

	// By default a provider with a key keeps the address the host gave it:
	// the key would go there.
	Host host(service);
	assertEqual(403, host.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"enabled\":true},{\"name\":\"anthropic\",\"baseUrl\":\"http://elsewhere.test\"}]}", "root", true).status);
	assertEqual(403, host.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"enabled\":true},{\"name\":\"openai\",\"servers\":[\"elsewhere\"]}]}", "root", true).status);
	assertEqual(std::string(), settingsOfProvider("anthropic").baseUrl);
	assertTrue(settingsOfProvider("openai").servers.empty());
	// Its address written out is no change, and a provider without a key moves.
	assertEqual(200, host.send("POST", "config",
		"{\"providers\":[{\"name\":\"ollama\",\"enabled\":true,\"models\":[\"m1\"],\"baseUrl\":\"http://[server]:11434/v1\",\"servers\":[\"lab\"]},{\"name\":\"anthropic\",\"baseUrl\":\"" + defaultUrl + "\"}]}", "root", true).status);
	assertEqual(std::string("lab"), settingsOfProvider("ollama").servers.at(0));

	// A host that lets a manager set the keys lets them move a provider, whose
	// key stays behind.
	ChatEndpoint::Params withKeys;
	withKeys.keysFromRequests = true;
	Host keyHost(service, withKeys);
	assertEqual(200, keyHost.send("POST", "token", "{\"provider\":\"anthropic\",\"token\":\"k1\"}", "root", true).status);
	assertTrue(service.hasKey("anthropic"));
	assertEqual(200, keyHost.send("POST", "config", "{\"providers\":[{\"name\":\"ollama\",\"enabled\":true,\"models\":[\"m1\"]},{\"name\":\"anthropic\",\"baseUrl\":\"http://elsewhere.test\"}]}", "root", true).status);
	assertTrue(!service.hasKey("anthropic"));
}


void ChatServiceTest::testEndpointCrossSite()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1", "m2"})}), fakeParams(pState));
	Host host(service);
	const auto header = [](const std::string& name, const std::string& value)
	{
		Poco::Net::NameValueCollection headers;
		headers.set(name, value);
		return headers;
	};

	// What a form of another site sends, which needs no preflight request, is
	// no JSON: 415, for a turn as for a change of the settings.
	for (const std::string& type: {std::string("text/plain"), std::string("application/x-www-form-urlencoded"), std::string("multipart/form-data; boundary=x")})
	{
		assertEqual(415, host.send("POST", "models", "{\"model\":\"m2\"}", "root", true, header("Content-Type", type)).status);
		assertEqual(415, host.send("POST", "chat/stream", HELLO, "alice", false, header("Content-Type", type)).status);
	}
	assertEqual(std::string("m1"), service.model());
	assertEqual(200, host.send("POST", "models", "{\"model\":\"m2\"}", "root", true, header("Content-Type", "application/json; charset=utf-8")).status);
	assertEqual(std::string("m2"), service.model());

	// A page of another host gets 403, also on a route without a body; a page
	// of the host the request goes to, and a client without an Origin header,
	// do not.
	const std::string self = "http://127.0.0.1:" + std::to_string(host.port());
	assertEqual(403, host.send("POST", "chat/cancel?chat=c1", "", "alice", false, header("Origin", "https://site.example")).status);
	assertEqual(403, host.send("POST", "models", "{\"model\":\"m1\"}", "root", true, header("Origin", "https://site.example")).status);
	assertEqual(403, host.send("GET", "health", "", "alice", false, header("Origin", "null")).status);
	assertEqual(std::string("m2"), service.model());
	assertEqual(200, host.send("POST", "chat/cancel?chat=c1", "", "alice", false, header("Origin", self)).status);
	assertEqual(200, host.send("POST", "chat/cancel?chat=c1").status);

	// The pages of an origin the host allows.
	ChatEndpoint::Params params;
	params.allowedOrigins = {"https://ui.example"};
	Host allowing(service, params);
	assertEqual(200, allowing.send("POST", "chat/cancel?chat=c1", "", "alice", false, header("Origin", "https://UI.example")).status);
	assertEqual(403, allowing.send("POST", "chat/cancel?chat=c1", "", "alice", false, header("Origin", "https://site.example")).status);
}


void ChatServiceTest::testEndpointRefusals()
{
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	ChatEndpoint::Params params;
	params.maxBodySize = 64;
	params.maxChatBodySize = 256;
	Host host(service, params);

	assertEqual(404, host.send("GET", "sessions").status);
	assertTrue(!ChatEndpoint::isRoute("sessions"));
	assertTrue(ChatEndpoint::isRoute("chat/stream"));
	assertEqual(413, host.send("POST", "models", "{\"model\":\"" + std::string(100, 'm') + "\"}", "root", true).status);
	assertEqual(413, host.send("POST", "chat/stream", "{\"messages\":[{\"role\":\"user\",\"content\":\"" + std::string(300, 'x') + "\"}]}").status);
	assertEqual(400, host.send("POST", "models", "[1]", "root", true).status);
	assertEqual(400, host.send("POST", "models", "{\"model\":7}", "root", true).status);
	assertEqual(405, host.send("DELETE", "models", "", "root", true).status);
}


void ChatServiceTest::testEndpointHeartbeat()
{
	auto pState = std::make_shared<FakeState>();
	pState->block = true;
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	ChatEndpoint::Params params;
	params.heartbeat = Poco::Timespan(0, 50000);
	Host host(service, params);

	Poco::Net::HTTPClientSession session("127.0.0.1", host.port());
	session.setTimeout(Poco::Timespan(20, 0));
	Poco::Net::HTTPRequest request("POST", "/api/ai/chat/stream", Poco::Net::HTTPMessage::HTTP_1_1);
	request.set("X-User", "alice");
	request.setContentType("application/json");
	request.setContentLength(static_cast<std::streamsize>(HELLO.size()));
	session.sendRequest(request) << HELLO;
	Poco::Net::HTTPResponse response;
	std::istream& in = session.receiveResponse(response);
	// While the model works, comments keep the stream alive.
	std::string line;
	bool working = false;
	while (!working && std::getline(in, line)) working = line == ": working";
	assertTrue(working);
	assertEqual(1, service.cancel("alice", "c1"));
	std::string rest;
	Poco::StreamCopier::copyToString(in, rest);
	assertTrue(rest.find("data: [DONE]") != std::string::npos);
}


void ChatServiceTest::testEndpointBodyBrokeOff()
{
	// A body that ends before its Content-Length is refused, even when what
	// came is a whole object.
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1", "m2"})}), fakeParams(pState));
	Host host(service);
	const std::string body = "{\"model\":\"m2\"}";
	std::ostringstream text;
	text << "POST /api/ai/models HTTP/1.1\r\nHost: 127.0.0.1\r\nX-User: root\r\nX-Manage: 1\r\nContent-Type: application/json\r\n";
	text << "Content-Length: " << body.size() + 50 << "\r\n\r\n" << body;
	const std::string bytes = text.str();
	Poco::Net::StreamSocket socket(Poco::Net::SocketAddress("127.0.0.1", host.port()));
	socket.setReceiveTimeout(Poco::Timespan(10, 0));
	socket.sendBytes(bytes.data(), static_cast<int>(bytes.size()));
	socket.shutdownSend();
	Poco::Net::SocketStream stream(socket);
	Poco::Net::HTTPResponse response;
	response.read(stream);
	assertEqual(400, static_cast<int>(response.getStatus()));
	assertEqual(std::string("m1"), service.model());
}


void ChatServiceTest::testStaleConnectionFailure()
{
	// A call that began before the settings changed, and then cannot connect,
	// tells nothing of the present settings.
	auto pState = std::make_shared<FakeState>();
	ProviderSettings ollama = entry("ollama", true, {"m1"});
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({ollama}), fakeParams(pState));
	pState->holdCall = true;
	pState->connectFails = true;
	Array conversation = conversationOf("hi");
	Events events;
	std::thread turn([&]()
		{
			service.chat(conversation, who("alice"), "c1", events.sink());
		});
	const bool started = pState->started.tryWait(10000);
	if (started)
	{
		ollama.timeout = 7;
		service.configure({ollama});
	}
	pState->releaseCall.set();
	turn.join();
	assertTrue(started);
	assertEqual(std::string("error,done"), events.types());
	assertEqual(std::string("provider_unreachable"), events.list[0]->getValue<std::string>("code"));
	assertTrue(!service.providerStatus()[0].probed);
}


void ChatServiceTest::testEndpointHeartbeatBounds()
{
	// A heartbeat of zero or less sends no comment, and one below the minimum
	// sends no more than one in 10 milliseconds.
	for (const Poco::Timespan& heartbeat: {Poco::Timespan(), Poco::Timespan(-1, 0), Poco::Timespan(0, 1)})
	{
		auto pState = std::make_shared<FakeState>();
		pState->block = true;
		ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
		ChatEndpoint::Params params;
		params.heartbeat = heartbeat;
		Host host(service, params);

		Poco::Net::HTTPClientSession session("127.0.0.1", host.port());
		session.setTimeout(Poco::Timespan(20, 0));
		Poco::Net::HTTPRequest request("POST", "/api/ai/chat/stream", Poco::Net::HTTPMessage::HTTP_1_1);
		request.set("X-User", "alice");
		request.setContentType("application/json");
		request.setContentLength(static_cast<std::streamsize>(HELLO.size()));
		session.sendRequest(request) << HELLO;
		Poco::Net::HTTPResponse response;
		std::istream& in = session.receiveResponse(response);
		std::string line;
		std::getline(in, line);
		assertEqual(std::string(": connected"), line);
		assertTrue(pState->started.tryWait(10000));
		if (heartbeat > Poco::Timespan())
		{
			// The comments of a fifth of a second.
			int comments = 0;
			const Poco::Timestamp reading;
			while (reading.elapsed() < 200000 && std::getline(in, line))
			{
				if (line == ": working") ++comments;
			}
			assertTrue(comments > 0);
			assertTrue(comments <= static_cast<int>(reading.elapsed() / 10000) + 2);
		}
		assertEqual(1, service.cancel("alice", "c1"));
		std::string rest;
		Poco::StreamCopier::copyToString(in, rest);
		if (heartbeat <= Poco::Timespan()) assertTrue(rest.find(": working") == std::string::npos);
		assertTrue(rest.find("data: [DONE]") != std::string::npos);
	}
}


void ChatServiceTest::testEndpointErrorClientGone()
{
	// An error reply that cannot be sent, as the client has gone away, is
	// dropped: handle() throws nothing, whatever the error.
	auto pState = std::make_shared<FakeState>();
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	ChatEndpoint endpoint(service);
	ChatEndpoint::Caller manager;
	manager.user = "root";
	manager.mayManage = true;
	struct Case
	{
		std::string method;
		std::string route;
		std::string body;
		int status;
	};
	const Case cases[] = {
		{Poco::Net::HTTPRequest::HTTP_GET, "chat/stream", "", 405},
		{Poco::Net::HTTPRequest::HTTP_POST, "models", "{\"model\":\"\"}", 400},
		{Poco::Net::HTTPRequest::HTTP_POST, "providers", "{\"provider\":\"nope\"}", 404},
		{Poco::Net::HTTPRequest::HTTP_GET, "sessions", "", 404}
	};
	for (const Case& c: cases)
	{
		GoneResponse response;
		BareRequest request(c.method, c.body, response);
		try
		{
			endpoint.handle(c.route, request, response, manager);
		}
		catch (...)
		{
			fail("handle() threw for " + c.method + " " + c.route);
		}
		assertEqual(1, response.tries);
		assertEqual(c.status, static_cast<int>(response.getStatus()));
	}
}


void ChatServiceTest::testEndpointClientGone()
{
	auto pState = std::make_shared<FakeState>();
	pState->block = true;
	ChatService service(std::make_shared<ToolRegistry>(), settingsOf({entry("ollama", true, {"m1"})}), fakeParams(pState));
	ChatEndpoint::Params params;
	params.heartbeat = Poco::Timespan(0, 20000);
	Host host(service, params);

	{
		Poco::Net::HTTPClientSession session("127.0.0.1", host.port());
		session.setTimeout(Poco::Timespan(20, 0));
		Poco::Net::HTTPRequest request("POST", "/api/ai/chat/stream", Poco::Net::HTTPMessage::HTTP_1_1);
		request.set("X-User", "alice");
		request.setContentType("application/json");
		request.setContentLength(static_cast<std::streamsize>(HELLO.size()));
		session.sendRequest(request) << HELLO;
		Poco::Net::HTTPResponse response;
		std::istream& in = session.receiveResponse(response);
		std::string line;
		std::getline(in, line);
		assertEqual(std::string(": connected"), line);
		assertTrue(pState->started.tryWait(10000));
		session.reset();
	}
	// The stream fails on its next comment, and the turn nobody reads ends.
	assertTrue(waitFor([&pState]() { return pState->cancelled.load() == 1; }));
}


void ChatServiceTest::setUp()
{
}


void ChatServiceTest::tearDown()
{
}


CppUnit::Test* ChatServiceTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ChatServiceTest");

	CppUnit_addTest(pSuite, ChatServiceTest, testTurn);
	CppUnit_addTest(pSuite, ChatServiceTest, testToolCall);
	CppUnit_addTest(pSuite, ChatServiceTest, testNoProvider);
	CppUnit_addTest(pSuite, ChatServiceTest, testModels);
	CppUnit_addTest(pSuite, ChatServiceTest, testFailover);
	CppUnit_addTest(pSuite, ChatServiceTest, testProviderDetail);
	CppUnit_addTest(pSuite, ChatServiceTest, testOneProbeAtATime);
	CppUnit_addTest(pSuite, ChatServiceTest, testCancel);
	CppUnit_addTest(pSuite, ChatServiceTest, testCancelOwnTurn);
	CppUnit_addTest(pSuite, ChatServiceTest, testSelect);
	CppUnit_addTest(pSuite, ChatServiceTest, testConfigure);
	CppUnit_addTest(pSuite, ChatServiceTest, testKeyStaysAtItsAddress);
	CppUnit_addTest(pSuite, ChatServiceTest, testStoreRefuses);
	CppUnit_addTest(pSuite, ChatServiceTest, testSettingsJSON);
	CppUnit_addTest(pSuite, ChatServiceTest, testDefaultProvider);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointHealth);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointStream);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointCancel);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointManage);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointKeyAddress);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointCrossSite);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointRefusals);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointHeartbeat);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointHeartbeatBounds);
	CppUnit_addTest(pSuite, ChatServiceTest, testStaleConnectionFailure);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointBodyBrokeOff);
	CppUnit_addTest(pSuite, ChatServiceTest, testProviderKind);
	CppUnit_addTest(pSuite, ChatServiceTest, testTokenLimit);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointErrorClientGone);
	CppUnit_addTest(pSuite, ChatServiceTest, testEndpointClientGone);

	return pSuite;
}
