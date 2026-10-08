//
// HandshakeTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HandshakeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include <sstream>
#include <vector>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;


namespace
{
	Object::Ptr initializeRequest(int id)
	{
		Object::Ptr params = new Object;
		params->set("protocolVersion", Poco::AI::MCP::PROTOCOL_VERSION);
		params->set("capabilities", Object::Ptr(new Object));
		return Request::make(Id(id), "initialize", params);
	}
}


HandshakeTest::HandshakeTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


HandshakeTest::~HandshakeTest()
{
}


void HandshakeTest::testInitialize()
{
	Server server;
	server.setServerInfo("test-server", "9.9");

	Object::Ptr resp = server.handleMessage(initializeRequest(1));
	assertTrue(!resp.isNull());
	assertTrue(resp->isObject("result"));

	Object::Ptr result = resp->getObject("result");
	assertEqual(Poco::AI::MCP::PROTOCOL_VERSION, result->getValue<std::string>("protocolVersion"));
	assertTrue(result->isObject("capabilities"));
	assertTrue(result->getObject("capabilities")->isObject("tools"));

	Object::Ptr info = result->getObject("serverInfo");
	assertEqual(std::string("test-server"), info->getValue<std::string>("name"));
	assertEqual(std::string("9.9"), info->getValue<std::string>("version"));
}


void HandshakeTest::testInitializedNotification()
{
	Server server;
	assertTrue(!server.dispatcher().initialized());

	// The initialized notification yields no reply but marks the session ready.
	Object::Ptr reply = server.handleMessage(Request::makeNotification("notifications/initialized", nullptr));
	assertTrue(reply.isNull());
	assertTrue(server.dispatcher().initialized());
}


void HandshakeTest::testPingBeforeInitialize()
{
	Server server;
	// ping is allowed even before initialize.
	Object::Ptr resp = server.handleMessage(Request::make(Id(1), "ping", nullptr));
	assertTrue(!resp.isNull());
	assertTrue(resp->isObject("result"));
	assertTrue(!resp->has("error"));
}


void HandshakeTest::testToolsRequireInitialize()
{
	Server server;
	// tools/list before initialize is rejected.
	Object::Ptr resp = server.handleMessage(Request::make(Id(1), "tools/list", nullptr));
	assertTrue(!resp.isNull());
	assertTrue(resp->isObject("error"));
	assertEqual(-32600, resp->getObject("error")->getValue<int>("code"));

	// After initialize it succeeds.
	server.handleMessage(initializeRequest(2));
	Object::Ptr ok = server.handleMessage(Request::make(Id(3), "tools/list", nullptr));
	assertTrue(ok->isObject("result"));
}


void HandshakeTest::testUnknownMethod()
{
	Server server;
	server.handleMessage(initializeRequest(1));
	Object::Ptr resp = server.handleMessage(Request::make(Id(2), "no/such/method", nullptr));
	assertTrue(resp->isObject("error"));
	assertEqual(-32601, resp->getObject("error")->getValue<int>("code"));
}


void HandshakeTest::testMethodMustBeString()
{
	Server server;

	// A request whose method is not a string is an invalid request, whatever
	// stands in its place, and none of them may throw.
	const std::vector<Poco::Dynamic::Var> methods{
		Poco::Dynamic::Var(),
		Poco::Dynamic::Var(5),
		Poco::Dynamic::Var(true),
		Poco::Dynamic::Var(Object::Ptr(new Object)),
		Poco::Dynamic::Var(Poco::JSON::Array::Ptr(new Poco::JSON::Array))};
	for (const auto& method: methods)
	{
		Object::Ptr request = new Object;
		request->set("jsonrpc", "2.0");
		request->set("id", 7);
		request->set("method", method);
		Object::Ptr resp = server.handleMessage(request);
		assertTrue(!resp.isNull());
		assertTrue(resp->isObject("error"));
		assertEqual(-32600, resp->getObject("error")->getValue<int>("code"));

		// The same without an id is a malformed notification: no reply.
		Object::Ptr notification = new Object;
		notification->set("jsonrpc", "2.0");
		notification->set("method", method);
		assertTrue(server.handleMessage(notification).isNull());
	}

	// No method at all.
	Object::Ptr request = new Object;
	request->set("jsonrpc", "2.0");
	request->set("id", 8);
	Object::Ptr resp = server.handleMessage(request);
	assertTrue(resp->isObject("error"));
	assertEqual(-32600, resp->getObject("error")->getValue<int>("code"));

	// Over the stream ingress it is an invalid request as well, not a parse error.
	std::istringstream in("{\"jsonrpc\":\"2.0\",\"id\":9,\"method\":null}\n");
	std::ostringstream out;
	server.serve(in, out);
	Poco::JSON::Parser parser;
	Object::Ptr reply = parser.parse(out.str()).extract<Object::Ptr>();
	assertTrue(reply->isObject("error"));
	assertEqual(-32600, reply->getObject("error")->getValue<int>("code"));
}


void HandshakeTest::setUp()
{
}


void HandshakeTest::tearDown()
{
}


CppUnit::Test* HandshakeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HandshakeTest");

	CppUnit_addTest(pSuite, HandshakeTest, testInitialize);
	CppUnit_addTest(pSuite, HandshakeTest, testInitializedNotification);
	CppUnit_addTest(pSuite, HandshakeTest, testPingBeforeInitialize);
	CppUnit_addTest(pSuite, HandshakeTest, testToolsRequireInitialize);
	CppUnit_addTest(pSuite, HandshakeTest, testUnknownMethod);
	CppUnit_addTest(pSuite, HandshakeTest, testMethodMustBeString);

	return pSuite;
}
