//
// Mutex_STD.h
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the MutexImpl and FastMutexImpl classes for std::mutex.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Mutex_STD_INCLUDED
#define Foundation_Mutex_STD_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <mutex>
#include <chrono>
#include <memory>


namespace Poco {


class Foundation_API MutexImpl_BaseMutex
	// Helper class to make std::recursive_timed_mutex and std::timed_mutex generic
{
public:
	virtual ~MutexImpl_BaseMutex() {}

	virtual void lock() = 0;
	virtual bool tryLock() = 0;
	virtual bool tryLock(long milliseconds) = 0;
	virtual void unlock() = 0;
};


template <class T>
class MutexImpl_MutexI : public MutexImpl_BaseMutex
{
public:
	MutexImpl_MutexI() : _mutex() {}

	void lock()
	{
		_mutex.lock();
	}

	bool tryLock()
	{
		return _mutex.try_lock();
	}

	bool tryLock(long milliseconds)
	{
		return _mutex.try_lock_for(std::chrono::milliseconds(milliseconds));
	}

	void unlock()
	{
		_mutex.unlock();
	}
private:
	T _mutex;
};


class Foundation_API MutexImpl
{
public:
	enum MutexTypeImpl
	{
		MUTEX_RECURSIVE_IMPL,
		MUTEX_NONRECURSIVE_IMPL,
	};

	MutexImpl(const MutexImpl&) = delete;
	MutexImpl& operator = (const MutexImpl&) = delete;

protected:
	explicit MutexImpl(MutexTypeImpl type);
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	std::unique_ptr<MutexImpl_BaseMutex> _mutex;
};


class Foundation_API FastMutexImpl
{
protected:
	FastMutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	std::timed_mutex _mutex;
};


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	_mutex->lock();
}

inline bool MutexImpl::tryLockImpl()
{
	return _mutex->tryLock();
}

inline void MutexImpl::unlockImpl()
{
	_mutex->unlock();
}

inline void FastMutexImpl::lockImpl()
{
	_mutex.lock();
}

inline bool FastMutexImpl::tryLockImpl()
{
	return _mutex.try_lock();
}

inline void FastMutexImpl::unlockImpl()
{
	_mutex.unlock();
}


} // namespace Poco


#endif // Foundation_Mutex_STD_INCLUDED
