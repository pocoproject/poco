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

	return pSuite;
}
