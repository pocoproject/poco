//
// StopwatchTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StopwatchTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Stopwatch.h"
#include "Poco/Timestamp.h"
#include "Poco/Thread.h"


using Poco::Stopwatch;
using Poco::Timestamp;
using Poco::Thread;


StopwatchTest::StopwatchTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


StopwatchTest::~StopwatchTest()
{
}


void StopwatchTest::testStopwatch()
{
	Stopwatch sw;
	sw.start();
	Thread::sleep(200);
	sw.stop();
	Timestamp::TimeDiff d = sw.elapsed();
	assertTrue (d >= 180000 && d <= 300000);	
	sw.start();	
	Thread::sleep(100);
	d = sw.elapsed();
	assertTrue (d >= 280000 && d <= 400000);	
	Thread::sleep(100);
	sw.stop();
	d = sw.elapsed();
	assertTrue (d >= 380000 && d <= 500000);
	sw.reset();
	sw.start();
	Thread::sleep(200);
	sw.stop();
	d = sw.elapsed();
	assertTrue (d >= 180000 && d <= 300000);	
}


void StopwatchTest::setUp()
{
}


void StopwatchTest::tearDown()
{
}


CppUnit::Test* StopwatchTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StopwatchTest");

	CppUnit_addTest(pSuite, StopwatchTest, testStopwatch);

	return pSuite;
}
