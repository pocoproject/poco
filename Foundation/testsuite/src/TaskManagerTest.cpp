//
// TaskManagerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TaskManagerTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
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
			_fail(false)
		{
		}
		
		void runTask()
		{
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

	private:
		Event _event;
		bool  _fail;
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
	
	class IncludingTask: public Task
	{
	public:
		IncludingTask(): Task("IncludingTask")
		{
		}

		void runTask()
		{
			setProgress(0.5);
			getOwner()->startSync(new SimpleTask);
			setProgress(1.0);
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
		bool       _started;
		bool       _cancelled;
		bool       _finished;
		Exception* _pException;
		float      _progress;
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

	class SimpleTaskQueue
	{
	public:
		SimpleTaskQueue(TaskManager& tm): _tm(tm)
		{
			_tm.addObserver(Observer<SimpleTaskQueue, TaskFinishedNotification>(*this, &SimpleTaskQueue::taskFinished));
		}

		void enqueue(Task* pTask)
		{
			_tasks.push_back(pTask);
		}

		void startQueue()
		{
			if (_tm.count() == 0 && _tasks.size())
			{
				Task* pTask = _tasks.back();
				// not thread-safe
				_tasks.pop_back();
				_tm.start(pTask);
			}
		}

		void taskFinished(TaskFinishedNotification* pNf)
		{
			if (_tasks.size())
			{
				Task* pTask = _tasks.back();
				// not thread-safe
				_tasks.pop_back();
				_tm.startSync(pTask);
			}
			pNf->release();
		}

	private:
		std::vector<Task*> _tasks;
		TaskManager& _tm;
	};
}


TaskManagerTest::TaskManagerTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


TaskManagerTest::~TaskManagerTest()
{
}


void TaskManagerTest::testFinish()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	tm.start(pTT.duplicate());
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
}


void TaskManagerTest::testCancel()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	tm.start(pTT.duplicate());
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
	assertTrue (to.cancelled());
	pTT->cont();
	while (pTT->state() != Task::TASK_FINISHED) Thread::sleep(50);
	assertTrue (pTT->state() == Task::TASK_FINISHED);
	assertTrue (to.finished());
	while (tm.count() == 1) Thread::sleep(50);
	list = tm.taskList();
	assertTrue (list.empty());
	assertTrue (!to.error());
}


void TaskManagerTest::testError()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	TaskObserver to;
	tm.addObserver(Observer<TaskObserver, TaskStartedNotification>(to, &TaskObserver::taskStarted));
	tm.addObserver(Observer<TaskObserver, TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	tm.addObserver(Observer<TaskObserver, TaskFailedNotification>(to, &TaskObserver::taskFailed));
	tm.addObserver(Observer<TaskObserver, TaskFinishedNotification>(to, &TaskObserver::taskFinished));
	tm.addObserver(Observer<TaskObserver, TaskProgressNotification>(to, &TaskObserver::taskProgress));
	AutoPtr<TestTask> pTT = new TestTask;
	tm.start(pTT.duplicate());
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
	assertTrue (pTT->state() == Task::TASK_FINISHED);
	assertTrue (to.finished());
	assertTrue (to.error() != 0);
	while (tm.count() == 1) Thread::sleep(50);
	list = tm.taskList();
	assertTrue (list.empty());
}


void TaskManagerTest::testCustom()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	
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
}


void TaskManagerTest::testMultiTasks()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	tm.start(new SimpleTask);
	tm.start(new SimpleTask);
	tm.start(new SimpleTask);
	
	TaskManager::TaskList list = tm.taskList();
	assertTrue (list.size() == 3);
	
	tm.cancelAll();
	while (tm.count() > 0) Thread::sleep(100);
	assertTrue (tm.count() == 0);
}


void TaskManagerTest::testTaskInclusion()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	IncludingTask* pTask = new IncludingTask;

	pTask->duplicate();

	tm.start(pTask);
	// wait for the included task to be started
	while (pTask->progress() < 0.5)
	{
		Thread::sleep(100);
	}
	Thread::sleep(100);
	assertTrue (tm.count() == 2);

	tm.cancelAll();
	while (tm.count() > 0) Thread::sleep(100);
	assertTrue (tm.count() == 0);
}


void TaskManagerTest::testTaskQueue()
{
	TaskManager tm(ThreadPool::TAP_UNIFORM_DISTRIBUTION);
	SimpleTaskQueue tq(tm);

	Task* pT1 = new SimpleTask;
	Task* pT2 = new SimpleTask;
	Task* pT3 = new SimpleTask;
	tq.enqueue(pT1);
	tq.enqueue(pT2);
	tq.startQueue();

	assertTrue (tm.count() == 1);
	Thread::sleep(500);
	assertTrue (pT1->state() == Task::TASK_RUNNING);
	assertTrue (pT2->state() == Task::TASK_IDLE);

	tq.enqueue(pT3);
	pT1->cancel();
	Thread::sleep(500);
	assertTrue (tm.count() == 1);
	assertTrue (pT2->state() == Task::TASK_RUNNING);
	assertTrue (pT3->state() == Task::TASK_IDLE);

	pT2->cancel();
	Thread::sleep(500);
	assertTrue (tm.count() == 1);
	assertTrue (pT3->state() == Task::TASK_RUNNING);

	tm.cancelAll();
	while (tm.count() > 0) Thread::sleep(100);
	assertTrue (tm.count() == 0);
}


void TaskManagerTest::testCustomThreadPool()
{
	ThreadPool  tp(2, 5, 120, POCO_THREAD_STACK_SIZE, ThreadPool::TAP_UNIFORM_DISTRIBUTION);
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
	
	tp.joinAll();
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
	CppUnit_addTest(pSuite, TaskManagerTest, testMultiTasks);
	CppUnit_addTest(pSuite, TaskManagerTest, testCustom);
	CppUnit_addTest(pSuite, TaskManagerTest, testCustomThreadPool);

	return pSuite;
}
