//
// SerializerTest.cpp
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/JWT/Serializer.h"


using namespace Poco::JWT;


SerializerTest::SerializerTest(const std::string& name):
	CppUnit::TestCase("SerializerTest")
{
}


SerializerTest::~SerializerTest()
{
}


void SerializerTest::setUp()
{
}


void SerializerTest::tearDown()
{
}


void SerializerTest::testSerializeEmpty()
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	std::string str = Serializer::serialize(*pObject);
	assert (str == "e30");
}


void SerializerTest::testSerializeAlgNone()
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("alg", std::string("none"));
	std::string str = Serializer::serialize(*pObject);
	assert (str == "eyJhbGciOiJub25lIn0");
}


void SerializerTest::testSerializeAlgHS256()
{
	Poco::JSON::Object::Ptr pObject = new Poco::JSON::Object;
	pObject->set("alg", std::string("HS256"));
	pObject->set("typ", std::string("JWT"));
	std::string str = Serializer::serialize(*pObject);
	assert (str == "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9");
}


void SerializerTest::testDeserializeEmpty()
{
	std::string serialized("e30");
	Poco::JSON::Object::Ptr pObject = Serializer::deserialize(serialized);
	assert (pObject->size() == 0);
}


void SerializerTest::testDeserializeAlgNone()
{
	std::string serialized("eyJhbGciOiJub25lIn0");
	Poco::JSON::Object::Ptr pObject = Serializer::deserialize(serialized);
	assert (pObject->size() == 1);
	assert (pObject->getValue<std::string>("alg") == "none");
}


void SerializerTest::testDeserializeAlgHS256()
{
	std::string serialized("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9");
	Poco::JSON::Object::Ptr pObject = Serializer::deserialize(serialized);
	assert (pObject->size() == 2);
	assert (pObject->getValue<std::string>("alg") == "HS256");
	assert (pObject->getValue<std::string>("typ") == "JWT");
}


void SerializerTest::testSplit()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");
	std::vector<std::string> parts = Serializer::split(jwt);
	assert (parts.size() == 3);
	assert (parts[0] == "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9");
	assert (parts[1] == "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ");
	assert (parts[2] == "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");
}


void SerializerTest::testSplitEmptySig()
{
	std::string jwt("eyJhbGciOiJub25lIn0.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.");
	std::vector<std::string> parts = Serializer::split(jwt);
	assert (parts.size() == 3);
	assert (parts[0] == "eyJhbGciOiJub25lIn0");
	assert (parts[1] == "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ");
	assert (parts[2] == "");
}


CppUnit::Test* SerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerializerTest");

	CppUnit_addTest(pSuite, SerializerTest, testSerializeEmpty);
	CppUnit_addTest(pSuite, SerializerTest, testSerializeAlgNone);
	CppUnit_addTest(pSuite, SerializerTest, testSerializeAlgHS256);
	CppUnit_addTest(pSuite, SerializerTest, testDeserializeEmpty);
	CppUnit_addTest(pSuite, SerializerTest, testDeserializeAlgNone);
	CppUnit_addTest(pSuite, SerializerTest, testDeserializeAlgHS256);
	CppUnit_addTest(pSuite, SerializerTest, testSplit);
	CppUnit_addTest(pSuite, SerializerTest, testSplitEmptySig);

	return pSuite;
}
