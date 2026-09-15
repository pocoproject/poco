//
// HeaderDeserializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "HeaderDeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/HeaderDeserializer.h"
#include "Poco/Net/HTTPRequest.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


HeaderDeserializerTest::HeaderDeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


HeaderDeserializerTest::~HeaderDeserializerTest()
{
}


void HeaderDeserializerTest::testParams()
{
	Poco::Net::HTTPRequest request;
	request.set("id", "1234");
	request.set("childId", "foo");
	HeaderDeserializer deserializer(request);
	
	int id;
	bool ok = deserializer.deserialize("id", true, id);
	assert (ok);
	assert (id == 1234);
	
	std::string childId;
	ok = deserializer.deserialize("childId", true, childId);
	assert (ok);
	assert (childId == "foo");
}


void HeaderDeserializerTest::testTypes()
{
	Poco::Net::HTTPRequest request;
	request.set("string", "foo");
	request.set("int", "42");
	request.set("bool", "false");
	request.set("double", "3.5");
	
	HeaderDeserializer deserializer(request);

	std::string stringValue;
	bool ok = deserializer.deserialize("string", true, stringValue);
	assert (ok);
	assert (stringValue == "foo");
	
	int intValue;
	ok = deserializer.deserialize("int", true, intValue);
	assert (ok);
	assert (intValue == 42);

	bool boolValue;
	ok = deserializer.deserialize("bool", true, boolValue);
	assert (ok);
	assert (boolValue == false);

	double doubleValue;
	ok = deserializer.deserialize("double", true, doubleValue);
	assert (ok);
	assert (doubleValue == 3.5);
}


void HeaderDeserializerTest::setUp()
{
}


void HeaderDeserializerTest::tearDown()
{
}


CppUnit::Test* HeaderDeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HeaderDeserializerTest");

	CppUnit_addTest(pSuite, HeaderDeserializerTest, testParams);
	CppUnit_addTest(pSuite, HeaderDeserializerTest, testTypes);

	return pSuite;
}
