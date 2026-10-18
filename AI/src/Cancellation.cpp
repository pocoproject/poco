//
// Cancellation.cpp
//
// Library: AI
// Package: Core
// Module:  Cancellation
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/Cancellation.h"
#include <utility>


namespace Poco::AI {


Cancellation::Request::Request(Cancellation& cancellation, std::function<void()> abort):
	_cancellation(cancellation)
{
	Poco::FastMutex::ScopedLock lock(_cancellation._mutex);
	_cancellation._abort = std::move(abort);
	if (_cancellation._cancelled && _cancellation._abort)
	{
		_cancellation._abort();
	}
}


Cancellation::Request::~Request()
{
	Poco::FastMutex::ScopedLock lock(_cancellation._mutex);
	_cancellation._abort = nullptr;
}


Cancellation::Cancellation()
{
}


Cancellation::~Cancellation()
{
}


void Cancellation::cancel()
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_cancelled = true;
	if (_abort)
	{
		_abort();
	}
}


} // namespace Poco::AI
