//
// NamedMutexTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NamedMutexTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/NamedMutex.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Timestamp.h"


using Poco::NamedMutex;
using Poco::Thread;
using Poco::Runnable;
using Poco::Timestamp;

#if POCO != POCO_OS_CYGWIN

static NamedMutex testMutex("TestMutex");

namespace
{
	class TestLock: public Runnable
	{
	public:
		void run()
		{
		
			testMutex.lock();
			_timestamp.update();
			testMutex.unlock();
		}

		const Timestamp& timestamp() const
		{
			return _timestamp;
		}

	private:
		Timestamp _timestamp;
	};

	class TestTryLock: public Runnable
	{
	public:
		TestTryLock(): _locked(false)
		{
		}
		
		void run()
		{
			if (testMutex.tryLock())
			{
				_locked = true;
				testMutex.unlock();
			}
		}

		bool locked() const
		{
			return _locked;
		}

	private:
		bool _locked;
	};
}


NamedMutexTest::NamedMutexTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


NamedMutexTest::~NamedMutexTest()
{
}


void NamedMutexTest::testLock()
{
	try {
		testMutex.lock();
		Thread thr;
		TestLock tl;
		thr.start(tl);
		Timestamp now;
		Thread::sleep(2000);
		testMutex.unlock();
		thr.join();
		assertTrue (tl.timestamp() > now);
	}
	catch(Poco::NotImplementedException e)
	{
#if POCO_OS != POCO_OS_ANDROID
		throw e;
#endif
	}
}


void NamedMutexTest::testTryLock()
{
	Thread thr1;
	TestTryLock ttl1;
	thr1.start(ttl1);
	thr1.join();
#if POCO_OS != POCO_OS_ANDROID
	assertTrue (ttl1.locked());
#endif
	try {
		testMutex.lock();
		Thread thr2;
		TestTryLock ttl2;
		thr2.start(ttl2);
		thr2.join();
		testMutex.unlock();
		assertTrue (!ttl2.locked());
	}
	catch(Poco::NotImplementedException e)
	{
#if POCO_OS != POCO_OS_ANDROID
		throw e;
#endif
	}
}
#endif

void NamedMutexTest::setUp()
{
}


void NamedMutexTest::tearDown()
{
}


CppUnit::Test* NamedMutexTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NamedMutexTest");

#if POCO != POCO_OS_CYGWIN
	CppUnit_addTest(pSuite, NamedMutexTest, testLock);
	CppUnit_addTest(pSuite, NamedMutexTest, testTryLock);
#endif

	return pSuite;
}
