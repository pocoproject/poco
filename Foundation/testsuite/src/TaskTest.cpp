//
// TaskTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TaskTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Task.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/AutoPtr.h"
#include <iostream>


using Poco::Task;
using Poco::Thread;
using Poco::Event;
using Poco::AutoPtr;


namespace
{
	class TestTask: public Task
	{
	public:
		TestTask(): Task("TestTask"),
			_started(false)
		{
		}

		void runTask()
		{
			_started = true;
			try
			{
				_event.wait();
				if (sleep(100))
					return;
				setProgress(0.5);
				_event.wait();
				if (isCancelled())
					return;
				setProgress(1.0);
				_event.wait();
			}
			catch(const Poco::Exception& e)
			{
				std::cerr << "TestTask::run(): " << e.displayText() << '\n';
			}
			catch(const std::exception& e)
			{
				std::cerr << "TestTask::run(): " << e.what() << '\n';
			}
			catch(...)
			{
				std::cerr << "TestTask::run(): unknown exception." << '\n';
			}
		}

		void cont()
		{
			try
			{
				_event.set();
			}
			catch(const Poco::SystemException& e)
			{
				std::cerr << "TestTask::cont(): " << e.displayText() << '\n';
			}
		}

		bool started() const
		{
			return _started;
		}

	private:
		Event _event;
		std::atomic<bool> _started;
	};
}


TaskTest::TaskTest(const std::string& name): CppUnit::TestCase(name)
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

	pTT->reset();
	assertTrue (pTT->progress() == 0);
	assertTrue (pTT->state() == Task::TASK_IDLE);
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
	while (pTT->state() != Task::TASK_RUNNING)
		Thread::sleep(50);
	assertTrue (pTT->progress() == 0);
	pTT->cancel();
	assertTrue (pTT->state() == Task::TASK_CANCELLING);
	pTT->cont();
	thr.join();
	assertTrue (pTT->state() == Task::TASK_FINISHED);
}


void TaskTest::testCancelNoStart()
{
	AutoPtr<TestTask> pTT = new TestTask;
	assertTrue (pTT->state() == Task::TASK_IDLE);
	pTT->cancel();
	assertTrue (pTT->state() == Task::TASK_CANCELLING);
	Thread thr;
	thr.start(*pTT);
	while (pTT->state() != Task::TASK_FINISHED)
		Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_FINISHED);
	assertFalse (pTT->started());
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
	CppUnit_addTest(pSuite, TaskTest, testCancelNoStart);

	return pSuite;
}
