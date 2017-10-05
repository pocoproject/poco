//
// Clock.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  Clock
//
// Copyright (c) 2013, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Clock.h"
#include "Poco/Exception.h"
#include "Poco/Timestamp.h"
#include <chrono>
#include <algorithm>
#undef min
#undef max
#include <limits>


#ifndef POCO_HAVE_CLOCK_GETTIME
	#if (defined(_POSIX_TIMERS) && defined(CLOCK_REALTIME)) || defined(POCO_VXWORKS) || defined(__QNX__)
		#ifndef __APPLE__ // See GitHub issue #1453 - not available before Mac OS 10.12/iOS 10
			#define POCO_HAVE_CLOCK_GETTIME
		#endif
	#endif
#endif


namespace Poco {


const Clock::ClockVal Clock::CLOCKVAL_MIN = std::numeric_limits<Clock::ClockVal>::min();
const Clock::ClockVal Clock::CLOCKVAL_MAX = std::numeric_limits<Clock::ClockVal>::max();


Clock::Clock()
{
	update();
}


Clock::Clock(ClockVal tv)
{
	_clock = tv;
}


Clock::Clock(const Clock& other)
{
	_clock = other._clock;
}


Clock::~Clock()
{
}


Clock& Clock::operator = (const Clock& other)
{
	_clock = other._clock;
	return *this;
}


Clock& Clock::operator = (ClockVal tv)
{
	_clock = tv;
	return *this;
}


void Clock::swap(Clock& timestamp)
{
	std::swap(_clock, timestamp._clock);
}


void Clock::update()
{
	_clock = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}


Clock::ClockDiff Clock::accuracy()
{
	ClockVal acc = static_cast<ClockVal>(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(std::chrono::steady_clock::duration(1)).count());
	return acc > 0 ? acc : 1;
}


bool Clock::monotonic()
{
	return std::chrono::steady_clock::is_steady;
}


} // namespace Poco
