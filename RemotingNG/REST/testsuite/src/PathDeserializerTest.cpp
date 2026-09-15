//
// PathDeserializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "PathDeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/PathDeserializer.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


PathDeserializerTest::PathDeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


PathDeserializerTest::~PathDeserializerTest()
{
}


void PathDeserializerTest::testParams()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/api/object/1234/child/foo");
	PathDeserializer deserializer("/api/object/{id}/child/{childId}", request);
	
	int id;
	bool ok = deserializer.deserialize("id", true, id);
	assert (ok);
	assert (id == 1234);
	
	std::string childId;
	ok = deserializer.deserialize("childId", true, childId);
	assert (ok);
	assert (childId == "foo");
}


void PathDeserializerTest::testTypes()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/foo/42/false/3.5");
	PathDeserializer deserializer("/{string}/{int}/{bool}/{double}", request);

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


void PathDeserializerTest::testAlternation()
{
	// Test alternation pattern: {name:(option1|option2|...)}
	Poco::Net::HTTPRequest request1(Poco::Net::HTTPRequest::HTTP_GET, "/api/sql/query");
	PathDeserializer deserializer1("/api/sql/{endpoint:(query|databases|config)}", request1);

	std::string endpoint;
	bool ok = deserializer1.deserialize("endpoint", true, endpoint);
	assert (ok);
	assert (endpoint == "query");

	// Test with different option
	Poco::Net::HTTPRequest request2(Poco::Net::HTTPRequest::HTTP_GET, "/api/sql/databases");
	PathDeserializer deserializer2("/api/sql/{endpoint:(query|databases|config)}", request2);

	ok = deserializer2.deserialize("endpoint", true, endpoint);
	assert (ok);
	assert (endpoint == "databases");

	// Test with third option
	Poco::Net::HTTPRequest request3(Poco::Net::HTTPRequest::HTTP_GET, "/api/sql/config");
	PathDeserializer deserializer3("/api/sql/{endpoint:(query|databases|config)}", request3);

	ok = deserializer3.deserialize("endpoint", true, endpoint);
	assert (ok);
	assert (endpoint == "config");
}


void PathDeserializerTest::testAlternationMultiple()
{
	// Test alternation with regular placeholder
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/devs/sql/query/1234");
	PathDeserializer deserializer("/devs/sql/{endpoint:(query|databases|config)}/{id}", request);

	std::string endpoint;
	bool ok = deserializer.deserialize("endpoint", true, endpoint);
	assert (ok);
	assert (endpoint == "query");

	int id;
	ok = deserializer.deserialize("id", true, id);
	assert (ok);
	assert (id == 1234);
}


void PathDeserializerTest::setUp()
{
}


void PathDeserializerTest::tearDown()
{
}


CppUnit::Test* PathDeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PathDeserializerTest");

	CppUnit_addTest(pSuite, PathDeserializerTest, testParams);
	CppUnit_addTest(pSuite, PathDeserializerTest, testTypes);
	CppUnit_addTest(pSuite, PathDeserializerTest, testAlternation);
	CppUnit_addTest(pSuite, PathDeserializerTest, testAlternationMultiple);

	return pSuite;
}
