//
// IOLock.cpp
//
// Library: Foundation
// Package: Threading
// Module:  IOLock
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/IOLock.h"
#include <chrono>
#include <thread>


namespace Poco {


bool IOLock::enter()
{
	_inProgress.store(true);
	if (_closed.load())
	{
		_inProgress.store(false, std::memory_order_release);
#ifdef POCO_HAVE_ATOMIC_WAIT
		_inProgress.notify_one();
#endif
		return false;
	}
	return true;
}


void IOLock::leave()
{
	_inProgress.store(false, std::memory_order_release);
#ifdef POCO_HAVE_ATOMIC_WAIT
	_inProgress.notify_one();
#endif
}


void IOLock::close()
{
	markClosed();
	wait();
}


void IOLock::markClosed()
{
	_closed.store(true);
}


void IOLock::wait()
{
#ifdef POCO_HAVE_ATOMIC_WAIT
	auto old = _inProgress.load(std::memory_order_acquire);
	// spurious wakeups possible, hence the loop
	while (_inProgress.load(std::memory_order_acquire))
		_inProgress.wait(old, std::memory_order_relaxed);
#else
	while (_inProgress.load())
		std::this_thread::sleep_for(std::chrono::microseconds(10));
#endif // POCO_HAVE_ATOMIC_WAIT
}


bool IOLock::tryWait(int milliseconds)
{
	auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
	while (_inProgress.load(std::memory_order_acquire))
	{
		if (std::chrono::steady_clock::now() >= deadline)
			return false;
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	return true;
}


} // namespace Poco
