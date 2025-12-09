//
// Timezone_WIN32.cpp
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
#include "Poco/UnicodeConverter.h"
#include "Poco/Exception.h"
#include "Poco/UnWindows.h"
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
		TIME_ZONE_INFORMATION info;
		GetTimeZoneInformation(&info);
		_tzOffset = -info.Bias*60;
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
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return dstFlag == TIME_ZONE_ID_DAYLIGHT ? -tzInfo.DaylightBias*60 : 0;
}


int Timezone::dst(const Poco::Timestamp& timestamp)
{
	if (isDst(timestamp))
	{
		TIME_ZONE_INFORMATION tzInfo;
		GetTimeZoneInformation(&tzInfo);
		return -tzInfo.DaylightBias*60;
	}
	else return 0;
}


bool Timezone::isDst(const Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct std::tm local;
	if (localtime_s(&local, &time))
		throw Poco::SystemException("cannot get local time DST flag");
	return local.tm_isdst > 0;
}


std::string Timezone::name()
{
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = dstFlag == TIME_ZONE_ID_DAYLIGHT ? tzInfo.DaylightName : tzInfo.StandardName;
	UnicodeConverter::toUTF8(ptr, result);
	return result;
}


std::string Timezone::standardName()
{
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = tzInfo.StandardName;
	UnicodeConverter::toUTF8(ptr, result);
	return result;
}


std::string Timezone::dstName()
{
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = tzInfo.DaylightName;
	UnicodeConverter::toUTF8(ptr, result);
	return result;
}


void Timezone::reloadCache()
{
	tzInfo.reload();
}


} // namespace Poco
