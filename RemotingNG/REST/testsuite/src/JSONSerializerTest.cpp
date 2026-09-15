//
// JSONSerializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "JSONSerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/JSONSerializer.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;
using namespace std::string_literals;


JSONSerializerTest::JSONSerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


JSONSerializerTest::~JSONSerializerTest()
{
}


void JSONSerializerTest::testError()
{
	std::string expectedResult(
		"{\n"
		"\t\"error\": \"Syntax error\",\n"
		"\t\"detail\": \"Testing\",\n"
		"\t\"code\": 1234\n"
		"}\n"
	);

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	Poco::SyntaxException exc("Testing"s, 1234);
	ser.serializeFaultMessage(""s, exc);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::testObject()
{
	std::string expectedResult(
		"{\n"
		"\t\"value\": \"foo\"\n"
		"}"
	);

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	ser.serializeStructBegin(""s);
	ser.serialize("value"s, "foo"s);
	ser.serializeStructEnd(""s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::testArray()
{
	std::string expectedResult(
		"[\n"
		"\t\"foo\",\n"
		"\t\"bar\"\n"
		"]"
	);

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	ser.serializeSequenceBegin(""s, 0);
	ser.serialize("value"s, "foo"s);
	ser.serialize("value"s, "bar"s);
	ser.serializeSequenceEnd(""s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::testComplexObject()
{
	std::string expectedResult(
		"{\n"
		"\t\"object\":\n"
		"\t{\n"
		"\t\t\"string\": \"foo\",\n"
		"\t\t\"number\": 12345,\n"
		"\t\t\"boolean\": true\n"
		"\t},\n"
		"\t\"array\":\n"
		"\t[\n"
		"\t\t\"foo\",\n"
		"\t\t\"bar\"\n"
		"\t]\n"
		"}"
	);

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	ser.serializeStructBegin(""s);
	ser.serializeStructBegin("object"s);
	ser.serialize("string"s, "foo"s);
	ser.serialize("number"s, static_cast<Poco::Int32>(12345));
	ser.serialize("boolean"s, true);
	ser.serializeStructEnd("object"s);
	ser.serializeSequenceBegin("array"s, 0);
	ser.serialize("value"s, "foo"s);
	ser.serialize("value"s, "bar"s);
	ser.serializeSequenceEnd("array"s);
	ser.serializeStructEnd(""s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::testComplexArray()
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

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
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


void JSONSerializerTest::testScalarBool()
{
	std::string expectedResult("true");

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	ser.serialize("value"s, true);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::testScalarInt()
{
	std::string expectedResult("42");

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	ser.serialize("value"s, 42);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::testScalarString()
{
	std::string expectedResult("\"Hello, world!\"");

	std::ostringstream ostr;
	JSONSerializer ser(ostr);
	ser.serialize("value"s, "Hello, world!"s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void JSONSerializerTest::setUp()
{
}


void JSONSerializerTest::tearDown()
{
}


CppUnit::Test* JSONSerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONSerializerTest");

	CppUnit_addTest(pSuite, JSONSerializerTest, testError);
	CppUnit_addTest(pSuite, JSONSerializerTest, testObject);
	CppUnit_addTest(pSuite, JSONSerializerTest, testArray);
	CppUnit_addTest(pSuite, JSONSerializerTest, testComplexObject);
	CppUnit_addTest(pSuite, JSONSerializerTest, testComplexArray);
	CppUnit_addTest(pSuite, JSONSerializerTest, testScalarBool);
	CppUnit_addTest(pSuite, JSONSerializerTest, testScalarInt);
	CppUnit_addTest(pSuite, JSONSerializerTest, testScalarString);

	return pSuite;
}
