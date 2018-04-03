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
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include "Poco/ScopedLock.h"
#include <atomic>


#if (POCO_OS == POCO_OS_CYGWIN || POCO_OS == POCO_OS_ANDROID)
#include "Poco/Mutex_POSIX.h"
#else
#include "Poco/Mutex_STD.h"
#endif

namespace Poco {


class Foundation_API Mutex: private MutexImpl
	/// A Mutex (mutual exclusion) is a synchronization
	/// mechanism used to control access to a shared resource
	/// in a concurrent (multithreaded) scenario.
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	enum MutexType
		/// The type of a mutex.
	{
		MUTEX_RECURSIVE = MUTEX_RECURSIVE_IMPL,      /// A recursive mutex
		MUTEX_NONRECURSIVE = MUTEX_NONRECURSIVE_IMPL /// A non-recursive mutex
	};

	typedef Poco::ScopedLock<Mutex> ScopedLock;
	
	explicit Mutex(MutexType type = MUTEX_RECURSIVE);
		/// Creates the Mutex.
		
	~Mutex();
		/// Destroys the Mutex.

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
	/// Locking a FastMutex is guaranteed to be at least as
	/// fast as locking a Mutex.  However, a FastMutex is not
	/// guaranteed to be either recursive or non-recursive.
	/// It is best suited to thread safe components like pools,
	/// caches and queues where locking is internal to the component.
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	typedef Poco::ScopedLock<FastMutex> ScopedLock;

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
	/// spinlock may not be the right choice - it is up to the user to
	/// choose the proper mutex type for their particular case.
	///
	/// Works with the ScopedLock class.
{
public:
	typedef Poco::ScopedLock<SpinlockMutex> ScopedLock;

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
	typedef Poco::ScopedLock<NullMutex> ScopedLock;
	
	NullMutex()
		/// Creates the NullMutex.
	{
	}
		
	~NullMutex()
		/// Destroys the NullMutex.
	{
	}

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
