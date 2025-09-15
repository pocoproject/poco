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
#include <mutex>


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

	ScopedLock(M& mutex, std::adopt_lock_t) : _mutex(mutex)
		/// Construct and assume already locked
	{
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
		_bOwns = true;
	}

	ScopedLockWithUnlock(M& mutex, long milliseconds): _pMutex(&mutex)
	{
		poco_assert(_pMutex != nullptr);

		_pMutex->lock(milliseconds);
		_bOwns = true;
	}

	ScopedLockWithUnlock(M& mutex, std::adopt_lock_t) : _pMutex(&mutex), _bOwns(true)
		/// Construct and assume already locked
	{
		poco_assert(_pMutex != nullptr);
	}

	ScopedLockWithUnlock(M& mutex, std::try_to_lock_t) : _pMutex(&mutex)
		/// Construct and try to lock
	{
		poco_assert(_pMutex != nullptr);

		_bOwns = _pMutex->tryLock();
	}

	ScopedLockWithUnlock(M& mutex, std::defer_lock_t) : _pMutex(&mutex), _bOwns(false)
		/// Construct but don't lock
	{
		poco_assert(_pMutex != nullptr);
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
		poco_assert(_bOwns == false);

		_pMutex->lock();
		_bOwns = true;
	}

	bool tryLock()
	{
		poco_assert(_pMutex != nullptr);
		poco_assert(_bOwns == false);

		_bOwns = _pMutex->tryLock();
	}

	void unlock()
	{
		if (_bOwns)
		{
			poco_assert(_pMutex != nullptr);

			_pMutex->unlock();
			_bOwns = false;
		}
	}

	bool ownsLock() const
	{
		return _bOwns;
	}

private:
	M* _pMutex;
	bool _bOwns;
};


} // namespace Poco


#endif // Foundation_ScopedLock_INCLUDED
