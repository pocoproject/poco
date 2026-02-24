//
// Mutex.cpp
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex.h"

#if defined(POCO_ENABLE_STD_MUTEX)
#include "Mutex_STD.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Mutex_WIN32.cpp"
#elif defined(POCO_VXWORKS)
#include "Mutex_VX.cpp"
#else
#include "Mutex_POSIX.cpp"
#endif


namespace Poco {


//
// Mutex
//

Mutex::Mutex()
{
}


Mutex::~Mutex()
{
}


//
// FastMutx
//

FastMutex::FastMutex()
{
}


FastMutex::~FastMutex()
{
}


//
// SpinlockMutex
//

namespace {

// Try to acquire the lock. On C++20, first polls with relaxed read
// to avoid cache line bouncing, then attempts acquire if lock appears free.
inline bool tryAcquire(std::atomic_flag& flag)
{
#if POCO_HAVE_CPP20_COMPILER
	if (!flag.test(std::memory_order_relaxed))
		if (!flag.test_and_set(std::memory_order_acquire))
			return true;
	return false;
#else
	return !flag.test_and_set(std::memory_order_acquire);
#endif
}

constexpr int SPIN_COUNT = 32;
constexpr int YIELD_COUNT = 16;

} // namespace


SpinlockMutex::SpinlockMutex()
{
}


SpinlockMutex::~SpinlockMutex()
{
}


void SpinlockMutex::lock()
{
	// Fast path: try to acquire immediately
	if (!_flag.test_and_set(std::memory_order_acquire))
		return;

	// Spin phase
	for (int i = 0; i < SPIN_COUNT; ++i)
		if (tryAcquire(_flag))
			return;

	// Yield phase
	for (int i = 0; i < YIELD_COUNT; ++i)
	{
		std::this_thread::yield();
		if (tryAcquire(_flag))
			return;
	}

	// Sleep phase
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		if (tryAcquire(_flag))
			return;
	}
}


void SpinlockMutex::lock(long milliseconds)
{
	// Fast path: try to acquire immediately
	if (!_flag.test_and_set(std::memory_order_acquire))
		return;

	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);

	// Spin phase
	for (int i = 0; i < SPIN_COUNT; ++i)
	{
		if (tryAcquire(_flag))
			return;
		if (now.isElapsed(diff)) throw TimeoutException();
	}

	// Yield phase
	for (int i = 0; i < YIELD_COUNT; ++i)
	{
		std::this_thread::yield();
		if (tryAcquire(_flag))
			return;
		if (now.isElapsed(diff)) throw TimeoutException();
	}

	// Sleep phase
	while (!now.isElapsed(diff))
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		if (tryAcquire(_flag))
			return;
	}
	throw TimeoutException();
}


bool SpinlockMutex::tryLock(long milliseconds)
{
	// Fast path: try to acquire immediately
	if (!_flag.test_and_set(std::memory_order_acquire))
		return true;

	Timestamp now;
	Timestamp::TimeDiff diff(Timestamp::TimeDiff(milliseconds)*1000);

	// Spin phase
	for (int i = 0; i < SPIN_COUNT; ++i)
	{
		if (tryAcquire(_flag))
			return true;
		if (now.isElapsed(diff)) return false;
	}

	// Yield phase
	for (int i = 0; i < YIELD_COUNT; ++i)
	{
		std::this_thread::yield();
		if (tryAcquire(_flag))
			return true;
		if (now.isElapsed(diff)) return false;
	}

	// Sleep phase
	while (!now.isElapsed(diff))
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		if (tryAcquire(_flag))
			return true;
	}
	return false;
}


} // namespace Poco
