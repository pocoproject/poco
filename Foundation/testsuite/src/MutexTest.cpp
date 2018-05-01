//
// MutexTest.cpp
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MutexTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"


using Poco::Mutex;
using Poco::Runnable;
using Poco::SystemException;
using Poco::Thread;
using Poco::Timestamp;

namespace
{
	class SelfDeadlockRunnable: public Runnable
	{
	public:
		SelfDeadlockRunnable():
			_ran(false)
		{
		}
		
		void run()
		{
			try
			{
				Mutex mtx(Mutex::MUTEX_NONRECURSIVE);
				mtx.lock();
				mtx.lock();
			}
			catch (...)
			{
			}

			_ran = true;
		}
		
		bool ran() const
		{
			return _ran;
		}
			
	private:
		bool _ran;
	};
}


MutexTest::MutexTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


MutexTest::~MutexTest()
{
}


void MutexTest::testMutexRecursion()
{
	Mutex mtx(Mutex::MUTEX_NONRECURSIVE);
	mtx.lock();
	
	bool success = mtx.tryLock();
	assertTrue (!success);
	
	Timestamp mark;
	success = mtx.tryLock(2000); // Wait 2 seconds
	assertTrue (!success);

	// Test that we waited approx. 2 sec
	Timestamp::TimeDiff elapsed = mark.elapsed();
	assertTrue (elapsed > 1000000);
	assertTrue (elapsed < 3000000);
	
	success = mtx.tryLock(0);
	assertTrue (!success);

	SelfDeadlockRunnable r;
	Thread t;

	t.start(r);
	success = t.tryJoin(2000);
	assertTrue (!success);
	assertTrue (!r.ran());
}


void MutexTest::testRecursiveMutexRecursion()
{
	Mutex mtx;
	mtx.lock();
	
	bool success = mtx.tryLock();
	assertTrue (success);
	
	Timestamp mark;
	success = mtx.tryLock(2000); // Wait 2 seconds
	assertTrue (success);

	// Test that we completed almost immediately
	Timestamp::TimeDiff elapsed = mark.elapsed();
	assertTrue (elapsed < 100000);
	
	success = mtx.tryLock(0);
	assertTrue (success);
}

void MutexTest::setUp()
{
}


void MutexTest::tearDown()
{
}


CppUnit::Test* MutexTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MutexTest");

	CppUnit_addTest(pSuite, MutexTest, testMutexRecursion);
	CppUnit_addTest(pSuite, MutexTest, testRecursiveMutexRecursion);

	return pSuite;
}
