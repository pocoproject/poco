//
// Mutex_STD.cpp
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2023, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex_STD.h"
#include "Poco/Timestamp.h"
#if !defined(POCO_NO_SYS_SELECT_H)
#include <sys/select.h>
#endif
#include <unistd.h>
#if defined(POCO_VXWORKS)
#include <timers.h>
#include <cstring>
#else
#include <sys/time.h>
#endif


namespace Poco {


	MutexImpl::MutexImpl() : _mutex()
	{
	}

	MutexImpl::~MutexImpl()
	{
	}


	bool MutexImpl::tryLockImpl(long milliseconds)
	{
		const int sleepMillis = 5;
		Timestamp now;
		Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
		do
		{
			bool rc = false;
			try
			{
				rc = _mutex.try_lock();
				if (rc)
					return true;
			}
			catch (std::exception &ex)
			{
				throw SystemException("cannot lock mutex", ex.what());
			}
#if defined(POCO_VXWORKS)
			struct timespec ts;
			ts.tv_sec = 0;
			ts.tv_nsec = sleepMillis*1000000;
			nanosleep(&ts, NULL);
#else
			struct timeval tv;
			tv.tv_sec  = 0;
			tv.tv_usec = sleepMillis * 1000;
			select(0, nullptr, nullptr, nullptr, &tv);
#endif
		}
		while (!now.isElapsed(diff));
		return false;

	}


	FastMutexImpl::FastMutexImpl(): _mutex()
	{
	}


	FastMutexImpl::~FastMutexImpl()
	{
	}

	bool FastMutexImpl::tryLockImpl(long milliseconds)
	{
		const int sleepMillis = 5;
		Timestamp now;
		Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);
		do
		{
			bool rc = false;
			try
			{
				rc = _mutex.try_lock();
				if (rc)
					return true;
			}
			catch (std::exception &ex)
			{
				throw SystemException("cannot lock mutex", ex.what());
			}
#if defined(POCO_VXWORKS)
			struct timespec ts;
			ts.tv_sec = 0;
			ts.tv_nsec = sleepMillis*1000000;
			nanosleep(&ts, NULL);
#else
			struct timeval tv;
			tv.tv_sec  = 0;
			tv.tv_usec = sleepMillis * 1000;
			select(0, NULL, NULL, NULL, &tv);
#endif
		}
		while (!now.isElapsed(diff));
		return false;

	}
} // namespace Poco
