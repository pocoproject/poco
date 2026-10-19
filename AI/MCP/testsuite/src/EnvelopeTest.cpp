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
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"
#include <sstream>
#include <streambuf>


using Poco::AI::MCP::Request;
using Poco::AI::MCP::Response;
using Poco::AI::MCP::Id;
using Poco::AI::MCP::ErrorCode;
using Poco::AI::MCP::MCPException;
using Poco::JSON::Object;


namespace
{
	Object::Ptr roundTrip(const Object::Ptr& message)
	{
		Poco::JSON::Parser parser;
		return parser.parse(Poco::AI::MCP::serialize(message)).extract<Object::Ptr>();
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


	int readError(std::istream& in, std::size_t maxSize)
		/// The code of the MCPException readMessage() throws, or 0.
	{
		try
		{
			(void) Poco::AI::MCP::readMessage(in, maxSize);
		}
		catch (const MCPException& exc)
		{
			return exc.code();
		}
		return 0;
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
	Object::Ptr note = roundTrip(Request::makeNotification("notifications/cancelled", nullptr));

	assertEqual(std::string("2.0"), note->getValue<std::string>("jsonrpc"));
	assertTrue(!note->has("id"));
	assertEqual(std::string("notifications/cancelled"), note->getValue<std::string>("method"));
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
	Object::Ptr resp = roundTrip(Response::error(Id(3), ErrorCode::METHOD_NOT_FOUND, "nope"));

	assertEqual(3, resp->getValue<int>("id"));
	assertTrue(resp->isObject("error"));
	assertTrue(!resp->has("result"));
	Object::Ptr error = resp->getObject("error");
	assertEqual(-32601, error->getValue<int>("code"));
	assertEqual(std::string("nope"), error->getValue<std::string>("message"));
}


void EnvelopeTest::testUnknownId()
{
	// An error before the id is known (a parse error) has no id: the MCP
	// schema leaves it out, where JSON-RPC 2.0 writes null.
	const std::string text = Poco::AI::MCP::serialize(Response::error(Id(), ErrorCode::PARSE_ERROR, "x"));
	assertTrue(text.find("\"id\"") == std::string::npos);

	Object::Ptr resp = roundTrip(Response::error(Id(), ErrorCode::PARSE_ERROR, "x"));
	assertTrue(!resp->has("id"));
	assertEqual(-32700, resp->getObject("error")->getValue<int>("code"));
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
	params->set("text", "two\nlines");
	const std::string text = Poco::AI::MCP::serialize(Request::make(Id(1), "tools/list", params));
	assertTrue(text.find('\n') == std::string::npos);
}


void EnvelopeTest::testValidIds()
{
	using Poco::AI::MCP::isValidId;
	assertTrue(isValidId(Id(1)));
	assertTrue(isValidId(Id(Poco::Int64(-5))));
	assertTrue(isValidId(Id(std::string("a"))));
	assertTrue(isValidId(Id(std::string())));
	assertTrue(!isValidId(Id()));
	assertTrue(!isValidId(Id(true)));
	assertTrue(!isValidId(Id(1.5)));
	assertTrue(!isValidId(Id(Object::Ptr(new Object))));
	assertTrue(!isValidId(Id(Poco::JSON::Array::Ptr(new Poco::JSON::Array))));
}


void EnvelopeTest::testIsResponseTo()
{
	using Poco::AI::MCP::isResponseTo;
	const Object::Ptr pResult = roundTrip(Response::result(Id(7), Object::Ptr(new Object)));
	assertTrue(isResponseTo(pResult, Id(7)));
	assertTrue(isResponseTo(pResult, Id(Poco::Int64(7))));
	assertTrue(!isResponseTo(pResult, Id(8)));
	// A string id and a number id never match.
	assertTrue(!isResponseTo(pResult, Id(std::string("7"))));

	const Object::Ptr pError = roundTrip(Response::error(Id(std::string("x")), ErrorCode::INTERNAL_ERROR, "e"));
	assertTrue(isResponseTo(pError, Id(std::string("x"))));
	assertTrue(!isResponseTo(pError, Id(std::string("y"))));

	// A request or a notification is not a response.
	assertTrue(!isResponseTo(roundTrip(Request::make(Id(7), "tools/list")), Id(7)));
	assertTrue(!isResponseTo(Object::Ptr(), Id(7)));
}


void EnvelopeTest::testReadMessage()
{
	std::istringstream in("\n  \r\n{\"a\":1}\r\n{\"b\":2}");
	Object::Ptr pFirst = Poco::AI::MCP::readMessage(in);
	assertTrue(!pFirst.isNull());
	assertEqual(1, pFirst->getValue<int>("a"));
	// The last line needs no newline.
	Object::Ptr pSecond = Poco::AI::MCP::readMessage(in);
	assertTrue(!pSecond.isNull());
	assertEqual(2, pSecond->getValue<int>("b"));
	assertTrue(Poco::AI::MCP::readMessage(in).isNull());
	assertTrue(Poco::AI::MCP::readMessage(in).isNull());
}


void EnvelopeTest::testReadMessageLimit()
{
	// A line longer than the limit is refused and skipped; the next line is read.
	std::istringstream in(std::string("{\"long\":\"") + std::string(100, 'x') + "\"}\n{\"short\":1}\n");
	assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), readError(in, 64));
	Object::Ptr pNext = Poco::AI::MCP::readMessage(in, 64);
	assertTrue(!pNext.isNull());
	assertEqual(1, pNext->getValue<int>("short"));

	// A line of exactly the limit is read, one byte more is refused: the
	// limit counts the line without its newline.
	const std::string atLimit = std::string("{\"k\":\"") + std::string(56, 'x') + "\"}";
	assertEqual(64u, static_cast<unsigned>(atLimit.size()));
	std::istringstream exact(atLimit + "\n");
	assertTrue(!Poco::AI::MCP::readMessage(exact, 64).isNull());
	std::istringstream over(std::string("{\"k\":\"") + std::string(57, 'x') + "\"}\n{\"after\":1}\n");
	try
	{
		(void) Poco::AI::MCP::readMessage(over, 64);
		fail("a line one byte over the limit must be refused");
	}
	catch (const Poco::AI::MCP::MessageTooLargeException& exc)
	{
		assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), exc.code());
	}
	assertEqual(1, Poco::AI::MCP::readMessage(over, 64)->getValue<int>("after"));
}


void EnvelopeTest::testReadMessageNotAnObject()
{
	// JSON that is not an object is not a valid request (-32600); what is
	// not JSON at all cannot be parsed (-32700).
	std::istringstream in("not json\n[1,2]\n\"text\"\n42\ntrue\nnull\n1,2\n{\"ok\":true}\n");
	assertEqual(static_cast<int>(ErrorCode::PARSE_ERROR), readError(in, 1024));
	assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), readError(in, 1024));
	assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), readError(in, 1024));
	assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), readError(in, 1024));
	assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), readError(in, 1024));
	assertEqual(static_cast<int>(ErrorCode::INVALID_REQUEST), readError(in, 1024));
	assertEqual(static_cast<int>(ErrorCode::PARSE_ERROR), readError(in, 1024));
	assertTrue(Poco::AI::MCP::readMessage(in)->getValue<bool>("ok"));
}


void EnvelopeTest::testWriteMessageFailure()
{
	FailingBuf buf;
	std::ostream out(&buf);
	try
	{
		Poco::AI::MCP::writeMessage(out, Request::make(Id(1), "tools/list"));
		fail("a failed stream must throw");
	}
	catch (const Poco::WriteFileException&)
	{
	}
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
	CppUnit_addTest(pSuite, EnvelopeTest, testUnknownId);
	CppUnit_addTest(pSuite, EnvelopeTest, testIdTypeFidelity);
	CppUnit_addTest(pSuite, EnvelopeTest, testSerializeIsSingleLine);
	CppUnit_addTest(pSuite, EnvelopeTest, testValidIds);
	CppUnit_addTest(pSuite, EnvelopeTest, testIsResponseTo);
	CppUnit_addTest(pSuite, EnvelopeTest, testReadMessage);
	CppUnit_addTest(pSuite, EnvelopeTest, testReadMessageLimit);
	CppUnit_addTest(pSuite, EnvelopeTest, testReadMessageNotAnObject);
	CppUnit_addTest(pSuite, EnvelopeTest, testWriteMessageFailure);

	return pSuite;
}
