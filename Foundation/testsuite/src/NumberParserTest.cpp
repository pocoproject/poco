//
// NumberParserTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NumberParserTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Types.h"
#include "Poco/Format.h"
#include "Poco/NumericString.h"
#include "Poco/MemoryStream.h"
#include "Poco/Stopwatch.h"
#include <iostream>
#include <iomanip>
#include <cstdio>


using Poco::NumberParser;
using Poco::NumberFormatter;
using Poco::SyntaxException;
using Poco::Int8;
using Poco::UInt8;
using Poco::Int16;
using Poco::UInt16;
using Poco::Int32;
using Poco::UInt32;
using Poco::Int64;
using Poco::UInt64;
using Poco::format;
using Poco::decimalSeparator;
using Poco::thousandSeparator;


NumberParserTest::NumberParserTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


NumberParserTest::~NumberParserTest()
{
}


void NumberParserTest::testParse()
{
	std::string sep(".,");

	for (int i = 0; i < 2; ++i)
	{
		char ts = sep[i];

		assertTrue (NumberParser::parse("123") == 123);
		assertTrue (NumberParser::parse(format("123%c456", ts), ts) == 123456);
		assertTrue (NumberParser::parse(format("1%c234%c567", ts, ts), ts) == 1234567);
	}

	assertTrue (NumberParser::parse("+123") == 123);
	assertTrue (NumberParser::parse("-123") == -123);
	assertTrue (NumberParser::parse("0") == 0);
	assertTrue (NumberParser::parse("000") == 0);
	assertTrue (NumberParser::parse("0123") == 123);
	assertTrue (NumberParser::parse("+0123") == 123);
	assertTrue (NumberParser::parse("-0123") == -123);
	assertTrue (NumberParser::parseUnsigned("123") == 123);
	assertTrue (NumberParser::parseHex("12AB") == 0x12ab);
	assertTrue (NumberParser::parseHex("0x12AB") == 0x12ab);
	assertTrue (NumberParser::parseHex("0X12AB") == 0x12ab);
	assertTrue (NumberParser::parseHex("0x99a") == 0x99a);
	assertTrue (NumberParser::parseHex("00") == 0);
	assertTrue (NumberParser::parseOct("123") == 0123);
	assertTrue (NumberParser::parseOct("0123") == 0123);
	assertTrue (NumberParser::parseOct("0") == 0);
	assertTrue (NumberParser::parseOct("000") == 0);

	assertTrue (NumberParser::parseBool("0") == false);
	assertTrue (NumberParser::parseBool("FALSE") == false);
	assertTrue (NumberParser::parseBool("no") == false);
	assertTrue (NumberParser::parseBool("1") == true);
	assertTrue (NumberParser::parseBool("True") == true);
	assertTrue (NumberParser::parseBool("YeS") == true);

#if defined(POCO_HAVE_INT64)
	assertTrue (NumberParser::parse64("123") == 123);
	assertTrue (NumberParser::parse64("-123") == -123);
	assertTrue (NumberParser::parse64("0123") == 123);
	assertTrue (NumberParser::parse64("-0123") == -123);
	assertTrue (NumberParser::parseUnsigned64("123") == 123);
	assertTrue (NumberParser::parseHex64("12AB") == 0x12ab);
	assertTrue (NumberParser::parseOct64("123") == 0123);
	assertTrue (NumberParser::parseOct64("0123") == 0123);
#endif

#ifndef POCO_NO_FPENVIRONMENT
	for (int i = 0; i < 2; ++i)
	{
		char ts = sep[i];
		for (int j = 0; j < 2; ++j)
		{
			char dp = sep[j];
			if (ts == dp) continue;

			assertEqualDelta(1.0, NumberParser::parseFloat(format("1", dp), dp, ts), 0.01);
			assertEqualDelta(0.0, NumberParser::parseFloat(format("0%c0", dp), dp, ts), 0.01);
			assertEqualDelta(0., NumberParser::parseFloat(format("0%c0", dp), dp, ts), 0.01);
			assertEqualDelta(.0, NumberParser::parseFloat(format("0%c0", dp), dp, ts), 0.01);
			assertEqualDelta(12.34, NumberParser::parseFloat(format("12%c34", dp), dp, ts), 0.01);
			assertEqualDelta(12.34, NumberParser::parseFloat(format("12%c34f", dp), dp, ts), 0.01);
			assertEqualDelta(12.34, NumberParser::parseFloat(format("12%c34", dp), dp, ts), 0.01);
			assertEqualDelta(-12.34, NumberParser::parseFloat(format("-12%c34", dp), dp, ts), 0.01);
			assertEqualDelta(.34, NumberParser::parseFloat(format("%c34", dp), dp, ts), 0.01);
			assertEqualDelta(-.34, NumberParser::parseFloat(format("-%c34", dp), dp, ts), 0.01);
			assertEqualDelta(12., NumberParser::parseFloat(format("12%c", dp), dp, ts), 0.01);
			assertEqualDelta(-12., NumberParser::parseFloat(format("-12%c", dp), dp, ts), 0.01);
			assertEqualDelta(12, NumberParser::parseFloat("12", dp, ts), 0.01);
			assertEqualDelta(-12, NumberParser::parseFloat("-12", dp, ts), 0.01);
			assertEqualDelta(12.34, NumberParser::parseFloat(format("12%c3456789012345678901234567890", dp), dp, ts), 0.01);

			assertEqualDelta(1234.3456789, NumberParser::parseFloat(format("1%c234%c3456789012345678901234567890", ts, dp), dp, ts), 0.00000001);
			assertEqualDelta(12345.3456789, NumberParser::parseFloat(format("12%c345%c3456789012345678901234567890", ts, dp), dp, ts), 0.00000001);
			assertEqualDelta(123456.3456789, NumberParser::parseFloat(format("123%c456%c3456789012345678901234567890", ts, dp), dp, ts), 0.00000001);
			assertEqualDelta(1234567.3456789, NumberParser::parseFloat(format("1%c234%c567%c3456789012345678901234567890", ts, ts, dp), dp, ts), 0.00000001);
			assertEqualDelta(12345678.3456789, NumberParser::parseFloat(format("12%c345%c678%c3456789012345678901234567890", ts, ts, dp), dp, ts), 0.00000001);
			assertEqualDelta(123456789.3456789, NumberParser::parseFloat(format("123%c456%c789%c3456789012345678901234567890", ts, ts, dp), dp, ts), 0.00000001);

			if ((std::numeric_limits<double>::max() / 10) < 1.23456e10)
				fail ("test value larger than max value for this platform");
			else
			{
				double d = 1.234e100;
				assertEqualDelta(d, NumberParser::parseFloat(format("1%c234e100", dp), dp, ts), 0.01);
				assertEqualDelta(d, NumberParser::parseFloat(format("1%c234E+100", dp), dp, ts), 0.01);
		
				d = 1.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("1%c234E-100", dp), dp, ts), 0.01);
		
				d = -1.234e100;
				assertEqualDelta(d, NumberParser::parseFloat(format("-1%c234e+100", dp), dp, ts), 0.01);
				assertEqualDelta(d, NumberParser::parseFloat(format("-1%c234E100", dp), dp, ts), 0.01);
		
				d = 1234.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("1%c234%c234e-100", ts, dp), dp, ts), 0.01);
				d = 12345.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("12%c345%c234e-100", ts, dp), dp, ts), 0.01);
				d = 123456.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("123%c456%c234e-100", ts, dp), dp, ts), 0.01);

				d = -1234.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("-1%c234%c234e-100", ts, dp), dp, ts), 0.01);
				d = -12345.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("-12%c345%c234e-100", ts, dp), dp, ts), 0.01);
				d = -123456.234e-100;
				assertEqualDelta(d, NumberParser::parseFloat(format("-123%c456%c234e-100", ts, dp), dp, ts), 0.01);
			}

			double d = 12.34e-10;
			assertEqualDelta(d, NumberParser::parseFloat(format("12%c34e-10", dp), dp, ts), 0.01);
			assertEqualDelta(-12.34, NumberParser::parseFloat(format("-12%c34", dp), dp, ts), 0.01);
	
			assertEqualDelta(12.34, NumberParser::parseFloat(format("12%c34", dp), dp, ts), 0.01);
		}
	}
#endif // POCO_NO_FPENVIRONMENT
}


void NumberParserTest::testLimits()
{
	assertTrue (testUpperLimit<Int8>());
	assertTrue (testLowerLimit<Int8>());
	assertTrue (testUpperLimit<UInt8>());
	assertTrue (testUpperLimit<Int16>());
	assertTrue (testLowerLimit<Int16>());
	assertTrue (testUpperLimit<UInt16>());
	assertTrue (testUpperLimit<Int32>());
	assertTrue (testLowerLimit<Int32>());
	assertTrue (testUpperLimit<UInt32>());
	assertTrue (testUpperLimit64<Int64>());
	assertTrue (testLowerLimit64<Int64>());
	assertTrue (testUpperLimit64<UInt64>());
}


void NumberParserTest::testParseError()
{
	char dp = decimalSeparator();
	if (dp == 0) dp = '.';
	char ts = thousandSeparator();
	if (ts == 0) ts = ',';
	assertTrue (dp != ts);

	try
	{
		NumberParser::parse("");
		NumberParser::parseBool("");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parse(" ");
		NumberParser::parseBool("");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parse(" 123");
		NumberParser::parseBool("");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parse("1 1");
		NumberParser::parseBool("");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parse("asd");
		NumberParser::parseBool("asd");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseUnsigned("a123");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseUnsigned("-123");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseHex("z23");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseHex("23z");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseHex("xxx");
		failmsg("must throw SyntaxException");
	}
	catch (SyntaxException&) {}

#if defined(POCO_HAVE_INT64)

	try
	{
		NumberParser::parse64("asd");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseUnsigned64("");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseHex64("zaz");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseHex64("12345z");
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

	try
	{
		NumberParser::parseHex64(format("123%c45", ts));
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

#endif // POCO_HAVE_INT64

#ifndef POCO_NO_FPENVIRONMENT
	try
	{
		NumberParser::parseFloat(format("a12%c3", dp));
		failmsg("must throw SyntaxException");
	} catch (SyntaxException&) { }

#endif // POCO_NO_FPENVIRONMENT
}


void NumberParserTest::setUp()
{
}


void NumberParserTest::tearDown()
{
}


CppUnit::Test* NumberParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NumberParserTest");

	CppUnit_addTest(pSuite, NumberParserTest, testParse);
	CppUnit_addTest(pSuite, NumberParserTest, testLimits);
	CppUnit_addTest(pSuite, NumberParserTest, testParseError);

	return pSuite;
}
