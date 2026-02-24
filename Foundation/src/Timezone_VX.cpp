//
// Timezone_VXX.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  Timezone
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Timezone.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
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

	int utcOffset()
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

private:
	void reloadNoLock()
	{
		std::time_t now = std::time(nullptr);
		struct std::tm t;
		gmtime_r(&now, &t);
		std::time_t utc = std::mktime(&t);
		_tzOffset = now - utc;
		cacheTZ();
	}

	void cacheTZ()
	{
		const char* tz = std::getenv("TIMEZONE");
		_cachedTZ = tz ? tz : "";
	}

	bool tzChanged() const
	{
		const char* tz = std::getenv("TIMEZONE");
		std::string currentTZ = tz ? tz : "";
		return currentTZ != _cachedTZ;
	}

	std::mutex _mutex;
	int _tzOffset;
	std::string _cachedTZ;
};


static TZInfo tzInfo;


int Timezone::utcOffset()
{
	return tzInfo.utcOffset();
}


int Timezone::dst()
{
	std::time_t now = std::time(nullptr);
	struct std::tm t;
#if defined(_VXWORKS_COMPATIBILITY_MODE) || (defined(_WRS_VXWORKS_MAJOR) && ((_WRS_VXWORKS_MAJOR < 6) || ((_WRS_VXWORKS_MAJOR == 6)  && (_WRS_VXWORKS_MINOR < 9))))
	if (localtime_r(&now, &t) != OK)
#else
	if (!localtime_r(&now, &t))
#endif
		throw Poco::SystemException("cannot get local time DST offset");
	return t.tm_isdst == 1 ? 3600 : 0;
}


int Timezone::dst(const Poco::Timestamp& timestamp)
{
	return isDst(timestamp) ? 3600 : 0;
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
	// format of TIMEZONE environment variable:
	// name_of_zone:<(unused)>:time_in_minutes_from_UTC:daylight_start:daylight_end
	std::string tz = Environment::get("TIMEZONE", "UTC");
	std::string::size_type pos = tz.find(':');
	if (pos != std::string::npos)
		return tz.substr(0, pos);
	else
		return tz;
}


std::string Timezone::standardName()
{
	return name();
}


std::string Timezone::dstName()
{
	return name();
}


void Timezone::reloadCache()
{
	tzInfo.reload();
}


} // namespace Poco
