//
// NumberFormatter.h
//
// Library: Foundation
// Package: Core
// Module:  NumberFormatter
//
// Definition of the NumberFormatter class.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NumberFormatter_INCLUDED
#define Foundation_NumberFormatter_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/NumericString.h"


namespace Poco {


class Foundation_API NumberFormatter
	/// The NumberFormatter class provides static methods
	/// for formatting numeric values into strings.
	///
	/// There are two kind of static member functions:
	///    * format* functions return a std::string containing
	///      the formatted value.
	///    * append* functions append the formatted value to
	///      an existing string.
{
public:
	enum BoolFormat
	{
		FMT_TRUE_FALSE,
		FMT_YES_NO,
		FMT_ON_OFF
	};

	enum class Options
		/// Options to control the format of the generated string.
	{
		DEFAULT = 0,
			/// formatHex defaults: No 0x prefix, uppercase hexadecimal values
		PREFIX = 1 << 0,
			/// formatHex: Prepend prefix 0x
		LOWERCASE = 1 << 1
			/// formatHex: Use lowercase letters for hexadecimal values
	};

	static const unsigned NF_MAX_INT_STRING_LEN = 32; // increase for 64-bit binary formatting support
	static const unsigned NF_MAX_FLT_STRING_LEN = POCO_MAX_FLT_STRING_LEN;

	static std::string format(int value);
		/// Formats an integer value in decimal notation.

	static std::string format(int value, int width);
		/// Formats an integer value in decimal notation,
		/// right justified in a field having at least
		/// the specified width.

	static std::string format0(int value, int width);
		/// Formats an integer value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static std::string formatHex(int value, Options options = Options::DEFAULT);
		/// Formats an int value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.
		/// The value is treated as unsigned.

	static std::string formatHex(int value, int width, Options options = Options::DEFAULT);
		/// Formats an int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.
		/// The value is treated as unsigned.

	static std::string format(unsigned value);
		/// Formats an unsigned int value in decimal notation.

	static std::string format(unsigned value, int width);
		/// Formats an unsigned long int in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static std::string format0(unsigned int value, int width);
		/// Formats an unsigned int value in decimal notation,
		/// right justified and zero-padded in a field having at
		/// least the specified width.

	static std::string formatHex(unsigned value, Options options = Options::DEFAULT);
		/// Formats an unsigned int value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(unsigned value, int width, Options options = Options::DEFAULT);
		/// Formats an unsigned value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string format(long value);
		/// Formats a long value in decimal notation.

	static std::string format(long value, int width);
		/// Formats a long value in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static std::string format0(long value, int width);
		/// Formats a long value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static std::string formatHex(long value, Options options = Options::DEFAULT);
		/// Formats a long value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(long value, int width, Options options = Options::DEFAULT);
		/// Formats a long value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string format(unsigned long value);
		/// Formats an unsigned long value in decimal notation.

	static std::string format(unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified in a field having at least the specified
		/// width.

	static std::string format0(unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified and zero-padded
		/// in a field having at least the specified width.

	static std::string formatHex(unsigned long value, Options options = Options::DEFAULT);
		/// Formats an unsigned long value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(unsigned long value, int width, Options options = Options::DEFAULT);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

#ifdef POCO_HAVE_INT64
#ifdef POCO_INT64_IS_LONG

	static std::string format(long long value);
		/// Formats a 64-bit integer value in decimal notation.

	static std::string format(long long value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(long long value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static std::string formatHex(long long value, Options options = Options::DEFAULT);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(long long value, int width, Options options = Options::DEFAULT);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string format(unsigned long long value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static std::string format(unsigned long long value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(unsigned long long value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

	static std::string formatHex(unsigned long long value, Options options = Options::DEFAULT);
		/// Formats an unsigned 64-bit value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(unsigned long long value, int width, Options options = Options::DEFAULT);
		/// Formats an unsigned 64-bit value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

#else // ifndef POCO_INT64_IS_LONG

	static std::string format(Int64 value);
		/// Formats a 64-bit integer value in decimal notation.

	static std::string format(Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static std::string formatHex(Int64 value, Options options = Options::DEFAULT);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(Int64 value, int width, Options options = Options::DEFAULT);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string format(UInt64 value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static std::string format(UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

	static std::string formatHex(UInt64 value, Options options = Options::DEFAULT);
		/// Formats an unsigned 64-bit integer in hexadecimal notation.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

	static std::string formatHex(UInt64 value, int width, Options options = Options::DEFAULT);
		/// Formats an unsigned 64-bit integer in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// Options (see NumberFormatter::Options) define the format of the
		/// generated string.

#endif // ifdef POCO_INT64_IS_LONG
#endif // ifdef POCO_HAVE_INT64

	static std::string format(float value);
		/// Formats a float value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 8 fractional digits.

	static std::string format(float value, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static std::string format(float value, int width, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static std::string format(double value);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 16 fractional digits.

	static std::string format(double value, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static std::string format(double value, int width, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static std::string format(const void* ptr);
		/// Formats a pointer in an eight (32-bit architectures) or
		/// sixteen (64-bit architectures) characters wide
		/// field in hexadecimal notation.

	static std::string format(bool value, BoolFormat format = FMT_TRUE_FALSE);
		/// Formats a bool value in decimal/text notation,
		/// according to format parameter.

	static void append(std::string& str, int value);
		/// Formats an integer value in decimal notation.

	static void append(std::string& str, int value, int width);
		/// Formats an integer value in decimal notation,
		/// right justified in a field having at least
		/// the specified width.

	static void append0(std::string& str, int value, int width);
		/// Formats an integer value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static void appendHex(std::string& str, int value, bool lowercase = false);
		/// Formats an int value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, int value, int width, bool lowercase = false);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, unsigned value);
		/// Formats an unsigned int value in decimal notation.

	static void append(std::string& str, unsigned value, int width);
		/// Formats an unsigned long int in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static void append0(std::string& str, unsigned int value, int width);
		/// Formats an unsigned int value in decimal notation,
		/// right justified and zero-padded in a field having at
		/// least the specified width.

	static void appendHex(std::string& str, unsigned value, bool lowercase = false);
		/// Formats an unsigned int value in hexadecimal notation.

	static void appendHex(std::string& str, unsigned value, int width, bool lowercase = false);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.

	static void append(std::string& str, long value);
		/// Formats a long value in decimal notation.

	static void append(std::string& str, long value, int width);
		/// Formats a long value in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static void append0(std::string& str, long value, int width);
		/// Formats a long value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static void appendHex(std::string& str, long value, bool lowercase = false);
		/// Formats an unsigned long value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, long value, int width, bool lowercase = false);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, unsigned long value);
		/// Formats an unsigned long value in decimal notation.

	static void append(std::string& str, unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified in a field having at least the specified
		/// width.

	static void append0(std::string& str, unsigned long value, int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified and zero-padded
		/// in a field having at least the specified width.

	static void appendHex(std::string& str, unsigned long value, bool lowercase = false);
		/// Formats an unsigned long value in hexadecimal notation.

	static void appendHex(std::string& str, unsigned long value, int width, bool lowercase = false);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

#ifdef POCO_HAVE_INT64
#ifdef POCO_INT64_IS_LONG

	static void append(std::string& str, long long value);
		/// Formats a 64-bit integer value in decimal notation.

	static void append(std::string& str, long long value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, long long value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static void appendHex(std::string& str, long long value, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, long long value, int width, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, unsigned long long value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static void append(std::string& str, unsigned long long value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, unsigned long long value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

	static void appendHex(std::string& str, unsigned long long value, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation.

	static void appendHex(std::string& str, unsigned long long value, int width, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

#else // ifndef POCO_INT64_IS_LONG

	static void append(std::string& str, Int64 value);
		/// Formats a 64-bit integer value in decimal notation.

	static void append(std::string& str, Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, Int64 value, int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static void appendHex(std::string& str, Int64 value, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, Int64 value, int width, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, UInt64 value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static void append(std::string& str, UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, UInt64 value, int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

	static void appendHex(std::string& str, UInt64 value, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation.

	static void appendHex(std::string& str, UInt64 value, int width, bool lowercase = false);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

#endif // ifdef POCO_INT64_IS_LONG
#endif // ifdef POCO_HAVE_INT64

	static void append(std::string& str, float value);
		/// Formats a float value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 8 fractional digits.

	static void append(std::string& str, float value, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static void append(std::string& str, float value, int width, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static void append(std::string& str, double value);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 16 fractional digits.

	static void append(std::string& str, double value, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static void append(std::string& str, double value, int width, int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static void append(std::string& str, const void* ptr);
		/// Formats a pointer in an eight (32-bit architectures) or
		/// sixteen (64-bit architectures) characters wide
		/// field in hexadecimal notation.

//
// Deprecated functions
//

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(int value, bool prefix);
		/// Formats an int value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(int value, int width, bool prefix);
		/// Formats an int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(unsigned value, bool prefix);
		/// Formats an unsigned int value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(unsigned value, int width, bool prefix);
		/// Formats an unsigned value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(long value, bool prefix);
		/// Formats a long value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(long value, int width, bool prefix);
		/// Formats a long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(unsigned long value, bool prefix);
		/// Formats an unsigned long value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(unsigned long value, int width, bool prefix);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

#ifdef POCO_HAVE_INT64
#ifdef POCO_INT64_IS_LONG

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(long long value, bool prefix);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(long long value, int width, bool prefix);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// If prefix is true, "0x" prefix is prepended to the resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(unsigned long long value, bool prefix);
		/// Formats an unsigned 64-bit integer value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(unsigned long long value, int width, bool prefix);
		/// Formats an unsigned 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width. If prefix is true, "0x" prefix is
		/// prepended to the resulting string.

#else // ifndef POCO_INT64_IS_LONG

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(Int64 value, bool prefix);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(Int64 value, int width, bool prefix);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// If prefix is true, "0x" prefix is prepended to the resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(UInt64 value, bool prefix);
		/// Formats an unsigned 64-bit integer value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	[[deprecated("use formatHex with options instead")]]
	static std::string formatHex(UInt64 value, int width, bool prefix);
		/// Formats an unsigned 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width. If prefix is true, "0x" prefix is
		/// prepended to the resulting string.

#endif // ifdef POCO_INT64_IS_LONG
#endif // ifdef POCO_HAVE_INT64

private:

	static bool isEnabled(NumberFormatter::Options options, NumberFormatter::Options opt);
};


//
// inlines
//


inline NumberFormatter::Options operator | (NumberFormatter::Options lhs, NumberFormatter::Options rhs)
{
    using T = std::underlying_type_t<NumberFormatter::Options>;
    return static_cast<NumberFormatter::Options>(static_cast<T>(lhs) | static_cast<T>(rhs));
}


inline NumberFormatter::Options& operator |= (NumberFormatter::Options& lhs, NumberFormatter::Options rhs)
{
    lhs = lhs | rhs;
    return lhs;
}


inline NumberFormatter::Options operator & (NumberFormatter::Options lhs, NumberFormatter::Options rhs)
{
    using T = std::underlying_type_t<NumberFormatter::Options>;
    return static_cast<NumberFormatter::Options>(static_cast<T>(lhs) & static_cast<T>(rhs));
}


inline NumberFormatter::Options& operator &= (NumberFormatter::Options& lhs, NumberFormatter::Options rhs)
{
    lhs = lhs & rhs;
    return lhs;
}


inline bool NumberFormatter::isEnabled(Options options, Options opt)
{
	using T = std::underlying_type_t<Options>;
    return static_cast<T>(options & opt) != 0;
}


inline std::string NumberFormatter::format(int value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(int value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(int value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(int value, Options options)
{
	return formatHex(static_cast<unsigned int>(value), options);
}


inline std::string NumberFormatter::formatHex(int value, int width, Options options)
{
	return formatHex(static_cast<unsigned int>(value), width, options);
}


inline std::string NumberFormatter::format(unsigned value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(unsigned value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(unsigned int value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned value, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX),-1, ' ', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned value, int width, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), width, '0', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


inline std::string NumberFormatter::format(long value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(long value, Options options)
{
	return formatHex(static_cast<unsigned long>(value), options);
}


inline std::string NumberFormatter::formatHex(long value, int width, Options options)
{
	return formatHex(static_cast<unsigned long>(value), width, options);
}


inline std::string NumberFormatter::format(unsigned long value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(unsigned long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(unsigned long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned long value, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), -1, ' ', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned long value, int width, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), width, '0', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}

#ifdef POCO_HAVE_INT64
#ifdef POCO_INT64_IS_LONG


inline std::string NumberFormatter::format(long long value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(long long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(long long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(long long value, Options options)
{
	return formatHex(static_cast<unsigned long long>(value), options);
}


inline std::string NumberFormatter::formatHex(long long value, int width, Options options)
{
	return formatHex(static_cast<unsigned long long>(value), width, options);
}


inline std::string NumberFormatter::format(unsigned long long value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(unsigned long long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(unsigned long long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned long long value, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), -1, ' ', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


inline std::string NumberFormatter::formatHex(unsigned long long value, int width, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), width, '0', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


#else // ifndef POCO_LONG_IS_64_BIT


inline std::string NumberFormatter::format(Int64 value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(Int64 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(Int64 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(Int64 value, Options options)
{
	return formatHex(static_cast<UInt64>(value), options);
}


inline std::string NumberFormatter::formatHex(long long value, int width, Options options)
{
	return formatHex(static_cast<UInt64>(value), width, options);
}


inline std::string NumberFormatter::format(UInt64 value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(UInt64 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(UInt64 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(UInt64 value, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), -1, ' ', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


inline std::string NumberFormatter::formatHex(UInt64 value, int width, Options options)
{
	std::string result;
	intToStr(value, 0x10, result, isEnabled(options, Options::PREFIX), width, '0', 0, isEnabled(options, Options::LOWERCASE));
	return result;
}


#endif // ifdef POCO_INT64_IS_LONG
#endif // ifdef POCO_HAVE_INT64


inline std::string NumberFormatter::format(float value)
{
	char buffer[POCO_MAX_FLT_STRING_LEN];
	floatToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(float value, int precision)
{
	if (precision < 0)
		throw InvalidArgumentException("NumberFormatter::format() requires non-negative precision.");
	char buffer[POCO_MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, POCO_MAX_FLT_STRING_LEN, value, precision);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(float value, int width, int precision)
{
	if (precision < 0)
		throw InvalidArgumentException("NumberFormatter::format() requires non-negative precision.");
	std::string result;
	floatToFixedStr(result, value, precision, width);
	return result;
}


inline std::string NumberFormatter::format(double value)
{
	char buffer[POCO_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, value);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(double value, int precision)
{
	if (precision < 0)
		throw InvalidArgumentException("NumberFormatter::format() requires non-negative precision.");
	char buffer[POCO_MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, POCO_MAX_FLT_STRING_LEN, value, precision);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(double value, int width, int precision)
{
	if (precision < 0)
		throw InvalidArgumentException("NumberFormatter::format() requires non-negative precision.");
	std::string result;
	doubleToFixedStr(result, value, precision, width);
	return result;
}


inline std::string NumberFormatter::format(const void* ptr)
{
	std::string result;
	append(result, ptr);
	return result;
}


} // namespace Poco


#endif // Foundation_NumberFormatter_INCLUDED
