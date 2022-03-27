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
#include "Poco/Mutex.h"
#include <ctime>


namespace Poco {


class TZInfo
{
public:
	TZInfo()
	{
		tzset();
	}

	static long gmtOffset(const struct std::tm& t)
	{
	#if defined(__CYGWIN__)
		return t.__TM_GMTOFF;
	#else
		return t.tm_gmtoff;
	#endif
	}

	const char* name(bool dst)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		tzset();
		return tzname[dst ? 1 : 0];
	}

private:
	Poco::FastMutex _mutex;
};


static TZInfo tzInfo;


int Timezone::utcOffset(const Poco::Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct std::tm local;
	if (!localtime_r(&time, &local))
		throw Poco::SystemException("cannot get UTC offset");
	struct std::tm utc;
	gmtime_r(&time, &utc);
	std::time_t utctime = std::mktime(&utc);
	return time - utctime;
}


int Timezone::utcOffset()
{
	return utcOffset(Poco::Timestamp());
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
	long dst = TZInfo::gmtOffset(local) - utcOffset(timestamp);
	return local.tm_isdst == 1 ? dst : 0;
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
