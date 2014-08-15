//
// NumberFormatter.cpp
//
// $Id: //poco/1.4/Foundation/src/NumberFormatter.cpp#4 $
//
// Library: Foundation
// Package: Core
// Module:  NumberFormatter
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NumberFormatter.h"
#include "Poco/MemoryStream.h"
#include <ios>
#include <sstream>
#include <iomanip>
#if !defined(POCO_NO_LOCALE)
#include <locale>
#endif
#include <cstdio>


namespace Poco {


std::string NumberFormatter::format(bool value, BoolFormat format)
{
	switch(format)
	{
		default:
		case FMT_TRUE_FALSE:
			if (value == true)
				return "true";
			return "false";
		case FMT_YES_NO:
			if (value == true)
				return "yes";
			return "no";
		case FMT_ON_OFF:
			if (value == true)
				return "on";
			return "off";
	}
}


void NumberFormatter::append(std::string& str, int value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, int value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned long>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned long>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


#if defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)


void NumberFormatter::append(std::string& str, Int64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, Int64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, Int64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<UInt64>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<UInt64>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


#endif // defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)


void NumberFormatter::append(std::string& str, float value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int precision)
{
	std::string result;
	str.append(doubleToStr(result, value, precision));
}


void NumberFormatter::append(std::string& str, double value, int width, int precision)
{
	std::string result;
	str.append(doubleToStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, const void* ptr)
{
	std::ostringstream os;

	os	<< std::hex
		<< std::setw(sizeof(ptr) * 2)
		<< std::setfill('0')
		<< setiosflags(std::ios::uppercase)
		<< ptr;

	str.append(os.str());
}


} // namespace Poco
