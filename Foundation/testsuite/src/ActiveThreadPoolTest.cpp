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
#include <vector>


using Poco::ActiveThreadPool;
using Poco::RunnableAdapter;
using Poco::Thread;
using Poco::Environment;


ActiveThreadPoolTest::ActiveThreadPoolTest(const std::string& name): CppUnit::TestCase(name)
{
}


ActiveThreadPoolTest::~ActiveThreadPoolTest()
{
}


void ActiveThreadPoolTest::testActiveThreadPool()
{
	ActiveThreadPool pool;

	assertTrue (pool.capacity() == static_cast<int>(Environment::processorCount()) + 1);

	RunnableAdapter<ActiveThreadPoolTest> ra(*this, &ActiveThreadPoolTest::count);

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


void ActiveThreadPoolTest::testActiveThreadLoadBalancing()
{
	Poco::AtomicCounter lttCount;
	ptrdiff_t lttPerTIDCount = 0;
	Poco::FastMutex mutex;
	class LongTimeTask : public Poco::Runnable
	{
		Poco::AtomicCounter &_counter;
		ptrdiff_t &_tidCounter;
		Poco::FastMutex &_mutex;
	public:
		LongTimeTask(Poco::AtomicCounter &counter, ptrdiff_t &tidCounter, Poco::FastMutex &mutex) :
			_counter(counter),
			_tidCounter(tidCounter),
			_mutex(mutex)
		{}
		void run() override
		{
			_counter++;
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				if (_tidCounter >= 0)
				{
					_tidCounter -= (ptrdiff_t)Poco::Thread::currentTid();
				}
				else
				{
					_tidCounter += (ptrdiff_t)Poco::Thread::currentTid();
				}
			}
			Poco::Thread::sleep(1 * 110);
		}
	};
	
	Poco::AtomicCounter sttCount;
	class ShortTimeTask : public Poco::Runnable
	{
		Poco::AtomicCounter &_counter;
	public:
		ShortTimeTask(Poco::AtomicCounter &counter) : _counter(counter) {}
		void run() override
		{
			_counter++;
			Poco::Thread::sleep(1);
		}
	};
	
	const int capacity = 2;
	const int taskCount = 100;
	const bool redistributeTasks = true;
	Poco::ActiveThreadPool pool(capacity, POCO_THREAD_STACK_SIZE, redistributeTasks);
	std::vector<std::unique_ptr<LongTimeTask>> lttVec(taskCount);
	std::vector<std::unique_ptr<ShortTimeTask>> sttVec(taskCount);
	for (int i = 0; i < taskCount; i++) {
		lttVec[i] = std::make_unique<LongTimeTask>(lttCount, lttPerTIDCount, mutex);
		pool.start(*(lttVec[i]));
		sttVec[i] = std::make_unique<ShortTimeTask>(sttCount);
		pool.start(*(sttVec[i]));
	}
	
	pool.joinAll();
	assertEqual(taskCount, lttCount.value());
	assertEqual(taskCount, sttCount.value());
	assertTrue(lttPerTIDCount != 0); // without optimization all tasks runs on single thread and this counter equal to 0, othrewise - no
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

	CppUnit_addTest(pSuite, ActiveThreadPoolTest, testActiveThreadPool);
	CppUnit_addTest(pSuite, ActiveThreadPoolTest, testActiveThreadLoadBalancing);

	return pSuite;
}
