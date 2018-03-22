//
// TeeStreamTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TeeStreamTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/TeeStream.h"
#include <sstream>


using Poco::TeeInputStream;
using Poco::TeeOutputStream;


TeeStreamTest::TeeStreamTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


TeeStreamTest::~TeeStreamTest()
{
}


void TeeStreamTest::testTeeInputStream()
{
	std::istringstream istr("foo");
	std::ostringstream ostr;
	TeeInputStream tis(istr);
	tis.addStream(ostr);
	std::string s;
	tis >> s;
	assertTrue (ostr.str() == "foo");
}


void TeeStreamTest::testTeeOutputStream()
{
	std::ostringstream ostr1;
	std::ostringstream ostr2;
	TeeOutputStream tos(ostr1);
	tos.addStream(ostr2);
	tos << "bar" << std::flush;
	assertTrue (ostr1.str() == "bar");
	assertTrue (ostr2.str() == "bar");
}


void TeeStreamTest::setUp()
{
}


void TeeStreamTest::tearDown()
{
}


CppUnit::Test* TeeStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TeeStreamTest");

	CppUnit_addTest(pSuite, TeeStreamTest, testTeeInputStream);
	CppUnit_addTest(pSuite, TeeStreamTest, testTeeOutputStream);

	return pSuite;
}
