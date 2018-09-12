//
// DateTimeFormat.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  DateTimeFormat
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <Poco/Exception.h>
#include "Poco/DateTimeFormat.h"
#include "Poco/RegularExpression.h"


namespace Poco {


const std::string DateTimeFormat::ISO8601_FORMAT("%Y-%m-%dT%H:%M:%S%z");
const std::string DateTimeFormat::ISO8601_FRAC_FORMAT("%Y-%m-%dT%H:%M:%s%z");
const std::string DateTimeFormat::ISO8601_REGEX("([\\+-]?\\d{4}(?!\\d{2}\\b))"
	"((-?)"
	"((0[1-9]|1[0-2])(\\3([12]\\d|0[1-9]|3[01]))?|W([0-4]\\d|5[0-2])(-?[1-7])?|"
	"(00[1-9]|0[1-9]\\d|[12]\\d{2}|3([0-5]\\d|6[1-6])))"
	"([T\\s]"
	"((([01]\\d|2[0-3])((:?)[0-5]\\d)?|24\\:?00)([\\.,]\\d+(?!:))?)?"
	"(\\17[0-5]\\d([\\.,]\\d+)?)?([A-I]|[K-Z]|([\\+-])([01]\\d|2[0-3]):?([0-5]\\d)?)?)?)?");

const std::string DateTimeFormat::RFC822_FORMAT("%w, %e %b %y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC822_REGEX("(((Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?"
	"\\d\\d? +"
	"((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +"
	"\\d\\d(\\d\\d)? +"
	"\\d\\d:\\d\\d(:\\d\\d)? +"
	"(([+\\-]?\\d\\d\\d\\d)|(UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|\\w)");

const std::string DateTimeFormat::RFC1123_FORMAT("%w, %e %b %Y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC1123_REGEX(DateTimeFormat::RFC822_REGEX);

const std::string DateTimeFormat::HTTP_FORMAT("%w, %d %b %Y %H:%M:%S %Z");
const std::string DateTimeFormat::HTTP_REGEX("(((Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?"
	"\\d\\d? +"
	"((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +"
	"\\d\\d(\\d\\d)? +\\d\\d:\\d\\d(:\\d\\d)? "
	"((UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|)?+"
	"(([+\\-]?\\d\\d\\d\\d)?|(UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|\\w)");

const std::string DateTimeFormat::RFC850_FORMAT("%W, %e-%b-%y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC850_REGEX(
	"(((Monday)|(Tuesday)|(Wednesday)|(Thursday)|(Friday)|(Saturday)|(Sunday)|"
	"(Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?"
	"\\d\\d?-((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec))-"
	"\\d\\d(\\d\\d)? +\\d\\d:\\d\\d(:\\d\\d)? "
	"((UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|)?+"
	"(([+\\-]?\\d\\d\\d\\d)?|(UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|\\w)");

const std::string DateTimeFormat::RFC1036_FORMAT("%W, %e %b %y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC1036_REGEX(
	"(((Monday)|(Tuesday)|(Wednesday)|(Thursday)|(Friday)|(Saturday)|(Sunday)), *)?"
	"\\d\\d? +"
	"((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +"
	"\\d\\d(\\d\\d)? +\\d\\d:\\d\\d(:\\d\\d)? "
	"((UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|)?+"
	"(([+\\-]?\\d\\d\\d\\d)?|(UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|\\w)");

const std::string DateTimeFormat::ASCTIME_FORMAT("%w %b %f %H:%M:%S %Y");
const std::string DateTimeFormat::ASCTIME_REGEX("((Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)) +"
	"((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +"
	"\\d\\d? +\\d\\d:\\d\\d:\\d\\d +(\\d\\d\\d\\d)");

const std::string DateTimeFormat::SORTABLE_FORMAT("%Y-%m-%d %H:%M:%S");
const std::string DateTimeFormat::SORTABLE_REGEX("(\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d:\\d\\d:\\d\\d)");


DateTimeFormat::Formatlist DateTimeFormat::FORMAT_LIST(
{
	DateTimeFormat::ISO8601_FORMAT,
	DateTimeFormat::ISO8601_FRAC_FORMAT,
	DateTimeFormat::RFC822_FORMAT,
	DateTimeFormat::RFC1123_FORMAT,
	DateTimeFormat::HTTP_FORMAT,
	DateTimeFormat::RFC850_FORMAT,
	DateTimeFormat::RFC1036_FORMAT,
	DateTimeFormat::ASCTIME_FORMAT,
	DateTimeFormat::SORTABLE_FORMAT
});


const std::string DateTimeFormat::WEEKDAY_NAMES[] =
{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};


const std::string DateTimeFormat::MONTH_NAMES[] =
{
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};


DateTimeFormat::RegexList DateTimeFormat::REGEX_LIST =
{
	&DateTimeFormat::ISO8601_REGEX,
	&DateTimeFormat::RFC822_REGEX,
	&DateTimeFormat::RFC1123_REGEX,
	&DateTimeFormat::HTTP_REGEX,
	&DateTimeFormat::RFC850_REGEX,
	&DateTimeFormat::RFC1036_REGEX,
	&DateTimeFormat::ASCTIME_REGEX,
	&DateTimeFormat::SORTABLE_REGEX
};


bool DateTimeFormat::hasFormat(const std::string& fmt)
{
	return FORMAT_LIST.find(fmt) != FORMAT_LIST.end();
}


bool DateTimeFormat::isValid(const std::string& dateTime)
{
	for (const auto& f : REGEX_LIST)
	{
		if (RegularExpression(*f).match(dateTime)) return true;
	}
	return false;
}


} // namespace Poco
