//
// MemoryStreamTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/MemoryStreamTest.cpp#2 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MemoryStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MemoryStream.h"


using Poco::MemoryInputStream;
using Poco::MemoryOutputStream;


MemoryStreamTest::MemoryStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


MemoryStreamTest::~MemoryStreamTest()
{
}


void MemoryStreamTest::testInput()
{
	const char* data = "This is a test";
	MemoryInputStream istr1(data, 14);
	
	int c = istr1.get();
	assert (c == 'T');
	c = istr1.get();
	assert (c == 'h');
	
	std::string str;
	istr1 >> str;
	assert (str == "is");
	
	char buffer[32];
	istr1.read(buffer, sizeof(buffer));
	assert (istr1.gcount() == 10);
	buffer[istr1.gcount()] = 0;
	assert (std::string(" is a test") == buffer);
	
	const char* data2 = "123";
	MemoryInputStream istr2(data2, 3);
	c = istr2.get();
	assert (c == '1');
	assert (istr2.good());
	c = istr2.get();
	assert (c == '2');
	istr2.unget();
	c = istr2.get();
	assert (c == '2');
	assert (istr2.good());
	c = istr2.get();
	assert (c == '3');
	assert (istr2.good());
	c = istr2.get();
	assert (c == -1);
	assert (istr2.eof());
}


void MemoryStreamTest::testOutput()
{
	char output[64];
	MemoryOutputStream ostr1(output, 64);
	ostr1 << "This is a test " << 42 << std::ends;
	assert (ostr1.charsWritten() == 18);
	assert (std::string("This is a test 42") == output);
	
	char output2[4];
	MemoryOutputStream ostr2(output2, 4);
	ostr2 << "test";
	assert (ostr2.good());
	ostr2 << 'x';
	assert (ostr2.fail());
}


void MemoryStreamTest::setUp()
{
}


void MemoryStreamTest::tearDown()
{
}


CppUnit::Test* MemoryStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MemoryStreamTest");

	CppUnit_addTest(pSuite, MemoryStreamTest, testInput);
	CppUnit_addTest(pSuite, MemoryStreamTest, testOutput);

	return pSuite;
}
