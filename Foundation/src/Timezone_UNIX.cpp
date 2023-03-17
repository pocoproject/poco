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


namespace Poco {


class TZInfo
{
public:
	TZInfo()
	{
		tzset();
	}

	int timeZone()
	{
		std::lock_guard<std::mutex> lock(_mutex);

	#if defined(__APPLE__)  || defined(__FreeBSD__) || defined (__OpenBSD__) || POCO_OS == POCO_OS_ANDROID // no timezone global var
		std::time_t now = std::time(NULL);
		struct std::tm t;
		gmtime_r(&now, &t);
		std::time_t utc = std::mktime(&t);
		return now - utc;
	#elif defined(__CYGWIN__)
		tzset();
		return -_timezone;
	#else
		tzset();
		return -timezone;
	#endif
	}

	const char* name(bool dst)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		tzset();
		return tzname[dst ? 1 : 0];
	}

private:
	std::mutex _mutex;
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


} // namespace Poco
