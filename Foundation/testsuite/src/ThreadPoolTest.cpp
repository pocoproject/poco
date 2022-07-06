//
// ThreadPoolTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ThreadPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"


using Poco::ThreadPool;
using Poco::RunnableAdapter;
using Poco::Thread;


ThreadPoolTest::ThreadPoolTest(const std::string& name): CppUnit::TestCase(name), _event(false)
{
}


ThreadPoolTest::~ThreadPoolTest()
{
}


void ThreadPoolTest::testThreadPool()
{
	ThreadPool pool(2, 3, 3);
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
	pool.start(ra);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 1);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 3);

	pool.start(ra);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 2);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 2);

	pool.start(ra);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.used() == 3);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 1);

	pool.start(ra);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.used() == 4);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 0);

	try
	{
		pool.start(ra);
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
	pool.start(ra);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.used() == 1);
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.available() == 3);

	pool.start(ra);
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

	return pSuite;
}
