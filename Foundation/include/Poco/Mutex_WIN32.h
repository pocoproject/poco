//
// Mutex_WIN32.h
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the MutexImpl and FastMutexImpl classes for WIN32.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Mutex_WIN32_INCLUDED
#define Foundation_Mutex_WIN32_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/Error.h"
#include "Poco/UnWindows.h"


namespace Poco {


class Foundation_API MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();

private:
	CRITICAL_SECTION _cs;
};


class Foundation_API FastMutexImpl
	/// Non-recursive mutex based on an SRW lock;
	/// a CRITICAL_SECTION would be recursive.
{
protected:
	FastMutexImpl() = default;
	~FastMutexImpl() = default;
	void lockImpl();
	[[nodiscard]] bool tryLockImpl();
	[[nodiscard]] bool tryLockImpl(long milliseconds);
	void unlockImpl();

private:
	SRWLOCK _lock = SRWLOCK_INIT;
};


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&_cs);
	}
	catch (...)
	{
		throw SystemException("cannot lock mutex", Error::getLastMessage());
	}
}


inline bool MutexImpl::tryLockImpl()
{
	try
	{
		return TryEnterCriticalSection(&_cs) != 0;
	}
	catch (...)
	{
	}
	throw SystemException("cannot lock mutex", Error::getLastMessage());
}


inline void MutexImpl::unlockImpl()
{
	LeaveCriticalSection(&_cs);
}


inline void FastMutexImpl::lockImpl()
{
	::AcquireSRWLockExclusive(&_lock);
}


inline bool FastMutexImpl::tryLockImpl()
{
	return ::TryAcquireSRWLockExclusive(&_lock) != 0;
}


inline void FastMutexImpl::unlockImpl()
{
	::ReleaseSRWLockExclusive(&_lock);
}


} // namespace Poco


#endif // Foundation_Mutex_WIN32_INCLUDED
