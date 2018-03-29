//
// NullStreamTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NullStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NullStream.h"


using Poco::NullInputStream;
using Poco::NullOutputStream;


NullStreamTest::NullStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


NullStreamTest::~NullStreamTest()
{
}


void NullStreamTest::testInput()
{
	NullInputStream istr;
	assertTrue (istr.good());
	assertTrue (!istr.eof());
	int c = istr.get();
	assertTrue (c == -1);
	assertTrue (istr.eof());
}


void NullStreamTest::testOutput()
{
	NullOutputStream ostr;
	assertTrue (ostr.good());
	ostr << "Hello, world!";
	assertTrue (ostr.good());
}


void NullStreamTest::setUp()
{
}


void NullStreamTest::tearDown()
{
}


CppUnit::Test* NullStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NullStreamTest");

	CppUnit_addTest(pSuite, NullStreamTest, testInput);
	CppUnit_addTest(pSuite, NullStreamTest, testOutput);

	return pSuite;
}
