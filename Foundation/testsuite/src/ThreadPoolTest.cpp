//
// ThreadPoolTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ThreadPoolTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/ThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"


using Poco::Event;
using Poco::ThreadPool;
using Poco::RunnableAdapter;
using Poco::Thread;


ThreadPoolTest::ThreadPoolTest(const std::string& rName): CppUnit::TestCase(rName), _event(Event::EVENT_MANUALRESET)
{
}


ThreadPoolTest::~ThreadPoolTest()
{
}


void ThreadPoolTest::startThreadPoolTest(int affinityPolicy)
{
	int cpu = -1;
	if (affinityPolicy == static_cast<int>(ThreadPool::TAP_CUSTOM))
	{
		cpu = 0;
	}

	ThreadPool pool(2, 3, 3, POCO_THREAD_STACK_SIZE, static_cast<ThreadPool::ThreadAffinityPolicy>(affinityPolicy));
	pool.setStackSize(1);

	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 0);
	assertTrue (pool.capacity() == 3);
	assertTrue (pool.available() == 3);
	pool.addCapacity(1);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 0);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 4);

	RunnableAdapter<ThreadPoolTest> ra(*this, &ThreadPoolTest::count);
	pool.start(ra, cpu);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 1);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 3);

	pool.start(ra, cpu);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 2);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 2);

	pool.start(ra, cpu);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.used() == 3);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 1);

	pool.start(ra, cpu);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.used() == 4);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 0);

	try
	{
		pool.start(ra, cpu);
		failmsg("thread pool exhausted - must throw exception");
	}	
	catch (Poco::NoThreadAvailableException&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception thrown");
	}
	
	_event.set(); // go!!!
	pool.joinAll();
	
	assertTrue (_count == 40000);
	
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.used() == 0);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 4);
	
	Thread::sleep(4000);

	pool.collect();
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 0);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 4);
	
	_count = 0;
	_event.reset();
	pool.start(ra, cpu);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 1);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 3);

	pool.start(ra, cpu);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 2);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 2);
	_event.set(); // go!!!
	pool.joinAll();

	assertTrue (_count == 20000);
	
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 0);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 4);	
}


void ThreadPoolTest::testThreadPool()
{
	startThreadPoolTest(Poco::ThreadPool::TAP_DEFAULT);
}


void ThreadPoolTest::testThreadPoolUniformDistribution()
{
	startThreadPoolTest(Poco::ThreadPool::TAP_UNIFORM_DISTRIBUTION);
}


void ThreadPoolTest::testThreadPoolCustomDistribution()
{
	startThreadPoolTest(Poco::ThreadPool::TAP_CUSTOM);
}


void ThreadPoolTest::setUp()
{
	_event.reset();
	_count = 0;
}


void ThreadPoolTest::tearDown()
{
}


void ThreadPoolTest::count()
{
	_event.wait();
	for (int i = 0; i < 10000; ++i)
	{
		_mutex.lock();
		++_count;
		_mutex.unlock();
	}
}


CppUnit::Test* ThreadPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ThreadPoolTest");

	CppUnit_addTest(pSuite, ThreadPoolTest, testThreadPool);
	CppUnit_addTest(pSuite, ThreadPoolTest, testThreadPoolUniformDistribution);
	CppUnit_addTest(pSuite, ThreadPoolTest, testThreadPoolCustomDistribution);

	return pSuite;
}
