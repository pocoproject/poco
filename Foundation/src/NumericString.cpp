//
// NumericString.h
//
// Library: Foundation
// Package: Core
// Module:  NumericString
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Bugcheck.h"
#define POCO_NUMERIC_STRING_PRIVATE
#include "Poco/NumericString.h"
#undef POCO_NUMERIC_STRING_PRIVATE

#ifndef POCO_HAS_FLOAT_CHARCONV
// +++ double conversion +++
// don't collide with standalone double_conversion library
#define double_conversion poco_double_conversion
#define UNIMPLEMENTED poco_bugcheck
#include "double-conversion.h"
#include "cached-powers.cc"
#include "bignum-dtoa.cc"
#include "bignum.cc"
#include "fast-dtoa.cc"
#include "fixed-dtoa.cc"
#include "strtod.cc"
#include "double-to-string.cc"
#include "string-to-double.cc"
// --- double conversion ---
poco_static_assert(POCO_MAX_FLT_STRING_LEN == double_conversion::kMaxSignificantDecimalDigits);
#endif // !POCO_HAS_FLOAT_CHARCONV

#include "Poco/String.h"
#include <memory>
#include <cctype>


namespace {


void pad(std::string& str, std::string::size_type precision, std::string::size_type width, char prefix = ' ', char decSep = '.')
	/// Pads the string with prefix space and postfix 0.
	/// Alternative prefix (e.g. zero instead of space) can be supplied by caller.
	/// Used only internally.
{
	// these cases should never happen, if they do, it's a library bug
	poco_assert_dbg (precision > 0);
	poco_assert_dbg (str.length());

	std::string::size_type decSepPos = str.find(decSep);
	if (decSepPos == std::string::npos)
	{
		str.append(1, decSep);
		decSepPos = str.size() - 1;
	}

	std::string::size_type frac = str.length() - decSepPos - 1;

	std::string::size_type ePos = str.find_first_of("eE");
	std::unique_ptr<std::string> eStr;
	if (ePos != std::string::npos)
	{
		eStr.reset(new std::string(str.substr(ePos, std::string::npos)));
		frac -= eStr->length();
		str = str.substr(0, str.length() - eStr->length());
	}

	if (frac != precision)
	{
		if (frac < precision)
		{
			str.append(precision - frac, '0');
		}
		else if ((frac > precision) && (decSepPos != std::string::npos))
		{
			int pos = static_cast<int>(decSepPos + 1 + precision);
			if (str[pos] >= '5') // we must round up
			{
				char carry = 0;
				if(str[--pos] == '9')
				{
					str[pos] = '0';
					carry = 1;
				}
				else
				{
					++str[pos];
					carry = 0;
				}
				while (--pos >= 0)
				{
					if(str[pos] == decSep) continue;
					if(carry)
					{
						if((str[pos] + carry) <= '9')
						{
							++str[pos];
							carry = 0;
						}
						else
						{
							str[pos] = '0';
							carry = 1;
						}
					}
				}
				if (carry) str.insert(str.begin(), 1, '1');
			}
			str = str.substr(0, decSepPos + 1 + precision);
		}
	}

	if (eStr.get()) str += *eStr;

	if (width && (str.length() < width)) str.insert(str.begin(), width - str.length(), prefix);
}


void insertThousandSep(std::string& str, char thSep, char decSep = '.')
	/// Inserts thousand separators.
	/// Used only internally.
{
	poco_assert (decSep != thSep);
	if (str.size() == 0) return;

	std::string::size_type exPos = str.find('e');
	if (exPos == std::string::npos) exPos = str.find('E');
	std::string::size_type decPos = str.find(decSep);
	// there's no rinsert, using forward iterator to go backwards
	std::string::iterator it = str.end();
	if (exPos != std::string::npos) it -= str.size() - exPos;

	if (decPos != std::string::npos)
	{
		while (it != str.begin())
		{
			--it;
			if (*it == decSep) break;
		}
	}
	int thCount = 0;
	if (it == str.end()) --it;
	for (; it != str.begin();)
	{
		std::string::iterator pos = it;
		std::string::value_type chr = *it;
		std::string::value_type prevChr = *--it;

		if (!std::isdigit(chr)) continue;

		if (++thCount == 3 && std::isdigit(prevChr))
			it = str.insert(pos, thSep);

		if (thCount == 3) thCount = 0;
	}
}


} // namespace


namespace Poco {


#ifdef POCO_HAS_FLOAT_CHARCONV

namespace {

/// Format a floating-point value using std::to_chars with shortest representation.
/// Chooses fixed or scientific notation based on the value's exponent and the
/// [lowDec, highDec] range, matching double-conversion's ToShortest behavior.
template <typename T>
void toShortestStr(char* buffer, int bufferSize, T value, int lowDec, int highDec)
{
	// Determine the exponent to choose the right notation upfront,
	// avoiding a format-then-check-then-reformat round trip.
	T absVal = value < 0 ? -value : value;
	bool useFixed = (absVal == 0) ||
		(absVal >= 1 ? static_cast<int>(std::floor(std::log10(absVal))) <= highDec
		             : static_cast<int>(std::floor(std::log10(absVal))) >= lowDec);

	if (useFixed)
	{
		// Fixed notation with trailing-zero trimming for shortest output
		auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value, std::chars_format::fixed);
		if (ec != std::errc()) { buffer[0] = '\0'; return; }
		*ptr = '\0';

		char* dot = static_cast<char*>(std::memchr(buffer, '.', ptr - buffer));
		if (dot != nullptr)
		{
			char* last = ptr - 1;
			while (last > dot && *last == '0') --last;
			*(last == dot ? last : last + 1) = '\0';
		}
	}
	else
	{
		auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value);
		if (ec != std::errc()) { buffer[0] = '\0'; return; }
		*ptr = '\0';
	}
}

} // namespace

void floatToStr(char* buffer, int bufferSize, float value, int lowDec, int highDec)
{
	toShortestStr(buffer, bufferSize, value, lowDec, highDec);
}


void floatToFixedStr(char* buffer, int bufferSize, float value, int precision)
{
	auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value, std::chars_format::fixed, precision);
	if (ec != std::errc()) { buffer[0] = '\0'; return; }
	*ptr = '\0';
}

#else // !POCO_HAS_FLOAT_CHARCONV

void floatToStr(char* buffer, int bufferSize, float value, int lowDec, int highDec)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP, lowDec, highDec, 0, 0);
	dc.ToShortestSingle(value, &builder);
	builder.Finalize();
}


void floatToFixedStr(char* buffer, int bufferSize, float value, int precision)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP, -std::numeric_limits<float>::digits10, std::numeric_limits<float>::digits10, 0, 0);
	dc.ToFixed(value, precision, &builder);
	builder.Finalize();
}

#endif // POCO_HAS_FLOAT_CHARCONV


std::string& floatToStr(std::string& str, float value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[POCO_MAX_FLT_STRING_LEN];
	floatToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


std::string& floatToFixedStr(std::string& str, float value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[POCO_MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, POCO_MAX_FLT_STRING_LEN, value, precision);
	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


#ifdef POCO_HAS_FLOAT_CHARCONV

void doubleToStr(char* buffer, int bufferSize, double value, int lowDec, int highDec)
{
	toShortestStr(buffer, bufferSize, value, lowDec, highDec);
}


void doubleToFixedStr(char* buffer, int bufferSize, double value, int precision)
{
	auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value, std::chars_format::fixed, precision);
	if (ec != std::errc()) { buffer[0] = '\0'; return; }
	*ptr = '\0';
}

#else // !POCO_HAS_FLOAT_CHARCONV

void doubleToStr(char* buffer, int bufferSize, double value, int lowDec, int highDec)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP, lowDec, highDec, 0, 0);
	dc.ToShortest(value, &builder);
	builder.Finalize();
}


void doubleToFixedStr(char* buffer, int bufferSize, double value, int precision)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP,
			-std::numeric_limits<double>::digits10, std::numeric_limits<double>::digits10, 0, 0);
	dc.ToFixed(value, precision, &builder);
	builder.Finalize();
}

#endif // POCO_HAS_FLOAT_CHARCONV


std::string& doubleToStr(std::string& str, double value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[POCO_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);

	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


std::string& doubleToFixedStr(std::string& str, double value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[POCO_MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, POCO_MAX_FLT_STRING_LEN, value, precision);

	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


#ifdef POCO_HAS_FLOAT_CHARCONV

namespace {

/// Helper: skip whitespace and check for inf/nan strings.
/// std::from_chars recognizes "inf"/"infinity"/"nan" on its own, but
/// double-conversion only matches the exact strings passed as parameters.
/// We replicate double-conversion behavior: only the exact inf/nan parameter
/// strings are recognized; everything else that isn't numeric returns NaN.
template <typename T>
T strToFloatImpl(const char* str, const char* inf, const char* nan)
{
	while (std::isspace(static_cast<unsigned char>(*str))) ++str;
	const char* end = str + std::strlen(str);
	while (end > str && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;

	if (str == end) return std::numeric_limits<T>::quiet_NaN();

	bool negative = (*str == '-');
	const char* p = negative ? str + 1 : str;
	const std::size_t plen = static_cast<std::size_t>(end - p);

	// Check for exact inf/nan match (must consume entire remaining string).
	// This must be done before from_chars because from_chars accepts
	// "inf", "infinity", and "nan" which may not match the caller's strings.
	const std::size_t infLen = std::strlen(inf);
	const std::size_t nanLen = std::strlen(nan);
	if (plen == infLen && std::strncmp(p, inf, infLen) == 0)
		return negative ? -std::numeric_limits<T>::infinity() : std::numeric_limits<T>::infinity();
	if (plen == nanLen && std::strncmp(p, nan, nanLen) == 0)
		return std::numeric_limits<T>::quiet_NaN();

	// Reject strings that start with letters (from_chars would parse "inf"/"nan")
	if (plen > 0 && std::isalpha(static_cast<unsigned char>(*p)))
		return std::numeric_limits<T>::quiet_NaN();

	T result = std::numeric_limits<T>::quiet_NaN();
	auto [ptr, ec] = std::from_chars(str, end, result);
	if (ec != std::errc() || ptr != end)
		return std::numeric_limits<T>::quiet_NaN();
	return result;
}

} // namespace

float strToFloat(const char* str, const char* inf, const char* nan)
{
	return strToFloatImpl<float>(str, inf, nan);
}


double strToDouble(const char* str, const char* inf, const char* nan)
{
	return strToFloatImpl<double>(str, inf, nan);
}

#else // !POCO_HAS_FLOAT_CHARCONV

float strToFloat(const char* str, const char* inf, const char* nan)
{
	using namespace double_conversion;

	int processed;
	int flags = StringToDoubleConverter::ALLOW_LEADING_SPACES |
		StringToDoubleConverter::ALLOW_TRAILING_SPACES;
	StringToDoubleConverter converter(flags, 0.0, Single::NaN(), inf, nan);
	float result = converter.StringToFloat(str, static_cast<int>(strlen(str)), &processed);
	return result;
}


double strToDouble(const char* str, const char* inf, const char* nan)
{
	using namespace double_conversion;
	int processed;
	int flags = StringToDoubleConverter::ALLOW_LEADING_SPACES |
		StringToDoubleConverter::ALLOW_TRAILING_SPACES;
	StringToDoubleConverter converter(flags, 0.0, Double::NaN(), inf, nan);
	double result = converter.StringToDouble(str, static_cast<int>(strlen(str)), &processed);
	return result;
}

#endif // POCO_HAS_FLOAT_CHARCONV


bool strToFloat(const std::string& str, float& result, char decSep, char thSep, const char* inf, const char* nan)
{
	std::string tmp(str);
	trimInPlace(tmp);
	removeInPlace(tmp, thSep);
	removeInPlace(tmp, 'f');
	replaceInPlace(tmp, decSep, '.');
	result = strToFloat(tmp.c_str(), inf, nan);
	return !FPEnvironment::isInfinite(result) &&
		!FPEnvironment::isNaN(result);
}


bool strToDouble(const std::string& str, double& result, char decSep, char thSep, const char* inf, const char* nan)
{
	if (str.empty()) return false;

	std::string tmp(str);
	trimInPlace(tmp);
	removeInPlace(tmp, thSep);
	replaceInPlace(tmp, decSep, '.');
	removeInPlace(tmp, 'f');
	result = strToDouble(tmp.c_str(), inf, nan);
	return !FPEnvironment::isInfinite(result) &&
		!FPEnvironment::isNaN(result);
}


} // namespace Poco