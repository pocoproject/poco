//
// DeserializerTest.cpp
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "DeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/JSONRPC/Deserializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <sstream>


using namespace Poco::RemotingNG::JSONRPC;


DeserializerTest::DeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


DeserializerTest::~DeserializerTest()
{
}


void DeserializerTest::testEmptyRequest()
{
	std::string request(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"method\": \"test\",\n"
		"\t\"params\":\n"
		"\t{\n"
		"\t}\n"
		"}\n"
	);

	Deserializer deser;
	std::istringstream istr(request);
	deser.setup(istr);
	std::string meth;
	deser.findMessage(meth);
	assert (meth == "test");
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
}


void DeserializerTest::testOneParamRequest()
{
	std::string request(
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

	Deserializer deser;
	std::istringstream istr(request);
	deser.setup(istr);
	std::string meth;
	deser.findMessage(meth);
	assert (meth == "test");
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	int p1;
	bool ok = deser.deserialize("p1", true, p1);
	assert (ok);
	assert (p1 == 42);
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
}


void DeserializerTest::testTwoParamRequest()
{
	std::string request(
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

	Deserializer deser;
	std::istringstream istr(request);
	deser.setup(istr);
	std::string meth;
	deser.findMessage(meth);
	assert (meth == "test");
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	int p1;
	bool ok = deser.deserialize("p1", true, p1);
	assert (ok);
	assert (p1 == 42);
	std::string p2;
	ok = deser.deserialize("p2", true, p2);
	assert (ok);
	assert (p2 == "foo");
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
}


void DeserializerTest::testEmptyReply()
{
	std::string reply(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"result\":\n"
		"\t{\n"
		"\t}\n"
		"}\n"
	);

	Deserializer deser;
	std::istringstream istr(reply);
	deser.setup(istr);
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testOneParamReply()
{
	std::string reply(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"result\":\n"
		"\t{\n"
		"\t\t\"return\": 42\n"
		"\t}\n"
		"}\n"
	);

	Deserializer deser;
	std::istringstream istr(reply);
	deser.setup(istr);
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	int ret;
	bool ok = deser.deserialize("return", true, ret);
	assert (ok);
	assert (ret == 42);
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testTwoParamReply()
{
	std::string reply(
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

	Deserializer deser;
	std::istringstream istr(reply);
	deser.setup(istr);
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	int ret;
	bool ok = deser.deserialize("return", true, ret);
	assert (ok);
	assert (ret == 42);
	std::string p1;
	ok = deser.deserialize("p1", true, p1);
	assert (ok);
	assert (p1 == "bar");
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testComplexRequest()
{
	std::string reply(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"id\": 0,\n"
		"\t\"result\":\n"
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

	Deserializer deser;
	std::istringstream istr(reply);
	deser.setup(istr);
	deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::UInt32 size;
	bool ok = deser.deserializeSequenceBegin("p1", true, size);
	assert (ok);
	assert (size == 2);
	int p1;
	ok = deser.deserialize("p1", true, p1);
	assert (ok);
	assert (p1 == 1);
	ok = deser.deserialize("p1", true, p1);
	assert (ok);
	assert (p1 == 2);
	deser.deserializeSequenceEnd("p1");
	ok = deser.deserializeStructBegin("p2", true);
	int p21;
	ok = deser.deserialize("p21", true, p21);
	assert (ok);
	assert (p21 == 1);
	int p22;
	ok = deser.deserialize("p22", true, p22);
	assert (ok);
	assert (p22 == 2);
	deser.deserializeStructEnd("p2");
	deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testError()
{
	std::string reply(
		"{\n"
		"\t\"jsonrpc\": \"2.0\",\n"
		"\t\"error\":\n"
		"\t{\n"
		"\t\t\"code\": 32000,\n"
		"\t\t\"message\": \"Syntax error\"\n"
		"\t}\n"
		"}\n"
	);

	try
	{
		Deserializer deser;
		std::istringstream istr(reply);
		deser.setup(istr);
		deser.deserializeMessageBegin("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		deser.deserializeMessageEnd("test", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.code() == 32000);
		assert (exc.message() == "Syntax error");
	}
}


void DeserializerTest::testObject()
{
	std::string object(
		"{\n"
		"\t\"value\": \"foo\"\n"
		"}"
	);

	Deserializer deser;
	std::istringstream istr(object);
	deser.setup(istr);
	deser.deserializeStructBegin("", true);
	std::string value;
	bool ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "foo");
	deser.deserializeStructEnd("");
}


void DeserializerTest::testUnexpectedArray()
{
	std::string array(
		"[\n"
		"\t\"foo\"\n"
		"]"
	);

	Deserializer deser;
	std::istringstream istr(array);
	deser.setup(istr);
	try
	{
		deser.deserializeStructBegin("", true);
		fail("expects object - must throw");
	}
	catch (Poco::RemotingNG::DeserializerException&)
	{
	}
}


void DeserializerTest::testArray()
{
	std::string array(
		"[\n"
		"\t\"foo\",\n"
		"\t\"bar\"\n"
		"]"
	);

	Deserializer deser;
	std::istringstream istr(array);
	deser.setup(istr);
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


void DeserializerTest::testUnexpectedObject()
{
	std::string object(
		"{\n"
		"\t\"value\": \"foo\"\n"
		"}"
	);

	Deserializer deser;
	std::istringstream istr(object);
	deser.setup(istr);
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


void DeserializerTest::testComplexObject()
{
	std::string object(
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

	Deserializer deser;
	std::istringstream istr(object);
	deser.setup(istr);
	deser.deserializeStructBegin("", true);
	deser.deserializeStructBegin("object", true);
	std::string value;
	bool ok = deser.deserialize("value", true, value);
	assert (ok);
	assert (value == "foo");
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


void DeserializerTest::testComplexArray()
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

	Deserializer deser;
	std::istringstream istr(array);
	deser.setup(istr);
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


void DeserializerTest::setUp()
{
}


void DeserializerTest::tearDown()
{
}


CppUnit::Test* DeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DeserializerTest");

	CppUnit_addTest(pSuite, DeserializerTest, testEmptyRequest);
	CppUnit_addTest(pSuite, DeserializerTest, testOneParamRequest);
	CppUnit_addTest(pSuite, DeserializerTest, testTwoParamRequest);
	CppUnit_addTest(pSuite, DeserializerTest, testEmptyReply);
	CppUnit_addTest(pSuite, DeserializerTest, testOneParamReply);
	CppUnit_addTest(pSuite, DeserializerTest, testTwoParamReply);
	CppUnit_addTest(pSuite, DeserializerTest, testComplexRequest);
	CppUnit_addTest(pSuite, DeserializerTest, testError);
	CppUnit_addTest(pSuite, DeserializerTest, testObject);
	CppUnit_addTest(pSuite, DeserializerTest, testUnexpectedArray);
	CppUnit_addTest(pSuite, DeserializerTest, testArray);
	CppUnit_addTest(pSuite, DeserializerTest, testUnexpectedObject);
	CppUnit_addTest(pSuite, DeserializerTest, testComplexObject);
	CppUnit_addTest(pSuite, DeserializerTest, testComplexArray);

	return pSuite;
}
