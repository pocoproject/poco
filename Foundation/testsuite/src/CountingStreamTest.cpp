//
// CountingStreamTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CountingStreamTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/CountingStream.h"
#include <sstream>


using Poco::CountingInputStream;
using Poco::CountingOutputStream;


CountingStreamTest::CountingStreamTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


CountingStreamTest::~CountingStreamTest()
{
}


void CountingStreamTest::testInput()
{
	char c;
	std::istringstream istr1("foo");
	CountingInputStream ci1(istr1);
	while (ci1.good()) ci1.get(c);
	assertTrue (ci1.lines() == 1);
	assertTrue (ci1.chars() == 3);
	assertTrue (ci1.pos() == 3);

	std::istringstream istr2("foo\nbar");
	CountingInputStream ci2(istr2);
	while (ci2.good()) ci2.get(c);
	assertTrue (ci2.lines() == 2);
	assertTrue (ci2.chars() == 7);
	assertTrue (ci2.pos() == 3);

	std::istringstream istr3("foo\nbar\n");
	CountingInputStream ci3(istr3);
	while (ci3.good()) ci3.get(c);
	assertTrue (ci3.lines() == 2);
	assertTrue (ci3.chars() == 8);
	assertTrue (ci3.pos() == 0);

	std::istringstream istr4("foo");
	CountingInputStream ci4(istr4);
	while (ci4.good()) ci4.get(c);
	ci4.addChars(10);
	ci4.addLines(2);
	ci4.addPos(3);
	assertTrue (ci4.lines() == 1 + 2);
	assertTrue (ci4.chars() == 3 + 10);
	assertTrue (ci4.pos() == 3 + 3);
}


void CountingStreamTest::testOutput()
{
	std::ostringstream ostr1;
	CountingOutputStream co1(ostr1);
	co1 << "foo";
	assertTrue (ostr1.str() == "foo");
	assertTrue (co1.lines() == 1);
	assertTrue (co1.chars() == 3);
	assertTrue (co1.pos() == 3);

	std::ostringstream ostr2;
	CountingOutputStream co2(ostr2);
	co2 << "foo\nbar";
	assertTrue (ostr2.str() == "foo\nbar");
	assertTrue (co2.lines() == 2);
	assertTrue (co2.chars() == 7);
	assertTrue (co2.pos() == 3);

	CountingOutputStream co3;
	co3 << "foo\nbar\n";
	assertTrue (co3.lines() == 2);
	assertTrue (co3.chars() == 8);
	assertTrue (co3.pos() == 0);

	std::ostringstream ostr4;
	CountingOutputStream co4(ostr4);
	co4 << "foo";
	co4.addChars(10);
	co4.addLines(2);
	co4.addPos(3);
	assertTrue (co4.lines() == 1 + 2);
	assertTrue (co4.chars() == 3 + 10);
	assertTrue (co4.pos() == 3 + 3);
}


void CountingStreamTest::setUp()
{
}


void CountingStreamTest::tearDown()
{
}


CppUnit::Test* CountingStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CountingStreamTest");

	CppUnit_addTest(pSuite, CountingStreamTest, testInput);
	CppUnit_addTest(pSuite, CountingStreamTest, testOutput);

	return pSuite;
}
