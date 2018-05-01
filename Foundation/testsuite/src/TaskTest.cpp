//
// TaskTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TaskTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Task.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/AutoPtr.h"


using Poco::Task;
using Poco::Thread;
using Poco::Event;
using Poco::AutoPtr;


namespace
{
	class TestTask: public Task
	{
	public:
		TestTask(): Task("TestTask")
		{
		}
		
		void runTask()
		{
			_event.wait();
			if (sleep(10))
				return;
			setProgress(0.5);
			_event.wait();
			if (isCancelled())
				return;
			setProgress(1.0);
			_event.wait();
		}
		
		void cont()
		{
			_event.set();
		}
		
	private:
		Event _event;
	};
}


TaskTest::TaskTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


TaskTest::~TaskTest()
{
}


void TaskTest::testFinish()
{
	AutoPtr<TestTask> pTT = new TestTask;
	assertTrue (pTT->state() == Task::TASK_IDLE);
	Thread thr;
	thr.start(*pTT);
	assertTrue (pTT->progress() == 0);
	pTT->cont();
	while (pTT->progress() != 0.5) Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_RUNNING);
	pTT->cont();
	while (pTT->progress() != 1.0) Thread::sleep(50);
	pTT->cont();
	thr.join();
	assertTrue (pTT->state() == Task::TASK_FINISHED);
}


void TaskTest::testCancel1()
{
	AutoPtr<TestTask> pTT = new TestTask;
	assertTrue (pTT->state() == Task::TASK_IDLE);
	Thread thr;
	thr.start(*pTT);
	assertTrue (pTT->progress() == 0);
	pTT->cont();
	while (pTT->progress() != 0.5) Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_RUNNING);
	pTT->cancel();
	assertTrue (pTT->state() == Task::TASK_CANCELLING);
	pTT->cont();
	thr.join();
	assertTrue (pTT->state() == Task::TASK_FINISHED);
}


void TaskTest::testCancel2()
{
	AutoPtr<TestTask> pTT = new TestTask;
	assertTrue (pTT->state() == Task::TASK_IDLE);
	Thread thr;
	thr.start(*pTT);
	assertTrue (pTT->progress() == 0);
	pTT->cancel();
	assertTrue (pTT->state() == Task::TASK_CANCELLING);
	pTT->cont();
	thr.join();
	assertTrue (pTT->state() == Task::TASK_FINISHED);
}


void TaskTest::setUp()
{
}


void TaskTest::tearDown()
{
}


CppUnit::Test* TaskTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TaskTest");

	CppUnit_addTest(pSuite, TaskTest, testFinish);
	CppUnit_addTest(pSuite, TaskTest, testCancel1);
	CppUnit_addTest(pSuite, TaskTest, testCancel2);

	return pSuite;
}
