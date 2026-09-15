//
// RawDeserializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RawDeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/RawDeserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


RawDeserializerTest::RawDeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


RawDeserializerTest::~RawDeserializerTest()
{
}


void RawDeserializerTest::testInt()
{
	int id = 1234;
	std::stringstream stream;
	stream << id;
	RawDeserializer deserializer(stream);
	
	int i;
	bool ok = deserializer.deserialize("id", true, i);
	assert (ok);
	assert (i == id);
}


void RawDeserializerTest::testString()
{
	std::string text = "The quick brown fox jumps over the lazy dog.";
	std::stringstream stream;
	stream << text;
	RawDeserializer deserializer(stream);
	
	std::string s;
	bool ok = deserializer.deserialize("text", true, s);
	assert (ok);
	assert (s == text);
}


void RawDeserializerTest::testBinary()
{
	std::vector<char> binary;
	for (char c = 0; c < 127; c++) binary.push_back(c);
	std::stringstream stream;
	stream.write(&binary[0], static_cast<std::streamsize>(binary.size()));	
	RawDeserializer deserializer(stream);

	std::vector<char> v;
	bool ok = deserializer.deserialize("binary", true, v);
	assert (ok);
	assert (v == binary);
}	


void RawDeserializerTest::setUp()
{
}


void RawDeserializerTest::tearDown()
{
}


CppUnit::Test* RawDeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RawDeserializerTest");

	CppUnit_addTest(pSuite, RawDeserializerTest, testInt);
	CppUnit_addTest(pSuite, RawDeserializerTest, testString);
	CppUnit_addTest(pSuite, RawDeserializerTest, testBinary);

	return pSuite;
}
