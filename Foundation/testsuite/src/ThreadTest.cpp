//
// ThreadTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ThreadTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadTarget.h"
#include "Poco/Event.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#if defined(__sun) && defined(__SVR4) && !defined(__EXTENSIONS__)
#define __EXTENSIONS__
#endif
#include <climits>
#include <iostream>


using Poco::Thread;
using Poco::Runnable;
using Poco::ThreadTarget;
using Poco::Event;


class MyRunnable: public Runnable
{
public:
	MyRunnable(): _ran(false)
	{
	}

	void run()
	{
		Thread* pThread = Thread::current();
		if (pThread)
		{
			_threadName = pThread->name();
			auto *pThreadImpl = reinterpret_cast<Poco::ThreadImpl *>(pThread);
			_osThreadName = pThreadImpl->getOSThreadNameImpl();
		}
		_ran = true;
		_event.wait();
	}

	bool ran() const
	{
		return _ran;
	}

	const std::string& threadName() const
	{
		return _threadName;
	}

	const std::string& osThreadName() const
	{
		return _osThreadName;
	}

	void notify()
	{
		_event.set();
	}

	static void staticFunc()
	{
		++_staticVar;
	}

	static int _staticVar;

private:
	bool _ran;
	std::string _threadName;
	std::string _osThreadName;
	Event _event;
};


int MyRunnable::_staticVar = 0;


void freeFunc()
{
	++MyRunnable::_staticVar;
}


void freeFunc(void* pData)
{
	MyRunnable::_staticVar += *reinterpret_cast<int*>(pData);
}


class NonJoinRunnable : public Runnable
{
public:
	NonJoinRunnable() : _finished(false)
	{
	}

	void run()
	{
		_finished = true;
	}

	bool finished() const
	{
		return _finished;
	}

private:
	std::atomic<bool> _finished;
};


class TrySleepRunnable : public Runnable
{
public:
	TrySleepRunnable() : _counter(0), _sleepy(true)
	{
	}

	void run()
	{
		_sleepy = !Thread::trySleep(300000);
		++_counter;
		_sleepy = !Thread::trySleep(300000);
		++_counter;
		_sleepy = !Thread::trySleep(100);
		++_counter;
	}

	int counter() const
	{
		return _counter;
	}

	bool isSleepy() const
	{
		return _sleepy;
	}

private:
	std::atomic<int> _counter;
	std::atomic<bool> _sleepy;
};


ThreadTest::ThreadTest(const std::string& name): CppUnit::TestCase(name)
{
}


ThreadTest::~ThreadTest()
{
}


void ThreadTest::testThread()
{
	Thread thread;
	MyRunnable r;
	assertTrue (!thread.isRunning());
	thread.start(r);
	Thread::sleep(200);
	assertTrue (thread.isRunning());
	r.notify();
	thread.join();
	assertTrue (!thread.isRunning());
	assertTrue (r.ran());
	assertTrue (!r.threadName().empty());
	assertTrue (!r.osThreadName().empty());
}


void ThreadTest::testNamedThread()
{
	Thread thread("MyThread");
	MyRunnable r;
	thread.start(r);
	r.notify();
	thread.join();
	assertTrue (r.ran());
	assertTrue (r.threadName() == "MyThread");
	assertTrue (r.osThreadName() == r.threadName());

	// name len > POCO_MAX_THREAD_NAME_LEN
	Thread thread2("0123456789aaaaaaaaaa9876543210");
	MyRunnable r2;
	thread2.start(r2);
	r2.notify();
	thread2.join();
	assertTrue (r2.ran());
	assertTrue (r2.osThreadName() == r2.threadName());
	assertTrue (r2.threadName().length() <= POCO_MAX_THREAD_NAME_LEN);
	assertTrue (std::string(r2.threadName(), 0, 7) == "0123456");
	assertTrue (std::string(r2.threadName(), r2.threadName().size() - 7) == "6543210");
}


void ThreadTest::testCurrent()
{
	assertNullPtr (Thread::current());
}


void ThreadTest::testThreads()
{
	Thread thread1("Thread1");
	Thread thread2("Thread2");
	Thread thread3("Thread3");
	Thread thread4("Thread4");

	MyRunnable r1;
	MyRunnable r2;
	MyRunnable r3;
	MyRunnable r4;
	assertTrue (!thread1.isRunning());
	assertTrue (!thread2.isRunning());
	assertTrue (!thread3.isRunning());
	assertTrue (!thread4.isRunning());
	thread1.start(r1);
	Thread::sleep(200);
	assertTrue (thread1.isRunning());
	assertTrue (!thread2.isRunning());
	assertTrue (!thread3.isRunning());
	assertTrue (!thread4.isRunning());
	thread2.start(r2);
	thread3.start(r3);
	thread4.start(r4);
	Thread::sleep(200);
	assertTrue (thread1.isRunning());
	assertTrue (thread2.isRunning());
	assertTrue (thread3.isRunning());
	assertTrue (thread4.isRunning());
	r4.notify();
	thread4.join();
	assertTrue (!thread4.isRunning());
	assertTrue (thread1.isRunning());
	assertTrue (thread2.isRunning());
	assertTrue (thread3.isRunning());
	r3.notify();
	thread3.join();
	assertTrue (!thread3.isRunning());
	r2.notify();
	thread2.join();
	assertTrue (!thread2.isRunning());
	r1.notify();
	thread1.join();
	assertTrue (!thread1.isRunning());
	assertTrue (r1.ran());
	assertTrue (r1.threadName() == "Thread1");
	assertTrue (r2.ran());
	assertTrue (r2.threadName() == "Thread2");
	assertTrue (r3.ran());
	assertTrue (r3.threadName() == "Thread3");
	assertTrue (r4.ran());
	assertTrue (r4.threadName() == "Thread4");
}


void ThreadTest::testJoin()
{
	Thread thread;
	MyRunnable r;
	assertTrue (!thread.isRunning());
	thread.start(r);
	Thread::sleep(200);
	assertTrue (thread.isRunning());
	assertTrue (!thread.tryJoin(100));
	r.notify();
	assertTrue (thread.tryJoin(500));
	assertTrue (!thread.isRunning());
}


void ThreadTest::testNotJoin()
{
	Thread thread;
	NonJoinRunnable r;
	thread.start(r);

	while (!r.finished())
	{
		Thread::sleep(10);
	}

	Thread::sleep(100);
	assertTrue (!thread.isRunning());
}


void ThreadTest::testTrySleep()
{
	Thread thread;
	TrySleepRunnable r;
	assertTrue (r.isSleepy());
	assertTrue (!thread.isRunning());
	assertTrue (r.counter() == 0);
	thread.start(r);
	assertTrue (thread.isRunning());
	assertTrue (r.counter() == 0);
	assertTrue (r.isSleepy());
	Thread::sleep(100);
	assertTrue (r.counter() == 0);
	assertTrue (r.isSleepy());
	thread.wakeUp(); Thread::sleep(10);
	assertTrue (r.counter() == 1);
	assertTrue (r.isSleepy());
	Thread::sleep(100);
	assertTrue (r.counter() == 1);
	thread.wakeUp(); Thread::sleep(10);
	assertTrue (r.counter() == 2);
	assertTrue (r.isSleepy());
	Thread::sleep(200);
	assertTrue (r.counter() == 3);
	assertTrue (!r.isSleepy());
	assertTrue (!thread.isRunning());
	thread.wakeUp();
	assertTrue (!thread.isRunning());
}


void ThreadTest::testNotRun()
{
	Thread thread;
}


void ThreadTest::testNotRunJoin()
{
	Thread thread;
	thread.join();
}


void ThreadTest::testThreadTarget()
{
	ThreadTarget te(&MyRunnable::staticFunc);
	Thread thread;

	assertTrue (!thread.isRunning());

	int tmp = MyRunnable::_staticVar;
	thread.start(te);
	thread.join();
	assertTrue (tmp + 1 == MyRunnable::_staticVar);

	ThreadTarget te1(freeFunc);
	assertTrue (!thread.isRunning());

	tmp = MyRunnable::_staticVar;
	thread.start(te1);
	thread.join();
	assertTrue (tmp + 1 == MyRunnable::_staticVar);
}


void ThreadTest::testThreadFunction()
{
	Thread thread;

	assertTrue (!thread.isRunning());

	MyRunnable::_staticVar = 0;
	int tmp = 1;
	thread.start(freeFunc, &tmp);
	thread.join();
	assertTrue (tmp == MyRunnable::_staticVar);

	assertTrue (!thread.isRunning());

	tmp = MyRunnable::_staticVar = 0;
	thread.start(freeFunc, &tmp);
	thread.join();
	assertTrue (0 == MyRunnable::_staticVar);
}


struct Functor
{
	void operator () ()
	{
		++MyRunnable::_staticVar;
	}
};


void ThreadTest::testThreadFunctor()
{
	Thread thread;

	assertTrue (!thread.isRunning());

	MyRunnable::_staticVar = 0;
	thread.startFunc(Functor());
	thread.join();
	assertTrue (1 == MyRunnable::_staticVar);

	assertTrue (!thread.isRunning());

	Thread thread2;

	assertTrue (!thread2.isRunning());

	MyRunnable::_staticVar = 0;
	thread.startFunc([] ()
	{
		MyRunnable::_staticVar++;
	});
	thread.join();
	assertTrue (1 == MyRunnable::_staticVar);

	assertTrue (!thread2.isRunning());
}


void ThreadTest::testThreadStackSize()
{
	int stackSize = 50000000;

	Thread thread;

	assertTrue (0 == thread.getStackSize());
	thread.setStackSize(stackSize);
	assertTrue (stackSize <= thread.getStackSize());
	MyRunnable::_staticVar = 0;
	int tmp = 1;
	thread.start(freeFunc, &tmp);
	thread.join();
	assertTrue (1 == MyRunnable::_staticVar);

	stackSize = 1;
	thread.setStackSize(stackSize);

#if defined(POCO_OS_FAMILY_BSD) // on BSD family, stack size is rounded
	#ifdef PTHREAD_STACK_MIN
		assertTrue (PTHREAD_STACK_MIN == thread.getStackSize());
	#else
		assertTrue (stackSize >= thread.getStackSize());
	#endif
#endif

// disabled on FreeBSD; segfaults due to stack overflow,
// possibly happens on other BSD OSes)
#if (POCO_OS == POCO_OS_FREE_BSD)
	{
		int tmp = MyRunnable::_staticVar;
		thread.start(freeFunc, &tmp);
		thread.join();
		assertTrue (tmp * 2 == MyRunnable::_staticVar);
		thread.setStackSize(0);
		assertTrue (0 == thread.getStackSize());
		tmp = MyRunnable::_staticVar;
		thread.start(freeFunc, &tmp);
		thread.join();
		assertTrue (tmp * 2 == MyRunnable::_staticVar);
	}
#endif
}


void ThreadTest::testSleep()
{
	Poco::Timestamp start;
	Thread::sleep(200);
	Poco::Timespan elapsed = start.elapsed();
	assertTrue (elapsed.totalMilliseconds() >= 190 && elapsed.totalMilliseconds() < 250);
}


void ThreadTest::testAffinity()
{
#if POCO_OS == POCO_OS_LINUX
	MyRunnable mr;
	Thread t;
	t.start(mr);
	assertTrue (t.setAffinity(0));
	assertEqual (t.getAffinity(), 0);
	mr.notify();
	t.join();
#else
	std::cout << "not implemented";
#endif
}


void ThreadTest::setUp()
{
}


void ThreadTest::tearDown()
{
}


CppUnit::Test* ThreadTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ThreadTest");

	CppUnit_addTest(pSuite, ThreadTest, testThread);
	CppUnit_addTest(pSuite, ThreadTest, testNamedThread);
	CppUnit_addTest(pSuite, ThreadTest, testCurrent);
	CppUnit_addTest(pSuite, ThreadTest, testThreads);
	CppUnit_addTest(pSuite, ThreadTest, testJoin);
	CppUnit_addTest(pSuite, ThreadTest, testNotJoin);
	CppUnit_addTest(pSuite, ThreadTest, testNotRun);
	CppUnit_addTest(pSuite, ThreadTest, testNotRunJoin);
	CppUnit_addTest(pSuite, ThreadTest, testTrySleep);
	CppUnit_addTest(pSuite, ThreadTest, testThreadTarget);
	CppUnit_addTest(pSuite, ThreadTest, testThreadFunction);
	CppUnit_addTest(pSuite, ThreadTest, testThreadFunctor);
	CppUnit_addTest(pSuite, ThreadTest, testThreadStackSize);
	CppUnit_addTest(pSuite, ThreadTest, testSleep);
	CppUnit_addTest(pSuite, ThreadTest, testAffinity);

	return pSuite;
}
