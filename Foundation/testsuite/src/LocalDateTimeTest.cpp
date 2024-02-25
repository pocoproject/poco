//
// LocalDateTimeTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "LocalDateTimeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/Timezone.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include <ctime>
#include <iostream>


using Poco::LocalDateTime;
using Poco::DateTime;
using Poco::Timestamp;
using Poco::Timespan;
using Poco::Timezone;
using std::strftime;


LocalDateTimeTest::LocalDateTimeTest(const std::string& name): CppUnit::TestCase(name)
{
}


LocalDateTimeTest::~LocalDateTimeTest()
{
}


void LocalDateTimeTest::testGregorian1()
{
	LocalDateTime dt(1970, 1, 1);
	assertTrue (dt.year() == 1970);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 1);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.dayOfWeek() == 4);
	// REMOVED: this fails when the current DST offset differs from
	//          the one on 1970-1-1
	//assertTrue (dt.tzd() == Timezone::tzd());
	assertTrue (dt.julianDay() == 2440587.5);

	dt.assign(2001, 9, 9, 1, 46, 40);
	assertTrue (dt.year() == 2001);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 9);
	assertTrue (dt.hour() == 1);
	assertTrue (dt.minute() == 46);
	assertTrue (dt.second() == 40);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.dayOfWeek() == 0);
	//assertTrue (dt.tzd() == Timezone::tzd());
	assertEqualDelta (dt.julianDay(), 2452161.574074, 0.000001);
}


void LocalDateTimeTest::testGregorian2()
{
	LocalDateTime dt(2*3600, 1970, 1, 1, 0, 0, 0, 0, 0);
	assertTrue (dt.year() == 1970);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 1);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.dayOfWeek() == 4);
	assertTrue (dt.tzd() == 2*3600);

	dt.assign(-7*3600, 2001, 9, 9, 1, 46, 40, 0, 0);
	assertTrue (dt.year() == 2001);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 9);
	assertTrue (dt.hour() == 1);
	assertTrue (dt.minute() == 46);
	assertTrue (dt.second() == 40);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.dayOfWeek() == 0);
	assertTrue (dt.tzd() == -7*3600);
}


void LocalDateTimeTest::testConversions()
{
	LocalDateTime dt1(2*3600, 2005, 1, 28, 14, 24, 44, 234, 0);
	LocalDateTime dt2(dt1.tzd(), dt1.utc());
	LocalDateTime dt3;
	dt3 = dt1;
	LocalDateTime dt4(dt2);
	LocalDateTime dt5(-4*3600, dt1.utc());

	assertTrue (dt2.year() == 2005);
	assertTrue (dt2.month() == 1);
	assertTrue (dt2.day() == 28);
	assertTrue (dt2.hour() == 14);
	assertTrue (dt2.minute() == 24);
	assertTrue (dt2.second() == 44);
	assertTrue (dt2.millisecond() == 234);
	assertTrue (dt2.dayOfWeek() == 5);
	assertTrue (dt2.tzd() == 2*3600);

	assertTrue (dt5.year() == 2005);
	assertTrue (dt5.month() == 1);
	assertTrue (dt5.day() == 28);
	assertTrue (dt5.hour() == 8);
	assertTrue (dt5.minute() == 24);
	assertTrue (dt5.second() == 44);
	assertTrue (dt5.millisecond() == 234);
	assertTrue (dt5.dayOfWeek() == 5);
	assertTrue (dt5.tzd() == -4*3600);

	DateTime dt6(2005, 1, 28, 14, 24, 44, 234, 0);
	LocalDateTime dt7(3600, dt6);
	LocalDateTime dt8(3600, dt6, false);
	LocalDateTime dt9(3600, dt6, true);

	assertTrue (dt7.hour() == 15);
	assertTrue (dt8.hour() == 14);
	assertTrue (dt9.hour() == 15);
}


void LocalDateTimeTest::testCalcs()
{
	LocalDateTime dt1(2005, 1, 1);
	assertTrue (dt1.dayOfYear() == 1);
	assertTrue (dt1.week(DateTime::MONDAY) == 0);
	dt1.assign(2005, 1, 3);
	assertTrue (dt1.dayOfYear() == 3);
	assertTrue (dt1.week(DateTime::MONDAY) == 1);
	dt1.assign(2005, 1, 9);
	assertTrue (dt1.dayOfYear() == 9);
	assertTrue (dt1.week(DateTime::MONDAY) == 1);
	dt1.assign(2005, 1, 10);
	assertTrue (dt1.dayOfYear() == 10);
	assertTrue (dt1.week(DateTime::MONDAY) == 2);
	dt1.assign(2005, 2, 1);
	assertTrue (dt1.dayOfYear() == 32);
	assertTrue (dt1.week(DateTime::MONDAY) == 5);
	dt1.assign(2005, 12, 31);
	assertTrue (dt1.week(DateTime::MONDAY) == 52);
	dt1.assign(2007, 1, 1);
	assertTrue (dt1.week(DateTime::MONDAY) == 1);
	dt1.assign(2007, 12, 31);
	assertTrue (dt1.week(DateTime::MONDAY) == 53);

	// Jan 1 is Mon
	dt1.assign(2001, 1, 1);
	assertTrue (dt1.week() == 1);
	dt1.assign(2001, 1, 7);
	assertTrue (dt1.week() == 1);
	dt1.assign(2001, 1, 8);
	assertTrue (dt1.week() == 2);
	dt1.assign(2001, 1, 21);
	assertTrue (dt1.week() == 3);
	dt1.assign(2001, 1, 22);
	assertTrue (dt1.week() == 4);

	// Jan 1 is Tue
	dt1.assign(2002, 1, 1);
	assertTrue (dt1.week() == 1);
	dt1.assign(2002, 1, 6);
	assertTrue (dt1.week() == 1);
	dt1.assign(2002, 1, 7);
	assertTrue (dt1.week() == 2);
	dt1.assign(2002, 1, 20);
	assertTrue (dt1.week() == 3);
	dt1.assign(2002, 1, 21);
	assertTrue (dt1.week() == 4);

	// Jan 1 is Wed
	dt1.assign(2003, 1, 1);
	assertTrue (dt1.week() == 1);
	dt1.assign(2003, 1, 5);
	assertTrue (dt1.week() == 1);
	dt1.assign(2003, 1, 6);
	assertTrue (dt1.week() == 2);
	dt1.assign(2003, 1, 19);
	assertTrue (dt1.week() == 3);
	dt1.assign(2003, 1, 20);
	assertTrue (dt1.week() == 4);

	// Jan 1 is Thu
	dt1.assign(2004, 1, 1);
	assertTrue (dt1.week() == 1);
	dt1.assign(2004, 1, 4);
	assertTrue (dt1.week() == 1);
	dt1.assign(2004, 1, 5);
	assertTrue (dt1.week() == 2);
	dt1.assign(2004, 1, 18);
	assertTrue (dt1.week() == 3);
	dt1.assign(2004, 1, 19);
	assertTrue (dt1.week() == 4);

	// Jan 1 is Fri
	dt1.assign(1999, 1, 1);
	assertTrue (dt1.week() == 0);
	dt1.assign(1999, 1, 3);
	assertTrue (dt1.week() == 0);
	dt1.assign(1999, 1, 4);
	assertTrue (dt1.week() == 1);
	dt1.assign(1999, 1, 17);
	assertTrue (dt1.week() == 2);
	dt1.assign(1999, 1, 18);
	assertTrue (dt1.week() == 3);

	// Jan 1 is Sat
	dt1.assign(2000, 1, 1);
	assertTrue (dt1.week() == 0);
	dt1.assign(2000, 1, 2);
	assertTrue (dt1.week() == 0);
	dt1.assign(2000, 1, 3);
	assertTrue (dt1.week() == 1);
	dt1.assign(2000, 1, 16);
	assertTrue (dt1.week() == 2);
	dt1.assign(2000, 1, 17);
	assertTrue (dt1.week() == 3);

	// Jan 1 is Sun
	dt1.assign(1995, 1, 1);
	assertTrue (dt1.week() == 0);
	dt1.assign(1995, 1, 2);
	assertTrue (dt1.week() == 1);
	dt1.assign(1995, 1, 3);
	assertTrue (dt1.week() == 1);
	dt1.assign(1995, 1, 15);
	assertTrue (dt1.week() == 2);
	dt1.assign(1995, 1, 16);
	assertTrue (dt1.week() == 3);
}


void LocalDateTimeTest::testAMPM()
{
	LocalDateTime dt1(2005, 1, 1, 0, 15, 30);
	assertTrue (dt1.isAM());
	assertTrue (!dt1.isPM());
	assertTrue (dt1.hourAMPM() == 12);

	dt1.assign(2005, 1, 1, 12, 15, 30);
	assertTrue (!dt1.isAM());
	assertTrue (dt1.isPM());
	assertTrue (dt1.hourAMPM() == 12);

	dt1.assign(2005, 1, 1, 13, 15, 30);
	assertTrue (!dt1.isAM());
	assertTrue (dt1.isPM());
	assertTrue (dt1.hourAMPM() == 1);
}


void LocalDateTimeTest::testRelational1()
{
	LocalDateTime dt1(2005, 1, 1, 0, 15, 30);
	LocalDateTime dt2(2005, 1, 2, 0, 15, 30);
	LocalDateTime dt3(dt1);

	assertTrue (dt1 < dt2);
	assertTrue (dt1 <= dt2);
	assertTrue (dt2 > dt1);
	assertTrue (dt2 >= dt1);
	assertTrue (dt1 != dt2);
	assertTrue (!(dt1 == dt2));

	assertTrue (dt1 == dt3);
	assertTrue (!(dt1 != dt3));
	assertTrue (dt1 >= dt3);
	assertTrue (dt1 <= dt3);
	assertTrue (!(dt1 > dt3));
	assertTrue (!(dt1 < dt3));
}


void LocalDateTimeTest::testRelational2()
{
	LocalDateTime dt1(2*3600, 2005, 1, 1, 15, 30, 0, 0, 0);
	LocalDateTime dt2(2*3600, 2005, 1, 1, 17, 30, 0, 0, 0);
	LocalDateTime dt3(5*3600, 2005, 1, 1, 18, 30, 0, 0, 0);

	assertTrue (dt1 < dt2);
	assertTrue (dt1 <= dt2);
	assertTrue (dt2 > dt1);
	assertTrue (dt2 >= dt1);
	assertTrue (dt1 != dt2);
	assertTrue (!(dt1 == dt2));

	assertTrue (dt1 == dt3);
	assertTrue (!(dt1 != dt3));
	assertTrue (dt1 >= dt3);
	assertTrue (dt1 <= dt3);
	assertTrue (!(dt1 > dt3));
	assertTrue (!(dt1 < dt3));
}


void LocalDateTimeTest::testArithmetics1()
{
	LocalDateTime dt1(2005, 1, 1, 0, 15, 30);
	LocalDateTime dt2(2005, 1, 2, 0, 15, 30);

	Timespan s = dt2 - dt1;
	assertTrue (s.days() == 1);

	LocalDateTime dt3 = dt1 + s;
	assertTrue (dt3 == dt2);

	dt3 -= s;
	assertTrue (dt3 == dt1);
	dt1 += s;
	assertTrue (dt1 == dt2);
}


void LocalDateTimeTest::testArithmetics2()
{
	LocalDateTime dt1(2*3600, 2005, 1, 1, 15, 30, 0, 0, 0);
	LocalDateTime dt2(5*3600, 2005, 1, 2, 18, 30, 0, 0, 0);

	Timespan s = dt2 - dt1;
	assertTrue (s.days() == 1);

	LocalDateTime dt3 = dt1 + s;
	assertTrue (dt3 == dt2);

	dt3 -= s;
	assertTrue (dt3 == dt1);
	dt1 += s;
	assertTrue (dt1 == dt2);
}


void LocalDateTimeTest::testSwap()
{
	LocalDateTime dt1(2005, 1, 1, 0, 15, 30);
	LocalDateTime dt2(2005, 1, 2, 0, 15, 30);

	assertTrue (dt1 < dt2);
	dt1.swap(dt2);
	assertTrue (dt2 < dt1);
}


void LocalDateTimeTest::testTimezone()
{
	std::time_t   tINCREMENT = (30 * 24 * 60 * 60); // 30 days
	Timespan      tsINCREMENT(30*Timespan::DAYS);
	LocalDateTime now;
	std::time_t   t = std::time(NULL);
	std::tm       then;
	bool          foundDST = false;

	then = *std::localtime(&t);
	if (then.tm_isdst >= 0)
	{
		std::string tzNow, tzThen;
		char tzBuf[48] = {0};
		if (0 == std::strftime(&tzBuf[0], sizeof(tzBuf), "%Z", &then))
			fail ("Insufficient character array length.");

		tzNow = tzThen = tzBuf;
		int iterations = 0;
		while (iterations < 14)
		{
			// Add one month until the timezone changes or we roll
			// over 13 months.
			t += tINCREMENT;
			then = *std::localtime(&t);
			std::strftime(&tzBuf[0], sizeof(tzBuf), "%Z", &then);
			tzThen = tzBuf;
			foundDST = (tzNow == tzThen);
			if (foundDST) break;
			++iterations;
		}
		if (foundDST)
		{
			// We found a timezone change that was induced by changing
			// the month, so we crossed a DST boundary. Now we can
			// actually do the test...
			//
			// Start with the current time and add 30 days for 13
			// iterations. Do this with both a LocalDateTime object and
			// a ANSI C time_t. Then create a LocalDateTime based on the
			// time_t and verify that the time_t calculated value is equal
			// to the LocalDateTime value. The comparision operator
			// verifies the _dateTime and _tzd members.
			LocalDateTime dt2;
			t = std::time(NULL);
			for (iterations = 0; iterations < 14; ++iterations)
			{
				t += tINCREMENT;
				dt2 += tsINCREMENT;
				then = *std::localtime(&t);

				// This is the tricky part. We have to use the constructor
				// from a UTC DateTime object that is constructed from the
				// time_t. The LocalDateTime constructor with integer
				// arguments, LocalDateTime(yr, mon, day, ...), assumes that
				// the time is already adjusted with respect to the time
				// zone. The DateTime conversion constructor, however, does
				// not. So we want to construct from the UTC time.
				//
				// The second tricky part is that we want to use the
				// sub-second information from the LocalDateTime object
				// since ANSI C time routines are not sub-second accurate.
				then = *std::gmtime(&t);
				LocalDateTime calcd(DateTime((then.tm_year + 1900),
											 (then.tm_mon + 1),
											 then.tm_mday,
											 then.tm_hour,
											 then.tm_min,
											 then.tm_sec,
											 dt2.millisecond(),
											 dt2.microsecond()));
				assertTrue (dt2 == calcd);
			}
		}
	}

	if (!foundDST)
	{
		std::cerr
			<< __FILE__ << ":" << __LINE__
			<< " - failed to locate DST boundary, timezone test skipped."
			<< std::endl;
	}
}


void LocalDateTimeTest::testTimezone2()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::vector<std::string> timezones = {
		"/usr/share/zoneinfo/America/Los_Angeles",
		"/usr/share/zoneinfo/Europe/London",
		"/usr/share/zoneinfo/Europe/Dublin", // special winter time of Ireland
		"/usr/share/zoneinfo/Europe/Prague",
	};

	std::cout << "\n";

	for (const std::string& tz : timezones)
	{
		setenv("TZ", tz.c_str(), 1); // POSIX-specific
		std::vector<LocalDateTime> times = {
			LocalDateTime(2022, 06, 29), // summer period
			LocalDateTime(2022, 01, 29), // winter period
		};
		for (const LocalDateTime& ldt : times) {
			std::time_t t = ldt.timestamp().epochTime();
			std::tm then;
			then = *std::localtime(&t);
#if POCO_OS == POCO_OS_SOLARIS
			assertTrue((mktime(&then)-t) * 1000 == ldt.tzd());
#else
			assertTrue (then.tm_gmtoff == ldt.tzd());
#endif
		}
		unsetenv("TZ");
	}
#endif
}


void LocalDateTimeTest::setUp()
{
}


void LocalDateTimeTest::tearDown()
{
}


CppUnit::Test* LocalDateTimeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LocalDateTimeTest");

	CppUnit_addTest(pSuite, LocalDateTimeTest, testGregorian1);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testGregorian2);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testConversions);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testCalcs);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testAMPM);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testRelational1);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testRelational2);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testArithmetics1);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testArithmetics2);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testSwap);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testTimezone);
	CppUnit_addTest(pSuite, LocalDateTimeTest, testTimezone2);

	return pSuite;
}
