//
// PathSerializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PathSerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/PathSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;
using namespace std::string_literals;


PathSerializerTest::PathSerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


PathSerializerTest::~PathSerializerTest()
{
}


void PathSerializerTest::testParams()
{
	Poco::Net::HTTPRequest request;
	PathSerializer serializer("/api/object/{id}/child/{childId}"s, request);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	serializer.close();
	assert (request.getURI() == "/api/object/1234/child/foo");
}


void PathSerializerTest::testParamsPreserveQuery()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/?query=bar"s);
	PathSerializer serializer("/api/object/{id}/child/{childId}"s, request);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	serializer.close();
	assert (request.getURI() == "/api/object/1234/child/foo?query=bar");
}


void PathSerializerTest::testParamsMissing()
{
	Poco::Net::HTTPRequest request;
	PathSerializer serializer("/api/object/{id}/child/{childId}"s, request);
	serializer.serialize("childId"s, "foo"s);
	try
	{
		serializer.close();
		fail("missing param - must throw");
	}
	catch (Poco::RemotingNG::SerializerException&)
	{
	}
}


void PathSerializerTest::testTypes()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/"s);
	PathSerializer serializer("/{string}/{int}/{bool}/{double}"s, request);
	serializer.serialize("string"s, "foo"s);
	serializer.serialize("int"s, static_cast<Poco::Int32>(42));
	serializer.serialize("bool"s, false);
	serializer.serialize("double"s, 3.5);
	serializer.close();
	assert (request.getURI() == "/foo/42/false/3.5");
}


void PathSerializerTest::testEscaping()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/"s);
	PathSerializer serializer("/{string1}/{string2}"s, request);
	serializer.serialize("string1"s, "foo/bar"s);
	serializer.serialize("string2"s, "a+b+c%d"s);
	serializer.close();
	assert (request.getURI() == "/foo/bar/a+b+c%25d");
}


void PathSerializerTest::setUp()
{
}


void PathSerializerTest::tearDown()
{
}


CppUnit::Test* PathSerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PathSerializerTest");

	CppUnit_addTest(pSuite, PathSerializerTest, testParams);
	CppUnit_addTest(pSuite, PathSerializerTest, testParamsPreserveQuery);
	CppUnit_addTest(pSuite, PathSerializerTest, testParamsMissing);
	CppUnit_addTest(pSuite, PathSerializerTest, testTypes);
	CppUnit_addTest(pSuite, PathSerializerTest, testEscaping);

	return pSuite;
}
