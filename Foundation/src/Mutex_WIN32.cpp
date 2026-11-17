//
// Mutex_WIN32.cpp
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
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include <type_traits>


namespace Poco {


namespace {


template <typename TryLock>
[[nodiscard]] bool pollTryLock(TryLock tryLock, long milliseconds)
	/// Calls tryLock() until it returns true or the given number of
	/// milliseconds has elapsed. Returns true if the lock was acquired.
	/// Win32 has no timed acquire for critical sections and SRW locks.
{
	static_assert(std::is_invocable_r_v<bool, TryLock&>, "pollTryLock() requires a callable that returns bool");

	constexpr DWORD SLEEP_MILLISECONDS = 5;
	const Timestamp start;
	const Timestamp::TimeDiff timeout = static_cast<Timestamp::TimeDiff>(milliseconds)*Timespan::MILLISECONDS;
	do
	{
		if (tryLock())
			return true;
		::Sleep(SLEEP_MILLISECONDS);
	}
	while (!start.isElapsed(timeout));
	return false;
}


} // namespace


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
	return pollTryLock([this] { return tryLockImpl(); }, milliseconds);
}


bool FastMutexImpl::tryLockImpl(long milliseconds)
{
	return pollTryLock([this] { return tryLockImpl(); }, milliseconds);
}


} // namespace Poco
