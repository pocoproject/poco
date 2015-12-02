//
// Semaphore_STD.cpp
//
// $Id: //poco/1.4/Foundation/src/Semaphore_STD.cpp#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Semaphore
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Semaphore_STD.h"
#include <system_error>


namespace Poco {


SemaphoreImpl::SemaphoreImpl(int n, int max) :
	_count(n), _max(max)
{
	poco_assert (n >= 0 && max > 0 && n <= max);
}


void SemaphoreImpl::setImpl()
{
	try
	{
		std::lock_guard<std::mutex> lock{ _mutex };
		++_count;
		_cv.notify_one();
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


void SemaphoreImpl::waitImpl()
{
	try
	{
		std::unique_lock<std::mutex> lock{ _mutex };
		_cv.wait(lock, [&] { return _count > 0; });
		--_count;
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


bool SemaphoreImpl::waitImpl(long milliseconds)
{
	try
	{
		std::unique_lock<std::mutex> lock{ _mutex };
		auto finished = _cv.wait_for(lock, std::chrono::milliseconds(milliseconds), [&] { return _count > 0; });

		if (finished)
			--_count;

		return finished;
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


} // namespace Poco
