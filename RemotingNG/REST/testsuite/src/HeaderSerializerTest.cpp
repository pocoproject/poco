//
// HeaderSerializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HeaderSerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/HeaderSerializer.h"
#include "Poco/Net/HTTPRequest.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;
using namespace std::string_literals;


HeaderSerializerTest::HeaderSerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


HeaderSerializerTest::~HeaderSerializerTest()
{
}


void HeaderSerializerTest::testParams()
{
	Poco::Net::HTTPRequest request;
	HeaderSerializer serializer(request);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	assert (request.get("id"s) == "1234");
	assert (request.get("childId"s) == "foo");
}


void HeaderSerializerTest::testTypes()
{
	Poco::Net::HTTPRequest request;
	HeaderSerializer serializer(request);
	serializer.serialize("string"s, "foo"s);
	serializer.serialize("int"s, static_cast<Poco::Int32>(42));
	serializer.serialize("bool"s, false);
	serializer.serialize("double"s, 3.5);
	assert (request.get("string"s) == "foo");
	assert (request.get("int"s) == "42");
	assert (request.get("bool"s) == "false");
	assert (request.get("double"s) == "3.5");
}


void HeaderSerializerTest::setUp()
{
}


void HeaderSerializerTest::tearDown()
{
}


CppUnit::Test* HeaderSerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HeaderSerializerTest");

	CppUnit_addTest(pSuite, HeaderSerializerTest, testParams);
	CppUnit_addTest(pSuite, HeaderSerializerTest, testTypes);

	return pSuite;
}
