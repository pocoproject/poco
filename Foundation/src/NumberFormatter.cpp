//
// NumberFormatter.cpp
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
#include <iomanip>
#if !defined(POCO_NO_LOCALE)
#include <locale>
#endif
#include <cstdio>


#if defined(_MSC_VER) || defined(__MINGW32__)
	#define I64_FMT "I64"
#elif defined(__APPLE__)
	#define I64_FMT "q"
#else
	#define I64_FMT "ll"
#endif


namespace Poco {


std::string NumberFormatter::format(const bool value, const BoolFormat format)
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
	const std::string result = intToStr(value, 10);
	str.append(result);
}


void NumberFormatter::append(std::string& str, int value, int width)
{
	const std::string result = intToStr(value, 10, false, width);
	str.append(result);
}


void NumberFormatter::append0(std::string& str, int value, int width)
{
	const std::string result = intToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, int value)
{
	const std::string result = uIntToStr(static_cast<unsigned int>(value), 0x10);
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, int value, int width)
{
	const std::string result = uIntToStr(static_cast<unsigned int>(value), 0x10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append(std::string& str, unsigned value)
{
	const std::string result = uIntToStr(value, 10);
	str.append(result);
}


void NumberFormatter::append(std::string& str, unsigned value, int width)
{
	const std::string result = uIntToStr(value, 10, false, width);
	str.append(result);
}


void NumberFormatter::append0(std::string& str, unsigned int value, int width)
{
	const std::string result = uIntToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, unsigned value)
{
	const std::string result = uIntToStr(value, 0x10);
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, unsigned value, int width)
{
	const std::string result = uIntToStr(value, 0x10, false, width, '0');
	str.append(result);
}


#ifndef POCO_LONG_IS_64_BIT


void NumberFormatter::append(std::string& str, long value)
{
	const std::string result = intToStr(value, 10);
	str.append(result);
}


void NumberFormatter::append(std::string& str, long value, int width)
{
	const std::string result = intToStr(value, 10, false, width);
	str.append(result);
}


void NumberFormatter::append0(std::string& str, long value, int width)
{
	const std::string result = intToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, long value)
{
	const std::string result = uIntToStr(static_cast<unsigned long>(value), 0x10);
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, long value, int width)
{
	const std::string result = uIntToStr(static_cast<unsigned long>(value), 0x10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append(std::string& str, unsigned long value)
{
	const std::string result = uIntToStr(value, 10);
	str.append(result);
}


void NumberFormatter::append(std::string& str, unsigned long value, int width)
{
	const std::string result = uIntToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append0(std::string& str, unsigned long value, int width)
{
	const std::string result = uIntToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value)
{
	const std::string result = uIntToStr(value, 0x10);
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value, int width)
{
	const std::string result = uIntToStr(value, 0x10, false, width, '0');
	str.append(result);
}


#endif // POCO_LONG_IS_64_BIT


void NumberFormatter::append(std::string& str, Int64 value)
{
	const std::string result = intToStr(value, 10);
	str.append(result);
}


void NumberFormatter::append(std::string& str, Int64 value, int width)
{
	const std::string result = intToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append0(std::string& str, Int64 value, int width)
{
	const std::string result = intToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, Int64 value)
{
	const std::string result = uIntToStr(static_cast<UInt64>(value), 0x10);
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, Int64 value, int width)
{
	const std::string result = uIntToStr(static_cast<UInt64>(value), 0x10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append(std::string& str, UInt64 value)
{
	const std::string result = uIntToStr(value, 10);
	str.append(result);
}


void NumberFormatter::append(std::string& str, UInt64 value, int width)
{
	const std::string result = uIntToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append0(std::string& str, UInt64 value, int width)
{
	const std::string result = uIntToStr(value, 10, false, width, '0');
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value)
{
	const std::string result = uIntToStr(value, 0x10);
	str.append(result);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value, int width)
{
	const std::string result = uIntToStr(value, 0x10, false, width, '0');
	str.append(result);
}


void NumberFormatter::append(std::string& str, float value)
{
	const std::string result = floatToStr(value);
	str.append(result);
}


void NumberFormatter::append(std::string& str, float value, int precision)
{
       const std::string result = floatToFixedStr(value, precision);
	str.append(result);
}


void NumberFormatter::append(std::string& str, float value, int width, int precision)
{
	const std::string result = floatToFixedStr(value, precision, width);
	str.append(result);
}


void NumberFormatter::append(std::string& str, double value)
{
	const std::string result = doubleToStr(value);
	str.append(result);
}


void NumberFormatter::append(std::string& str, double value, int precision)
{
	const std::string result = doubleToFixedStr(value, precision);
	str.append(result);
}


void NumberFormatter::append(std::string& str, double value, int width, int precision)
{
       const std::string result = doubleToFixedStr(value, precision, width);
       str.append(result);
}


void NumberFormatter::append(std::string& str, const void* ptr)
{
	char buffer[24];
#if defined(POCO_PTR_IS_64_BIT)
	#if defined(POCO_LONG_IS_64_BIT)
		std::sprintf(buffer, "%016lX", (long) ptr);
	#else
		std::sprintf(buffer, "%016" I64_FMT "X", (UIntPtr) ptr);
	#endif
#else
	std::sprintf(buffer, "%08lX", (UIntPtr) ptr);
#endif
	str.append(buffer);
}


} // namespace Poco
