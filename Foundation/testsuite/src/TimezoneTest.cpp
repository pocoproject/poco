//
// TimezoneTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TimezoneTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Timezone.h"
#include <iostream>


using Poco::Timezone;


TimezoneTest::TimezoneTest(const std::string& name): CppUnit::TestCase(name)
{
}


TimezoneTest::~TimezoneTest()
{
}


void TimezoneTest::testTimezone()
{
	std::string name = Timezone::name();
	std::string stdName = Timezone::standardName();
	std::string dstName = Timezone::dstName();
	std::cout << "Timezone Names: " << name << ", " << stdName << ", " << dstName << std::endl;
	int utcOffset = Timezone::utcOffset();
	std::cout << "UTC Offset: " << utcOffset << std::endl;
	int dst = Timezone::dst();
	std::cout << "DST Offset: " << dst << std::endl;
}


void TimezoneTest::testUtcOffsetCaching()
{
	// Test that utcOffset returns consistent values (cached at startup)
	int offset1 = Timezone::utcOffset();
	int offset2 = Timezone::utcOffset();
	int offset3 = Timezone::utcOffset();

	assertTrue (offset1 == offset2);
	assertTrue (offset2 == offset3);

	// UTC offset should be within valid range (-12 to +14 hours in seconds)
	assertTrue (offset1 >= -12 * 3600);
	assertTrue (offset1 <= 14 * 3600);

	// UTC offset should be a multiple of 15 minutes (900 seconds)
	// as all real-world timezones use 15-minute increments
	assertTrue (offset1 % 900 == 0);
}


void TimezoneTest::setUp()
{
}


void TimezoneTest::tearDown()
{
}


CppUnit::Test* TimezoneTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TimezoneTest");

	CppUnit_addTest(pSuite, TimezoneTest, testTimezone);
	CppUnit_addTest(pSuite, TimezoneTest, testUtcOffsetCaching);

	return pSuite;
}
