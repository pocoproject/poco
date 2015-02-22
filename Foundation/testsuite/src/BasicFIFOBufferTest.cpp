//
// BasicFIFOBufferTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/BasicFIFOBufferTest.cpp#1 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BasicFIFOBufferTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FIFOBuffer.h"
#include <iostream>
#include <memory>
#include <cstring>

// NOTE: using for certain namespace
using Poco::BasicFIFOBuffer;
using std::memcpy;

BasicFIFOBufferTest::BasicFIFOBufferTest(const std::string& name): CppUnit::TestCase(name)
{
}


BasicFIFOBufferTest::~BasicFIFOBufferTest()
{
}


void BasicFIFOBufferTest::testNextWrite()
{
	// String length is 88 characters.
	std::string text("The Quick Brown Dog Jumps Over The Lazy Fox.");
	BasicFIFOBuffer<char> buffer(128);
	buffer.write(text.data(), text.size());
	char c_buffer[buffer.size()];
	std::memset(c_buffer, 0, buffer.size());
	
	buffer.read(c_buffer, 4);

	assert(std::string(c_buffer, 4) == std::string("The "));
	
	buffer.peek(c_buffer, buffer.used());
	assert(std::string(c_buffer, buffer.used()) ==
			std::string("Quick Brown Dog Jumps Over The Lazy Fox."));

	memcpy(buffer.next(), "The ", 4);
	buffer.advance(4);

	buffer.peek(c_buffer, buffer.used());

	assert(std::string("Quick Brown Dog Jumps Over The Lazy Fox.The ") == std::string(c_buffer, buffer.used()));

	/*
	std::istringstream istr("foo");
	std::ostringstream ostr;
	TeeInputStream tis(istr);
	tis.addStream(ostr);
	std::string s;
	tis >> s;
	assert (ostr.str() == "foo");
	*/
}

void BasicFIFOBufferTest::setUp()
{
}


void BasicFIFOBufferTest::tearDown()
{
}


CppUnit::Test* BasicFIFOBufferTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BasicFIFOBufferTest");

	CppUnit_addTest(pSuite, BasicFIFOBufferTest, testNextWrite);

	return pSuite;
}
