//
// TimerTest.cpp
//
// $Id: //poco/1.4/Util/testsuite/src/TimerTest.cpp#1 $
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TimerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTaskAdapter.h"


using Poco::Util::Timer;
using Poco::Util::TimerTask;
using Poco::Util::TimerTaskAdapter;
using Poco::Timestamp;


TimerTest::TimerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TimerTest::~TimerTest()
{
}


void TimerTest::testSchedule()
{
	Timer timer;
	
	Timestamp time;
	time += 1000000;
	
	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);
	
	assert (pTask->lastExecution() == 0);
	
	timer.schedule(pTask, time);
	
	_event.wait();
	assert (pTask->lastExecution() >= time);
}


void TimerTest::testScheduleInterval()
{
	Timer timer;
	
	Timestamp time;
	
	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);
	
	assert (pTask->lastExecution() == 0);
	
	timer.schedule(pTask, 500, 500);
	
	_event.wait();
	assert (time.elapsed() >= 590000);
	assert (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assert (time.elapsed() >= 1190000);
	assert (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assert (time.elapsed() >= 1790000);
	assert (pTask->lastExecution().elapsed() < 130000);

	pTask->cancel();
	assert (pTask->isCancelled());
}


void TimerTest::testScheduleAtFixedRate()
{
	Timer timer;
	
	Timestamp time;
	
	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);
	
	assert (pTask->lastExecution() == 0);
	
	timer.scheduleAtFixedRate(pTask, 500, 500);
	
	_event.wait();
	assert (time.elapsed() >= 500000);
	assert (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assert (time.elapsed() >= 1000000);
	assert (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assert (time.elapsed() >= 1500000);
	assert (pTask->lastExecution().elapsed() < 130000);

	pTask->cancel();
	assert (pTask->isCancelled());
}


void TimerTest::setUp()
{
}


void TimerTest::tearDown()
{
}


void TimerTest::onTimer(TimerTask& task)
{
	Poco::Thread::sleep(100);
	_event.set();
}


CppUnit::Test* TimerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TimerTest");

	CppUnit_addTest(pSuite, TimerTest, testSchedule);
	CppUnit_addTest(pSuite, TimerTest, testScheduleInterval);
	CppUnit_addTest(pSuite, TimerTest, testScheduleAtFixedRate);

	return pSuite;
}
