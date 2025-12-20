//
// SpinlockMutexTest.cpp
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SpinlockMutexTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Mutex.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include <atomic>


using Poco::SpinlockMutex;
using Poco::Thread;
using Poco::Runnable;
using Poco::Timestamp;
using Poco::TimeoutException;


namespace
{
	class Counter: public Runnable
	{
	public:
		Counter(SpinlockMutex& mutex, int& count, int iterations):
			_mutex(mutex),
			_count(count),
			_iterations(iterations)
		{
		}

		void run()
		{
			for (int i = 0; i < _iterations; ++i)
			{
				SpinlockMutex::ScopedLock lock(_mutex);
				++_count;
			}
		}

	private:
		SpinlockMutex& _mutex;
		int& _count;
		int _iterations;
	};


	class LockHolder: public Runnable
	{
	public:
		LockHolder(SpinlockMutex& mutex, std::atomic<bool>& holding, std::atomic<bool>& release):
			_mutex(mutex),
			_holding(holding),
			_release(release)
		{
		}

		void run()
		{
			SpinlockMutex::ScopedLock lock(_mutex);
			_holding = true;
			// Hold lock until told to release
			while (!_release)
			{
				Thread::sleep(1);
			}
		}

	private:
		SpinlockMutex& _mutex;
		std::atomic<bool>& _holding;
		std::atomic<bool>& _release;
	};
}


SpinlockMutexTest::SpinlockMutexTest(const std::string& name): CppUnit::TestCase(name)
{
}


SpinlockMutexTest::~SpinlockMutexTest()
{
}


void SpinlockMutexTest::testLock()
{
	SpinlockMutex mutex;

	// Basic lock/unlock
	mutex.lock();
	mutex.unlock();

	// Scoped lock
	{
		SpinlockMutex::ScopedLock lock(mutex);
	}
}


void SpinlockMutexTest::testTryLock()
{
	SpinlockMutex mutex;

	// Should succeed on unlocked mutex
	assertTrue(mutex.tryLock());
	mutex.unlock();

	// Lock it
	mutex.lock();

	// tryLock on same thread should fail (not recursive)
	// Note: This tests the behavior, but we can't actually test
	// from the same thread since it would deadlock. We test from
	// another thread instead.

	mutex.unlock();
}


void SpinlockMutexTest::testLockTimeout()
{
	SpinlockMutex mutex;
	mutex.lock();

	// Start a thread that will try to lock with timeout
	class TimeoutRunner: public Runnable
	{
	public:
		TimeoutRunner(SpinlockMutex& m): _mutex(m), _timedOut(false) {}
		void run()
		{
			try
			{
				_mutex.lock(50);  // 50ms timeout
				_mutex.unlock();
			}
			catch (TimeoutException&)
			{
				_timedOut = true;
			}
		}
		bool timedOut() const { return _timedOut; }
	private:
		SpinlockMutex& _mutex;
		bool _timedOut;
	};

	TimeoutRunner runner(mutex);
	Thread thread;
	thread.start(runner);
	thread.join();

	assertTrue(runner.timedOut());

	mutex.unlock();
}


void SpinlockMutexTest::testTryLockTimeout()
{
	SpinlockMutex mutex;
	mutex.lock();

	class TryLockRunner: public Runnable
	{
	public:
		TryLockRunner(SpinlockMutex& m): _mutex(m), _acquired(false) {}
		void run()
		{
			_acquired = _mutex.tryLock(50);  // 50ms timeout
			if (_acquired) _mutex.unlock();
		}
		bool acquired() const { return _acquired; }
	private:
		SpinlockMutex& _mutex;
		bool _acquired;
	};

	TryLockRunner runner(mutex);
	Thread thread;
	thread.start(runner);
	thread.join();

	assertFalse(runner.acquired());

	mutex.unlock();

	// Now it should succeed
	TryLockRunner runner2(mutex);
	Thread thread2;
	thread2.start(runner2);
	thread2.join();

	assertTrue(runner2.acquired());
}


void SpinlockMutexTest::testAdaptiveSpinning()
{
	// Test that the adaptive spinning works correctly by having
	// another thread hold the lock while we try to acquire it.
	// This verifies the spin-yield-sleep phases don't deadlock.
	SpinlockMutex mutex;
	std::atomic<bool> holding(false);
	std::atomic<bool> release(false);

	LockHolder holder(mutex, holding, release);
	Thread thread;
	thread.start(holder);

	// Wait for holder to acquire the lock
	while (!holding) Thread::sleep(1);

	// Now try to acquire lock with timeout - should fail since holder has it
	Timestamp start;
	bool acquired = mutex.tryLock(50);  // 50ms timeout
	Timestamp::TimeDiff elapsed = start.elapsed();

	// Should not have acquired (holder still has it)
	assertFalse(acquired);
	// Should have taken close to timeout (at least 40ms)
	assertTrue(elapsed >= 40000);

	// Tell holder to release and try again
	release = true;
	thread.join();

	// Now we should be able to acquire
	assertTrue(mutex.tryLock());
	mutex.unlock();
}


void SpinlockMutexTest::testContention()
{
	SpinlockMutex mutex;
	int count = 0;
	const int iterations = 10000;
	const int numThreads = 4;

	std::vector<Thread> threads(numThreads);
	std::vector<Counter*> counters;

	for (int i = 0; i < numThreads; ++i)
	{
		counters.push_back(new Counter(mutex, count, iterations));
	}

	for (int i = 0; i < numThreads; ++i)
	{
		threads[i].start(*counters[i]);
	}

	for (int i = 0; i < numThreads; ++i)
	{
		threads[i].join();
	}

	assertEqual(numThreads * iterations, count);

	for (auto* c : counters) delete c;
}


void SpinlockMutexTest::setUp()
{
}


void SpinlockMutexTest::tearDown()
{
}


CppUnit::Test* SpinlockMutexTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SpinlockMutexTest");

	CppUnit_addTest(pSuite, SpinlockMutexTest, testLock);
	CppUnit_addTest(pSuite, SpinlockMutexTest, testTryLock);
	CppUnit_addTest(pSuite, SpinlockMutexTest, testLockTimeout);
	CppUnit_addTest(pSuite, SpinlockMutexTest, testTryLockTimeout);
	CppUnit_addTest(pSuite, SpinlockMutexTest, testAdaptiveSpinning);
	CppUnit_addTest(pSuite, SpinlockMutexTest, testContention);

	return pSuite;
}
