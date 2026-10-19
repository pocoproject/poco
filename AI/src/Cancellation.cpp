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
	_cancellation(cancellation),
	_id(0)
{
	Poco::FastMutex::ScopedLock lock(_cancellation._mutex);
	_id = ++_cancellation._nextId;
	auto it = _cancellation._aborts.emplace(_id, std::move(abort)).first;
	if (_cancellation._cancelled && it->second)
	{
		try
		{
			it->second();
		}
		catch (...)
		{
			// The destructor does not run when the constructor throws.
			_cancellation._aborts.erase(it);
			throw;
		}
	}
}


Cancellation::Request::~Request()
{
	Poco::FastMutex::ScopedLock lock(_cancellation._mutex);
	_cancellation._aborts.erase(_id);
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
	for (const auto& entry: _aborts)
	{
		if (!entry.second) continue;
		try
		{
			entry.second();
		}
		catch (...)
		{
		}
	}
}


} // namespace Poco::AI
