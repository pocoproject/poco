//
// IOLock.cpp
//
// Library: Foundation
// Package: Threading
// Module:  IOLock
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.,
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
	_inProgress.store(true, std::memory_order_release);
	if (_closed.load(std::memory_order_acquire))
	{
		_inProgress.store(false, std::memory_order_release);
		return false;
	}
	return true;
}


void IOLock::close()
{
	markClosed();
	wait();
}


void IOLock::markClosed()
{
	_closed.store(true, std::memory_order_release);
}


void IOLock::wait()
{
	while (_inProgress.load(std::memory_order_acquire))
		std::this_thread::yield();
}


bool IOLock::tryWait(int milliseconds)
{
	auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
	while (_inProgress.load(std::memory_order_acquire))
	{
		if (std::chrono::steady_clock::now() >= deadline)
			return false;
		std::this_thread::yield();
	}
	return true;
}


} // namespace Poco
