//
// RawSerializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "RawSerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/RawSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;
using namespace std::string_literals;


RawSerializerTest::RawSerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


RawSerializerTest::~RawSerializerTest()
{
}


void RawSerializerTest::testInt()
{
	int id = 1234;
	std::stringstream stream;
	RawSerializer serializer(stream);
	serializer.serialize("id"s, id);
	int i = 0;
	stream >> i;
	assert (i == id);
}


void RawSerializerTest::testString()
{
	std::string text = "The quick brown fox jumps over the lazy dog.";
	std::stringstream stream;
	RawSerializer serializer(stream);
	serializer.serialize("text"s, text);
	std::string s;
	Poco::StreamCopier::copyToString(stream, s);
	assert (s == text);
}


void RawSerializerTest::testBinary()
{
	std::vector<char> binary;
	for (char c = 0; c < 127; c++) binary.push_back(c);
	std::stringstream stream;
	RawSerializer serializer(stream);
	serializer.serialize("binary"s, binary);
	std::vector<char> v;
	int ch = stream.get();
	while (ch != -1) 
	{ 
		v.push_back(static_cast<char>(ch)); 
		ch = stream.get(); 
	}
	assert (v == binary);
}	


void RawSerializerTest::setUp()
{
}


void RawSerializerTest::tearDown()
{
}


CppUnit::Test* RawSerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RawSerializerTest");

	CppUnit_addTest(pSuite, RawSerializerTest, testInt);
	CppUnit_addTest(pSuite, RawSerializerTest, testString);
	CppUnit_addTest(pSuite, RawSerializerTest, testBinary);

	return pSuite;
}
