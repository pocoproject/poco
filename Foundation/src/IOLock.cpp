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
	_inProgress.store(true);
	if (_closed.load())
	{
		_inProgress.store(false);
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
	_closed.store(true);
}


void IOLock::wait()
{
	while (_inProgress.load())
		std::this_thread::sleep_for(std::chrono::microseconds(10));
}


bool IOLock::tryWait(int milliseconds)
{
	auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
	while (_inProgress.load())
	{
		if (std::chrono::steady_clock::now() >= deadline)
			return false;
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	return true;
}


} // namespace Poco
