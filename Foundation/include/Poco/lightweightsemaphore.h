//
// LightWeightSemaphore.h
//
// Library: Foundation
// Package: Core
// Module:	LightWeightSemaphore
//
// Copyright Copyright (c) 2013-2020, Cameron Desrochers. All rights reserved.
// Extracted from https://github.com/cameron314/concurrentqueue lib and adapted for poco: Bychuk Alexander 2023
//
// SPDX-License-Identifier:	BSL-1.0
//

// Provides an efficient implementation of a semaphore (LightweightSemaphore).
// This is an extension of Jeff Preshing's sempahore implementation (licensed 
// under the terms of its separate zlib license) that has been adapted and
// extended by Cameron Desrochers.

#pragma once

#include <cstddef> // For std::size_t
#include <atomic>
#include <type_traits> // For std::make_signed<T>
#include <cassert>
#include "Poco/Semaphore.h"

namespace Poco {

// Code in the mpmc_sema namespace below is an adaptation of Jeff Preshing's
// portable + lightweight semaphore implementations, originally from
// https://github.com/preshing/cpp11-on-multicore/blob/master/common/sema.h
// LICENSE:
// Copyright (c) 2015 Jeff Preshing
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//	claim that you wrote the original software. If you use this software
//	in a product, an acknowledgement in the product documentation would be
//	appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//	misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.



//---------------------------------------------------------
// LightweightSemaphore
//---------------------------------------------------------
class LightweightSemaphore
{
public:
	using ssize_t = std::make_signed<std::size_t>::type;

private:
	std::atomic<ssize_t> m_count;
	Poco::Semaphore m_sema;
	int m_maxSpins;

	bool waitWithPartialSpinning(std::int64_t timeout_millisecs = -1)
	{
		ssize_t oldCount = 0;
		int spin = m_maxSpins;
		while (--spin >= 0)
		{
			oldCount = m_count.load(std::memory_order_relaxed);
			if ((oldCount > 0) && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire, std::memory_order_relaxed))
				return true;
			std::atomic_signal_fence(std::memory_order_acquire);	 // Prevent the compiler from collapsing the loop.
		}
		oldCount = m_count.fetch_sub(1, std::memory_order_acquire);
		if (oldCount > 0)
			return true;
		if (timeout_millisecs < 0)
		{
			
			try
			{
				m_sema.wait();
				return true;
			}
			catch(Poco::SystemException &) {}
		}
		if (timeout_millisecs > 0)
		{
			try
			{
				m_sema.wait(timeout_millisecs);
				return true;
			}
			catch(Poco::SystemException &) {}
		}
		// At this point, we've timed out waiting for the semaphore, but the
		// count is still decremented indicating we may still be waiting on
		// it. So we have to re-adjust the count, but only if the semaphore
		// wasn't signaled enough times for us too since then. If it was, we
		// need to release the semaphore too.
		while (true)
		{
			oldCount = m_count.load(std::memory_order_acquire);
			if (oldCount >= 0 && m_sema.tryWait(timeout_millisecs))
				return true;
			if (oldCount < 0 && m_count.compare_exchange_strong(oldCount, oldCount + 1, std::memory_order_relaxed, std::memory_order_relaxed))
				return false;
		}
	}

	ssize_t waitManyWithPartialSpinning(ssize_t max, std::int64_t timeout_millisecs = -1)
	{
		assert(max > 0);
		ssize_t oldCount = 0;
		int spin = m_maxSpins;
		while (--spin >= 0)
		{
			oldCount = m_count.load(std::memory_order_relaxed);
			if (oldCount > 0)
			{
				ssize_t newCount = oldCount > max ? oldCount - max : 0;
				if (m_count.compare_exchange_strong(oldCount, newCount, std::memory_order_acquire, std::memory_order_relaxed))
					return oldCount - newCount;
			}
			std::atomic_signal_fence(std::memory_order_acquire);
		}
		oldCount = m_count.fetch_sub(1, std::memory_order_acquire);
		if (oldCount <= 0)
		{
			auto calculateOldClount = [&timeout_millisecs, this]() -> ssize_t {
				ssize_t oldCount = 0;
				while (true)
				{
					oldCount = m_count.load(std::memory_order_acquire);
					if (oldCount >= 0 && m_sema.tryWait(timeout_millisecs))
						break;
					if (oldCount < 0 && m_count.compare_exchange_strong(oldCount, oldCount + 1, std::memory_order_relaxed, std::memory_order_relaxed))
						return 0;
				}
				return oldCount;
			};
			if (timeout_millisecs == 0)
			{
				oldCount = calculateOldClount();
				if (oldCount == 0) return oldCount;
			}
			else if (timeout_millisecs < 0)
			{
				try
				{
					m_sema.wait();
				}
				catch (Poco::SystemException &)
				{
					oldCount = calculateOldClount();
					if (oldCount == 0) return oldCount;
				}
				
			}
			else if (timeout_millisecs > 0)
			{
				try
				{
					m_sema.wait(timeout_millisecs);
				}
				catch (...) {
					oldCount = calculateOldClount();
					if (oldCount == 0) return oldCount;
				}
			}
		}
		if (max > 1)
			return 1 + tryWaitMany(max - 1);
		return 1;
	}

public:
	LightweightSemaphore(ssize_t initialCount = 0, int maxSpins = 10000) : m_count(initialCount), m_sema(initialCount), m_maxSpins(maxSpins)
	{
		assert(initialCount >= 0);
		assert(maxSpins >= 0);
	}

	bool tryWait()
	{
		ssize_t oldCount = m_count.load(std::memory_order_relaxed);
		while (oldCount > 0)
		{
			if (m_count.compare_exchange_weak(oldCount, oldCount - 1, std::memory_order_acquire, std::memory_order_relaxed))
				return true;
		}
		return false;
	}

	bool wait()
	{
		return tryWait() || waitWithPartialSpinning();
	}

	bool wait(std::int64_t timeout_usecs)
	{
		return tryWait() || waitWithPartialSpinning(timeout_usecs);
	}

	// Acquires between 0 and (greedily) max, inclusive
	ssize_t tryWaitMany(ssize_t max)
	{
		assert(max >= 0);
		ssize_t oldCount = m_count.load(std::memory_order_relaxed);
		while (oldCount > 0)
		{
			ssize_t newCount = oldCount > max ? oldCount - max : 0;
			if (m_count.compare_exchange_weak(oldCount, newCount, std::memory_order_acquire, std::memory_order_relaxed))
				return oldCount - newCount;
		}
		return 0;
	}

	// Acquires at least one, and (greedily) at most max
	ssize_t waitMany(ssize_t max, std::int64_t timeout_usecs)
	{
		assert(max >= 0);
		ssize_t result = tryWaitMany(max);
		if (result == 0 && max > 0)
			result = waitManyWithPartialSpinning(max, timeout_usecs);
		return result;
	}
	
	ssize_t waitMany(ssize_t max)
	{
		ssize_t result = waitMany(max, -1);
		assert(result > 0);
		return result;
	}

	void signal(ssize_t count = 1)
	{
		assert(count >= 0);
		ssize_t oldCount = m_count.fetch_add(count, std::memory_order_release);
		ssize_t toRelease = -oldCount < count ? -oldCount : count;
		if (toRelease > 0)
		{
			while (count-- > 0)
			{
				bool setOk = false;
				do {
					try
					{
						m_sema.set();
						setOk = true;
					}
					catch(Poco::SystemException &ex)
					{ }
				}
				while(!setOk);
				
			}
		}
	}
	
	std::size_t availableApprox() const
	{
		ssize_t count = m_count.load(std::memory_order_relaxed);
		return count > 0 ? static_cast<std::size_t>(count) : 0;
	}
};

}   // end namespace Poco
