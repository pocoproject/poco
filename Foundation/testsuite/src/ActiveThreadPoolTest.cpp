//
// ActiveThreadPoolTest.cpp
//
// Copyright (c) 2004-2023, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ActiveThreadPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ActiveThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Environment.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


using Poco::ActiveThreadPool;
using Poco::RunnableAdapter;
using Poco::Thread;
using Poco::Environment;
using Poco::Runnable;
using Poco::RefCountedObject;
using Poco::AutoPtr;


namespace
{
	class TestPriorityRunnable: public Runnable, public RefCountedObject
	{
	public:
		using Ptr = AutoPtr<TestPriorityRunnable>;

		TestPriorityRunnable(int n, Poco::FastMutex& mutex, std::vector<int>& result):
			_n(n),
			_mutex(mutex),
			_result(result)
		{}

		virtual void run() override
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_result.push_back(_n);
		}

	private:
		int _n;
		Poco::FastMutex& _mutex;
		std::vector<int>& _result;
	};
}


ActiveThreadPoolTest::ActiveThreadPoolTest(const std::string& name): CppUnit::TestCase(name)
{
}


ActiveThreadPoolTest::~ActiveThreadPoolTest()
{
}


void ActiveThreadPoolTest::testActiveThreadPool1()
{
	ActiveThreadPool pool;
	assertTrue (pool.capacity() == static_cast<int>(Environment::processorCount()) + 1);
	RunnableAdapter<ActiveThreadPoolTest> ra(*this, &ActiveThreadPoolTest::count);

	_count = 0;
	try
	{
		for (int i = 0; i < 2000; ++i)
		{
			pool.start(ra);
		}
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}
	pool.joinAll();
	assertTrue (_count == 2000);

	_count = 0;
	try
	{
		for (int i = 0; i < 1000; ++i)
		{
			pool.start(ra);
		}
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}
	pool.joinAll();
	assertTrue (_count == 1000);
}


void ActiveThreadPoolTest::testActiveThreadPool2()
{
	ActiveThreadPool pool;
	RunnableAdapter<ActiveThreadPoolTest> ra(*this, &ActiveThreadPoolTest::count);

	pool.setExpiryTimeout(10);
	assertTrue (pool.expiryTimeout() == 10);

	_count = 0;
	try
	{
		for (int i = 0; i < pool.capacity(); ++i)
		{
			pool.start(ra);
		}
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}

	// wait for the threads to expire
	Thread::sleep(pool.expiryTimeout() * pool.capacity());

	try
	{
		for (int i = 0; i < pool.capacity(); ++i)
		{
			pool.start(ra); // reuse expired threads
		}
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}

	// wait for the threads to expire
	Thread::sleep(pool.expiryTimeout() * pool.capacity());
	pool.joinAll(); // join with no active threads
	assertTrue (_count == pool.capacity() * 2);
}

void ActiveThreadPoolTest::testActiveThreadPool3()
{
	Poco::FastMutex mutex;
	std::vector<int> result;
	ActiveThreadPool pool(1);
	std::vector<TestPriorityRunnable::Ptr> runnables;

	mutex.lock(); // lock, to make sure runnables are queued
	try
	{
		for (int priority = 0; priority < 1000; ++priority)
		{
			TestPriorityRunnable::Ptr r = new TestPriorityRunnable(priority, mutex, result);
			runnables.push_back(r);
			pool.start(*r, priority);
		}
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}
	mutex.unlock(); // unlock, to let runnables go

	pool.joinAll();
	std::vector<int> mock;
	mock.push_back(0); // 0 is the first result
	for (int i = 999; i > 0; --i)
	{
		mock.push_back(i); // other results should sort by priority
	}

	assertTrue (std::equal(result.begin(), result.end(), mock.begin(), mock.end()));
}

void ActiveThreadPoolTest::setUp()
{
	_count = 0;
}


void ActiveThreadPoolTest::tearDown()
{
}


void ActiveThreadPoolTest::count()
{
	++_count;
}


CppUnit::Test* ActiveThreadPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ActiveThreadPoolTest");

	CppUnit_addTest(pSuite, ActiveThreadPoolTest, testActiveThreadPool1);
	CppUnit_addTest(pSuite, ActiveThreadPoolTest, testActiveThreadPool2);
	CppUnit_addTest(pSuite, ActiveThreadPoolTest, testActiveThreadPool3);

	return pSuite;
}
