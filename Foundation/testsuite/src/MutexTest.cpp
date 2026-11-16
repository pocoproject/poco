//
// MutexTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MutexTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include <type_traits>


using Poco::Mutex;
using Poco::FastMutex;
using Poco::ScopedLockWithUnlock;
using Poco::Thread;
using Poco::Runnable;
using Poco::Event;


namespace
{
	constexpr long TRY_LOCK_TIMEOUT_MILLISECONDS = 20;


	class Holder: public Runnable
		/// Holds the mutex until released, so the main thread can observe a failed tryLock.
	{
	public:
		Holder(FastMutex& mutex): _mutex(mutex)
		{
		}

		void run() override
		{
			_mutex.lock();
			_locked.set();
			_release.wait();
			_mutex.unlock();
		}

		void waitLocked()
		{
			_locked.wait();
		}

		void release()
		{
			_release.set();
		}

	private:
		FastMutex& _mutex;
		Event _locked;
		Event _release;
	};


	template <typename M>
	class TryLocker: public Runnable
	{
	public:
		static_assert(std::is_same_v<M, Mutex> || std::is_same_v<M, FastMutex>, "TryLocker requires Poco::Mutex or Poco::FastMutex");

		explicit TryLocker(M& mutex): _mutex(mutex)
		{
		}

		void run() override
		{
			_locked = _mutex.tryLock();
			if (_locked) _mutex.unlock();
		}

		bool locked() const
		{
			return _locked;
		}

	private:
		M& _mutex;
		bool _locked = false;
	};


	template <typename M>
	[[nodiscard]] bool tryLockFromOtherThread(M& mutex)
		/// A recursive Mutex can be locked again by its owner, and std::mutex::try_lock()
		/// by the owner is undefined, so a held lock is observed from another thread.
	{
		TryLocker<M> locker(mutex);
		Thread thread;
		thread.start(locker);
		thread.join();
		return locker.locked();
	}
}


MutexTest::MutexTest(const std::string& name): CppUnit::TestCase(name)
{
}


MutexTest::~MutexTest()
{
}


void MutexTest::testScopedLockWithUnlockTryLock()
{
	FastMutex mutex;

	{
		ScopedLockWithUnlock<FastMutex> lock(mutex, std::defer_lock);
		assertFalse (lock.ownsLock());

		assertTrue (lock.tryLock());
		assertTrue (lock.ownsLock());
		assertFalse (tryLockFromOtherThread(mutex));

		lock.unlock();
		assertFalse (lock.ownsLock());
		assertTrue (tryLockFromOtherThread(mutex));
	}

	{
		Holder holder(mutex);
		Thread thread;
		thread.start(holder);
		holder.waitLocked();

		ScopedLockWithUnlock<FastMutex> lock(mutex, std::defer_lock);
		assertFalse (lock.tryLock());
		assertFalse (lock.ownsLock());

		holder.release();
		thread.join();

		assertTrue (lock.tryLock());
		assertTrue (lock.ownsLock());
	}

	assertTrue (tryLockFromOtherThread(mutex));
}


void MutexTest::testFastMutexNotRecursive()
{
	FastMutex mutex;
	mutex.lock();
#ifndef POCO_ENABLE_STD_MUTEX
	assertFalse (mutex.tryLock());
	assertFalse (mutex.tryLock(TRY_LOCK_TIMEOUT_MILLISECONDS));
#endif
	assertFalse (tryLockFromOtherThread(mutex));
	mutex.unlock();

	assertTrue (mutex.tryLock());
	mutex.unlock();
	assertTrue (tryLockFromOtherThread(mutex));
}


void MutexTest::testMutexRecursive()
{
	Mutex mutex;
	mutex.lock();
	assertTrue (mutex.tryLock());
	assertTrue (mutex.tryLock(TRY_LOCK_TIMEOUT_MILLISECONDS));

	mutex.unlock();
	mutex.unlock();
	assertFalse (tryLockFromOtherThread(mutex));

	mutex.unlock();
	assertTrue (tryLockFromOtherThread(mutex));
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

	CppUnit_addTest(pSuite, MutexTest, testScopedLockWithUnlockTryLock);
	CppUnit_addTest(pSuite, MutexTest, testFastMutexNotRecursive);
	CppUnit_addTest(pSuite, MutexTest, testMutexRecursive);

	return pSuite;
}
