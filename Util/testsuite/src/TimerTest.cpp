//
// TimerTest.cpp
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
using Poco::Clock;


TimerTest::TimerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TimerTest::~TimerTest()
{
}


void TimerTest::testScheduleTimestamp()
{
	Timer timer;

	Timestamp time;
	time += 1000000;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	timer.schedule(pTask, time);

	_event.wait();
	assertTrue (pTask->lastExecution() >= time);
}


void TimerTest::testScheduleClock()
{
	Timer timer;

	// As reference
	Timestamp time;
	time += 1000000;

	Clock clock;
	clock += 1000000;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	timer.schedule(pTask, clock);

	_event.wait();
	assertTrue (pTask->lastExecution() >= time);
}


void TimerTest::testScheduleInterval()
{
	Timer timer;

	Timestamp time;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	timer.schedule(pTask, 500, 500);

	_event.wait();
	assertTrue (time.elapsed() >= 590000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1190000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1790000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	pTask->cancel();
	assertTrue (pTask->isCancelled());
}


void TimerTest::testScheduleIntervalTimestamp()
{
	Timer timer;

	Timestamp time;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	Timestamp scheduleTime;
	scheduleTime += 500 * 1000;

	timer.schedule(pTask, scheduleTime, 500);

	_event.wait();
	assertTrue (time.elapsed() >= 590000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1190000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1790000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	pTask->cancel();
	assertTrue (pTask->isCancelled());
}


void TimerTest::testScheduleIntervalClock()
{
	Timer timer;

	Timestamp time;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	Clock scheduleClock;
	scheduleClock += 500 * 1000;

	timer.schedule(pTask, scheduleClock, 500);

	_event.wait();
	assertTrue (time.elapsed() >= 590000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1190000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1790000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	pTask->cancel();
	assertTrue (pTask->isCancelled());
}


void TimerTest::testScheduleAtFixedRate()
{
	Timer timer;

	Timestamp time;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	timer.scheduleAtFixedRate(pTask, 500, 500);

	_event.wait();
	assertTrue (time.elapsed() >= 500000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1000000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	_event.wait();
	assertTrue (time.elapsed() >= 1500000);
	assertTrue (pTask->lastExecution().elapsed() < 130000);

	pTask->cancel();
	assertTrue (pTask->isCancelled());
}


void TimerTest::testCancel()
{
	Timer timer;

	Timestamp time;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	assertTrue (pTask->lastExecution() == 0);

	timer.scheduleAtFixedRate(pTask, 5000, 5000);

	pTask->cancel();
	assertTrue (pTask->isCancelled());

	try
	{
		timer.scheduleAtFixedRate(pTask, 5000, 5000);
		fail("must not reschedule a cancelled task");
	}
	catch (Poco::IllegalStateException&)
	{
	}
	catch (Poco::Exception&)
	{
		fail("bad exception thrown");
	}
}


void TimerTest::testCancelAllStop()
{
	{
		Timer timer;

		TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

		// We are scheduling a timer event in 100ms
		Timestamp time;
		time += 100000;
		timer.schedule(pTask, time);

		timer.cancel(false);
		// Timer should fire in 100ms and onTimer has 100ms sleep in it.
		// So we are waiting 2 times that plus a small buffer that to make sure that event was never executed.
		bool timerExecuted = _event.tryWait(200 + 50);
		assertFalse (timerExecuted);
	}

	assertTrue (true); // don't hang
}


void TimerTest::testCancelAllWaitStop()
{
	{
		Timer timer;

		TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

		// We are scheduling a timer event in 100ms
		Timestamp time;
		time += 100000;
		timer.schedule(pTask, time);

		timer.cancel(true);
		// Timer should fire in 100ms and onTimer has 100ms sleep in it.
		// So we are waiting 2 times that plus a small buffer that to make sure that event was never executed.
		bool timerExecuted = _event.tryWait(200 + 50);
		assertFalse (timerExecuted);
	}

	assertTrue (true); // don't hang
}


void TimerTest::testMultiCancelAllWaitStop()
{
	Timer timer;

	// We will schedule a task and wait for it to start.
	// After that we will schedule 2 cancel Notifications, one async and the other sync.
	// But we want to make sure that both are scheduled and present in internal queue, thus we need to wait for this
	// first task to start.
	Poco::Event startEvent;
	Poco::Event canceledScheduledEvent;
	timer.schedule(Timer::func([&startEvent, &canceledScheduledEvent]()
	{
		startEvent.set();
		canceledScheduledEvent.wait();
		Poco::Thread::sleep(100);
	}), Poco::Clock());
	// We wait for simple task to start.
	startEvent.wait();
	// Schedule async cancel notification.
	timer.cancel();
	// Now allow simple task to proceed to sleep, in other words give time for next cancel to block.
	canceledScheduledEvent.set();
	// Schedule sync cancel, now we should have 2 cancel notifications in internal queue.
	timer.cancel(true);

	assertTrue (true); // don't hang
}


void TimerTest::testFunc()
{
	Timer timer;

	std::atomic<int> count(0);
	timer.schedule(Timer::func([&count]()
	{
		count++;
	}), Poco::Clock());
	Poco::Thread::sleep(100);

	assertTrue (count == 1);
}


void TimerTest::testIdle()
{
	Timer timer;

	assertTrue (timer.idle());

	Timestamp time;
	time += 1000000;

	TimerTask::Ptr pTask = new TimerTaskAdapter<TimerTest>(*this, &TimerTest::onTimer);

	timer.schedule(pTask, time);

	assertFalse (timer.idle());

	_event.wait();
	assertTrue (pTask->lastExecution() >= time);

	assertTrue (timer.idle());
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

	CppUnit_addTest(pSuite, TimerTest, testScheduleTimestamp);
	CppUnit_addTest(pSuite, TimerTest, testScheduleClock);
	CppUnit_addTest(pSuite, TimerTest, testScheduleInterval);
	CppUnit_addTest(pSuite, TimerTest, testScheduleIntervalTimestamp);
	CppUnit_addTest(pSuite, TimerTest, testScheduleIntervalClock);
	CppUnit_addTest(pSuite, TimerTest, testScheduleAtFixedRate);
	CppUnit_addTest(pSuite, TimerTest, testCancel);
	CppUnit_addTest(pSuite, TimerTest, testCancelAllStop);
	CppUnit_addTest(pSuite, TimerTest, testCancelAllWaitStop);
	CppUnit_addTest(pSuite, TimerTest, testMultiCancelAllWaitStop);
	CppUnit_addTest(pSuite, TimerTest, testFunc);
	CppUnit_addTest(pSuite, TimerTest, testIdle);

	return pSuite;
}
