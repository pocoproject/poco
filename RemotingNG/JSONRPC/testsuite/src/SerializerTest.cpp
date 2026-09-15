//
// SerializerTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "SerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include <sstream>


using namespace Poco::RemotingNG::JSONRPC;
using namespace std::string_literals;


SerializerTest::SerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


SerializerTest::~SerializerTest()
{
}


void SerializerTest::testEmptyRequest()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"method\": \"test\",\n"
		"\t\"params\":\n"
		"\t{\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testOneParamRequest()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"method\": \"test\",\n"
		"\t\"params\":\n"
		"\t{\n"
		"\t\t\"p1\": 42\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("p1", 42);
	ser.serializeMessageEnd("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testTwoParamRequest()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"method\": \"test\",\n"
		"\t\"params\":\n"
		"\t{\n"
		"\t\t\"p1\": 42,\n"
		"\t\t\"p2\": \"foo\"\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("p1", 42);
	ser.serialize("p2", "foo"s);
	ser.serializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testEmptyReply()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"result\":\n"
		"\t{\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serializeMessageEnd("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testOneParamReply()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"result\":\n"
		"\t{\n"
		"\t\t\"return\": 42\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serialize("return"s, 42);
	ser.serializeMessageEnd("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testTwoParamReply()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"result\":\n"
		"\t{\n"
		"\t\t\"return\": 42,\n"
		"\t\t\"p1\": \"bar\"\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serialize("return"s, 42);
	ser.serialize("p1"s, "bar"s);
	ser.serializeMessageEnd("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testComplexRequest()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"method\": \"test\",\n"
		"\t\"params\":\n"
		"\t{\n"
		"\t\t\"p1\":\n"
		"\t\t[\n"
		"\t\t\t1,\n"
		"\t\t\t2\n"
		"\t\t],\n"
		"\t\t\"p2\":\n"
		"\t\t{\n"
		"\t\t\t\"p21\": 1,\n"
		"\t\t\t\"p22\": 2\n"
		"\t\t}\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeMessageBegin("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeSequenceBegin("p1"s, 2);
	ser.serialize("v1"s, 1);
	ser.serialize("v2"s, 2);
	ser.serializeSequenceEnd("p1"s);
	ser.serializeStructBegin("p2"s);
	ser.serialize("p21"s, 1);
	ser.serialize("p22"s, 2);
	ser.serializeStructEnd("p2"s);
	ser.serializeMessageEnd("test"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testError()
{
	std::string expectedResult(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"error\":\n"
		"\t{\n"
		"\t\t\"code\": 32000,\n"
		"\t\t\"message\": \"Syntax error\"\n"
		"\t}\n"
		"}\n"
	);

	Serializer ser;
	std::ostringstream ostr;
	Poco::SyntaxException exc;
	ser.setup(ostr);
	ser.serializeFaultMessage(""s, exc);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testObject()
{
	std::string expectedResult(
		"{\n"
		"\t\"value\": \"foo\"\n"
		"}"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeStructBegin("");
	ser.serialize("value"s, "foo"s);
	ser.serializeStructEnd("");
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testArray()
{
	std::string expectedResult(
		"[\n"
		"\t\"foo\",\n"
		"\t\"bar\"\n"
		"]"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeSequenceBegin("", 0);
	ser.serialize("value"s, "foo"s);
	ser.serialize("value"s, "bar"s);
	ser.serializeSequenceEnd(""s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testComplexObject()
{
	std::string expectedResult(
		"{\n"
		"\t\"object\":\n"
		"\t{\n"
		"\t\t\"value\": \"foo\"\n"
		"\t},\n"
		"\t\"array\":\n"
		"\t[\n"
		"\t\t\"foo\",\n"
		"\t\t\"bar\"\n"
		"\t]\n"
		"}"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeStructBegin("");
	ser.serializeStructBegin("object"s);
	ser.serialize("value"s, "foo"s);
	ser.serializeStructEnd("object");
	ser.serializeSequenceBegin("array"s, 0);
	ser.serialize("value"s, "foo"s);
	ser.serialize("value"s, "bar"s);
	ser.serializeSequenceEnd("array"s);
	ser.serializeStructEnd(""s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testComplexArray()
{
	std::string expectedResult(
		"[\n"
		"\t[\n"
		"\t\t\"foo\",\n"
		"\t\t\"bar\"\n"
		"\t],\n"
		"\t[\n"
		"\t\t\"f00\",\n"
		"\t\t\"baz\"\n"
		"\t]\n"
		"]"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.serializeSequenceBegin(""s, 0);
	ser.serializeSequenceBegin("array"s, 0);
	ser.serialize("value"s, "foo"s);
	ser.serialize("value"s, "bar"s);
	ser.serializeSequenceEnd("array"s);
	ser.serializeSequenceBegin("array"s, 0);
	ser.serialize("value"s, "f00"s);
	ser.serialize("value"s, "baz"s);
	ser.serializeSequenceEnd("array"s);
	ser.serializeSequenceEnd(""s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::setUp()
{
}


void SerializerTest::tearDown()
{
}


CppUnit::Test* SerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerializerTest");

	CppUnit_addTest(pSuite, SerializerTest, testEmptyRequest);
	CppUnit_addTest(pSuite, SerializerTest, testOneParamRequest);
	CppUnit_addTest(pSuite, SerializerTest, testTwoParamRequest);
	CppUnit_addTest(pSuite, SerializerTest, testEmptyReply);
	CppUnit_addTest(pSuite, SerializerTest, testOneParamReply);
	CppUnit_addTest(pSuite, SerializerTest, testTwoParamReply);
	CppUnit_addTest(pSuite, SerializerTest, testComplexRequest);
	CppUnit_addTest(pSuite, SerializerTest, testError);
	CppUnit_addTest(pSuite, SerializerTest, testObject);
	CppUnit_addTest(pSuite, SerializerTest, testArray);
	CppUnit_addTest(pSuite, SerializerTest, testComplexObject);
	CppUnit_addTest(pSuite, SerializerTest, testComplexArray);

	return pSuite;
}
