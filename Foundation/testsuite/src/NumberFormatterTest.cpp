//
// NumberFormatterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NumberFormatterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NumberFormatter.h"
#include <sstream>


using Poco::NumberFormatter;
using Poco::Int64;
using Poco::UInt64;


NumberFormatterTest::NumberFormatterTest(const std::string& name): CppUnit::TestCase(name)
{
}


NumberFormatterTest::~NumberFormatterTest()
{
}


void NumberFormatterTest::testFormat()
{
	assertTrue (NumberFormatter::format(123) == "123");
	assertTrue (NumberFormatter::format(-123) == "-123");
	assertTrue (NumberFormatter::format(-123, 5) == " -123");

	assertTrue (NumberFormatter::format((unsigned) 123) == "123");
	assertTrue (NumberFormatter::format((unsigned) 123, 5) == "  123");
	assertTrue (NumberFormatter::format0((unsigned) 123, 5) == "00123");

	assertTrue (NumberFormatter::format((long) 123) == "123");
	assertTrue (NumberFormatter::format((long) -123) == "-123");
	assertTrue (NumberFormatter::format((long) -123, 5) == " -123");

	assertTrue (NumberFormatter::format((unsigned long) 123) == "123");
	assertTrue (NumberFormatter::format((unsigned long) 123, 5) == "  123");

	assertTrue (NumberFormatter::format(123) == "123");
	assertTrue (NumberFormatter::format(-123) == "-123");
	assertTrue (NumberFormatter::format(-123, 5) == " -123");

#if defined(POCO_HAVE_INT64)
	assertTrue (NumberFormatter::format((Int64) 123) == "123");
	assertTrue (NumberFormatter::format((Int64) -123) == "-123");
	assertTrue (NumberFormatter::format((Int64) -123, 5) == " -123");

	assertTrue (NumberFormatter::format((UInt64) 123) == "123");
	assertTrue (NumberFormatter::format((UInt64) 123, 5) == "  123");
#if defined(POCO_LONG_IS_64_BIT)
	assertTrue (NumberFormatter::format((long long) 123) == "123");
	assertTrue (NumberFormatter::format((long long) -123) == "-123");
	assertTrue (NumberFormatter::format((long long) -123, 5) == " -123");

	assertTrue (NumberFormatter::format((unsigned long long) 123) == "123");
	assertTrue (NumberFormatter::format((unsigned long long) 123, 5) == "  123");
#endif
#endif

	if (sizeof(void*) == 4)
	{
		assertTrue (NumberFormatter::format((void*) 0x12345678) == "12345678");
	}
	else
	{
		assertTrue (NumberFormatter::format((void*) 0x12345678) == "0000000012345678");
	}
}


void NumberFormatterTest::testFormat0()
{
	assertTrue (NumberFormatter::format0(123, 5) == "00123");
	assertTrue (NumberFormatter::format0(-123, 5) == "-0123");
	assertTrue (NumberFormatter::format0((long) 123, 5) == "00123");
	assertTrue (NumberFormatter::format0((long) -123, 5) == "-0123");
	assertTrue (NumberFormatter::format0((unsigned long) 123, 5) == "00123");

#if defined(POCO_HAVE_INT64)
	assertTrue (NumberFormatter::format0((Int64) 123, 5) == "00123");
	assertTrue (NumberFormatter::format0((Int64) -123, 5) == "-0123");
	assertTrue (NumberFormatter::format0((UInt64) 123, 5) == "00123");
#if defined(POCO_LONG_IS_64_BIT)
	assertTrue (NumberFormatter::format0((long long) 123, 5) == "00123");
	assertTrue (NumberFormatter::format0((long long) -123, 5) == "-0123");
	assertTrue (NumberFormatter::format0((unsigned long long) 123, 5) == "00123");
#endif
#endif
}


void NumberFormatterTest::testFormatBool()
{
	assertTrue (NumberFormatter::format(true, NumberFormatter::FMT_TRUE_FALSE) == "true");
	assertTrue (NumberFormatter::format(false, NumberFormatter::FMT_TRUE_FALSE) == "false");
	assertTrue (NumberFormatter::format(true, NumberFormatter::FMT_YES_NO) == "yes");
	assertTrue (NumberFormatter::format(false, NumberFormatter::FMT_YES_NO) == "no");
	assertTrue (NumberFormatter::format(true, NumberFormatter::FMT_ON_OFF) == "on");
	assertTrue (NumberFormatter::format(false, NumberFormatter::FMT_ON_OFF) == "off");
}


void NumberFormatterTest::testFormatHex()
{
	using Opt = NumberFormatter::Options;

	assertTrue (NumberFormatter::formatHex(0x12) == "12");
	assertTrue (NumberFormatter::formatHex(0xab) == "AB");
	assertTrue (NumberFormatter::formatHex(0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex(0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex(0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex(0xab, 4, Opt::LOWERCASE) == "00ab");

	assertTrue (NumberFormatter::formatHex((unsigned) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((unsigned) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, 4, Opt::LOWERCASE) == "00ab");

	assertTrue (NumberFormatter::formatHex((long) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((long) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((long) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((long) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((long) 0xab, 4) == "00AB");

	assertTrue (NumberFormatter::formatHex((unsigned long) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, 4, Opt::LOWERCASE) == "00ab");

#if defined(POCO_HAVE_INT64)
	assertTrue (NumberFormatter::formatHex((Int64) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((Int64) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, 4, Opt::LOWERCASE) == "00ab");

	assertTrue (NumberFormatter::formatHex((UInt64) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((UInt64) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, 4, Opt::LOWERCASE) == "00ab");
#if defined(POCO_LONG_IS_64_BIT)
	assertTrue (NumberFormatter::formatHex((long long) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((long long) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((long long) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, 4, Opt::LOWERCASE) == "00ab");

	assertTrue (NumberFormatter::formatHex((unsigned long long) 0x12) == "12");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab) == "AB");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, Opt::LOWERCASE) == "ab");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0x12, 4) == "0012");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, 4) == "00AB");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, 4, Opt::LOWERCASE) == "00ab");
#endif
#endif

#if defined(POCO_TEST_DEPRECATED)
	// Deprecated functions
	assertTrue (NumberFormatter::formatHex(0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned) 0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((long) 0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((Int64) 0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((UInt64) 0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((long long) 0x12, true) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0x12, true) == "0x12");
#endif

	assertTrue (NumberFormatter::formatHex(0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex(0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex(0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex(0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex(0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex(0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex(0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex(0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex(0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");

	assertTrue (NumberFormatter::formatHex((unsigned) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((unsigned) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((unsigned) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((unsigned) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");

	assertTrue (NumberFormatter::formatHex((long) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((long) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((long) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((long) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((long) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((long) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((long) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((long) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((long) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");

	assertTrue (NumberFormatter::formatHex((unsigned long) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((unsigned long) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");

#if defined(POCO_HAVE_INT64)
	assertTrue (NumberFormatter::formatHex((Int64) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((Int64) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((Int64) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((Int64) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");

	assertTrue (NumberFormatter::formatHex((UInt64) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((UInt64) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((UInt64) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((UInt64) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");
#if defined(POCO_LONG_IS_64_BIT)
	assertTrue (NumberFormatter::formatHex((long long) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((long long) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((long long) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((long long) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");

	assertTrue (NumberFormatter::formatHex((unsigned long long) 0x12, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0x12, 4, Opt::PREFIX) == "0x12");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, 4, Opt::PREFIX) == "0xAB");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, 4, Opt::PREFIX | Opt::LOWERCASE) == "0xab");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0x12, 6, Opt::PREFIX) == "0x0012");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, 6, Opt::PREFIX) == "0x00AB");
	assertTrue (NumberFormatter::formatHex((unsigned long long) 0xab, 6, Opt::PREFIX | Opt::LOWERCASE) == "0x00ab");
#endif
#endif
}


void NumberFormatterTest::testFormatFloat()
{
	assertTrue (NumberFormatter::format(1.0f) == "1");
	assertTrue (NumberFormatter::format(1.23f) == "1.23");
	assertTrue (NumberFormatter::format(-1.23f) == "-1.23");
	assertTrue (NumberFormatter::format(0.1f) == "0.1");
	assertTrue (NumberFormatter::format(-0.1f) == "-0.1");
	assertTrue (NumberFormatter::format(1.23) == "1.23");
	assertTrue (NumberFormatter::format(-1.23) == "-1.23");
	assertTrue (NumberFormatter::format(1.0) == "1");
	assertTrue (NumberFormatter::format(-1.0) == "-1");
	assertTrue (NumberFormatter::format(0.1) == "0.1");
	assertTrue (NumberFormatter::format(-0.1) == "-0.1");

	int decDigits = std::numeric_limits<double>::digits10;
	std::ostringstream ostr;
	ostr << "0." << std::string(decDigits - 1, '0') << '1';
	assertTrue (NumberFormatter::format(1 / std::pow(10., decDigits)) == ostr.str());

	ostr.str("");
	ostr << "1e-" << decDigits + 1;
	std::string str (ostr.str());
	std::string str1 (NumberFormatter::format(1 / std::pow(10., decDigits + 1)));
	assertTrue (NumberFormatter::format(1 / std::pow(10., decDigits + 1)) == ostr.str());

	assertTrue (NumberFormatter::format(12.25) == "12.25");
	assertTrue (NumberFormatter::format(12.25, 4) == "12.2500");
	assertTrue (NumberFormatter::format(12.25, 8, 4) == " 12.2500");

	assertTrue (NumberFormatter::format(12.45f, 2) == "12.45");

	assertTrue (NumberFormatter::format(-12.25) == "-12.25");
	assertTrue (NumberFormatter::format(-12.25, 4) == "-12.2500");
	assertTrue (NumberFormatter::format(-12.25, 10, 4) == "  -12.2500");
	assertTrue (NumberFormatter::format(-12.25, 10, 2) == "    -12.25");
	assertTrue (NumberFormatter::format(-12.25, 10, 1) == "     -12.3");

	assertTrue (NumberFormatter::format(50.0, 3) == "50.000");
	assertTrue (NumberFormatter::format(50.0f, 3) == "50.000");
	assertTrue (NumberFormatter::format(50.123, 3) == "50.123");
	assertTrue (NumberFormatter::format(50.123f, 3) == "50.123");
	assertTrue (NumberFormatter::format(50.123, 0) == "50");
	assertTrue (NumberFormatter::format(50.123f, 0) == "50");
	assertTrue (NumberFormatter::format(50.546, 0) == "51");
	assertTrue (NumberFormatter::format(50.546f, 0) == "51");
	assertTrue (NumberFormatter::format(50.546f, 2) == "50.55");

	try
	{
		Poco::NumberFormatter::format(0.1, -1);
		fail ("NumberFormatter::format() must throw on negative precision");
	}
	catch(Poco::InvalidArgumentException&){}

	try
	{
		Poco::NumberFormatter::format(0.1, 2, -1);
		fail ("NumberFormatter::format() must throw on negative precision");
	}
	catch(Poco::InvalidArgumentException&){}
}


void NumberFormatterTest::testAppend()
{
	std::string s;
	NumberFormatter::append(s, 123);
	assertTrue (s == "123");
	s.erase();
	NumberFormatter::append(s, 123, 4);
	assertTrue (s == " 123");
	s.erase();
	NumberFormatter::append0(s, 123, 5);
	assertTrue (s == "00123");
	s.erase();
	NumberFormatter::appendHex(s, 0xDEAD);
	assertTrue (s == "DEAD");
	s.erase();
	NumberFormatter::appendHex(s, 0xDEAD, 6);
	assertTrue (s == "00DEAD");
	s.erase();
	NumberFormatter::append(s, 123u);
	assertTrue (s == "123");
	s.erase();
	NumberFormatter::append(s, 123u, 4);
	assertTrue (s == " 123");
	s.erase();
	NumberFormatter::append0(s, 123u, 5);
	assertTrue (s == "00123");


	s.erase();
	NumberFormatter::append(s, 123.4);
	assertTrue (s == "123.4");
	s.erase();
	NumberFormatter::append(s, 123.4567, 2);
	assertTrue (s == "123.46");
	s.erase();
	NumberFormatter::append(s, 123.4567, 10, 5);
	assertTrue (s == " 123.45670");
	s.erase();
	NumberFormatter::append(s, 123., 2);
	assertTrue (s == "123.00");
	s.erase();
	NumberFormatter::append(s, static_cast<double>(1234567), 2);
	assertTrue (s == "1234567.00");
	s.erase();
	NumberFormatter::append(s, 1234567.0, 10, 1);
	assertTrue (s == " 1234567.0");
}


void NumberFormatterTest::setUp()
{
}


void NumberFormatterTest::tearDown()
{
}


CppUnit::Test* NumberFormatterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NumberFormatterTest");

	CppUnit_addTest(pSuite, NumberFormatterTest, testFormat);
	CppUnit_addTest(pSuite, NumberFormatterTest, testFormat0);
	CppUnit_addTest(pSuite, NumberFormatterTest, testFormatBool);
	CppUnit_addTest(pSuite, NumberFormatterTest, testFormatHex);
	CppUnit_addTest(pSuite, NumberFormatterTest, testFormatFloat);
	CppUnit_addTest(pSuite, NumberFormatterTest, testAppend);

	return pSuite;
}
