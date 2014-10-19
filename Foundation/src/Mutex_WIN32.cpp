//
// Mutex_WIN32.cpp
//
// $Id: //poco/1.4/Foundation/src/Mutex_WIN32.cpp#1 $
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


#include "Poco/Mutex_WIN32.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"


namespace Poco {


MutexImpl::MutexImpl()
{
	// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
	// the return only checks if the input address of &_cs was valid, so it is safe to omit it
	InitializeCriticalSectionAndSpinCount(&_cs, 4000);
}


MutexImpl::~MutexImpl()
{
	DeleteCriticalSection(&_cs);
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
			if (TryEnterCriticalSection(&_cs) == TRUE)
				return true;
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		Sleep(sleepMillis);
	}
	while (!now.isElapsed(diff));
	return false;
}


FastMutexImpl::FastMutexImpl()
	: _lockCount(0)
{
	// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
	// the return only checks if the input address of &_cs was valid, so it is safe to omit it
	InitializeCriticalSectionAndSpinCount(&_cs, 4000);
}


FastMutexImpl::~FastMutexImpl()
{
	DeleteCriticalSection(&_cs);
}


void FastMutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&_cs);
		++_lockCount;

		if (_lockCount > 1)
		{
			// We're trying to go recursive so self-deadlock
			Thread::current()->join();
		}
	}
	catch (...)
	{
		throw SystemException("cannot lock mutex");
	}
}


bool FastMutexImpl::tryLockImpl()
{
	try
	{
		if (TryEnterCriticalSection(&_cs) == 0)
			return false;

		if (_lockCount > 0)
		{
			// We're trying to go recursive
			LeaveCriticalSection(&_cs);
			return false;
		}
		
		++_lockCount;
		return true;
	}
	catch (...)
	{
	}
	throw SystemException("cannot lock mutex");
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
			if (TryEnterCriticalSection(&_cs) == TRUE)
			{
				if (_lockCount == 0)
				{
					++_lockCount;
					return true;
				}

				// We're trying to go recursive
				LeaveCriticalSection(&_cs);
				Sleep(milliseconds);
				return false;
			}
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		Sleep(sleepMillis);
	} while (!now.isElapsed(diff));
	return false;
}


} // namespace Poco
