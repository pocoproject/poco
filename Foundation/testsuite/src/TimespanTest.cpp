//
// TimespanTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TimespanTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Timespan.h"


using Poco::Timespan;


TimespanTest::TimespanTest(const std::string& name): CppUnit::TestCase(name)
{
}


TimespanTest::~TimespanTest()
{
}


void TimespanTest::testConversions()
{
	Timespan ts;
	assertTrue (ts.totalMicroseconds() == 0);
	ts = Timespan::DAYS;
	assertTrue (ts.totalMicroseconds() == Timespan::DAYS);
	assertTrue (ts.totalMilliseconds() == 86400000);
	assertTrue (ts.totalSeconds() == 86400);
	assertTrue (ts.totalMinutes() == 60*24);
	assertTrue (ts.totalHours() == 24);
	assertTrue (ts.days() == 1);
	
	assertTrue (ts.microseconds() == 0);
	assertTrue (ts.milliseconds() == 0);
	assertTrue (ts.seconds() == 0);
	assertTrue (ts.minutes() == 0);
	assertTrue (ts.hours() == 0);
	
	ts.assign(2, 12, 30, 10, 123456);
	assertTrue (ts.microseconds() == 456);
	assertTrue (ts.milliseconds() == 123);
	assertTrue (ts.seconds() == 10);
	assertTrue (ts.minutes() == 30);
	assertTrue (ts.hours() == 12);
	assertTrue (ts.days() == 2);

	ts.assign(0, 36, 30, 10, 123456);
	assertTrue (ts.microseconds() == 456);
	assertTrue (ts.milliseconds() == 123);
	assertTrue (ts.useconds() == 123456);
	assertTrue (ts.seconds() == 10);
	assertTrue (ts.minutes() == 30);
	assertTrue (ts.hours() == 12);
	assertTrue (ts.days() == 1);

	ts.assign(0, 0, 2190, 10, 123456);
	assertTrue (ts.microseconds() == 456);
	assertTrue (ts.milliseconds() == 123);
	assertTrue (ts.useconds() == 123456);
	assertTrue (ts.seconds() == 10);
	assertTrue (ts.minutes() == 30);
	assertTrue (ts.hours() == 12);
	assertTrue (ts.days() == 1);
}


void TimespanTest::testComparisons()
{
	Timespan ts1(10000000);
	Timespan ts2(20000000);
	Timespan ts3(20000000);
	
	assertTrue (ts1 != ts2);
	assertTrue (!(ts1 == ts2));
	assertTrue (ts1 <= ts2);
	assertTrue (ts1 < ts2);
	assertTrue (ts2 > ts1);
	assertTrue (ts2 >= ts1);
	
	assertTrue (ts2 == ts3);
	assertTrue (!(ts2 != ts3));
	assertTrue (ts2 >= ts3);
	assertTrue (ts2 <= ts3);
	assertTrue (!(ts2 > ts3));
	assertTrue (!(ts2 < ts3));
	
	assertTrue (ts1 == 10000000);
	assertTrue (ts1 != 20000000);
	assertTrue (ts1 <= 10000000);
	assertTrue (ts1 <= 20000000);
	assertTrue (ts1 >= 10000000);
	assertTrue (ts1 >= 5000000);
	assertTrue (ts1 < 20000000);
	assertTrue (ts1 > 5000000);
}


void TimespanTest::testArithmetics()
{
	Timespan ts1(100000000);
	Timespan ts2(50000000);
	Timespan ts3;
	ts3 = ts1 + ts2;
	assertTrue (ts3 == 150000000);
	ts3 = ts1 + 30000000;
	assertTrue (ts3 == 130000000);
	ts3 = ts1 - ts2;
	assertTrue (ts3 == 50000000);
	ts3 = ts1 - 20000000;
	assertTrue (ts3 == 80000000);
	ts3 += 20000000;
	assertTrue (ts3 == ts1);
	ts3 -= ts2;
	assertTrue (ts3 == ts2);
}


void TimespanTest::testSwap()
{
	Timespan ts1(10000000);
	Timespan ts2(50000000);
	
	assertTrue (ts1 < ts2);
	ts1.swap(ts2);
	assertTrue (ts2 < ts1);
}


void TimespanTest::setUp()
{
}


void TimespanTest::tearDown()
{
}


CppUnit::Test* TimespanTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TimespanTest");

	CppUnit_addTest(pSuite, TimespanTest, testConversions);
	CppUnit_addTest(pSuite, TimespanTest, testComparisons);
	CppUnit_addTest(pSuite, TimespanTest, testArithmetics);
	CppUnit_addTest(pSuite, TimespanTest, testSwap);

	return pSuite;
}
