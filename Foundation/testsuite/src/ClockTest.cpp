//
// ClockTest.cpp
//
// Copyright (c) 2013, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ClockTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Clock.h"
#include "Poco/Thread.h"
#include <iostream>


using Poco::Clock;
using Poco::Thread;


ClockTest::ClockTest(const std::string& name): CppUnit::TestCase(name)
{
}


ClockTest::~ClockTest()
{
}


void ClockTest::testClock()
{
	Clock t1;
	Thread::sleep(200);
	Clock t2;
	Clock t3 = t2;
	assertTrue (t1 != t2);
	assertTrue (!(t1 == t2));
	assertTrue (t2 > t1);
	assertTrue (t2 >= t1);
	assertTrue (!(t1 > t2));
	assertTrue (!(t1 >= t2));
	assertTrue (t2 == t3);
	assertTrue (!(t2 != t3));
	assertTrue (t2 >= t3);
	assertTrue (t2 <= t3);
	Clock::ClockDiff d = (t2 - t1);
	assertTrue (d >= 180000 && d <= 300000);

	Clock::ClockDiff acc = Clock::accuracy();
	assertTrue (acc > 0 && acc < Clock::resolution());
	std::cout << "Clock accuracy: " << acc << std::endl;

	t1.swap(t2);
	assertTrue (t1 > t2);
	t2.swap(t1);

	Clock now;
	Thread::sleep(201);
	assertTrue (now.elapsed() >= 200000);
	assertTrue (now.isElapsed(200000));
	assertTrue (!now.isElapsed(2000000));
}


void ClockTest::setUp()
{
}


void ClockTest::tearDown()
{
}


CppUnit::Test* ClockTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ClockTest");

	CppUnit_addTest(pSuite, ClockTest, testClock);

	return pSuite;
}
