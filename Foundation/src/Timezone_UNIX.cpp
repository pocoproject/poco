//
// Timezone_UNIX.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  Timezone
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Timezone.h"
#include "Poco/Exception.h"
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <string>


namespace Poco {


class TZInfo
{
public:
	TZInfo()
	{
		reload();
	}

	int timeZone()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (tzChanged())
			reloadNoLock();
		return _tzOffset;
	}

	void reload()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		reloadNoLock();
	}

	const char* name(bool dst)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		tzset();
		return tzname[dst ? 1 : 0];
	}

private:
	void reloadNoLock()
	{
		tzset();
		_tzOffset = computeTimeZone();
		cacheTZ();
	}

	void cacheTZ()
	{
		const char* tz = std::getenv("TZ");
		_cachedTZ = tz ? tz : "";
	}

	bool tzChanged() const
	{
		const char* tz = std::getenv("TZ");
		std::string currentTZ = tz ? tz : "";
		return currentTZ != _cachedTZ;
	}

	static int computeTimeZone()
	{
#if defined(__APPLE__)  || defined(__FreeBSD__) || defined (__OpenBSD__) || POCO_OS == POCO_OS_ANDROID // no timezone global var
		// Get offset from a date when DST is not active.
		// Check both January and July - one of them won't have DST.
		struct std::tm jan = {};
		jan.tm_year = 2024 - 1900;
		jan.tm_mon = 0;  // January
		jan.tm_mday = 15;
		jan.tm_hour = 12;
		std::time_t jan_time = std::mktime(&jan);
		struct std::tm jan_local;
		localtime_r(&jan_time, &jan_local);

		if (jan_local.tm_isdst <= 0)
			return static_cast<int>(jan_local.tm_gmtoff);

		// January has DST (Southern Hemisphere), try July
		struct std::tm jul = {};
		jul.tm_year = 2024 - 1900;
		jul.tm_mon = 6;  // July
		jul.tm_mday = 15;
		jul.tm_hour = 12;
		std::time_t jul_time = std::mktime(&jul);
		struct std::tm jul_local;
		localtime_r(&jul_time, &jul_local);
		return static_cast<int>(jul_local.tm_gmtoff);
#elif defined(__CYGWIN__)
		return -_timezone;
#else
		return -timezone;
#endif
	}

	std::mutex _mutex;
	int _tzOffset;
	std::string _cachedTZ;
};


static TZInfo tzInfo;


int Timezone::utcOffset()
{
	return tzInfo.timeZone();
}


int Timezone::dst()
{
	return dst(Poco::Timestamp());
}


int Timezone::dst(const Poco::Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct std::tm local;
	if (!localtime_r(&time, &local))
		throw Poco::SystemException("cannot get local time DST offset");
	if (local.tm_isdst > 0)
	{
#if defined(__CYGWIN__)
		return local.__TM_GMTOFF - utcOffset();
#elif defined(_BSD_SOURCE) || defined(__APPLE__)  || defined(__FreeBSD__) || defined (__OpenBSD__) || POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
		return local.tm_gmtoff - utcOffset();
#else
		return 3600;
#endif
	}
	else return 0;
}


bool Timezone::isDst(const Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct std::tm* tms = std::localtime(&time);
	if (!tms) throw Poco::SystemException("cannot get local time DST flag");
	return tms->tm_isdst > 0;
}


std::string Timezone::name()
{
	return std::string(tzInfo.name(dst() != 0));
}


std::string Timezone::standardName()
{
	return std::string(tzInfo.name(false));
}


std::string Timezone::dstName()
{
	return std::string(tzInfo.name(true));
}


void Timezone::reloadCache()
{
	tzInfo.reload();
}


} // namespace Poco
