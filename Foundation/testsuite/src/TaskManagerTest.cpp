//
// TaskManagerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TaskManagerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/TaskManager.h"
#include "Poco/Task.h"
#include "Poco/TaskNotification.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Event.h"
#include "Poco/Observer.h"
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"
#include <iostream>


using Poco::TaskManager;
using Poco::Task;
using Poco::NotificationCenter;
using Poco::TaskStartedNotification;
using Poco::TaskCancelledNotification;
using Poco::TaskFinishedNotification;
using Poco::TaskFailedNotification;
using Poco::TaskProgressNotification;
using Poco::TaskCustomNotification;
using Poco::Thread;
using Poco::ThreadPool;
using Poco::Event;
using Poco::Observer;
using Poco::Exception;
using Poco::NoThreadAvailableException;
using Poco::SystemException;
using Poco::NullPointerException;
using Poco::AutoPtr;


namespace
{
	class TestTask: public Task
	{
	public:
		TestTask():
			Task("TestTask"),
			_fail(false),
			_started(false)
		{
		}

		void runTask()
		{
			_started = true;
			_event.wait();
			setProgress(0.5);
			_event.wait();
			if (isCancelled())
				return;
			if (_fail)
				throw SystemException("warp core breach detected");
			setProgress(1.0);
			_event.wait();
		}

		void fail()
		{
			_fail = true;
		}

		void cont()
		{
			_event.set();
		}

		bool started() const
		{
			return _started;
		}

	private:
		Event _event;
		std::atomic<bool> _fail;
		std::atomic<bool> _started;
	};

	class SimpleTask: public Task
	{
	public:
		SimpleTask(): Task("SimpleTask")
		{
		}

		void runTask()
		{
			sleep(10000);
		}
	};

	class TaskObserver
	{
	public:
		TaskObserver():
			_started(false),
			_cancelled(false),
			_finished(false),
			_pException(0),
			_progress(0.0)
		{
		}

		~TaskObserver()
		{
			delete _pException;
		}

		void taskStarted(TaskStartedNotification* pNf)
		{
			_started = true;
			pNf->release();
		}

		void taskCancelled(TaskCancelledNotification* pNf)
		{
			_cancelled = true;
			pNf->release();
		}

		void taskFinished(TaskFinishedNotification* pNf)
		{
			_finished = true;
			pNf->release();
		}

		void taskFailed(TaskFailedNotification* pNf)
		{
			_pException = pNf->reason().clone();
			pNf->release();
		}

		void taskProgress(TaskProgressNotification* pNf)
		{
			_progress = pNf->progress();
			pNf->release();
		}

		bool started() const
		{
			return _started;
		}

		bool cancelled() const
		{
			return _cancelled;
		}

		bool finished() const
		{
			return _finished;
		}

		float progress() const
		{
			return _progress;
		}

		Exception* error() const
		{
			return _pException;
		}

	private:
		std::atomic<bool>       _started;
		std::atomic<bool>       _cancelled;
		std::atomic<bool>       _finished;
		std::atomic<Exception*> _pException;
		std::atomic<float>      _progress;
	};


	template <typename T>
	class CustomNotificationTask: public Task
	{
	public:
		CustomNotificationTask(const T& t):
			Task("CustomNotificationTask"),
			_custom(t)
		{
		}

		void runTask()
		{
			sleep(10000);
		}

		void setCustom(const T& custom)
		{
			_custom = custom;
			postNotification(new TaskCustomNotification<T>(this, _custom));
		}

	private:
		T _custom;
	};


	template <class C>
	class CustomTaskObserver
	{
	public:
		CustomTaskObserver(const C& custom): _custom(custom)
		{
		}

		~CustomTaskObserver()
		{
		}

		void taskCustom(TaskCustomNotification<C>* pNf)
		{
			_custom = pNf->custom();
			pNf->release();
		}

		const C& custom() const
		{
			return _custom;
		}

	private:
		C _custom;
	};
}


TaskManagerTest::TaskManagerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TaskManagerTest::~TaskManagerTest()
{
}


void TaskManagerTest::testFinish()
{
	TaskManager tm;
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	tm.start(pTT.duplicate());
	while (pTT->state() < Task::TASK_RUNNING) Thread::sleep(50);
	assertTrue (pTT->progress() == 0);
	Thread::sleep(200);
	pTT->cont();
	while (to.progress() == 0) Thread::sleep(50);
	assertTrue (to.progress() == 0.5);
	assertTrue (to.started());
	assertTrue (pTT->state() == Task::TASK_RUNNING);
	TaskManager::TaskList list = tm.taskList();
	assertTrue (list.size() == 1);
	assertTrue (tm.count() == 1);
	pTT->cont();
	while (pTT->progress() != 1.0) Thread::sleep(50);
	pTT->cont();
	while (pTT->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_FINISHED);
	while (!to.finished()) Thread::sleep(50);
	assertTrue (to.finished());
	while (tm.count() == 1) Thread::sleep(50);
	list = tm.taskList();
	assertTrue (list.empty());
	assertTrue (!to.error());
	tm.cancelAll();
	tm.joinAll();
	tm.removeObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.removeObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.removeObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.removeObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.removeObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
}


void TaskManagerTest::testCancel()
{
	TaskManager tm;
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	tm.start(pTT.duplicate());
	while (pTT->state() < Task::TASK_RUNNING) Thread::sleep(50);
	assertTrue (pTT->progress() == 0);
	Thread::sleep(200);
	pTT->cont();
	while (pTT->progress() != 0.5) Thread::sleep(50);
	assertTrue (to.progress() == 0.5);
	assertTrue (to.started());
	assertTrue (pTT->state() == Task::TASK_RUNNING);
	TaskManager::TaskList list = tm.taskList();
	assertTrue (list.size() == 1);
	assertTrue (tm.count() == 1);
	tm.cancelAll();
	while (pTT->state() != Task::TASK_CANCELLING) Thread::sleep(50);
	pTT->cont();
	assertTrue (to.cancelled());
	pTT->cont();
	while (pTT->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_FINISHED);
	while (!to.finished()) Thread::sleep(50);
	assertTrue (to.finished());
	while (tm.count() == 1) Thread::sleep(50);
	list = tm.taskList();
	assertTrue (list.empty());
	assertTrue (!to.error());
	tm.cancelAll();
	tm.joinAll();
	tm.removeObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.removeObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.removeObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.removeObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.removeObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
}


void TaskManagerTest::testError()
{
	TaskManager tm;
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	assertTrue (tm.start(pTT.duplicate()));
	while (pTT->state() < Task::TASK_RUNNING) Thread::sleep(50);
	assertTrue (pTT->progress() == 0);
	Thread::sleep(200);
	pTT->cont();
	while (pTT->progress() != 0.5) Thread::sleep(50);
	assertTrue (to.progress() == 0.5);
	assertTrue (to.started());
	assertTrue (pTT->state() == Task::TASK_RUNNING);
	TaskManager::TaskList list = tm.taskList();
	assertTrue (list.size() == 1);
	assertTrue (tm.count() == 1);
	pTT->fail();
	pTT->cont();
	while (pTT->state() != Task::TASK_FINISHED) Thread::sleep(50);
	pTT->cont();
	while (pTT->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_FINISHED);
	while (!to.finished()) Thread::sleep(50);
	assertTrue (to.finished());
	assertTrue (to.error() != 0);
	while (tm.count() == 1) Thread::sleep(50);
	list = tm.taskList();
	assertTrue (list.empty());
	tm.cancelAll();
	tm.joinAll();
	tm.removeObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.removeObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.removeObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.removeObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.removeObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
}


void TaskManagerTest::testCustom()
{
	TaskManager tm;

	CustomTaskObserver<int> ti(0);
	tm.addObserver(
		Observer<CustomTaskObserver<int>, TaskCustomNotification<int> >
			(ti, &CustomTaskObserver<int>::taskCustom));

	AutoPtr<CustomNotificationTask<int> > pCNT1 = new CustomNotificationTask<int>(0);
	tm.start(pCNT1.duplicate());
	assertTrue (ti.custom() == 0);

	for (int i = 1; i < 10; ++i)
	{
		pCNT1->setCustom(i);
		assertTrue (ti.custom() == i);
	}

	CustomTaskObserver<std::string> ts("");
	tm.addObserver(
		Observer<CustomTaskObserver<std::string>, TaskCustomNotification<std::string> >
			(ts, &CustomTaskObserver<std::string>::taskCustom));

	AutoPtr<CustomNotificationTask<std::string> > pCNT2 = new CustomNotificationTask<std::string>("");
	tm.start(pCNT2.duplicate());
	assertTrue (tm.taskList().size() == 2);
	assertTrue (ts.custom() == "");
	std::string str("notify me");
	pCNT2->setCustom(str);
	assertTrue (ts.custom() == str);

	S s;
	s.i = 0;
	s.str = "";

	CustomTaskObserver<S*> ptst(&s);

	tm.addObserver(
		Observer<CustomTaskObserver<S*>, TaskCustomNotification<S*> >
			(ptst, &CustomTaskObserver<S*>::taskCustom));

	AutoPtr<CustomNotificationTask<S*> > pCNT3 = new CustomNotificationTask<S*>(&s);
	tm.start(pCNT3.duplicate());
	assertTrue (tm.taskList().size() == 3);
	assertTrue (ptst.custom()->i == 0);
	assertTrue (ptst.custom()->str == "");
	s.i = 123;
	s.str = "123";
	pCNT3->setCustom(&s);
	assertTrue (ptst.custom()->i == 123);
	assertTrue (ptst.custom()->str == "123");

	s.i = 0;
	s.str = "";

	CustomTaskObserver<S> tst(s);

	tm.addObserver(
		Observer<CustomTaskObserver<S>, TaskCustomNotification<S> >
			(tst, &CustomTaskObserver<S>::taskCustom));

	AutoPtr<CustomNotificationTask<S> > pCNT4 = new CustomNotificationTask<S>(s);
	tm.start(pCNT4.duplicate());
	assertTrue (tm.taskList().size() == 4);
	assertTrue (tst.custom().i == 0);
	assertTrue (tst.custom().str == "");
	s.i = 123;
	s.str = "123";
	pCNT4->setCustom(s);
	assertTrue (tst.custom().i == 123);
	assertTrue (tst.custom().str == "123");

	AutoPtr<SimpleTask> pST = new SimpleTask;
	tm.start(pST.duplicate());
	assertTrue (tm.taskList().size() == 5);

	tm.cancelAll();
	while (tm.count() > 0) Thread::sleep(50);
	assertTrue (tm.count() == 0);
	tm.joinAll();
}


void TaskManagerTest::testCancelNoStart()
{
	TaskManager tm;
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	pTT->cancel();
	assertTrue (pTT->isCancelled());
	assertFalse(tm.start(pTT.duplicate()));
	assertTrue (pTT->progress() == 0);
	assertTrue (pTT->isCancelled());
	assertFalse (pTT->hasOwner());
	tm.removeObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.removeObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.removeObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.removeObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.removeObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
}


void TaskManagerTest::testMultiTasks()
{
	TaskManager tm;

	AutoPtr<SimpleTask> pTT1 = new SimpleTask;
	AutoPtr<SimpleTask> pTT2 = new SimpleTask;
	AutoPtr<SimpleTask> pTT3 = new SimpleTask;

	tm.start(pTT1.duplicate());
	tm.start(pTT2.duplicate());
	tm.start(pTT3.duplicate());

	assertTrue (pTT1->hasOwner());
	assertTrue (pTT2->hasOwner());
	assertTrue (pTT3->hasOwner());

	TaskManager::TaskList list = tm.taskList();
	assertTrue (list.size() == 3);

	tm.cancelAll();
	while (tm.count() > 0) Thread::sleep(100);
	assertTrue (tm.count() == 0);
	tm.joinAll();

	while (pTT1->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertFalse (pTT1->hasOwner());
	while (pTT2->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertFalse (pTT2->hasOwner());
	while (pTT3->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertFalse (pTT3->hasOwner());
}


void TaskManagerTest::testCustomThreadPool()
{
	ThreadPool  tp(2, 5, 120);
	TaskManager tm(tp);

	// fill up the thread pool
	for (int i=0; i < tp.capacity(); ++i)
	{
		tm.start(new SimpleTask);
	}
	assertTrue (tp.allocated() == tp.capacity());
	assertTrue (tm.count() == tp.allocated());

	// the next one should fail
	try
	{
		tm.start(new SimpleTask);
		failmsg("thread pool exhausted - must throw exception");
	}
	catch (NoThreadAvailableException const&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}

	assertTrue (tm.count() == tp.allocated());

	tm.cancelAll();
	tm.joinAll();
}

void TaskManagerTest::setUp()
{
}


void TaskManagerTest::tearDown()
{
}


CppUnit::Test* TaskManagerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TaskManagerTest");

	CppUnit_addTest(pSuite, TaskManagerTest, testFinish);
	CppUnit_addTest(pSuite, TaskManagerTest, testCancel);
	CppUnit_addTest(pSuite, TaskManagerTest, testError);
	CppUnit_addTest(pSuite, TaskManagerTest, testCancelNoStart);
	CppUnit_addTest(pSuite, TaskManagerTest, testMultiTasks);
	CppUnit_addTest(pSuite, TaskManagerTest, testCustom);
	CppUnit_addTest(pSuite, TaskManagerTest, testCustomThreadPool);

	return pSuite;
}
