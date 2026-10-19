//
// MCPHostTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MCPHostTest.h"
#include "TestHTTPHost.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCPHost.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/StreamClient.h"
#include "Poco/AI/MCP/Client.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include <memory>
#include <string>
#include <vector>


using Poco::AI::MCPHost;
using Poco::AI::ToolRegistry;
using Poco::AI::MCP::Server;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::RequestContext;
using Poco::AI::MCP::HTTP::HTTPClient;
using Poco::JSON::Object;


namespace
{
	ToolResult echo(const Object::Ptr& pArgs)
	{
		ToolResult result;
		result.addText(pArgs->optValue<std::string>("message", ""));
		return result;
	}


	void buildServer(Server& server, const std::string& prefix)
	{
		Poco::AI::MCP::Implementation info;
		info.name = prefix + "-server";
		info.version = "1";
		server.setServerInfo(info);
		server.registerTool(prefix + "_echo", "Echoes.", nullptr, echo);
	}


	class ToollessClient: public Poco::AI::MCP::Client
		/// Talks to a server whose discover result names no tools capability,
		/// and keeps the methods it is asked to send.
	{
	public:
		std::vector<std::string> methods;

	protected:
		Object::Ptr exchange(const Object::Ptr& request) override
		{
			const std::string method = request->getValue<std::string>("method");
			methods.push_back(method);
			Object::Ptr pReply = new Object;
			pReply->set("jsonrpc", "2.0");
			pReply->set("id", request->get("id"));
			if (method != "server/discover")
			{
				Object::Ptr pError = new Object;
				pError->set("code", -32601);
				pError->set("message", "Method not found: " + method);
				pReply->set("error", pError);
				return pReply;
			}
			Poco::JSON::Array::Ptr pVersions = new Poco::JSON::Array;
			pVersions->add(std::string(Poco::AI::MCP::PROTOCOL_VERSION));
			Object::Ptr pCapabilities = new Object;
			pCapabilities->set("resources", Object::Ptr(new Object));
			Object::Ptr pResult = new Object;
			pResult->set("supportedVersions", pVersions);
			pResult->set("capabilities", pCapabilities);
			pResult->set("resultType", "complete");
			pReply->set("result", pResult);
			return pReply;
		}
	};


	class StreamServer
		/// Serves a Server over a pair of pipes on a thread of its own, for a StreamClient.
	{
	public:
		explicit StreamServer(Server& server):
			_server(server)
		{
			_thread.startFunc([this]()
			{
				Poco::PipeInputStream in(_toServer);
				Poco::PipeOutputStream out(_toClient);
				try
				{
					_server.serve(in, out);
				}
				catch (const Poco::Exception&)
				{
				}
			});
			_pOut = std::make_unique<Poco::PipeOutputStream>(_toServer);
			_pIn = std::make_unique<Poco::PipeInputStream>(_toClient);
		}

		~StreamServer()
		{
			_pOut.reset();
			_toServer.close(Poco::Pipe::CLOSE_WRITE);
			_thread.join();
		}

		std::unique_ptr<Poco::AI::MCP::Client> client()
		{
			return std::make_unique<Poco::AI::MCP::StreamClient>(*_pIn, *_pOut);
		}

	private:
		Server& _server;
		Poco::Pipe _toServer;
		Poco::Pipe _toClient;
		std::unique_ptr<Poco::PipeOutputStream> _pOut;
		std::unique_ptr<Poco::PipeInputStream> _pIn;
		Poco::Thread _thread;
	};
}


MCPHostTest::MCPHostTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


MCPHostTest::~MCPHostTest()
{
}


void MCPHostTest::testRegisterIntoOverHTTP()
{
	Server server;
	buildServer(server, "web");
	TestHTTPHost host(server);
	host.start();

	MCPHost mcpHost;
	mcpHost.addServer("web", std::make_unique<HTTPClient>(Poco::URI(host.url())));
	assertEqual(std::size_t(1), mcpHost.serverCount());
	ToolRegistry registry;
	assertEqual(std::size_t(1), mcpHost.registerInto(registry));
	assertTrue(registry.hasTool("web_echo"));
	assertEqual(std::string("web"), mcpHost.toolOwners().at("web_echo"));
	ToolResult result = registry.executeTool("web_echo", Poco::JSON::Parser().parse("{\"message\":\"over http\"}").extract<Object::Ptr>());
	assertEqual(std::string("over http"), result.content()->getObject(0)->getValue<std::string>("text"));
}


void MCPHostTest::testRegisterIntoOverStreams()
{
	Server server;
	buildServer(server, "local");
	StreamServer streams(server);

	MCPHost mcpHost;
	mcpHost.addServer("local", streams.client());
	ToolRegistry registry;
	assertEqual(std::size_t(1), mcpHost.registerInto(registry));
	ToolResult result = registry.executeTool("local_echo", Poco::JSON::Parser().parse("{\"message\":\"over streams\"}").extract<Object::Ptr>());
	assertEqual(std::string("over streams"), result.content()->getObject(0)->getValue<std::string>("text"));
}


void MCPHostTest::testNamesTheModelsRefuse()
{
	Server server;
	server.registerTool("fs.read", "Dots are fine for MCP.", nullptr, echo);
	server.registerTool(std::string(65, 'n'), "Too long for a model.", nullptr, echo);
	server.registerTool("good-name_1", "Fine.", nullptr, echo);
	TestHTTPHost host(server);
	host.start();

	MCPHost mcpHost;
	mcpHost.addServer("odd", std::make_unique<HTTPClient>(Poco::URI(host.url())));
	ToolRegistry registry;
	std::vector<std::string> skipped;
	assertEqual(std::size_t(1), mcpHost.registerInto(registry, &skipped));
	assertTrue(registry.hasTool("good-name_1"));
	assertEqual(2u, static_cast<unsigned>(skipped.size()));
	assertTrue(skipped[0].find("odd: fs.read") == 0 || skipped[1].find("odd: fs.read") == 0);
}


void MCPHostTest::testCollisions()
{
	Server first;
	first.registerTool("shared", "From the first.", nullptr, echo);
	Server second;
	second.registerTool("shared", "From the second.", nullptr, echo);
	second.registerTool("native", "Clashes with a native tool.", nullptr, echo);
	TestHTTPHost firstHost(first);
	firstHost.start();
	TestHTTPHost secondHost(second);
	secondHost.start();

	ToolRegistry registry;
	registry.registerTool("native", "", nullptr, [](const Object::Ptr&) { return std::string("native"); });
	MCPHost mcpHost;
	mcpHost.addServer("one", std::make_unique<HTTPClient>(Poco::URI(firstHost.url())));
	mcpHost.addServer("two", std::make_unique<HTTPClient>(Poco::URI(secondHost.url())));
	std::vector<std::string> skipped;
	assertEqual(std::size_t(1), mcpHost.registerInto(registry, &skipped));
	assertEqual(std::string("one"), mcpHost.toolOwners().at("shared"));
	assertEqual(2u, static_cast<unsigned>(skipped.size()));
	// The first server owns the name; the native tool is untouched.
	assertEqual(std::string("two: native (the name is taken)"), skipped[0]);
	assertEqual(std::string("two: shared (the name is taken by one)"), skipped[1]);
	assertEqual(std::string("native"), registry.executeTool("native", nullptr).content()->getObject(0)->getValue<std::string>("text"));
}


void MCPHostTest::testResultPassesThrough()
{
	Server server;
	Poco::AI::MCP::Tool tool;
	tool.name = "structured";
	tool.outputSchema = Poco::JSON::Parser().parse("{\"type\":\"object\"}").extract<Object::Ptr>();
	tool.annotations = Poco::JSON::Parser().parse("{\"readOnlyHint\":true}").extract<Object::Ptr>();
	tool.handler = [](const Object::Ptr&, const RequestContext&)
	{
		ToolResult result;
		result.setStructuredContent(Poco::Dynamic::Var(Poco::JSON::Parser().parse("{\"answer\":42}").extract<Object::Ptr>()));
		result.setError();
		return result;
	};
	server.registerTool(tool);
	TestHTTPHost host(server);
	host.start();

	MCPHost mcpHost;
	mcpHost.addServer("s", std::make_unique<HTTPClient>(Poco::URI(host.url())));
	ToolRegistry registry;
	(void) mcpHost.registerInto(registry);
	// The tool keeps its output schema and annotations.
	const Poco::AI::MCP::Tool registered = registry.tools()[0];
	assertTrue(!registered.outputSchema.isNull());
	assertTrue(registered.annotations->getValue<bool>("readOnlyHint"));
	// The result keeps its error flag and structured content.
	ToolResult result = registry.executeTool("structured", nullptr);
	assertTrue(result.isError());
	assertEqual(42, result.structuredContent().extract<Object::Ptr>()->getValue<int>("answer"));
	assertEqual(std::string("{\"answer\":42}"), Poco::AI::toolResultText(result));
}


void MCPHostTest::testAddServerFailure()
{
	MCPHost mcpHost;
	try
	{
		mcpHost.addServer("gone", std::make_unique<HTTPClient>(Poco::URI("http://127.0.0.1:1/mcp")));
		fail("an unreachable server must throw");
	}
	catch (const Poco::Exception&)
	{
	}
	assertEqual(std::size_t(0), mcpHost.serverCount());
	try
	{
		mcpHost.addServer("none", std::unique_ptr<Poco::AI::MCP::Client>());
		fail("a null client must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
}


void MCPHostTest::testDuplicateLabel()
{
	Server server;
	buildServer(server, "dup");
	TestHTTPHost host(server);
	host.start();
	MCPHost mcpHost;
	mcpHost.addServer("same", std::make_unique<HTTPClient>(Poco::URI(host.url())));
	try
	{
		mcpHost.addServer("same", std::make_unique<HTTPClient>(Poco::URI(host.url())));
		fail("a label must be unique");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	assertEqual(std::size_t(1), mcpHost.serverCount());
}


void MCPHostTest::testServerWithoutTools()
{
	// A server that offers no tools is not asked for them.
	auto pClient = std::make_unique<ToollessClient>();
	ToollessClient& client = *pClient;
	MCPHost mcpHost;
	mcpHost.addServer("docs", std::move(pClient));
	assertEqual(std::size_t(1), mcpHost.serverCount());
	assertEqual(1u, static_cast<unsigned>(client.methods.size()));
	assertEqual(std::string("server/discover"), client.methods[0]);
	ToolRegistry registry;
	assertEqual(std::size_t(0), mcpHost.registerInto(registry));
}


void MCPHostTest::testPrefixedNames()
{
	// Under PREFIXED naming, tools of the same name on two servers keep apart,
	// and a name with a dot becomes one the models take; each still calls its
	// server's tool by the server's name.
	Server first;
	first.registerTool("fs.read", "Reads.", nullptr, [](const Object::Ptr&) { ToolResult result; result.addText("first"); return result; });
	Server second;
	second.registerTool("fs.read", "Reads.", nullptr, [](const Object::Ptr&) { ToolResult result; result.addText("second"); return result; });
	TestHTTPHost firstHost(first);
	firstHost.start();
	TestHTTPHost secondHost(second);
	secondHost.start();

	MCPHost mcpHost;
	assertTrue(mcpHost.getNaming() == MCPHost::Naming::REMOTE);
	mcpHost.setNaming(MCPHost::Naming::PREFIXED);
	assertTrue(mcpHost.getNaming() == MCPHost::Naming::PREFIXED);
	mcpHost.addServer("one", std::make_unique<HTTPClient>(Poco::URI(firstHost.url())));
	mcpHost.addServer("two hosts", std::make_unique<HTTPClient>(Poco::URI(secondHost.url())));
	ToolRegistry registry;
	std::vector<std::string> skipped;
	assertEqual(std::size_t(2), mcpHost.registerInto(registry, &skipped));
	assertTrue(skipped.empty());
	assertEqual(std::string("first"), registry.executeTool("one_fs_read", nullptr).content()->getObject(0)->getValue<std::string>("text"));
	assertEqual(std::string("second"), registry.executeTool("two_hosts_fs_read", nullptr).content()->getObject(0)->getValue<std::string>("text"));
	assertEqual(std::string("two hosts"), mcpHost.toolOwners().at("two_hosts_fs_read"));
}


void MCPHostTest::testCancelledRunCallsNoTool()
{
	// A tool of a run that is cancelled is not called on its server.
	Server server;
	int calls = 0;
	server.registerTool("count", "Counts.", nullptr, [&calls](const Object::Ptr&) { ++calls; return ToolResult(); });
	TestHTTPHost host(server);
	host.start();
	MCPHost mcpHost;
	mcpHost.addServer("s", std::make_unique<HTTPClient>(Poco::URI(host.url())));
	ToolRegistry registry;
	(void) mcpHost.registerInto(registry);

	RequestContext context;
	context.cancelled = []() { return true; };
	ToolResult result = registry.executeTool("count", nullptr, context);
	assertTrue(result.isError());
	assertEqual(0, calls);
	(void) registry.executeTool("count", nullptr);
	assertEqual(1, calls);
}


void MCPHostTest::setUp()
{
}


void MCPHostTest::tearDown()
{
}


CppUnit::Test* MCPHostTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MCPHostTest");

	CppUnit_addTest(pSuite, MCPHostTest, testRegisterIntoOverHTTP);
	CppUnit_addTest(pSuite, MCPHostTest, testRegisterIntoOverStreams);
	CppUnit_addTest(pSuite, MCPHostTest, testNamesTheModelsRefuse);
	CppUnit_addTest(pSuite, MCPHostTest, testCollisions);
	CppUnit_addTest(pSuite, MCPHostTest, testResultPassesThrough);
	CppUnit_addTest(pSuite, MCPHostTest, testAddServerFailure);
	CppUnit_addTest(pSuite, MCPHostTest, testDuplicateLabel);
	CppUnit_addTest(pSuite, MCPHostTest, testServerWithoutTools);
	CppUnit_addTest(pSuite, MCPHostTest, testPrefixedNames);
	CppUnit_addTest(pSuite, MCPHostTest, testCancelledRunCallsNoTool);

	return pSuite;
}
