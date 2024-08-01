//
// DateTimeParser.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  DateTimeParser
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"
#include <iostream>

namespace {
	using ParseIter = std::string::const_iterator;

	[[nodiscard]] ParseIter skipNonDigits(ParseIter it, ParseIter end)
	{
		while (it != end && !Poco::Ascii::isDigit(*it))
		{
			++it;
		}
		return it;
	}


	[[nodiscard]] ParseIter skipDigits(ParseIter it, ParseIter end)
	{
		while (it != end && Poco::Ascii::isDigit(*it))
		{
			++it;
		}
		return it;
	}


	int parseNumberN(const std::string& dtStr, ParseIter& it, ParseIter end, int n)
	{
		ParseIter numStart = end;
		int i = 0;

		for (; it != end && i < n && Poco::Ascii::isDigit(*it); ++it, ++i)
		{
			if (numStart == end)
			{
				numStart = it;
			}
		}

		if (numStart == end)
		{
			throw Poco::SyntaxException("Invalid DateTimeString: " + dtStr + ", No number found to parse");
		}

		std::string number(numStart, it);
		try
		{
			return std::stoi(number);
		}
		catch(const std::exception& e)
		{
			throw Poco::SyntaxException("Invalid DateTimeString: " + dtStr + ", invalid number: " + number);
		}
	}
}

namespace Poco {


void DateTimeParser::parse(const std::string& fmt, const std::string& dtStr, DateTime& dateTime, int& timeZoneDifferential)
{
	const auto str = Poco::trim(dtStr);

	if (fmt.empty() || str.empty())
	{
		throw SyntaxException("Invalid DateTimeString: " + dtStr);
	}
	else if (DateTimeFormat::hasFormat(fmt) && !DateTimeFormat::isValid(str))
	{
		throw SyntaxException("Invalid DateTimeString: " + dtStr);
	}
	
	const auto parse_number = [&dtStr](ParseIter& it, ParseIter end)
	{
		ParseIter numStart = end;

		for (; it != end && Poco::Ascii::isDigit(*it); ++it)
		{
			if (numStart == end)
			{
				numStart = it;
			}
		}

		if (numStart == end)
		{
			throw Poco::SyntaxException("Invalid DateTimeString: " + dtStr + ", No number found to parse");
		}

		std::string number(numStart, it);
		try
		{
			return std::stoi(number);
		}
		catch(const std::exception& e)
		{
			throw SyntaxException("Invalid DateTimeString: " + dtStr + ", invalid number: " + number);
		}
	};

	

	const auto parseFractionalN = [dtStr](ParseIter& it, ParseIter end, int n)
	{
		ParseIter numStart = end;
		int i = 0;

		for (; it != end && i < n && Poco::Ascii::isDigit(*it); ++it, ++i)
		{
			if (numStart == end)
			{
				numStart = it;
			}
		}

		if (numStart == end)
		{
			return 0;
		}

		std::string number(numStart, it);
		int result = 0;
		try
		{
			result = std::stoi(number);
		}
		catch(const std::exception& e)
		{
			throw SyntaxException("Invalid DateTimeString: " + dtStr + ", invalid number: " + number);
		}
		
		while (i++ < n) result *= 10;

		return result;
	};

	int year   = 0;
	int month  = 0;
	int day    = 0;
	int hour   = 0;
	int minute = 0;
	int second = 0;
	int millis = 0;
	int micros = 0;
	int tzd    = 0;

	bool dayParsed = false;
	bool monthParsed = false;

	std::string::const_iterator it   = str.begin();
	std::string::const_iterator end  = str.end();
	std::string::const_iterator itf  = fmt.begin();
	std::string::const_iterator endf = fmt.end();

	while (itf != endf && it != end)
	{
		if (*itf == '%')
		{
			if (++itf != endf)
			{
				switch (*itf)
				{
				case 'w': // Weekday, abbreviated
				case 'W': // Weekday
					while (it != end && Ascii::isSpace(*it)) ++it;
					while (it != end && Ascii::isAlpha(*it)) ++it;
					break;
				case 'b':
				case 'B':
					month = parseMonth(it, end);
					monthParsed = true;
					break;
				case 'd':
				case 'e':
				case 'f':
					it = skipNonDigits(it, end);
					day = parseNumberN(dtStr, it, end, 2);
					dayParsed = true;
					break;
				case 'm':
				case 'n':
				case 'o':
					it = skipNonDigits(it, end);
					month = parseNumberN(dtStr, it, end, 2);
					monthParsed = true;
					break;
				case 'y':
					it = skipNonDigits(it, end);
					year = parseNumberN(dtStr, it, end, 2);
					if (year >= 69)
						year += 1900;
					else
						year += 2000;
					break;
				case 'Y':
					it = skipNonDigits(it, end);
					year = parseNumberN(dtStr, it, end, 4);
					break;
				case 'r':
					it = skipNonDigits(it, end);
					year = parse_number(it, end);

					if (year < 1000)
					{
						if (year >= 69)
							year += 1900;
						else
							year += 2000;
					}
					break;
				case 'H':
				case 'h':
					it = skipNonDigits(it, end);
					hour = parseNumberN(dtStr, it, end, 2);
					break;
				case 'a':
				case 'A':
					hour = parseAMPM(it, end, hour);
					break;
				case 'M':
					it = skipNonDigits(it, end);
					minute = parseNumberN(dtStr, it, end, 2);
					break;
				case 'S':
					it = skipNonDigits(it, end);
					second = parseNumberN(dtStr, it, end, 2);
					break;
				case 's':
					it = skipNonDigits(it, end);
					second = parseNumberN(dtStr, it, end, 2);

					if (it != end && (*it == '.' || *it == ','))
					{
						++it;

						if (it != end && !Ascii::isDigit(*it))
						{
							throw SyntaxException("Invalid DateTimeString: " + dtStr + ", missing millisecond");
						}
						
						millis = parseFractionalN(it, end, 3);
						micros = parseFractionalN(it, end, 3);
						it = skipDigits(it, end);
					}
					break;
				case 'i':
					it = skipNonDigits(it, end);
					millis = parseNumberN(dtStr, it, end, 3);
					break;
				case 'c':
					it = skipNonDigits(it, end);
					millis = parseNumberN(dtStr, it, end, 1);
					millis *= 100;
					break;
				case 'F':
					it = skipNonDigits(it, end);
					millis = parseNumberN(dtStr, it, end, 3);
					micros = parseNumberN(dtStr, it, end, 3);
					it = skipDigits(it, end);
					break;
				case 'z':
				case 'Z':
					tzd = parseTZD(it, end);
					break;
				}
				++itf;
			}
		}
		else ++itf;
	}
	if (!monthParsed) month = 1;
	if (!dayParsed) day = 1;
	if (DateTime::isValid(year, month, day, hour, minute, second, millis, micros))
		dateTime.assign(year, month, day, hour, minute, second, millis, micros);
	else
		throw SyntaxException("date/time component out of range");

	timeZoneDifferential = tzd;
}


DateTime DateTimeParser::parse(const std::string& fmt, const std::string& str, int& timeZoneDifferential)
{
	DateTime result;
	parse(fmt, str, result, timeZoneDifferential);
	return result;
}


bool DateTimeParser::tryParse(const std::string& fmt, const std::string& str, DateTime& dateTime, int& timeZoneDifferential)
{
	try
	{
		parse(fmt, str, dateTime, timeZoneDifferential);
	}
	catch (const Exception&)
	{
		return false;
	}
	return true;
}


void DateTimeParser::parse(const std::string& str, DateTime& dateTime, int& timeZoneDifferential)
{
	if (!tryParse(str, dateTime, timeZoneDifferential))
		throw SyntaxException("Unsupported or invalid date/time format");
}


DateTime DateTimeParser::parse(const std::string& str, int& timeZoneDifferential)
{
	DateTime result;
	if (tryParse(str, result, timeZoneDifferential))
		return result;
	else
		throw SyntaxException("Unsupported or invalid date/time format");
}


bool DateTimeParser::tryParse(const std::string& dtStr, DateTime& dateTime, int& timeZoneDifferential)
{
	const auto str = Poco::trim(dtStr);

	if (str.length() < 4) return false;

	if (str[3] == ',')
		return tryParse("%w, %e %b %r %H:%M:%S %Z", str, dateTime, timeZoneDifferential);
	else if (str[3] == ' ')
		return tryParse(DateTimeFormat::ASCTIME_FORMAT, str, dateTime, timeZoneDifferential);
	else if (str.find(',') < 10)
		return tryParse("%W, %e %b %r %H:%M:%S %Z", str, dateTime, timeZoneDifferential);
	else if (Ascii::isDigit(str[0]))
	{
		if (str.find(' ') != std::string::npos || str.length() == 10)
			return tryParse(DateTimeFormat::SORTABLE_FORMAT, str, dateTime, timeZoneDifferential);
		else if (str.find('.') != std::string::npos || str.find(',') != std::string::npos)
			return tryParse(DateTimeFormat::ISO8601_FRAC_FORMAT, str, dateTime, timeZoneDifferential);
		else
			return tryParse(DateTimeFormat::ISO8601_FORMAT, str, dateTime, timeZoneDifferential);
	}
	else return false;
}


int DateTimeParser::parseTZD(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	struct Zone
	{
		const char* designator;
		int         timeZoneDifferential;
		bool        allowsDifference;
	};

	static const Zone zones[] =
	{
		{"Z",           0, true},
		{"UT",          0, true},
		{"GMT",         0, true},
		{"BST",    1*3600, false},
		{"IST",    1*3600, false},
		{"WET",         0, false},
		{"WEST",   1*3600, false},
		{"CET",    1*3600, false},
		{"CEST",   2*3600, false},
		{"EET",    2*3600, false},
		{"EEST",   3*3600, false},
		{"MSK",    3*3600, false},
		{"MSD",    4*3600, false},
		{"NST",   -3*3600-1800, false},
		{"NDT",   -2*3600-1800, false},
		{"AST",   -4*3600, false},
		{"ADT",   -3*3600, false},
		{"EST",   -5*3600, false},
		{"EDT",   -4*3600, false},
		{"CST",   -6*3600, false},
		{"CDT",   -5*3600, false},
		{"MST",   -7*3600, false},
		{"MDT",   -6*3600, false},
		{"PST",   -8*3600, false},
		{"PDT",   -7*3600, false},
		{"AKST",  -9*3600, false},
		{"AKDT",  -8*3600, false},
		{"HST",  -10*3600, false},
		{"AEST",  10*3600, false},
		{"AEDT",  11*3600, false},
		{"ACST",   9*3600+1800, false},
		{"ACDT",  10*3600+1800, false},
		{"AWST",   8*3600, false},
		{"AWDT",   9*3600, false}
	};

	int tzd = 0;
	while (it != end && Ascii::isSpace(*it)) ++it;
	const Zone* zone = nullptr;
	std::string designator;
	if (it != end)
	{
		if (Ascii::isAlpha(*it))
		{
			designator += *it++;
			if (it != end && Ascii::isAlpha(*it)) designator += *it++;
			if (it != end && Ascii::isAlpha(*it)) designator += *it++;
			if (it != end && Ascii::isAlpha(*it)) designator += *it++;
			for (unsigned i = 0; i < sizeof(zones)/sizeof(Zone); ++i)
			{
				if (designator == zones[i].designator)
				{
					zone = &(zones[i]);
					tzd = zone->timeZoneDifferential;
					break;
				}
			}
		}
		if (!designator.empty() && !zone)
			throw SyntaxException("Unknown timezone designator "s + designator);

		if (it != end && (*it == '+' || *it == '-'))
		{
			// Time difference is allowed only for some timezone designators in general
			// Some formats prevent even that with regular expression
			if (zone && !zone->allowsDifference)
				throw SyntaxException("Timezone does not allow difference "s + zone->designator);

			int sign = *it == '+' ? 1 : -1;
			++it;
			int hours = 0;
			try
			{
				hours = parseNumberN("", it, end, 2);
			}
			catch(const SyntaxException& e)
			{
				throw SyntaxException("Timezone invalid number: hours");
			}
			
			if (hours < 0 || hours > 23)
				throw SyntaxException("Timezone difference hours out of range");
			if (it != end && *it == ':') ++it;
			int minutes = 0;
			try
			{
				minutes = parseNumberN("", it, end, 2);
			}
			catch(const SyntaxException& e)
			{
				throw SyntaxException("Timezone invalid number: minutes");
			}
			
			if (minutes < 0 || minutes > 59)
				throw SyntaxException("Timezone difference minutes out of range");
			tzd += sign*(hours*3600 + minutes*60);
		}
	}
	return tzd;
}


int DateTimeParser::parseMonth(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	std::string month;
	while (it != end && (Ascii::isSpace(*it) || Ascii::isPunct(*it))) ++it;
	bool isFirst = true;
	while (it != end && Ascii::isAlpha(*it))
	{
		char ch = (*it++);
		if (isFirst) { month += Ascii::toUpper(ch); isFirst = false; }
		else month += Ascii::toLower(ch);
	}
	if (month.length() < 3) throw SyntaxException("Month name must be at least three characters long", month);
	for (int i = 0; i < 12; ++i)
	{
		if (DateTimeFormat::MONTH_NAMES[i].find(month) == 0)
			return i + 1;
	}
	throw SyntaxException("Not a valid month name", month);
}


int DateTimeParser::parseDayOfWeek(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	std::string dow;
	while (it != end && (Ascii::isSpace(*it) || Ascii::isPunct(*it))) ++it;
	bool isFirst = true;
	while (it != end && Ascii::isAlpha(*it))
	{
		char ch = (*it++);
		if (isFirst) { dow += Ascii::toUpper(ch); isFirst = false; }
		else dow += Ascii::toLower(ch);
	}
	if (dow.length() < 3) throw SyntaxException("Weekday name must be at least three characters long", dow);
	for (int i = 0; i < 7; ++i)
	{
		if (DateTimeFormat::WEEKDAY_NAMES[i].find(dow) == 0)
			return i;
	}
	throw SyntaxException("Not a valid weekday name", dow);
}


int DateTimeParser::parseAMPM(std::string::const_iterator& it, const std::string::const_iterator& end, int hour)
{
	std::string ampm;
	while (it != end && (Ascii::isSpace(*it) || Ascii::isPunct(*it))) ++it;
	while (it != end && Ascii::isAlpha(*it))
	{
		char ch = (*it++);
		ampm += Ascii::toUpper(ch);
	}
	if (ampm == "AM")
	{
		if (hour == 12)
			return 0;
		else
			return hour;
	}
	else if (ampm == "PM")
	{
		if (hour < 12)
			return hour + 12;
		else
			return hour;
	}
	else throw SyntaxException("Not a valid AM/PM designator", ampm);
}


} // namespace Poco
