//
// Mutex_STD.cpp
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex_STD.h"
#include "Poco/Timestamp.h"
#include <thread>


namespace Poco {


MutexImpl::MutexImpl(MutexTypeImpl type)
	: _mutex(type == MUTEX_RECURSIVE_IMPL ?
		std::unique_ptr<MutexImpl_BaseMutex>(new MutexImpl_MutexI<std::recursive_timed_mutex>()) :
		std::unique_ptr<MutexImpl_BaseMutex>(new MutexImpl_MutexI<std::timed_mutex>()))
{
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
	const int sleepMillis = 5;
	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);

	do
	{
		try
		{
			if (_mutex->tryLock(milliseconds))
				return true;
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis));
	} while (!now.isElapsed(diff));
	return false;
}


FastMutexImpl::FastMutexImpl() : _mutex()
{
}


bool FastMutexImpl::tryLockImpl(long milliseconds)
{
	const int sleepMillis = 5;
	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
	do
	{
		try
		{
			if (_mutex.try_lock_for(std::chrono::milliseconds(milliseconds)))
				return true;
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis));
	}
	while (!now.isElapsed(diff));
	return false;
}



} // namespace Poco
