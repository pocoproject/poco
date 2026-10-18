//
// StreamIngressTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StreamIngressTest.h"
#include "MCPTestHelpers.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::ErrorCode;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;
using MCPTest::request;
using MCPTest::errorCode;
using MCPTest::code;


namespace
{
	void buildServer(Server& server)
	{
		server.registerTool("echo", "Echo back the message.", nullptr,
			[](const Object::Ptr& pArgs)
			{
				ToolResult toolResult;
				toolResult.addText(pArgs->optValue<std::string>("message", ""));
				return toolResult;
			});
	}


	std::vector<Object::Ptr> serve(Server& server, const std::string& input, std::size_t maxMessageSize = Poco::AI::MCP::DEFAULT_MAX_MESSAGE_SIZE)
		/// Serves the input and returns the replies, one per output line.
	{
		std::istringstream in(input);
		std::ostringstream out;
		server.serve(in, out, maxMessageSize);
		std::vector<Object::Ptr> replies;
		std::istringstream lines(out.str());
		std::string line;
		while (std::getline(lines, line))
		{
			Poco::JSON::Parser parser;
			replies.push_back(parser.parse(line).extract<Object::Ptr>());
		}
		return replies;
	}


	std::string line(const Object::Ptr& pMessage)
	{
		return Poco::AI::MCP::serialize(pMessage) + "\n";
	}


	class FailingBuf: public std::streambuf
		/// A stream buffer whose every write fails.
	{
	protected:
		int_type overflow(int_type) override
		{
			return traits_type::eof();
		}
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
	buildServer(server);
	Object::Ptr pArgs = new Object;
	pArgs->set("message", "hi");
	Object::Ptr pCall = new Object;
	pCall->set("name", "echo");
	pCall->set("arguments", pArgs);

	// Blank lines and CRLF line ends are fine; notifications get no reply.
	const std::string input = "\n\r\n" + line(request(Id(1), "server/discover"))
		+ line(Request::makeNotification("notifications/cancelled"))
		+ Poco::AI::MCP::serialize(request(Id(2), "tools/call", pCall)) + "\r\n"
		+ "   \n" + Poco::AI::MCP::serialize(request(Id(3), "tools/list"));
	std::vector<Object::Ptr> replies = serve(server, input);
	assertEqual(3u, static_cast<unsigned>(replies.size()));
	assertEqual(1, replies[0]->getValue<int>("id"));
	assertEqual(2, replies[1]->getValue<int>("id"));
	assertEqual(std::string("hi"), replies[1]->getObject("result")->getArray("content")->getObject(0)->getValue<std::string>("text"));
	assertEqual(3, replies[2]->getValue<int>("id"));
}


void StreamIngressTest::testParseError()
{
	Server server;
	buildServer(server);
	std::vector<Object::Ptr> replies = serve(server, "this is not json\n" + line(request(Id(1), "tools/list")));
	assertEqual(2u, static_cast<unsigned>(replies.size()));
	assertEqual(code(ErrorCode::PARSE_ERROR), errorCode(replies[0]));
	assertTrue(replies[0]->isNull("id"));
	// Serving goes on.
	assertEqual(0, errorCode(replies[1]));
}


void StreamIngressTest::testBatchRejected()
{
	Server server;
	buildServer(server);
	std::vector<Object::Ptr> replies = serve(server, "[" + Poco::AI::MCP::serialize(request(Id(1), "tools/list")) + "]\n");
	assertEqual(1u, static_cast<unsigned>(replies.size()));
	assertEqual(code(ErrorCode::INVALID_REQUEST), errorCode(replies[0]));
	assertTrue(replies[0]->isNull("id"));
}


void StreamIngressTest::testOversizedLine()
{
	Server server;
	buildServer(server);
	Object::Ptr pArgs = new Object;
	pArgs->set("message", std::string(4096, 'x'));
	Object::Ptr pCall = new Object;
	pCall->set("name", "echo");
	pCall->set("arguments", pArgs);
	std::vector<Object::Ptr> replies = serve(server, line(request(Id(1), "tools/call", pCall)) + line(request(Id(2), "tools/list")), 1024);
	assertEqual(2u, static_cast<unsigned>(replies.size()));
	assertEqual(code(ErrorCode::INVALID_REQUEST), errorCode(replies[0]));
	assertTrue(replies[0]->isNull("id"));
	assertEqual(2, replies[1]->getValue<int>("id"));
	assertEqual(0, errorCode(replies[1]));
}


void StreamIngressTest::testOutputFailureStops()
{
	Server server;
	buildServer(server);
	std::istringstream in(line(request(Id(1), "tools/list")) + line(request(Id(2), "tools/list")));
	FailingBuf buf;
	std::ostream out(&buf);
	try
	{
		server.serve(in, out);
		fail("serve() must stop when its output fails");
	}
	catch (const Poco::WriteFileException&)
	{
	}
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
	CppUnit_addTest(pSuite, StreamIngressTest, testOversizedLine);
	CppUnit_addTest(pSuite, StreamIngressTest, testOutputFailureStops);

	return pSuite;
}
