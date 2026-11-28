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


namespace {


// Julian days of -4713-01-01 00:00:00 and 10000-01-01 00:00:00, the
// range of the years DateTime accepts (the upper bound is exclusive).
constexpr double MIN_JULIAN_DAY = -327.5;
constexpr double MAX_JULIAN_DAY = 5373484.5;


} // namespace


DateTime::DateTime()
{
	Timestamp now;
	_utcTime = now.utcTime();
	computeGregorian(julianDay());
	computeDaytime();
	checkValid();
}


DateTime::DateTime(const tm& tmStruct):
	// Note: Member variables are short to minimize memory footprint.
	// All valid DateTime values fit within short range; inputs are validated by checkValid().
	_year(static_cast<short>(tmStruct.tm_year + 1900)),
	_month(static_cast<short>(tmStruct.tm_mon + 1)),
	_day(static_cast<short>(tmStruct.tm_mday)),
	_hour(static_cast<short>(tmStruct.tm_hour)),
	_minute(static_cast<short>(tmStruct.tm_min)),
	_second(static_cast<short>(tmStruct.tm_sec)),
	_millisecond(0),
	_microsecond(0)
{
	checkValid();
	_utcTime = toUtcTime(toJulianDay(_year, _month, _day)) +
		10*(_hour*Timespan::HOURS + _minute*Timespan::MINUTES + _second*Timespan::SECONDS);
}


DateTime::DateTime(const Timestamp& timestamp):
	_utcTime(timestamp.utcTime())
{
	computeGregorian(julianDay());
	computeDaytime();
	checkValid();
}


DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond):
	_year(static_cast<short>(year)),
	_month(static_cast<short>(month)),
	_day(static_cast<short>(day)),
	_hour(static_cast<short>(hour)),
	_minute(static_cast<short>(minute)),
	_second(static_cast<short>(second)),
	_millisecond(static_cast<short>(millisecond)),
	_microsecond(static_cast<short>(microsecond))
{
	checkValid(year, month, day, hour, minute, second, millisecond, microsecond);
	_utcTime = toUtcTime(toJulianDay(year, month, day)) +
			10 * (hour*Timespan::HOURS + minute*Timespan::MINUTES + second*Timespan::SECONDS +
				  millisecond*Timespan::MILLISECONDS + microsecond);
}


DateTime::DateTime(double julianDay):
	// toUtcTime() rejects a julian day out of range before any conversion
	_utcTime(toUtcTime(julianDay))
{
	computeGregorian(julianDay);
	checkValid();
}


DateTime::DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff):
	_utcTime(utcTime + diff*10)
{
	computeGregorian(julianDay());
	computeDaytime();
	checkValid();
}


DateTime& DateTime::operator = (const Timestamp& timestamp)
{
	// the constructor validates; an invalid value leaves *this unchanged
	DateTime dateTime(timestamp);
	swap(dateTime);
	return *this;
}


DateTime& DateTime::operator = (double julianDay)
{
	// the constructor validates; an invalid value leaves *this unchanged
	DateTime dateTime(julianDay);
	swap(dateTime);
	return *this;
}


DateTime& DateTime::assign(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	checkValid(year, month, day, hour, minute, second, millisecond, microsecond);

	_utcTime     = toUtcTime(toJulianDay(year, month, day)) + 10*(hour*Timespan::HOURS + minute*Timespan::MINUTES + second*Timespan::SECONDS + millisecond*Timespan::MILLISECONDS + microsecond);
	_year        = static_cast<short>(year);
	_month       = static_cast<short>(month);
	_day         = static_cast<short>(day);
	_hour        = static_cast<short>(hour);
	_minute      = static_cast<short>(minute);
	_second      = static_cast<short>(second);
	_millisecond = static_cast<short>(millisecond);
	_microsecond = static_cast<short>(microsecond);

	return *this;
}


void DateTime::swap(DateTime& dateTime) noexcept
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
	for (int month = 1; month < _month; ++month)
		doy += daysOfMonth(_year, month);
	doy += _day;
	return doy;
}


int DateTime::daysOfMonth(int year, int month)
{
	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month == 2 && isLeapYear(year))
		return 29;
	else if (month < 1 || month > 12)
		return 0;
	return daysOfMonthTable[month];
}


void DateTime::checkValid()
{
	checkValid(_year, _month, _day, _hour, _minute, _second, _millisecond, _microsecond);
}


void DateTime::checkValid(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	if (!isValid(year, month, day, hour, minute, second, millisecond, microsecond))
		throw Poco::InvalidArgumentException(Poco::format("Date time is %d-%d-%dT%d:%d:%d.%d.%d\n"
				"Valid values:\n"
				"-4713 <= year <= 9999\n"
				"1 <= month <= 12\n"
				"1 <= day <=  %d\n"
				"0 <= hour <= 23\n"
				"0 <= minute <= 59\n"
				"0 <= second <= 60\n"
				"0 <= millisecond <= 999\n"
				"0 <= microsecond <= 999",
				year, month, day, hour, minute,
				second, millisecond, microsecond,
				daysOfMonth(year, month)));
}


bool DateTime::isValid(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	return
		(year >= -4713 && year <= 9999) &&
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
	// the constructor validates; an invalid value leaves *this unchanged
	DateTime dateTime(_utcTime, span.totalMicroseconds());
	swap(dateTime);
	return *this;
}


DateTime& DateTime::operator -= (const Timespan& span)
{
	// the constructor validates; an invalid value leaves *this unchanged
	DateTime dateTime(_utcTime, -span.totalMicroseconds());
	swap(dateTime);
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


Timestamp::UtcTimeVal DateTime::toUtcTime(double julianDay)
{
	// check before the conversion: converting a non-finite or out of range
	// double to an integer is undefined
	if (!std::isfinite(julianDay) || julianDay < MIN_JULIAN_DAY || julianDay >= MAX_JULIAN_DAY)
		throw Poco::InvalidArgumentException(Poco::format("Julian day %f is out of range\n"
				"Valid values:\n"
				"%f <= julian day < %f",
				julianDay, MIN_JULIAN_DAY, MAX_JULIAN_DAY));
	return static_cast<Timestamp::UtcTimeVal>((julianDay - 2299160.5)*864000000000.0);
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
		_day = static_cast<short>(_day - daysOfMonth(_year, _month));
		if (++_month > 12)
		{
			++_year;
			_month -= 12;
		}
	}
}


void DateTime::computeGregorian(double julianDay)
{
	double z    = std::floor(julianDay - 1721118.5);
	double r    = julianDay - 1721118.5 - z;
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
}


void DateTime::computeDaytime()
{
	Timestamp::UtcTimeVal ut(_utcTime);
	if (ut < 0) {
		// GH3723: UtcTimeVal is negative for pre-gregorian dates
		// move it 1600 years to the future
		// keeping hour, minute, second,... for corrections
		ut += Int64(86400)*1000*1000*10*1600*365;
	}
	Timespan span(ut/10);
	int hour = span.hours();
	// Due to double rounding issues, the previous call to computeGregorian()
	// may have crossed into the next or previous day. We need to correct that.
	if (hour == 23 && _hour == 0)
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
	else if (hour == 0 && _hour == 23)
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
	_hour        = static_cast<short>(hour);
	_minute      = static_cast<short>(span.minutes());
	_second      = static_cast<short>(span.seconds());
	_millisecond = static_cast<short>(span.milliseconds());
	_microsecond = static_cast<short>(span.microseconds());
}


} // namespace Poco
