//
// StreamIngressTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StreamIngressTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Client.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <sstream>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Client;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	void registerEcho(Server& server)
	{
		server.registerTool("echo", "Echo back the message.", new Object,
			[](Object::Ptr args)
			{
				ToolResult result;
				result.addText(args && args->has("message") ? args->getValue<std::string>("message") : std::string());
				return result;
			});
	}

	Object::Ptr initializeRequest(int id)
	{
		Object::Ptr params = new Object;
		params->set("protocolVersion", Poco::AI::MCP::PROTOCOL_VERSION);
		params->set("capabilities", Object::Ptr(new Object));
		return Request::make(Id(id), "initialize", params);
	}

	std::string firstText(const ToolResult& result)
	{
		Array::Ptr content = result.content();
		if (content && content->size() > 0 && content->getObject(0)->has("text"))
		{
			return content->getObject(0)->getValue<std::string>("text");
		}
		return std::string();
	}

	int countResponses(const std::string& output)
	{
		std::istringstream rs(output);
		std::string line;
		int count = 0;
		while (std::getline(rs, line))
		{
			if (line.empty())
			{
				continue;
			}
			Poco::JSON::Parser parser;
			Object::Ptr obj = parser.parse(line).extract<Object::Ptr>();
			(void) obj;
			++count;
		}
		return count;
	}

	class ServeRunnable: public Poco::Runnable
	{
	public:
		ServeRunnable(Server& server, Poco::Pipe& in, Poco::Pipe& out):
			_server(server), _in(in), _out(out)
		{
		}

		void run() override
		{
			Poco::PipeInputStream input(_in);
			Poco::PipeOutputStream output(_out);
			_server.serve(input, output);
		}

	private:
		Server& _server;
		Poco::Pipe& _in;
		Poco::Pipe& _out;
	};
}


StreamIngressTest::StreamIngressTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


StreamIngressTest::~StreamIngressTest()
{
}


void StreamIngressTest::testFraming()
{
	Server server;
	registerEcho(server);

	std::ostringstream in;
	in << Poco::AI::MCP::serialize(initializeRequest(1)) << "\n";
	in << "\n";   // blank line is skipped
	in << Poco::AI::MCP::serialize(Request::makeNotification("notifications/initialized", nullptr)) << "\n";
	in << Poco::AI::MCP::serialize(Request::make(Id(2), "ping", nullptr)) << "\n";
	in << Poco::AI::MCP::serialize(Request::make(Id(3), "tools/list", nullptr)) << "\n";

	std::istringstream input(in.str());
	std::ostringstream output;
	server.serve(input, output);

	// Three requests get replies; the notification gets none. Every reply parses
	// as its own JSON object (one message per line).
	assertEqual(3, countResponses(output.str()));
}


void StreamIngressTest::testParseError()
{
	Server server;
	std::istringstream input("{ this is not json\n");
	std::ostringstream output;
	server.serve(input, output);

	Poco::JSON::Parser parser;
	Object::Ptr resp = parser.parse(output.str()).extract<Object::Ptr>();
	assertTrue(resp->isObject("error"));
	assertEqual(-32700, resp->getObject("error")->getValue<int>("code"));
}


void StreamIngressTest::testBatchRejected()
{
	Server server;
	// A top-level array (JSON-RPC batch) is unsupported in this revision.
	std::istringstream input("[]\n");
	std::ostringstream output;
	server.serve(input, output);

	Poco::JSON::Parser parser;
	Object::Ptr resp = parser.parse(output.str()).extract<Object::Ptr>();
	assertTrue(resp->isObject("error"));
	assertEqual(-32600, resp->getObject("error")->getValue<int>("code"));
}


void StreamIngressTest::testClientRoundTrip()
{
	Server server;
	server.setServerInfo("stream-server", "1.0.0");
	registerEcho(server);

	Poco::Pipe toServer;
	Poco::Pipe toClient;
	ServeRunnable runnable(server, toServer, toClient);
	Poco::Thread thread;
	thread.start(runnable);

	Poco::PipeOutputStream clientOut(toServer);
	Poco::PipeInputStream clientIn(toClient);
	Client client(clientIn, clientOut);

	Object::Ptr initResult = client.initialize("test-client", "1.0.0");
	assertEqual(Poco::AI::MCP::PROTOCOL_VERSION, initResult->getValue<std::string>("protocolVersion"));

	Array::Ptr tools = client.listTools();
	assertEqual(1, static_cast<int>(tools->size()));

	Object::Ptr args = new Object;
	args->set("message", "hi");
	assertEqual(std::string("hi"), firstText(client.callTool("echo", args)));

	client.ping();

	toServer.close(Poco::Pipe::CLOSE_WRITE);
	thread.join();
}


void StreamIngressTest::setUp()
{
}


void StreamIngressTest::tearDown()
{
}


CppUnit::Test* StreamIngressTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StreamIngressTest");

	CppUnit_addTest(pSuite, StreamIngressTest, testFraming);
	CppUnit_addTest(pSuite, StreamIngressTest, testParseError);
	CppUnit_addTest(pSuite, StreamIngressTest, testBatchRejected);
	CppUnit_addTest(pSuite, StreamIngressTest, testClientRoundTrip);

	return pSuite;
}
