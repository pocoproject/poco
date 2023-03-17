//
// DateTimeFormatterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DateTimeFormatterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"

using Poco::DateTime;
using Poco::Timestamp;
using Poco::Timespan;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;


DateTimeFormatterTest::DateTimeFormatterTest(const std::string& name)
	: CppUnit::TestCase(name)
{
	// Linker regresion SF #3288584
	std::string message;
	Poco::LocalDateTime now;
	Poco::DateTimeFormatter::append(message, now, "%H:%M:%S.%i");
}


DateTimeFormatterTest::~DateTimeFormatterTest()
{
}


void DateTimeFormatterTest::testISO8601()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT);
	assertTrue (str == "2005-01-08T12:30:00Z");

	str = DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT, 3600);
	assertTrue (str == "2005-01-08T12:30:00+01:00");

	str = DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT, -3600);
	assertTrue (str == "2005-01-08T12:30:00-01:00");
}


void DateTimeFormatterTest::testISO8601Frac()
{
	DateTime dt(2005, 1, 8, 12, 30, 00, 12, 34);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FRAC_FORMAT);
	assertTrue (str == "2005-01-08T12:30:00.012034Z");

	str = DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FRAC_FORMAT, 3600);
	assertTrue (str == "2005-01-08T12:30:00.012034+01:00");

	str = DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FRAC_FORMAT, -3600);
	assertTrue (str == "2005-01-08T12:30:00.012034-01:00");
}


void DateTimeFormatterTest::testISO8601Timestamp()
{
	DateTime dt(1582, 10, 15, 1, 2, 34, 56, 78);
	Timestamp ts = dt.timestamp();

	std::string str = DateTimeFormatter::format(ts, DateTimeFormat::ISO8601_FRAC_FORMAT);
	assertTrue (str == "1582-10-15T01:02:34.056078Z");

	DateTime preGregorian(1580, 1, 1, 1, 2, 34, 56, 78);
	ts = preGregorian.timestamp();

	str = DateTimeFormatter::format(ts, DateTimeFormat::ISO8601_FRAC_FORMAT);
	assertTrue (str == "1580-01-01T01:02:34.056078Z");
}


void DateTimeFormatterTest::testRFC822()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::RFC822_FORMAT);
	assertTrue (str == "Sat, 8 Jan 05 12:30:00 GMT");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC822_FORMAT, 3600);
	assertTrue (str == "Sat, 8 Jan 05 12:30:00 +0100");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC822_FORMAT, -3600);
	assertTrue (str == "Sat, 8 Jan 05 12:30:00 -0100");
}


void DateTimeFormatterTest::testRFC1123()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::RFC1123_FORMAT);
	assertTrue (str == "Sat, 8 Jan 2005 12:30:00 GMT");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC1123_FORMAT, 3600);
	assertTrue (str == "Sat, 8 Jan 2005 12:30:00 +0100");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC1123_FORMAT, -3600);
	assertTrue (str == "Sat, 8 Jan 2005 12:30:00 -0100");
}


void DateTimeFormatterTest::testHTTP()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::HTTP_FORMAT);
	assertTrue (str == "Sat, 08 Jan 2005 12:30:00 GMT");

	str = DateTimeFormatter::format(dt, DateTimeFormat::HTTP_FORMAT, 3600);
	assertTrue (str == "Sat, 08 Jan 2005 12:30:00 +0100");

	str = DateTimeFormatter::format(dt, DateTimeFormat::HTTP_FORMAT, -3600);
	assertTrue (str == "Sat, 08 Jan 2005 12:30:00 -0100");
}


void DateTimeFormatterTest::testRFC850()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::RFC850_FORMAT);
	assertTrue (str == "Saturday, 8-Jan-05 12:30:00 GMT");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC850_FORMAT, 3600);
	assertTrue (str == "Saturday, 8-Jan-05 12:30:00 +0100");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC850_FORMAT, -3600);
	assertTrue (str == "Saturday, 8-Jan-05 12:30:00 -0100");
}


void DateTimeFormatterTest::testRFC1036()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::RFC1036_FORMAT);
	assertTrue (str == "Saturday, 8 Jan 05 12:30:00 GMT");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC1036_FORMAT, 3600);
	assertTrue (str == "Saturday, 8 Jan 05 12:30:00 +0100");

	str = DateTimeFormatter::format(dt, DateTimeFormat::RFC1036_FORMAT, -3600);
	assertTrue (str == "Saturday, 8 Jan 05 12:30:00 -0100");
}


void DateTimeFormatterTest::testASCTIME()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::ASCTIME_FORMAT);
	assertTrue (str == "Sat Jan  8 12:30:00 2005");
}


void DateTimeFormatterTest::testSORTABLE()
{
	DateTime dt(2005, 1, 8, 12, 30, 00);

	std::string str = DateTimeFormatter::format(dt, DateTimeFormat::SORTABLE_FORMAT);
	assertTrue (str == "2005-01-08 12:30:00");
}


void DateTimeFormatterTest::testCustom()
{
	DateTime dt(2005, 1, 8, 12, 30, 00, 250);

	std::string str = DateTimeFormatter::format(dt, "%w/%W/%b/%B/%d/%e/%f/%m/%n/%o/%y/%Y/%H/%h/%a/%A/%M/%S/%i/%c/%z/%Z/%%");
	assertTrue (str == "Sat/Saturday/Jan/January/08/8/ 8/01/1/ 1/05/2005/12/12/pm/PM/30/00/250/2/Z/GMT/%");
}


void DateTimeFormatterTest::testTimespan()
{
	Timespan ts(1, 1, 1, 1, 1000);
	std::string str = DateTimeFormatter::format(ts);
	assertTrue (str == "1d 01:01:01.001");

	Timespan ts1(1, 24, 1, 1, 1000);
	str = DateTimeFormatter::format(ts1);
	assertTrue (str == "2d 00:01:01.001");

	Timespan ts2(1, 25, 1, 1, 1000);
	str = DateTimeFormatter::format(ts2);
	assertTrue (str == "2d 01:01:01.001");

	Timespan ts3(5, 4, 3, 2, 1000);
	str = DateTimeFormatter::format(ts3, "%i.%S:%M:%H d%d %%");
	assertTrue (str == "001.02:03:04 d5 %");

	Timespan ts4(0, 24, 60, 60, 1001000);
	str = DateTimeFormatter::format(ts4);
	assertTrue (str == "1d 01:01:01.001");

	Timespan ts5(2, 11, 30, 20, 0);
	str = DateTimeFormatter::format(ts5, "%h %m %s");
	assertTrue (str == "59 3570 214220");
}


void DateTimeFormatterTest::setUp()
{
}


void DateTimeFormatterTest::tearDown()
{
}


CppUnit::Test* DateTimeFormatterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DateTimeFormatterTest");

	CppUnit_addTest(pSuite, DateTimeFormatterTest, testISO8601);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testISO8601Frac);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testISO8601Timestamp);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testRFC822);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testRFC1123);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testHTTP);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testRFC850);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testRFC1036);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testASCTIME);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testSORTABLE);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testCustom);
	CppUnit_addTest(pSuite, DateTimeFormatterTest, testTimespan);

	return pSuite;
}
