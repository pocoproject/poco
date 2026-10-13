//
// EnvelopeTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EnvelopeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Object.h"


using Poco::AI::MCP::Request;
using Poco::AI::MCP::Response;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;


namespace
{
	Object::Ptr roundTrip(const Object::Ptr& message)
	{
		Poco::JSON::Parser parser;
		return parser.parse(Poco::AI::MCP::serialize(message)).extract<Object::Ptr>();
	}
}


EnvelopeTest::EnvelopeTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


EnvelopeTest::~EnvelopeTest()
{
}


void EnvelopeTest::testRequestBuild()
{
	Object::Ptr params = new Object;
	params->set("k", "v");
	Object::Ptr req = roundTrip(Request::make(Id(7), "tools/list", params));

	assertEqual(std::string("2.0"), req->getValue<std::string>("jsonrpc"));
	assertEqual(7, req->getValue<int>("id"));
	assertEqual(std::string("tools/list"), req->getValue<std::string>("method"));
	assertTrue(req->isObject("params"));
}


void EnvelopeTest::testNotificationBuild()
{
	Object::Ptr note = roundTrip(Request::makeNotification("notifications/initialized", nullptr));

	assertEqual(std::string("2.0"), note->getValue<std::string>("jsonrpc"));
	assertTrue(!note->has("id"));
	assertEqual(std::string("notifications/initialized"), note->getValue<std::string>("method"));
}


void EnvelopeTest::testResultRoundTrip()
{
	Object::Ptr result = new Object;
	result->set("ok", true);
	Object::Ptr resp = roundTrip(Response::result(Id(std::string("abc")), result));

	assertEqual(std::string("abc"), resp->getValue<std::string>("id"));
	assertTrue(resp->isObject("result"));
	assertTrue(!resp->has("error"));
	assertTrue(resp->getObject("result")->getValue<bool>("ok"));
}


void EnvelopeTest::testErrorRoundTrip()
{
	Object::Ptr resp = roundTrip(Response::error(Id(3), Poco::AI::MCP::ErrorCode::MethodNotFound, "nope"));

	assertEqual(3, resp->getValue<int>("id"));
	assertTrue(resp->isObject("error"));
	assertTrue(!resp->has("result"));
	Object::Ptr error = resp->getObject("error");
	assertEqual(-32601, error->getValue<int>("code"));
	assertEqual(std::string("nope"), error->getValue<std::string>("message"));
}


void EnvelopeTest::testNullId()
{
	// A pre-dispatch error (parse error) has no request id, so id must be null.
	const std::string text = Poco::AI::MCP::serialize(Response::error(Id(), Poco::AI::MCP::ErrorCode::ParseError, "x"));
	assertTrue(text.find("\"id\":null") != std::string::npos);

	Object::Ptr resp = roundTrip(Response::error(Id(), Poco::AI::MCP::ErrorCode::ParseError, "x"));
	assertTrue(resp->has("id"));
	assertTrue(resp->isNull("id"));
}


void EnvelopeTest::testIdTypeFidelity()
{
	// Numeric ids stay numeric; string ids stay strings.
	Object::Ptr numeric = roundTrip(Response::result(Id(42), Object::Ptr(new Object)));
	assertEqual(42, numeric->getValue<int>("id"));

	Object::Ptr str = roundTrip(Response::result(Id(std::string("id-1")), Object::Ptr(new Object)));
	assertEqual(std::string("id-1"), str->getValue<std::string>("id"));
}


void EnvelopeTest::testSerializeIsSingleLine()
{
	Object::Ptr params = new Object;
	params->set("a", "b");
	const std::string text = Poco::AI::MCP::serialize(Request::make(Id(1), "ping", params));
	assertTrue(text.find('\n') == std::string::npos);
}


void EnvelopeTest::setUp()
{
}


void EnvelopeTest::tearDown()
{
}


CppUnit::Test* EnvelopeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EnvelopeTest");

	CppUnit_addTest(pSuite, EnvelopeTest, testRequestBuild);
	CppUnit_addTest(pSuite, EnvelopeTest, testNotificationBuild);
	CppUnit_addTest(pSuite, EnvelopeTest, testResultRoundTrip);
	CppUnit_addTest(pSuite, EnvelopeTest, testErrorRoundTrip);
	CppUnit_addTest(pSuite, EnvelopeTest, testNullId);
	CppUnit_addTest(pSuite, EnvelopeTest, testIdTypeFidelity);
	CppUnit_addTest(pSuite, EnvelopeTest, testSerializeIsSingleLine);

	return pSuite;
}
