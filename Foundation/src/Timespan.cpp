//
// Timespan.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  Timespan
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Timespan.h"
#include <algorithm>


namespace Poco {


const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS      = 1000*Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES      =   60*Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS        =   60*Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS         =   24*Timespan::HOURS;


Timespan::Timespan():
	_span(0)
{
}

	
Timespan::Timespan(TimeDiff microSeconds):
	_span(microSeconds)
{
}


Timespan::Timespan(long otherSeconds, long otherMicroSeconds):
	_span(TimeDiff(otherSeconds)*SECONDS + otherMicroSeconds)
{
}

	
Timespan::Timespan(int otherDays, int otherHours, int otherMinutes, int otherSeconds, int otherMicroSeconds):
	_span(TimeDiff(otherMicroSeconds) + TimeDiff(otherSeconds)*SECONDS + TimeDiff(otherMinutes)*MINUTES + TimeDiff(otherHours)*HOURS + TimeDiff(otherDays)*DAYS)
{
}


Timespan::Timespan(const Timespan& timespan):
	_span(timespan._span)
{
}


Timespan& Timespan::operator = (const Timespan& timespan)
{
	_span = timespan._span;
	return *this;
}


Timespan& Timespan::operator = (TimeDiff microSeconds)
{
	_span = microSeconds;
	return *this;
}


Timespan& Timespan::assign(int otherDays, int otherHours, int otherMinutes, int otherSeconds, int otherMicroSeconds)
{
	_span = TimeDiff(otherMicroSeconds) + TimeDiff(otherSeconds)*SECONDS + TimeDiff(otherMinutes)*MINUTES + TimeDiff(otherHours)*HOURS + TimeDiff(otherDays)*DAYS;
	return *this;
}


Timespan& Timespan::assign(long otherSeconds, long otherMicroSeconds)
{
	_span = TimeDiff(otherSeconds)*SECONDS + TimeDiff(otherMicroSeconds);
	return *this;
}


void Timespan::swap(Timespan& timespan)
{
	std::swap(_span, timespan._span);
}


Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(_span + d._span);
}


Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(_span - d._span);
}


Timespan& Timespan::operator += (const Timespan& d)
{
	_span += d._span;
	return *this;
}


Timespan& Timespan::operator -= (const Timespan& d)
{
	_span -= d._span;
	return *this;
}


Timespan Timespan::operator + (TimeDiff microSeconds) const
{
	return Timespan(_span + microSeconds);
}


Timespan Timespan::operator - (TimeDiff microSeconds) const
{
	return Timespan(_span - microSeconds);
}


Timespan& Timespan::operator += (TimeDiff microSeconds)
{
	_span += microSeconds;
	return *this;
}


Timespan& Timespan::operator -= (TimeDiff microSeconds)
{
	_span -= microSeconds;
	return *this;
}


} // namespace Poco
