//
// Mutex_WINCE.cpp
//
// $Id: //poco/1.4/Foundation/src/Mutex_WINCE.cpp#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex_WINCE.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"


namespace Poco {


MutexImpl::MutexImpl(MutexTypeImpl type)
	: _lockCount(0)
	, _recursive(type == MUTEX_RECURSIVE_IMPL)
{
	_mutex = CreateMutexW(NULL, FALSE, NULL);
	if (!_mutex) throw SystemException("cannot create mutex");
}


MutexImpl::~MutexImpl()
{
	CloseHandle(_mutex);
}


void MutexImpl::lockImpl()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		++_lockCount;
		if (!_recursive && _lockCount > 1)
		{
			// We're trying to go recursive so self-deadlock
			Thread::current()->join();
		}
		return;
	default:
		throw SystemException("cannot lock mutex");
	}
}


bool MutexImpl::tryLockImpl()
{
	switch (WaitForSingleObject(_mutex, 0))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		if (!_recursive && _lockCount > 0)
		{
			ReleaseMutex(_mutex);
			return false;
		}
		++_lockCount;
		return true;
	default:
		throw SystemException("cannot lock mutex");
	}
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
			if (tryLockImpl())
				return true;
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		Sleep(sleepMillis);
	} while (!now.isElapsed(diff));
	return false;
}


void MutexImpl::unlockImpl()
{
	ReleaseMutex(_mutex);
}


FastMutexImpl::FastMutexImpl()
{
	_mutex = CreateMutexW(NULL, FALSE, NULL);
	if (!_mutex) throw SystemException("cannot create mutex");
}


FastMutexImpl::~FastMutexImpl()
{
	CloseHandle(_mutex);
}


void FastMutexImpl::lockImpl()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		throw SystemException("cannot lock mutex");
	}
}


bool FastMutexImpl::tryLockImpl()
{
	switch (WaitForSingleObject(_mutex, 0))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw SystemException("cannot lock mutex");		
	}
}


bool FastMutexImpl::tryLockImpl(long milliseconds)
{
	switch (WaitForSingleObject(_mutex, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw SystemException("cannot lock mutex");		
	}
}


void FastMutexImpl::unlockImpl()
{
	ReleaseMutex(_mutex);
}


} // namespace Poco
