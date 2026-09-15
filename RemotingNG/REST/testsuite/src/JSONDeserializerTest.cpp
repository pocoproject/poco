//
// JSONDeserializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "JSONDeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/Deserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


JSONDeserializerTest::JSONDeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


JSONDeserializerTest::~JSONDeserializerTest()
{
}


void JSONDeserializerTest::testError()
{
	std::string reply(
		"{\n"
		"\t\"error\": \"Exception\",\n"
		"\t\"detail\": \"Detail Text\",\n"
		"\t\"code\": 1234\n"
		"}\n"
	);

	try
	{
		std::istringstream istr(reply);
		JSONDeserializer deser(istr);
		deser.deserializeError("Internal Server Error", 500);
		deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		fail("must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.code() == 500);
		assert (exc.message() == "Internal Server Error: Exception: Detail Text");
	}
}


void JSONDeserializerTest::testObject()
{
	std::string object(
		"{\n"
		"\t\"value\": \"foo\"\n"
		"}"
	);

	std::istringstream istr(object);
	JSONDeserializer deser(istr);
	deser.deserializeStructBegin("", true);
	std::string value;
	bool ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "foo");
	deser.deserializeStructEnd("");
}


void JSONDeserializerTest::testUnexpectedArray()
{
	std::string array(
		"[\n"
		"\t\"foo\"\n"
		"]"
	);

	std::istringstream istr(array);
	JSONDeserializer deser(istr);
	try
	{
		deser.deserializeStructBegin("", true);
		fail("expects object - must throw");
	}
	catch (Poco::RemotingNG::DeserializerException&)
	{
	}
}


void JSONDeserializerTest::testArray()
{
	std::string array(
		"[\n"
		"\t\"foo\",\n"
		"\t\"bar\"\n"
		"]"
	);

	std::istringstream istr(array);
	JSONDeserializer deser(istr);
	Poco::UInt32 length;
	deser.deserializeSequenceBegin("", true, length);
	assert (length == 2);
	std::string value;
	bool ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "foo");
	ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "bar");
	deser.deserializeSequenceEnd("");
}


void JSONDeserializerTest::testUnexpectedObject()
{
	std::string object(
		"{\n"
		"\t\"value\": \"foo\"\n"
		"}"
	);

	std::istringstream istr(object);
	JSONDeserializer deser(istr);
	Poco::UInt32 length;
	try
	{
		deser.deserializeSequenceBegin("", true, length);
		fail ("expected array - must throw");
	}
	catch (Poco::RemotingNG::DeserializerException&)
	{
	}
}


void JSONDeserializerTest::testComplexObject()
{
	std::string object(
		"{\n"
		"\t\"object\":\n"
		"\t{\n"
		"\t\t\"string\": \"foo\",\n"
		"\t\t\"number\": 42,\n"
		"\t\t\"boolean\": true\n"
		"\t},\n"
		"\t\"array\":\n"
		"\t[\n"
		"\t\t\"foo\",\n"
		"\t\t\"bar\"\n"
		"\t]\n"
		"}"
	);

	std::istringstream istr(object);
	JSONDeserializer deser(istr);
	deser.deserializeStructBegin("", true);
	deser.deserializeStructBegin("object", true);
	std::string value;
	bool ok = deser.deserialize("string", true, value);
	assert (ok);
	assert (value == "foo");
	int number;
	ok = deser.deserialize("number", true, number);
	assert (ok);
	assert (number == 42);
	bool boolean;
	ok = deser.deserialize("boolean", true, boolean);
	assert (ok);
	assert (boolean == true);
	deser.deserializeStructEnd("object");
	Poco::UInt32 length;
	deser.deserializeSequenceBegin("array", true, length);
	assert (length == 2);
	ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "foo");
	ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "bar");
	deser.deserializeSequenceEnd("array");
	deser.deserializeStructEnd("");
}


void JSONDeserializerTest::testComplexArray()
{
	std::string array(
		"[\n"
		"\t[\n"
		"\t\t\"foo\"\n"
		"\t],\n"
		"\t[\n"
		"\t\t\"f00\",\n"
		"\t\t\"bar\",\n"
		"\t\t\"baz\"\n"
		"\t]\n"
		"]"
	);

	std::istringstream istr(array);
	JSONDeserializer deser(istr);
	Poco::UInt32 length;
	deser.deserializeSequenceBegin("", true, length);
	assert (length == 2);
	deser.deserializeSequenceBegin("array", true, length);
	assert (length == 1);
	std::string value;
	bool ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "foo");
	deser.deserializeSequenceEnd("array");

	deser.deserializeSequenceBegin("array", true, length);
	assert (length == 3);
	ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "f00");
	ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "bar");
	ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "baz");
	deser.deserializeSequenceEnd("array");
	deser.deserializeSequenceEnd("");
}


void JSONDeserializerTest::testScalarBool()
{
	std::string scalar("true");

	std::istringstream istr(scalar);
	JSONDeserializer deser(istr);
	bool value;
	bool ok = deser.deserialize("", true, value);
	assert (ok);
	assert (value);
}


void JSONDeserializerTest::testScalarInt()
{
	std::string scalar("42");

	std::istringstream istr(scalar);
	JSONDeserializer deser(istr);
	int value;
	bool ok = deser.deserialize("", true, value);
	assert (ok);
	assert (value == 42);
}


void JSONDeserializerTest::testScalarString()
{
	std::string scalar("\"Hello, world!\"");

	std::istringstream istr(scalar);
	JSONDeserializer deser(istr);
	std::string value;
	bool ok = deser.deserialize("", true, value);
	assert (ok);
	assert (value == "Hello, world!");
}


void JSONDeserializerTest::testNullValue()
{
	std::string object(
		"{\n"
		"\t\"value\": null\n"
		"}"
	);

	std::istringstream istr(object);
	JSONDeserializer deser(istr);
	deser.deserializeStructBegin("", true);
	std::string value;
	try
	{
		(void) deser.deserialize("value", true, value);
		fail("null value, must throw");
	}
	catch (Poco::NullValueException& exc)
	{
		assert (exc.message() == "value");
	}
	deser.deserializeStructEnd("");
}


void JSONDeserializerTest::setUp()
{
}


void JSONDeserializerTest::tearDown()
{
}


CppUnit::Test* JSONDeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONDeserializerTest");

	CppUnit_addTest(pSuite, JSONDeserializerTest, testError);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testObject);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testUnexpectedArray);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testArray);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testUnexpectedObject);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testComplexObject);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testComplexArray);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testScalarBool);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testScalarInt);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testScalarString);
	CppUnit_addTest(pSuite, JSONDeserializerTest, testNullValue);

	return pSuite;
}
