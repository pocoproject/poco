//
// DateTime.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  DateTime
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <algorithm>
#include <cmath>
#include <ctime>


namespace Poco {


DateTime::DateTime()
{
	Timestamp now;
	_utcTime = now.utcTime();
	computeGregorian(julianDay());
	computeDaytime();
}


DateTime::DateTime(const tm& tmStruct):
	_year(tmStruct.tm_year + 1900),
	_month(tmStruct.tm_mon + 1),
	_day(tmStruct.tm_mday),
	_hour(tmStruct.tm_hour),
	_minute(tmStruct.tm_min),
	_second(tmStruct.tm_sec),
	_millisecond(0),
	_microsecond(0)
{
	if (isValid(_year, _month, _day, _hour, _minute, _second, _millisecond, _microsecond))
	{
		_utcTime = toUtcTime(toJulianDay(_year, _month, _day)) + 10*(_hour*Timespan::HOURS + _minute*Timespan::MINUTES + _second*Timespan::SECONDS);
	}
	else
	{
		throw Poco::InvalidArgumentException(Poco::format("Date time is %hd-%hd-%hdT%hd:%hd:%hd.%hd.%hd\n"
														  "Valid values:\n"
														  "0 <= year <= 9999\n"
														  "1 <= month <= 12\n"
														  "1 <= day <=  %d\n"
														  "0 <= hour <= 23\n"
														  "0 <= minute <= 59\n"
														  "0 <= second <= 60\n"
														  "0 <= millisecond <= 999\n"
														  "0 <= microsecond <= 999",
														  _year, _month, _day, _hour, _minute,
														  _second, _millisecond, _microsecond,
														  daysOfMonth(_year, _month)));
	}
}


DateTime::DateTime(const Timestamp& timestamp):
	_utcTime(timestamp.utcTime())
{
	computeGregorian(julianDay());
	computeDaytime();
}


DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond):
	_year(year),
	_month(month),
	_day(day),
	_hour(hour),
	_minute(minute),
	_second(second),
	_millisecond(millisecond),
	_microsecond(microsecond)
{
	if (isValid(_year, _month, _day, _hour, _minute, _second, _millisecond, _microsecond))
	{
		_utcTime = toUtcTime(toJulianDay(_year, _month, _day)) +
			10 * (_hour * Timespan::HOURS + _minute * Timespan::MINUTES + _second * Timespan::SECONDS +
				_millisecond * Timespan::MILLISECONDS + _microsecond);
	}
	else
	{
		throw Poco::InvalidArgumentException(Poco::format("Date time is %hd-%hd-%hdT%hd:%hd:%hd.%hd.%hd\n"
			"Valid values:\n"
			"0 <= year <= 9999\n"
			"1 <= month <= 12\n"
			"1 <= day <=  %d\n"
			"0 <= hour <= 23\n"
			"0 <= minute <= 59\n"
			"0 <= second <= 60\n"
			"0 <= millisecond <= 999\n"
			"0 <= microsecond <= 999",
			_year, _month, _day, _hour, _minute,
			_second, _millisecond, _microsecond,
			daysOfMonth(_year, _month)));
	}
}

DateTime::DateTime(double otherJulianDay):
	_utcTime(toUtcTime(otherJulianDay))
{
	computeGregorian(otherJulianDay);
}


DateTime::DateTime(Timestamp::UtcTimeVal otherUtcTime, Timestamp::TimeDiff diff):
	_utcTime(otherUtcTime + diff*10)
{
	computeGregorian(julianDay());
	computeDaytime();
}


DateTime::DateTime(const DateTime& dateTime):
	_utcTime(dateTime._utcTime),
	_year(dateTime._year),
	_month(dateTime._month),
	_day(dateTime._day),
	_hour(dateTime._hour),
	_minute(dateTime._minute),
	_second(dateTime._second),
	_millisecond(dateTime._millisecond),
	_microsecond(dateTime._microsecond)
{
}


DateTime::~DateTime()
{
}


DateTime& DateTime::operator = (const DateTime& dateTime)
{
	if (&dateTime != this)
	{
		_utcTime     = dateTime._utcTime;
		_year        = dateTime._year;
		_month       = dateTime._month;
		_day         = dateTime._day;
		_hour        = dateTime._hour;
		_minute      = dateTime._minute;
		_second      = dateTime._second;
		_millisecond = dateTime._millisecond;
		_microsecond = dateTime._microsecond;
	}
	return *this;
}

	
DateTime& DateTime::operator = (const Timestamp& otherTimestamp)
{
	_utcTime = otherTimestamp.utcTime();
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}


DateTime& DateTime::operator = (double otherJulianDay)
{
	_utcTime = toUtcTime(otherJulianDay);
	computeGregorian(otherJulianDay);
	return *this;
}


DateTime& DateTime::assign(int otherYear, int otherMonth, int otherDay, int otherHour, int otherMinute, int otherSecond, int otherMillisecond, int otherMicrosecond)
{
	if(isValid(otherYear, otherMonth, otherDay, otherHour, otherMinute, otherSecond, otherMillisecond, otherMicrosecond))
	{
		_utcTime     = toUtcTime(toJulianDay(otherYear, otherMonth, otherDay)) + 10*(otherHour*Timespan::HOURS + otherMinute*Timespan::MINUTES + otherSecond*Timespan::SECONDS + otherMillisecond*Timespan::MILLISECONDS + otherMicrosecond);
		_year        = static_cast<short>(otherYear);
		_month       = static_cast<short>(otherMonth);
		_day         = static_cast<short>(otherDay);
		_hour        = static_cast<short>(otherHour);
		_minute      = static_cast<short>(otherMinute);
		_second      = static_cast<short>(otherSecond);
		_millisecond = static_cast<short>(otherMillisecond);
		_microsecond = static_cast<short>(otherMicrosecond);
	}
	else
	{
		throw Poco::InvalidArgumentException(Poco::format("Date time is %d-%d-%dT%d:%d:%d:%d:%d\n"
			"Valid values:\n"
			"0 <= year <= 9999\n"
			"1 <= month <= 12\n"
			"1 <= day <=  %d\n"
			"0 <= hour <= 23\n"
			"0 <= minute <= 59\n"
			"0 <= second <= 60\n"
			"0 <= millisecond <= 999\n"
			"0 <= microsecond <= 999",
			_year, _month, _day, _hour, _minute,
			_second, _millisecond, _microsecond,
			daysOfMonth(_year, _month)));
	}

	return *this;
}


void DateTime::swap(DateTime& dateTime)
{
	std::swap(_utcTime, dateTime._utcTime);
	std::swap(_year, dateTime._year);
	std::swap(_month, dateTime._month);
	std::swap(_day, dateTime._day);
	std::swap(_hour, dateTime._hour);
	std::swap(_minute, dateTime._minute);
	std::swap(_second, dateTime._second);
	std::swap(_millisecond, dateTime._millisecond);
	std::swap(_microsecond, dateTime._microsecond);
}


int DateTime::dayOfWeek() const
{
	return int((std::floor(julianDay() + 1.5))) % 7;
}


int DateTime::dayOfYear() const
{
	int doy = 0;
	for (int currentMonth = 1; currentMonth < _month; ++currentMonth)
		doy += daysOfMonth(_year, currentMonth);
	doy += _day;
	return doy;
}


int DateTime::daysOfMonth(int year, int month)
{
	poco_assert (month >= 1 && month <= 12);

	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if (month == 2 && isLeapYear(year))
		return 29;
	else
		return daysOfMonthTable[month];
}


bool DateTime::isValid(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	return
		(year >= 0 && year <= 9999) &&
		(month >= 1 && month <= 12) &&
		(day >= 1 && day <= daysOfMonth(year, month)) &&
		(hour >= 0 && hour <= 23) &&
		(minute >= 0 && minute <= 59) &&
		(second >= 0 && second <= 60) &&
		(millisecond >= 0 && millisecond <= 999) &&
		(microsecond >= 0 && microsecond <= 999);
}


int DateTime::week(int firstDayOfWeek) const
{
	poco_assert (firstDayOfWeek >= 0 && firstDayOfWeek <= 6);

	/// find the first firstDayOfWeek.
	int baseDay = 1;
	while (DateTime(_year, 1, baseDay).dayOfWeek() != firstDayOfWeek) ++baseDay;

	int doy  = dayOfYear();
	int offs = baseDay <= 4 ? 0 : 1;
	if (doy < baseDay)
		return offs;
	else
		return (doy - baseDay)/7 + 1 + offs;
}


double DateTime::julianDay() const
{
	return toJulianDay(_utcTime);
}


DateTime DateTime::operator + (const Timespan& span) const
{
	return DateTime(_utcTime, span.totalMicroseconds());
}


DateTime DateTime::operator - (const Timespan& span) const
{
	return DateTime(_utcTime, -span.totalMicroseconds());
}


Timespan DateTime::operator - (const DateTime& dateTime) const
{
	return Timespan((_utcTime - dateTime._utcTime)/10);
}


DateTime& DateTime::operator += (const Timespan& span)
{
	_utcTime += span.totalMicroseconds()*10;
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}


DateTime& DateTime::operator -= (const Timespan& span)
{
	_utcTime -= span.totalMicroseconds()*10;
	computeGregorian(julianDay());
	computeDaytime();
	return *this;
}


tm DateTime::makeTM() const
{
	tm tmStruct;

	tmStruct.tm_sec = _second;
	tmStruct.tm_min = _minute;
	tmStruct.tm_hour = _hour;
	tmStruct.tm_mday = _day;
	poco_assert (_month > 0);
	tmStruct.tm_mon = _month - 1;
	poco_assert (_year >= 1900);
	tmStruct.tm_year = _year - 1900;
	tmStruct.tm_wday = dayOfWeek();
	int doy = dayOfYear();
	poco_assert (_year >0);
	tmStruct.tm_yday = doy - 1;
	tmStruct.tm_isdst = -1;

	return tmStruct;
}


void DateTime::makeUTC(int tzd)
{
	operator -= (Timespan(((Timestamp::TimeDiff) tzd)*Timespan::SECONDS));
}

	
void DateTime::makeLocal(int tzd)
{
	operator += (Timespan(((Timestamp::TimeDiff) tzd)*Timespan::SECONDS));
}


double DateTime::toJulianDay(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	// lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
	static int lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337};

	// day to double
	double dday = double(day) + ((double((hour*60 + minute)*60 + second)*1000 + millisecond)*1000 + microsecond)/86400000000.0;
	if (month < 3)
	{
		month += 12;
		--year;
	}
	double dyear = double(year);
	return dday + lookup[month] + 365*year + std::floor(dyear/4) - std::floor(dyear/100) + std::floor(dyear/400) + 1721118.5;
}


void DateTime::checkLimit(short& lower, short& higher, short limit)
{
	if (lower >= limit)
	{
		higher += short(lower / limit);
		lower   = short(lower % limit);
	}
}


void DateTime::normalize()
{
	checkLimit(_microsecond, _millisecond, 1000);
	checkLimit(_millisecond, _second, 1000);
	checkLimit(_second, _minute, 60);
	checkLimit(_minute, _hour, 60);
	checkLimit(_hour, _day, 24);

	if (_day > daysOfMonth(_year, _month))
	{
		_day -= static_cast<short>(daysOfMonth(_year, _month));
		if (++_month > 12)
		{
			++_year;
			_month -= 12;
		}
	}
}


void DateTime::computeGregorian(double otherJulianDay)
{
	double z    = std::floor(otherJulianDay - 1721118.5);
	double r    = otherJulianDay - 1721118.5 - z;
	double g    = z - 0.25;
	double a    = std::floor(g / 36524.25);
	double b    = a - std::floor(a/4);
	_year       = short(std::floor((b + g)/365.25));
	double c    = b + z - std::floor(365.25*_year);
	_month      = short(std::floor((5*c + 456)/153));
	double dday = c - std::floor((153.0*_month - 457)/5) + r;
	_day        = short(dday);
	if (_month > 12)
	{
		++_year;
		_month -= 12;
	}
	r      *= 24;
	_hour   = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 60;
	_minute = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 60;
	_second = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 1000;
	_millisecond = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 1000;
	_microsecond = short(r + 0.5);

	normalize();

	poco_assert_dbg (_month >= 1 && _month <= 12);
	poco_assert_dbg (_day >= 1 && _day <= daysOfMonth(_year, _month));
	poco_assert_dbg (_hour >= 0 && _hour <= 23);
	poco_assert_dbg (_minute >= 0 && _minute <= 59);
	poco_assert_dbg (_second >= 0 && _second <= 59);
	poco_assert_dbg (_millisecond >= 0 && _millisecond <= 999);
	poco_assert_dbg (_microsecond >= 0 && _microsecond <= 999);
}


void DateTime::computeDaytime()
{
	Timespan span(_utcTime/10);
	int spanHour = span.hours();
	// Due to double rounding issues, the previous call to computeGregorian()
	// may have crossed into the next or previous day. We need to correct that.
	if (spanHour == 23 && _hour == 0)
	{
		_day--;
		if (_day == 0)
		{
			_month--;
			if (_month == 0)
			{
				_month = 12;
				_year--;
			}
			_day = static_cast<short>(daysOfMonth(_year, _month));
		}
	}
	else if (spanHour == 0 && _hour == 23)
	{
		_day++;
		if (_day > daysOfMonth(_year, _month))
		{
			_month++;
			if (_month > 12)
			{
				_month = 1;
				_year++;
			}
			_day = 1;
		}
	}
	_hour        = static_cast<short>(spanHour);
	_minute      = static_cast<short>(span.minutes());
	_second      = static_cast<short>(span.seconds());
	_millisecond = static_cast<short>(span.milliseconds());
	_microsecond = static_cast<short>(span.microseconds());
}


} // namespace Poco
