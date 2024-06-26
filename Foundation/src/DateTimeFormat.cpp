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

// NOTE: Must be in sync with DateTimeParser::parseTZD
// TODO: Validate timezone strings separately and simplify regex?
#define TIMEZONES_REGEX_PART \
	"(UT)|(GMT)|(BST)|(IST)|(WET)|(WEST)|(CET)|(CEST)|(EET)|(EEST)|(EST)|(MSK)|" \
	"(MSD)|(NST)|(NDT)|(AST)|(ADT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|" \
	"(PDT)|(AKST)|(AKDT)|(HST)|(AEST)|(AEDT)|(ACST)|(ACDT)|(AWST)|(AWDT)"

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
	"(([+\\-]?\\d\\d\\d\\d)|" TIMEZONES_REGEX_PART "|\\w)");

const std::string DateTimeFormat::RFC1123_FORMAT("%w, %e %b %Y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC1123_REGEX(DateTimeFormat::RFC822_REGEX);

const std::string DateTimeFormat::HTTP_FORMAT("%w, %d %b %Y %H:%M:%S %Z");
const std::string DateTimeFormat::HTTP_REGEX("(((Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?"
	"\\d\\d? +"
	"((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +"
	"\\d\\d(\\d\\d)? +\\d\\d:\\d\\d(:\\d\\d)? "
	"(" TIMEZONES_REGEX_PART "|)?+"
	"(([+\\-]?\\d\\d\\d\\d)?|" TIMEZONES_REGEX_PART "|\\w)");

const std::string DateTimeFormat::RFC850_FORMAT("%W, %e-%b-%y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC850_REGEX(
	"(((Monday)|(Tuesday)|(Wednesday)|(Thursday)|(Friday)|(Saturday)|(Sunday)|"
	"(Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?"
	"\\d\\d?-((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec))-"
	"\\d\\d(\\d\\d)? +\\d\\d:\\d\\d(:\\d\\d)? "
	"(" TIMEZONES_REGEX_PART "|)?+"
	"(([+\\-]?\\d\\d\\d\\d)?|" TIMEZONES_REGEX_PART "|\\w)");

const std::string DateTimeFormat::RFC1036_FORMAT("%W, %e %b %y %H:%M:%S %Z");
const std::string DateTimeFormat::RFC1036_REGEX(
	"(((Monday)|(Tuesday)|(Wednesday)|(Thursday)|(Friday)|(Saturday)|(Sunday)), *)?"
	"\\d\\d? +"
	"((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +"
	"\\d\\d(\\d\\d)? +\\d\\d:\\d\\d(:\\d\\d)? "
	"(" TIMEZONES_REGEX_PART "|)?+"
	"(([+\\-]?\\d\\d\\d\\d)?|" TIMEZONES_REGEX_PART "|\\w)");

// It would perhaps be useful to add RFC 2822 (successor of 822)
// https://www.rfc-editor.org/rfc/rfc2822#section-3.3

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


bool DateTimeFormat::hasFormat(const std::string& fmt)
{
	return FORMAT_LIST.find(fmt) != FORMAT_LIST.end();
}


bool DateTimeFormat::isValid(const std::string& dateTime)
{
        static const RegularExpression regexList[] = {
                RegularExpression(DateTimeFormat::ISO8601_REGEX),
                RegularExpression(DateTimeFormat::RFC822_REGEX),
                RegularExpression(DateTimeFormat::RFC1123_REGEX),
                RegularExpression(DateTimeFormat::HTTP_REGEX),
                RegularExpression(DateTimeFormat::RFC850_REGEX),
                RegularExpression(DateTimeFormat::RFC1036_REGEX),
                RegularExpression(DateTimeFormat::ASCTIME_REGEX),
                RegularExpression(DateTimeFormat::SORTABLE_REGEX)
        };
		
        for (const auto& f : regexList)
        {
            if (f.match(dateTime)) return true;
        }
        return false;
}


} // namespace Poco
