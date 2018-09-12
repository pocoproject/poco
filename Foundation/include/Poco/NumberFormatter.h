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

	static const unsigned NF_MAX_INT_STRING_LEN = 32; // increase for 64-bit binary formatting support
	static const unsigned NF_MAX_FLT_STRING_LEN = POCO_MAX_FLT_STRING_LEN;

	static std::string format(const int value);
		/// Formats an integer value in decimal notation.

	static std::string format(const int value, const int width);
		/// Formats an integer value in decimal notation,
		/// right justified in a field having at least
		/// the specified width.

	static std::string format0(const int value, const int width);
		/// Formats an integer value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static std::string formatHex(const int value, const bool prefix = false);
		/// Formats an int value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	static std::string formatHex(const int value, const int width, const bool prefix = false);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	static std::string format(const unsigned value);
		/// Formats an unsigned int value in decimal notation.

	static std::string format(const unsigned value, const int width);
		/// Formats an unsigned long int in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static std::string format0(const unsigned int value, const int width);
		/// Formats an unsigned int value in decimal notation,
		/// right justified and zero-padded in a field having at
		/// least the specified width.

	static std::string formatHex(const unsigned value, const bool prefix = false);
		/// Formats an unsigned int value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	static std::string formatHex(const unsigned value, const int width, const bool prefix = false);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

#ifndef POCO_LONG_IS_64_BIT

	static std::string format(const long value);
		/// Formats a long value in decimal notation.

	static std::string format(const long value, const int width);
		/// Formats a long value in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static std::string format0(const long value, const int width);
		/// Formats a long value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static std::string formatHex(const long value, const bool prefix = false);
		/// Formats an unsigned long value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	static std::string formatHex(const long value, const int width, const bool prefix = false);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	static std::string format(const unsigned long value);
		/// Formats an unsigned long value in decimal notation.

	static std::string format(const unsigned long value, const int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified in a field having at least the specified
		/// width.

	static std::string format0(const unsigned long value, const int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified and zero-padded
		/// in a field having at least the specified width.

	static std::string formatHex(const unsigned long value, const bool prefix = false);
		/// Formats an unsigned long value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	static std::string formatHex(const unsigned long value, const int width, const bool prefix = false);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

#endif // POCO_LONG_IS_64_BIT

	static std::string format(const Int64 value);
		/// Formats a 64-bit integer value in decimal notation.

	static std::string format(const Int64 value, const int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(const Int64 value, const int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static std::string formatHex(const Int64 value, const bool prefix = false);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.
		/// The value is treated as unsigned.

	static std::string formatHex(const Int64 value, const int width, const bool prefix = false);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.
		/// If prefix is true, "0x" prefix is prepended to the resulting string.

	static std::string format(const UInt64 value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static std::string format(const UInt64 value, const int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static std::string format0(const UInt64 value, const int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

	static std::string formatHex(const UInt64 value, const bool prefix = false);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// If prefix is true, "0x" prefix is prepended to the
		/// resulting string.

	static std::string formatHex(const UInt64 value, const int width, const bool prefix = false);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width. If prefix is true, "0x" prefix is
		/// prepended to the resulting string.

	static std::string format(const float value);
		/// Formats a float value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 8 fractional digits.

	static std::string format(const float value, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static std::string format(const float value, const int width, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static std::string format(const double value);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 16 fractional digits.

	static std::string format(const double value, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static std::string format(const double value, const int width, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static std::string format(const void* ptr);
		/// Formats a pointer in an eight (32-bit architectures) or
		/// sixteen (64-bit architectures) characters wide
		/// field in hexadecimal notation.

	static std::string format(const bool value, const BoolFormat format = FMT_TRUE_FALSE);
		/// Formats a bool value in decimal/text notation,
		/// according to format parameter.

	static void append(std::string& str, const int value);
		/// Formats an integer value in decimal notation.

	static void append(std::string& str, const int value, const int width);
		/// Formats an integer value in decimal notation,
		/// right justified in a field having at least
		/// the specified width.

	static void append0(std::string& str, const int value, const int width);
		/// Formats an integer value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static void appendHex(std::string& str, const int value);
		/// Formats an int value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, const int value, const int width);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, const unsigned value);
		/// Formats an unsigned int value in decimal notation.

	static void append(std::string& str, const unsigned value, const int width);
		/// Formats an unsigned long int in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static void append0(std::string& str, const unsigned int value, const int width);
		/// Formats an unsigned int value in decimal notation,
		/// right justified and zero-padded in a field having at
		/// least the specified width.

	static void appendHex(std::string& str, const unsigned value);
		/// Formats an unsigned int value in hexadecimal notation.

	static void appendHex(std::string& str, const unsigned value, const int width);
		/// Formats a int value in hexadecimal notation,
		/// right justified and zero-padded in
		/// a field having at least the specified width.

#ifndef POCO_LONG_IS_64_BIT

	static void append(std::string& str, const long value);
		/// Formats a long value in decimal notation.

	static void append(std::string& str, const long value, const int width);
		/// Formats a long value in decimal notation,
		/// right justified in a field having at least the
		/// specified width.

	static void append0(std::string& str, const long value, const int width);
		/// Formats a long value in decimal notation,
		/// right justified and zero-padded in a field
		/// having at least the specified width.

	static void appendHex(std::string& str, const long value);
		/// Formats an unsigned long value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, const long value, const int width);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, const unsigned long value);
		/// Formats an unsigned long value in decimal notation.

	static void append(std::string& str, const unsigned long value, const int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified in a field having at least the specified
		/// width.

	static void append0(std::string& str, const unsigned long value, const int width);
		/// Formats an unsigned long value in decimal notation,
		/// right justified and zero-padded
		/// in a field having at least the specified width.

	static void appendHex(std::string& str, const unsigned long value);
		/// Formats an unsigned long value in hexadecimal notation.

	static void appendHex(std::string& str, const unsigned long value, const int width);
		/// Formats an unsigned long value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

#endif // POCO_LONG_IS_64_BIT

	static void append(std::string& str, const Int64 value);
		/// Formats a 64-bit integer value in decimal notation.

	static void append(std::string& str, const Int64 value, const int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, const Int64 value, const int width);
		/// Formats a 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static void appendHex(std::string& str, const Int64 value);
		/// Formats a 64-bit integer value in hexadecimal notation.
		/// The value is treated as unsigned.

	static void appendHex(std::string& str, const Int64 value, const int width);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.
		/// The value is treated as unsigned.

	static void append(std::string& str, const UInt64 value);
		/// Formats an unsigned 64-bit integer value in decimal notation.

	static void append(std::string& str, const UInt64 value, const int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified in a field having at least the specified width.

	static void append0(std::string& str, const UInt64 value, const int width);
		/// Formats an unsigned 64-bit integer value in decimal notation,
		/// right justified and zero-padded in a field having at least the
		/// specified width.

	static void appendHex(std::string& str, const UInt64 value);
		/// Formats a 64-bit integer value in hexadecimal notation.

	static void appendHex(std::string& str, const UInt64 value, const int width);
		/// Formats a 64-bit integer value in hexadecimal notation,
		/// right justified and zero-padded in a field having at least
		/// the specified width.

	static void append(std::string& str, const float value);
		/// Formats a float value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 8 fractional digits.

	static void append(std::string& str, const float value, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static void append(std::string& str, const float value, const int width, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static void append(std::string& str, const double value);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %g format with a precision of 16 fractional digits.

	static void append(std::string& str, const double value, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// according to std::printf's %f format with the given precision.

	static void append(std::string& str, const double value, const int width, const int precision);
		/// Formats a double value in decimal floating-point notation,
		/// right justified in a field of the specified width,
		/// with the number of fractional digits given in precision.

	static void append(std::string& str, const void* ptr);
		/// Formats a pointer in an eight (32-bit architectures) or
		/// sixteen (64-bit architectures) characters wide
		/// field in hexadecimal notation.

private:
};


//
// inlines
//

inline std::string NumberFormatter::format(const int value)
{
	return intToStr(value, 10);
}


inline std::string NumberFormatter::format(const int value, const int width)
{
	return intToStr(value, 10, false, width, ' ');
}


inline std::string NumberFormatter::format0(const int value, const int width)
{
	return intToStr(value, 10, false, width, '0');
}


inline std::string NumberFormatter::formatHex(const int value, const bool prefix)
{
	return uIntToStr(static_cast<unsigned int>(value), 0x10, prefix);
}


inline std::string NumberFormatter::formatHex(const int value, const int width, const bool prefix)
{
	return uIntToStr(static_cast<unsigned int>(value), 0x10, prefix, width, '0');
}


inline std::string NumberFormatter::format(const unsigned value)
{
	return uIntToStr(value, 10);
}


inline std::string NumberFormatter::format(const unsigned value, const int width)
{
	return uIntToStr(value, 10, false, width, ' ');
}


inline std::string NumberFormatter::format0(const unsigned int value, const int width)
{
	return uIntToStr(value, 10, false, width, '0');
}


inline std::string NumberFormatter::formatHex(const unsigned value, const bool prefix)
{
	return uIntToStr(value, 0x10, prefix);
}


inline std::string NumberFormatter::formatHex(const unsigned value, const int width, const bool prefix)
{
	return uIntToStr(value, 0x10, prefix, width, '0');
}


#ifndef POCO_LONG_IS_64_BIT


inline std::string NumberFormatter::format(const long value)
{
	return intToStr(value, 10);
}


inline std::string NumberFormatter::format(const long value, const int width)
{
	return intToStr(value, 10, false, width, ' ');
}


inline std::string NumberFormatter::format0(const long value, const int width)
{
	return intToStr(value, 10, false, width, '0');
}


inline std::string NumberFormatter::formatHex(const long value, const bool prefix)
{
	return uIntToStr(static_cast<unsigned long>(value), 0x10, prefix);
}


inline std::string NumberFormatter::formatHex(const long value, const int width, const bool prefix)
{
	return uIntToStr(static_cast<unsigned long>(value), 0x10, prefix, width, '0');
}


inline std::string NumberFormatter::format(const unsigned long value)
{
	return uIntToStr(value, 10);
}


inline std::string NumberFormatter::format(const unsigned long value, const int width)
{
	return uIntToStr(value, 10, false, width, ' ');
}


inline std::string NumberFormatter::format0(const unsigned long value, const int width)
{
	return uIntToStr(value, 10, false, width, '0');
}


inline std::string NumberFormatter::formatHex(const unsigned long value, const bool prefix)
{
	return uIntToStr(value, 0x10, prefix);
}


inline std::string NumberFormatter::formatHex(const unsigned long value, const int width, const bool prefix)
{
	return uIntToStr(value, 0x10, prefix, width, '0');
}


#endif // POCO_LONG_IS_64_BIT


inline std::string NumberFormatter::format(const Int64 value)
{
	return intToStr(value, 10);
}


inline std::string NumberFormatter::format(const Int64 value, const int width)
{
	return intToStr(value, 10, false, width, ' ');
}


inline std::string NumberFormatter::format0(const Int64 value, const int width)
{
	return intToStr(value, 10, false, width, '0');
}


inline std::string NumberFormatter::formatHex(const Int64 value, const bool prefix)
{
	return uIntToStr(static_cast<UInt64>(value), 0x10, prefix);
}


inline std::string NumberFormatter::formatHex(const Int64 value, const int width, const bool prefix)
{
	return uIntToStr(static_cast<UInt64>(value), 0x10, prefix, width, '0');
}


inline std::string NumberFormatter::format(const UInt64 value)
{
	return uIntToStr(value, 10);
}


inline std::string NumberFormatter::format(const UInt64 value, const int width)
{
	return uIntToStr(value, 10, false, width, ' ');
}


inline std::string NumberFormatter::format0(const UInt64 value, const int width)
{
	return uIntToStr(value, 10, false, width, '0');
}


inline std::string NumberFormatter::formatHex(const UInt64 value, const bool prefix)
{
	return uIntToStr(value, 0x10, prefix);
}


inline std::string NumberFormatter::formatHex(UInt64 value, int width, bool prefix)
{
	return uIntToStr(value, 0x10, prefix, width, '0');
}


inline std::string NumberFormatter::format(const float value)
{
       return floatToStr(value);
}


inline std::string NumberFormatter::format(const float value, const int precision)
{
	return floatToStr(value, precision);
}


inline std::string NumberFormatter::format(const float value, const int width, const int precision)
{
	return floatToFixedStr(value, precision, width);
}


inline std::string NumberFormatter::format(const double value)
{
	return doubleToStr(value);
}


inline std::string NumberFormatter::format(const double value, const int precision)
{
	return doubleToFixedStr(value, precision);
}


inline std::string NumberFormatter::format(const double value, const int width, const int precision)
{
	return doubleToFixedStr(value, precision, width);
}


inline std::string NumberFormatter::format(const void* ptr)
{
	std::string result;
	append(result, ptr);
	return result;
}


} // namespace Poco


#endif // Foundation_NumberFormatter_INCLUDED
