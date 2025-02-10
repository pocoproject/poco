//
// ScopedLock.h
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the ScopedLock template class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ScopedLock_INCLUDED
#define Foundation_ScopedLock_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


template <class M>
class ScopedLock
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a Mutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
{
public:
	explicit ScopedLock(M& mutex): _mutex(mutex)
	{
		_mutex.lock();
	}

	ScopedLock(M& mutex, long milliseconds): _mutex(mutex)
	{
		_mutex.lock(milliseconds);
	}

	~ScopedLock()
	{
		try
		{
			_mutex.unlock();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	ScopedLock() = delete;
	ScopedLock(const ScopedLock&) = delete;
	ScopedLock& operator=(const ScopedLock&) = delete;

private:
	M& _mutex;
};


template <class M>
class ScopedLockWithUnlock
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a Mutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
	/// The unlock() member function allows for manual
	/// unlocking of the mutex.
{
public:
	explicit ScopedLockWithUnlock(M& mutex): _pMutex(&mutex)
	{
		poco_assert(_pMutex != nullptr);

		_pMutex->lock();
		_locked = true;
	}

	ScopedLockWithUnlock(M& mutex, long milliseconds): _pMutex(&mutex)
	{
		poco_assert(_pMutex != nullptr);

		_pMutex->lock(milliseconds);
		_locked = true;
	}

	~ScopedLockWithUnlock()
	{
		try
		{
			unlock();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	ScopedLockWithUnlock() = delete;
	ScopedLockWithUnlock(const ScopedLockWithUnlock&) = delete;
	ScopedLockWithUnlock& operator=(const ScopedLockWithUnlock&) = delete;

	void lock()
	{
		poco_assert(_pMutex != nullptr);
		poco_assert(_locked == false);

		_pMutex->lock();
		_locked = true;
	}

	void unlock()
	{
		if (_locked)
		{
			poco_assert(_pMutex != nullptr);

			_pMutex->unlock();
			_locked = false;
		}
	}

private:
	M* _pMutex;
	bool _locked = false;
};


} // namespace Poco


#endif // Foundation_ScopedLock_INCLUDED
