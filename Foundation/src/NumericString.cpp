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
// Unity-build: .cc files are #included directly, not compiled separately.
// The namespace rename avoids symbol collisions if the application also
// links a standalone double-conversion library.
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
#include <cctype>
#include <cmath>


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

	const std::string::size_type ePos = str.find_first_of("eE");
	std::string eStr;
	if (ePos != std::string::npos)
	{
		eStr = str.substr(ePos);
		frac -= eStr.length();
		str.erase(ePos);
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

	if (!eStr.empty()) str += eStr;

	if (width && (str.length() < width)) str.insert(str.begin(), width - str.length(), prefix);
}


void insertThousandSep(std::string& str, char thSep, char decSep = '.')
	/// Inserts thousand separators.
	/// Used only internally.
{
	poco_assert (decSep != thSep);
	if (str.empty()) return;

	std::string::size_type exPos = str.find('e');
	if (exPos == std::string::npos) exPos = str.find('E');
	const std::string::size_type decPos = str.find(decSep);
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
	// Handle NaN, infinity, and -0 directly -- log10 is undefined for these.
	if (!std::isfinite(value) || value == T(0))
	{
		auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value);
		if (ec != std::errc()) { buffer[0] = '\0'; return; }
		*ptr = '\0';
		return;
	}

	// Compute the base-10 exponent robustly. std::floor(std::log10(x)) can
	// be off by one near exact powers of 10 due to floating-point rounding
	// (e.g. log10(1000.0) may return 2.999..., floor gives 2 instead of 3).
	// Verify with a power-of-10 multiplication to correct off-by-one.
	const T absVal = value < 0 ? -value : value;
	int exp10 = static_cast<int>(std::floor(std::log10(absVal)));
	// Correct off-by-one: if 10^(exp10+1) <= absVal, we underestimated
	T threshold = 1;
	for (int i = 0; i < exp10 + 1 && i < 20; ++i) threshold *= 10;
	if (threshold <= absVal) ++exp10;

	const bool useFixed = (exp10 >= lowDec && exp10 <= highDec);

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

/// Adjust a fixed-notation string to the requested precision by truncating
/// or zero-padding the fractional digits. Uses round-half-up rounding to
/// match double-conversion's behavior.
void adjustPrecision(char* buffer, char* end, int precision)
{
	char* dot = static_cast<char*>(std::memchr(buffer, '.', end - buffer));
	if (!dot)
	{
		if (precision <= 0) return; // no decimal point needed
		// Append decimal point and pad with zeros
		dot = end;
		*end++ = '.';
		for (int i = 0; i < precision; ++i) *end++ = '0';
		*end = '\0';
		return;
	}

	// First digit position (skip sign if present)
	char* const firstDigit = (buffer[0] == '-') ? buffer + 1 : buffer;

	// Propagate carry backward through digits, skipping '.' and '-'.
	// Returns true if carry overflows past the first digit.
	auto propagateCarry = [&](char* from) -> bool {
		for (char* p = from; p >= firstDigit; --p)
		{
			if (*p == '.') continue;
			if (*p < '9') { ++(*p); return false; }
			*p = '0';
		}
		return true; // carry past first digit
	};

	// Insert a leading '1' after the sign (if any), shifting digits right.
	auto insertLeadingOne = [&](char* upTo) {
		std::memmove(firstDigit + 1, firstDigit, upTo - firstDigit);
		*firstDigit = '1';
	};

	if (precision <= 0)
	{
		// Round to integer: check first fractional digit
		if (dot + 1 < end && *(dot + 1) >= '5')
		{
			if (propagateCarry(dot - 1))
			{
				insertLeadingOne(dot);
				++dot;
			}
		}
		*dot = '\0';
		return;
	}

	const int frac = static_cast<int>(end - dot - 1);
	if (frac <= precision)
	{
		// Pad with trailing zeros
		for (int i = frac; i < precision; ++i) *end++ = '0';
		*end = '\0';
	}
	else
	{
		// Truncate with round-half-up
		char* cutPos = dot + 1 + precision;
		if (*cutPos >= '5')
		{
			if (propagateCarry(cutPos - 1))
			{
				insertLeadingOne(cutPos);
				cutPos[1] = '\0';
				return;
			}
		}
		*cutPos = '\0';
	}
}

} // namespace

void floatToStr(char* buffer, int bufferSize, float value, int lowDec, int highDec)
{
	toShortestStr(buffer, bufferSize, value, lowDec, highDec);
}


// std::to_chars(fixed, precision) is 15-20% slower than std::to_chars(fixed)
// without precision on both Apple Clang and GCC (benchmarked 2M random values):
//   auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value, std::chars_format::fixed, precision);
// We use the faster no-precision overload and manually adjust to the requested
// precision with round-half-up rounding, matching double-conversion's behavior.
void floatToFixedStr(char* buffer, int bufferSize, float value, int precision)
{
	auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value, std::chars_format::fixed);
	if (ec != std::errc()) { buffer[0] = '\0'; return; }
	*ptr = '\0';
	adjustPrecision(buffer, ptr, precision);
}

#else // !POCO_HAS_FLOAT_CHARCONV

void floatToStr(char* buffer, int bufferSize, float value, int lowDec, int highDec)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO | DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP, lowDec, highDec, 0, 0);
	dc.ToShortestSingle(value, &builder);
	builder.Finalize();
}


void floatToFixedStr(char* buffer, int bufferSize, float value, int precision)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO | DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP, -std::numeric_limits<float>::digits10, std::numeric_limits<float>::digits10, 0, 0);
	dc.ToFixed(value, precision, &builder);
	builder.Finalize();
}

#endif // POCO_HAS_FLOAT_CHARCONV


namespace {

/// Common implementation for float/double-to-string overloads.
/// When precisionApplied is true (fixed-precision path), the char* overload
/// already produced the correct precision via adjustPrecision, so only
/// width-padding is applied. When false (shortest path), pad() handles
/// both precision and width.
template <typename T, typename CharConvFn>
std::string& floatToStrCommon(std::string& str, T value, int precision, int width,
	char thSep, char decSep, bool precisionApplied, CharConvFn charConvFn)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[POCO_MAX_FLT_STRING_LEN];
	charConvFn(buffer, POCO_MAX_FLT_STRING_LEN, value);
	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);

	if (precisionApplied)
	{
		if (width && str.length() < static_cast<std::size_t>(width))
			str.insert(str.begin(), width - str.length(), ' ');
	}
	else
	{
		if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	}
	return str;
}

} // namespace


std::string& floatToStr(std::string& str, float value, int precision, int width, char thSep, char decSep)
{
	return floatToStrCommon(str, value, precision, width, thSep, decSep, false,
		[](char* buf, int sz, float v) { floatToStr(buf, sz, v); });
}


std::string& floatToFixedStr(std::string& str, float value, int precision, int width, char thSep, char decSep)
{
	return floatToStrCommon(str, value, precision, width, thSep, decSep, true,
		[precision](char* buf, int sz, float v) { floatToFixedStr(buf, sz, v, precision); });
}


#ifdef POCO_HAS_FLOAT_CHARCONV

void doubleToStr(char* buffer, int bufferSize, double value, int lowDec, int highDec)
{
	toShortestStr(buffer, bufferSize, value, lowDec, highDec);
}


void doubleToFixedStr(char* buffer, int bufferSize, double value, int precision)
{
	auto [ptr, ec] = std::to_chars(buffer, buffer + bufferSize, value, std::chars_format::fixed);
	if (ec != std::errc()) { buffer[0] = '\0'; return; }
	*ptr = '\0';
	adjustPrecision(buffer, ptr, precision);
}

#else // !POCO_HAS_FLOAT_CHARCONV

void doubleToStr(char* buffer, int bufferSize, double value, int lowDec, int highDec)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO | DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP, lowDec, highDec, 0, 0);
	dc.ToShortest(value, &builder);
	builder.Finalize();
}


void doubleToFixedStr(char* buffer, int bufferSize, double value, int precision)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO | DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, POCO_FLT_INF, POCO_FLT_NAN, POCO_FLT_EXP,
			-std::numeric_limits<double>::digits10, std::numeric_limits<double>::digits10, 0, 0);
	dc.ToFixed(value, precision, &builder);
	builder.Finalize();
}

#endif // POCO_HAS_FLOAT_CHARCONV


std::string& doubleToStr(std::string& str, double value, int precision, int width, char thSep, char decSep)
{
	return floatToStrCommon(str, value, precision, width, thSep, decSep, false,
		[](char* buf, int sz, double v) { doubleToStr(buf, sz, v); });
}


std::string& doubleToFixedStr(std::string& str, double value, int precision, int width, char thSep, char decSep)
{
	return floatToStrCommon(str, value, precision, width, thSep, decSep, true,
		[precision](char* buf, int sz, double v) { doubleToFixedStr(buf, sz, v, precision); });
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

	const bool negative = (*str == '-');
	const char* const p = negative ? str + 1 : str;
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

	T result{};
	const auto [ptr, ec] = std::from_chars(str, end, result);
	if (ptr != end)
		return std::numeric_limits<T>::quiet_NaN();
	if (ec == std::errc())
		return result;
	// errc::result_out_of_range means underflow or overflow.
	// from_chars may leave result unmodified on range error (GCC behavior),
	// so fall back to strtod which correctly returns 0 or +/-HUGE_VAL.
	if (ec == std::errc::result_out_of_range)
		return static_cast<T>(std::strtod(str, nullptr));
	return std::numeric_limits<T>::quiet_NaN();
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


namespace {

/// Common implementation for string-to-float/double overloads.
/// Strips whitespace, thousand separators, and the 'f' suffix, replaces
/// the decimal separator, then delegates to the char* parsing overload.
/// The 'f' suffix stripping allows C/C++ float literals ("1.23f") to be
/// parsed. It is also applied for double parsing for backward compatibility.
template <typename T, typename ParseFn>
bool strToFloatImpl(const std::string& str, T& result, char decSep, char thSep,
	const char* inf, const char* nan, ParseFn parseFn)
{
	if (str.empty()) return false;

	std::string tmp(str);
	trimInPlace(tmp);
	removeInPlace(tmp, thSep);
	removeInPlace(tmp, 'f');
	replaceInPlace(tmp, decSep, '.');
	result = parseFn(tmp.c_str(), inf, nan);
	return !FPEnvironment::isInfinite(result) &&
		!FPEnvironment::isNaN(result);
}

} // namespace


bool strToFloat(const std::string& str, float& result, char decSep, char thSep, const char* inf, const char* nan)
{
	return strToFloatImpl(str, result, decSep, thSep, inf, nan,
		[](const char* s, const char* i, const char* n) -> float { return strToFloat(s, i, n); });
}


bool strToDouble(const std::string& str, double& result, char decSep, char thSep, const char* inf, const char* nan)
{
	return strToFloatImpl(str, result, decSep, thSep, inf, nan,
		[](const char* s, const char* i, const char* n) -> double { return strToDouble(s, i, n); });
}


} // namespace Poco