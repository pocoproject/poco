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


using Poco::FastMutex;
using Poco::ScopedLockWithUnlock;
using Poco::Thread;
using Poco::Runnable;
using Poco::Event;


namespace
{
	class Holder: public Runnable
		/// Holds the mutex until released, so the main thread can observe a failed tryLock.
	{
	public:
		Holder(FastMutex& mutex): _mutex(mutex)
		{
		}

		void run()
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


	class TryLocker: public Runnable
	{
	public:
		TryLocker(FastMutex& mutex): _mutex(mutex)
		{
		}

		void run()
		{
			_locked = _mutex.tryLock();
			if (_locked) _mutex.unlock();
		}

		bool locked() const
		{
			return _locked;
		}

	private:
		FastMutex& _mutex;
		bool _locked = false;
	};


	bool tryLockFromOtherThread(FastMutex& mutex)
		/// FastMutex is recursive on Windows, so a locked mutex can only be
		/// observed as locked from a thread that does not already own it.
	{
		TryLocker locker(mutex);
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

	return pSuite;
}
