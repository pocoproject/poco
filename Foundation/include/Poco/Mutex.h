//
// Mutex.h
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the Mutex and FastMutex classes.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Mutex_INCLUDED
#define Foundation_Mutex_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/ScopedLock.h"
#include "Poco/Timestamp.h"
#include <atomic>

#ifdef POCO_ENABLE_STD_MUTEX
#include "Poco/Mutex_STD.h"
#else
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/Mutex_WIN32.h"
#elif defined(POCO_VXWORKS)
#include "Poco/Mutex_VX.h"
#else
#include "Poco/Mutex_POSIX.h"
#endif
#endif


namespace Poco {


class Foundation_API Mutex: private MutexImpl
	/// A Mutex (mutual exclusion) is a synchronization
	/// mechanism used to control access to a shared resource
	/// in a concurrent (multithreaded) scenario.
	/// Mutexes are recursive, that is, the same mutex can be
	/// locked multiple times by the same thread (but, of course,
	/// not by other threads).
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	using ScopedLock = Poco::ScopedLock<Mutex>;
	using ScopedLockWithUnlock = Poco::ScopedLockWithUnlock<Mutex>;

	Mutex();
		/// creates the Mutex.

	~Mutex();
		/// destroys the Mutex.

	void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

	void lock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread. Throws a TimeoutException
		/// if the mutex can not be locked within the given timeout.
		///
		/// Performance Note: On most platforms (including Windows), this member function is
		/// implemented using a loop calling (the equivalent of) tryLock() and Thread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

	bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another thread.
		/// Returns true if the mutex was successfully locked.

	bool tryLock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread.
		/// Returns true if the mutex was successfully locked.
		///
		/// Performance Note: On most platforms (including Windows), this member function is
		/// implemented using a loop calling (the equivalent of) tryLock() and Thread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

	void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

private:
	Mutex(const Mutex&);
	Mutex& operator = (const Mutex&);
};


class Foundation_API FastMutex: private FastMutexImpl
	/// A FastMutex (mutual exclusion) is similar to a Mutex.
	/// Unlike a Mutex, however, a FastMutex is not recursive,
	/// which means that a deadlock will occur if the same
	/// thread tries to lock a mutex it has already locked again.
	/// Locking a FastMutex is faster than locking a recursive Mutex.
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	using ScopedLock = Poco::ScopedLock<FastMutex>;
	using ScopedLockWithUnlock = Poco::ScopedLockWithUnlock<FastMutex>;

	FastMutex();
		/// creates the Mutex.

	~FastMutex();
		/// destroys the Mutex.

	void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

	void lock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread. Throws a TimeoutException
		/// if the mutex can not be locked within the given timeout.
		///
		/// Performance Note: On most platforms (including Windows), this member function is
		/// implemented using a loop calling (the equivalent of) tryLock() and Thread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

	bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another thread.
		/// Returns true if the mutex was successfully locked.

	bool tryLock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread.
		/// Returns true if the mutex was successfully locked.
		///
		/// Performance Note: On most platforms (including Windows), this member function is
		/// implemented using a loop calling (the equivalent of) tryLock() and Thread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

	void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

private:
	FastMutex(const FastMutex&);
	FastMutex& operator = (const FastMutex&);
};


class Foundation_API SpinlockMutex
	/// A SpinlockMutex, implemented in terms of std::atomic_flag, as
	/// busy-wait mutual exclusion.
	///
	/// While in some cases (eg. locking small blocks of code)
	/// busy-waiting may be an optimal solution, in many scenarios
	/// spinlock may not be the right choice (especially on single-core
	/// systems) - it is up to the user to choose the proper mutex type
	/// for their particular case.
	///
	/// Works with the ScopedLock class.
{
public:
	using ScopedLock = Poco::ScopedLock<SpinlockMutex>;
	using ScopedLockWithUnlock = Poco::ScopedLockWithUnlock<SpinlockMutex>;

	SpinlockMutex();
		/// Creates the SpinlockMutex.

	~SpinlockMutex();
		/// Destroys the SpinlockMutex.

	void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

	void lock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread. Throws a TimeoutException
		/// if the mutex can not be locked within the given timeout.

	bool tryLock();
		/// Tries to lock the mutex. Returns immediately, false
		/// if the mutex is already held by another thread, true
		/// if the mutex was successfully locked.

	bool tryLock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread.
		/// Returns true if the mutex was successfully locked.

	void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

private:
	std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};


class Foundation_API NullMutex
	/// A NullMutex is an empty mutex implementation
	/// which performs no locking at all. Useful in policy driven design
	/// where the type of mutex used can be now a template parameter allowing the user to switch
	/// between thread-safe and not thread-safe depending on his need
	/// Works with the ScopedLock class
{
public:
	using ScopedLock = Poco::ScopedLock<NullMutex>;
	using ScopedLockWithUnlock = Poco::ScopedLockWithUnlock<NullMutex>;

	NullMutex() = default;
		/// Creates the NullMutex.

	~NullMutex() = default;
		/// Destroys the NullMutex.

	void lock()
		/// Does nothing.
	{
	}

	void lock(long)
		/// Does nothing.
	{
	}

	bool tryLock()
		/// Does nothing and always returns true.
	{
		return true;
	}

	bool tryLock(long)
		/// Does nothing and always returns true.
	{
		return true;
	}

	void unlock()
		/// Does nothing.
	{
	}
};


//
// inlines
//

//
// Mutex
//

inline void Mutex::lock()
{
	lockImpl();
}


inline void Mutex::lock(long milliseconds)
{
	if (!tryLockImpl(milliseconds))
		throw TimeoutException();
}


inline bool Mutex::tryLock()
{
	return tryLockImpl();
}


inline bool Mutex::tryLock(long milliseconds)
{
	return tryLockImpl(milliseconds);
}


inline void Mutex::unlock()
{
	unlockImpl();
}


//
// FastMutex
//

inline void FastMutex::lock()
{
	lockImpl();
}


inline void FastMutex::lock(long milliseconds)
{
	if (!tryLockImpl(milliseconds))
		throw TimeoutException();
}


inline bool FastMutex::tryLock()
{
	return tryLockImpl();
}


inline bool FastMutex::tryLock(long milliseconds)
{
	return tryLockImpl(milliseconds);
}


inline void FastMutex::unlock()
{
	unlockImpl();
}


//
// SpinlockMutex
//

inline void SpinlockMutex::lock()
{
	while (_flag.test_and_set(std::memory_order_acquire));
}


inline void SpinlockMutex::lock(long milliseconds)
{
	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
	while (_flag.test_and_set(std::memory_order_acquire))
	{
		if (now.isElapsed(diff)) throw TimeoutException();
	}
}


inline bool SpinlockMutex::tryLock()
{
	return !_flag.test_and_set(std::memory_order_acquire);
}


inline bool SpinlockMutex::tryLock(long milliseconds)
{
	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
	while (_flag.test_and_set(std::memory_order_acquire))
	{
		if (now.isElapsed(diff)) return false;
	}
	return true;
}


inline void SpinlockMutex::unlock()
{
	_flag.clear(std::memory_order_release);
}


} // namespace Poco


#endif // Foundation_Mutex_INCLUDED
