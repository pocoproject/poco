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


#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Poco/NumberFormatter.h"
#include "Poco/MemoryStream.h"
#include <iomanip>
#if !defined(POCO_NO_LOCALE)
#include <locale>
#endif
#include <cstdio>
#include <cinttypes>


#if defined(_MSC_VER) || defined(__MINGW32__)
	#define I64_FMT "I64"
#elif defined(__APPLE__)
	#define I64_FMT "q"
#else
	#define I64_FMT "ll"
#endif



namespace Poco {


std::string NumberFormatter::format(bool value, BoolFormat format)
{
	switch (format)
	{
	case FMT_YES_NO:
		return value ? "yes" : "no";

	case FMT_ON_OFF:
		return value ? "on" : "off";

	default: // including FMT_TRUE_FALSE:
		return value ? "true" : "false";
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


void NumberFormatter::appendHex(std::string& str, int value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<unsigned int>(value), 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, int value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<unsigned int>(value), 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, width, '0', 0, lowercase);
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


void NumberFormatter::appendHex(std::string& str, long value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<unsigned long>(value), 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<unsigned long>(value), 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


#ifdef POCO_HAVE_INT64
#ifdef POCO_INT64_IS_LONG


void NumberFormatter::append(std::string& str, long long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, long long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, long long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long long value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<unsigned long long>(value), 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long long value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<unsigned long long>(value), 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned long long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long long value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long long value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


#else // ifndef POCO_LONG_IS_64_BIT


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


void NumberFormatter::appendHex(std::string& str, Int64 value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<UInt64>(value), 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int64 value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(static_cast<UInt64>(value), 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, -1, ' ', 0, lowercase);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value, int width, bool lowercase)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 0x10, result, sz, false, width, '0', 0, lowercase);
	str.append(result, sz);
}


#endif // ifdef POCO_INT64_IS_LONG
#endif // ifdef POCO_HAVE_INT64


void NumberFormatter::append(std::string& str, float value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, float value, int precision)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, POCO_MAX_FLT_STRING_LEN, value, precision);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, float value, int width, int precision)
{
	std::string result;
	str.append(floatToFixedStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, double value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int precision)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, POCO_MAX_FLT_STRING_LEN, value, precision);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int width, int precision)
{
	std::string result;
	str.append(doubleToFixedStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, const void* ptr)
{
	char buffer[24];
#if defined(POCO_PTR_IS_64_BIT)
	std::snprintf(buffer, sizeof(buffer), "%016" PRIXPTR, (UIntPtr) ptr);
#else
	std::snprintf(buffer, sizeof(buffer), "%08" PRIXPTR, (UIntPtr) ptr);
#endif
	str.append(buffer);
}


//
// Deprecated functions
//


std::string NumberFormatter::formatHex(int value, bool prefix)
{
	return formatHex(static_cast<unsigned int>(value), prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(int value, int width, bool prefix)
{
	return formatHex(static_cast<unsigned int>(value), width, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(unsigned value, bool prefix)
{
	return formatHex(value, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(unsigned value, int width, bool prefix)
{
	return formatHex(value, width, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(long value, bool prefix)
{
	return formatHex(static_cast<unsigned long>(value), prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(long value, int width, bool prefix)
{
	return formatHex(static_cast<unsigned long>(value), width, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(unsigned long value, bool prefix)
{
	return formatHex(value, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(unsigned long value, int width, bool prefix)
{
	return formatHex(value, width, prefix ? Options::PREFIX : Options::DEFAULT);
}


#ifdef POCO_HAVE_INT64
#ifdef POCO_INT64_IS_LONG


std::string NumberFormatter::formatHex(long long value, bool prefix)
{
	return formatHex(static_cast<unsigned long long>(value), prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(long long value, int width, bool prefix)
{
	return formatHex(static_cast<unsigned long long>(value), width, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(unsigned long long value, bool prefix)
{
	return formatHex(value, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(unsigned long long value, int width, bool prefix)
{
	return formatHex(value, width, prefix ? Options::PREFIX : Options::DEFAULT);
}


#else // ifndef POCO_LONG_IS_64_BIT


std::string NumberFormatter::formatHex(Int64 value, bool prefix)
{
	return formatHex(static_cast<UInt64>(value), prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(Int64 value, int width, bool prefix)
{
	return formatHex(static_cast<UInt64>(value), width, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(UInt64 value, bool prefix)
{
	return formatHex(value, prefix ? Options::PREFIX : Options::DEFAULT);
}


std::string NumberFormatter::formatHex(UInt64 value, int width, bool prefix)
{
	return formatHex(value, width, prefix ? Options::PREFIX : Options::DEFAULT);
}


#endif // ifdef POCO_INT64_IS_LONG
#endif // ifdef POCO_HAVE_INT64


} // namespace Poco
