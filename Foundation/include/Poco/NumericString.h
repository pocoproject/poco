//
// NumericString.h
//
// Library: Foundation
// Package: Core
// Module:  NumericString
//
// Numeric string utility functions.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NumericString_INCLUDED
#define Foundation_NumericString_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Buffer.h"
#include "Poco/FPEnvironment.h"
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif
#include <limits>
#include <cmath>
#include <cctype>
#if !defined(POCO_NO_LOCALE)
	#include <locale>
#endif


#ifdef POCO_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4146)
#endif // POCO_COMPILER_MSVC


// binary numbers are supported, thus 64 (bits) + 1 (string terminating zero)
#define POCO_MAX_INT_STRING_LEN 65
// value from strtod.cc (double_conversion::kMaxSignificantDecimalDigits)
#define POCO_MAX_FLT_STRING_LEN 780

#define POCO_FLT_INF "inf"
#define POCO_FLT_NAN "nan"
#define POCO_FLT_EXP 'e'


namespace Poco {


namespace Impl {

	template<bool SIGNED, typename T>
	class IsNegativeImpl;

	template<typename T>
	class IsNegativeImpl<true, T>
	{
	public:
		bool operator()(T x) { return x < 0; }
	};

	template<typename T>
	class IsNegativeImpl<false, T>
	{
	public:
		bool operator()(T /*x*/) { return false; }
	};

}


template<typename T>
inline bool isNegative(T x)
{
	using namespace Impl;
	return IsNegativeImpl<std::numeric_limits<T>::is_signed, T>()(x);
}


template<typename To, typename From>
inline bool isIntOverflow(From val)
{
	poco_assert_dbg (std::numeric_limits<From>::is_integer);
	poco_assert_dbg (std::numeric_limits<To>::is_integer);
	bool ret;
	if (std::numeric_limits<To>::is_signed)
	{
		ret = (!std::numeric_limits<From>::is_signed &&
			  (std::uintmax_t)val > (std::uintmax_t)INTMAX_MAX) ||
			  (std::intmax_t)val  < (std::intmax_t)std::numeric_limits<To>::min() ||
			  (std::intmax_t)val  > (std::intmax_t)std::numeric_limits<To>::max();
	}
	else
	{
		ret = isNegative(val) ||
				(std::uintmax_t)val > (std::uintmax_t)std::numeric_limits<To>::max();
	}
	return ret;
}


template <typename F, typename T>
inline T& isSafeIntCast(F from)
	/// Returns true if it is safe to cast
	/// integer from F to T.
{
	if (!isIntOverflow<T, F>(from)) return true;
	return false;
}


template <typename F, typename T>
inline T& safeIntCast(F from, T& to)
	/// Returns csted value if it is safe
	/// to cast integer from F to T,
	/// otherwise throws BadCastException.
{
	if (!isIntOverflow<T, F>(from))
	{
		to = static_cast<T>(from);
		return to;
	}
	throw BadCastException("safeIntCast: Integer overflow");
}

inline char decimalSeparator()
	/// Returns decimal separator from global locale or
	/// default '.' for platforms where locale is unavailable.
{
#if !defined(POCO_NO_LOCALE)
	return std::use_facet<std::numpunct<char> >(std::locale()).decimal_point();
#else
	return '.';
#endif
}


inline char thousandSeparator()
	/// Returns thousand separator from global locale or
	/// default ',' for platforms where locale is unavailable.
{
#if !defined(POCO_NO_LOCALE)
	return std::use_facet<std::numpunct<char> >(std::locale()).thousands_sep();
#else
	return ',';
#endif
}


//
// String to Number Conversions
//

template <typename I>
bool strToInt(const char* pStr, I& outResult, short base, char thSep = ',')
	/// Converts zero-terminated character array to integer number;
	/// Thousand separators are recognized for base10 and current locale;
	/// they are silently skipped and not verified for correct positioning.
	/// It is not allowed to convert a negative number to unsigned integer.
	///
	/// Function returns true if successful. If parsing was unsuccessful,
	/// the return value is false with the result value undetermined.
{
	poco_assert_dbg (base == 2 || base == 8 || base == 10 || base == 16);

	if (!pStr) return false;
	while (std::isspace(*pStr)) ++pStr;
	if (*pStr == '\0') return false;
	bool negative = false;
	if ((base == 10) && (*pStr == '-'))
	{
		if (!std::numeric_limits<I>::is_signed) return false;
		negative = true;
		++pStr;
	}
	else if (*pStr == '+') ++pStr;

	// all numbers are parsed as positive; the sign
	// for negative numbers is adjusted after parsing
	::uintmax_t limitCheck = std::numeric_limits<I>::max();
	if (negative)
	{
		poco_assert_dbg(std::numeric_limits<I>::is_signed);
		// to cover the entire range, (-min > max) has to be
		// taken into account;
		// to avoid overflow for the largest int size,
		// we resort to ::copysign() (ie. floating-point)
		if (sizeof(I) == sizeof(::intmax_t))
			limitCheck = static_cast<::uintmax_t>(::copysign(std::numeric_limits<I>::min(), 1));
		else
		{
			::intmax_t i = std::numeric_limits<I>::min();
			limitCheck = -i;
		}
	}

	std::uintmax_t result = 0;
	for (; *pStr != '\0'; ++pStr)
	{
		if  (result > (limitCheck / base)) return false;
		switch (*pStr)
		{
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
			{
				char add = (*pStr - '0');
				if ((limitCheck - result) < add) return false;
				result = result * base + add;
			}
			break;

		case '8': case '9':
			if ((base == 10) || (base == 0x10))
			{
				char  add = (*pStr - '0');
				if ((limitCheck - result) < add) return false;
				result = result * base + add;
			}
			else return false;

			break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			{
				if (base != 0x10) return false;
				char  add = (*pStr - 'a');
				if ((limitCheck - result) < add) return false;
				result = result * base + (10 + add);
			}
			break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			{
				if (base != 0x10) return false;
				char add = (*pStr - 'A');
				if ((limitCheck - result) < add) return false;
				result = result * base + (10 + add);
			}
			break;

		case '.':
			if ((base == 10) && (thSep == '.')) break;
			else return false;

		case ',':
			if ((base == 10) && (thSep == ',')) break;
			else return false;

		case ' ':
			if ((base == 10) && (thSep == ' ')) break;

		default:
			return false;
		}
	}

	if (negative && (base == 10))
	{
		poco_assert_dbg(std::numeric_limits<I>::is_signed);
		::intmax_t i;
		if (sizeof(I) == sizeof(::intmax_t))
			i = static_cast<::intmax_t>(::copysign(result, -1));
		else
			i = static_cast<::intmax_t>(-result);
		if (isIntOverflow<I>(i)) return false;
		outResult = static_cast<I>(i);
	}
	else
	{
		if (isIntOverflow<I>(result)) return false;
		outResult = static_cast<I>(result);
	}

	return true;
}


template <typename I>
bool strToInt(const std::string& str, I& result, short base, char thSep = ',')
	/// Converts string to integer number;
	/// This is a wrapper function, for details see see the
	/// bool strToInt(const char*, I&, short, char) implementation.
{
	return strToInt(str.c_str(), result, base, thSep);
}


//
// Number to String Conversions
//

namespace Impl {

	class Ptr
		/// Utility char pointer wrapper class.
		/// Class ensures increment/decrement remain within boundaries.
	{
	public:
		Ptr(char* ptr, std::size_t offset): _beg(ptr), _cur(ptr), _end(ptr + offset)
		{
		}
	
		char*& operator ++ () // prefix
		{
			checkBounds(_cur + 1);
			return ++_cur;
		}

		char* operator ++ (int) // postfix
		{
			checkBounds(_cur + 1);
			char* tmp = _cur++;
			return tmp;
		}
	
		char*& operator -- () // prefix
		{
			checkBounds(_cur - 1);
			return --_cur;
		}

		char* operator -- (int) // postfix
		{
			checkBounds(_cur - 1);
			char* tmp = _cur--;
			return tmp;
		}

		char*& operator += (int incr)
		{
			checkBounds(_cur + incr);
			return _cur += incr;
		}

		char*& operator -= (int decr)
		{
			checkBounds(_cur - decr);
			return _cur -= decr;
		}

		operator char* () const
		{
			return _cur;
		}

		std::size_t span() const
		{
			return _end - _beg;
		}

	private:
		void checkBounds(char* ptr)
		{
			if (ptr > _end) throw RangeException();
		}

		const char* _beg;
		char*       _cur;
		const char* _end;
};	

} // namespace Impl


template <typename T>
bool intToStr(T value,
	unsigned short base,
	char* result,
	std::size_t& size,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0)
	/// Converts integer to string. Numeric bases from binary to hexadecimal are supported.
	/// If width is non-zero, it pads the return value with fill character to the specified width.
	/// When padding is zero character ('0'), it is prepended to the number itself; all other
	/// paddings are prepended to the formatted result with minus sign or base prefix included
	/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal,
	/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
	/// Formatted string has at least [width] total length.
{
	if (base < 2 || base > 0x10)
	{
		*result = '\0';
		return false;
	}

	Impl::Ptr ptr(result, size);
	int thCount = 0;
	T tmpVal;
	do
	{
		tmpVal = value;
		value /= base;
		*ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
		if (thSep && (base == 10) && (++thCount == 3))
		{
			*ptr++ = thSep;
			thCount = 0;
		}
	} while (value);

	if ('0' == fill)
	{
		if (tmpVal < 0) --width;
		if (prefix && base == 010) --width;
		if (prefix && base == 0x10) width -= 2;
		while ((ptr - result) < width) *ptr++ = fill;
	}

	if (prefix && base == 010) *ptr++ = '0';
	else if (prefix && base == 0x10)
	{
		*ptr++ = 'x';
		*ptr++ = '0';
	}

	if (tmpVal < 0) *ptr++ = '-';

	if ('0' != fill)
	{
		while ((ptr - result) < width) *ptr++ = fill;
	}

	size = ptr - result;
	poco_assert_dbg (size <= ptr.span());
	poco_assert_dbg ((-1 == width) || (size >= size_t(width)));
	*ptr-- = '\0';

	char* ptrr = result;
	char tmp;
	while(ptrr < ptr)
	{
		 tmp    = *ptr;
		*ptr--  = *ptrr;
		*ptrr++ = tmp;
	}

	return true;
}


template <typename T>
bool uIntToStr(T value,
	unsigned short base,
	char* result,
	std::size_t& size,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0)
	/// Converts unsigned integer to string. Numeric bases from binary to hexadecimal are supported.
	/// If width is non-zero, it pads the return value with fill character to the specified width.
	/// When padding is zero character ('0'), it is prepended to the number itself; all other
	/// paddings are prepended to the formatted result with minus sign or base prefix included
	/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal,
	/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
	/// Formatted string has at least [width] total length.
{
	if (base < 2 || base > 0x10)
	{
		*result = '\0';
		return false;
	}
	
	Impl::Ptr ptr(result, size);
	int thCount = 0;
	T tmpVal;
	do
	{
		tmpVal = value;
		value /= base;
		*ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
		if (thSep && (base == 10) && (++thCount == 3))
		{
			*ptr++ = thSep;
			thCount = 0;
		}
	} while (value);
	
	if ('0' == fill)
	{
		if (prefix && base == 010) --width;
		if (prefix && base == 0x10) width -= 2;
		while ((ptr - result) < width) *ptr++ = fill;
	}
	
	if (prefix && base == 010) *ptr++ = '0';
	else if (prefix && base == 0x10)
	{
		*ptr++ = 'x';
		*ptr++ = '0';
	}
	
	if ('0' != fill)
	{
		while ((ptr - result) < width) *ptr++ = fill;
	}
	
	size = ptr - result;
	poco_assert_dbg (size <= ptr.span());
	poco_assert_dbg ((-1 == width) || (size >= size_t(width)));
	*ptr-- = '\0';
	
	char* ptrr = result;
	char tmp;
	while(ptrr < ptr)
	{
		tmp    = *ptr;
		*ptr--  = *ptrr;
		*ptrr++ = tmp;
	}
	
	return true;
}

template <typename T>
bool intToStr(T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
  /// Converts integer to string; This is a wrapper function, for details see see the
  /// bool intToStr(T, unsigned short, char*, int, int, char, char) implementation.
{
  char res[POCO_MAX_INT_STRING_LEN] = { 0 };
  std::size_t size = POCO_MAX_INT_STRING_LEN;
  bool ret = intToStr(number, base, res, size, prefix, width, fill, thSep);
  result.assign(res, size);
  return ret;
}


template <typename T>
std::string intToStr (T number, unsigned short base, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
	/// Converts integer to string; This is a wrapper function, for details see see the
	/// bool intToStr(T, unsigned short, char*, int, int, char, char) implementation.
{
	std::string result;
	char res[POCO_MAX_INT_STRING_LEN] = {0};
	std::size_t size = POCO_MAX_INT_STRING_LEN;
	const bool ret = intToStr(number, base, res, size, prefix, width, fill, thSep);
	if (ret) result.assign(res, size);
	return result;
}

template <typename T>
bool uIntToStr(T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
  /// Converts unsigned integer to string; This is a wrapper function, for details see see the
  /// bool uIntToStr(T, unsigned short, char*, int, int, char, char) implementation.
{
  char res[POCO_MAX_INT_STRING_LEN] = { 0 };
  std::size_t size = POCO_MAX_INT_STRING_LEN;
  bool ret = uIntToStr(number, base, res, size, prefix, width, fill, thSep);
  result.assign(res, size);
  return ret;
}
	
	
template <typename T>
std::string uIntToStr (T number, unsigned short base, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
	/// Converts unsigned integer to string; This is a wrapper function, for details see see the
	/// bool uIntToStr(T, unsigned short, char*, int, int, char, char) implementation.
{
	std::string result;
	char res[POCO_MAX_INT_STRING_LEN] = {0};
	std::size_t size = POCO_MAX_INT_STRING_LEN;
	const bool ret = uIntToStr(number, base, res, size, prefix, width, fill, thSep);
	if (ret) result.assign(res, size);
	return result;
}


//
// Wrappers for double-conversion library (http://code.google.com/p/double-conversion/).
//
// Library is the implementation of the algorithm described in Florian Loitsch's paper:
// http://florian.loitsch.com/publications/dtoa-pldi2010.pdf
//


Foundation_API void floatToStr(char* buffer,
	int bufferSize,
	float value,
	int lowDec = -std::numeric_limits<float>::digits10,
	int highDec = std::numeric_limits<float>::digits10);
	/// Converts a float value to string. Converted string must be shorter than bufferSize.
	/// Conversion is done by computing the shortest string of digits that correctly represents
	/// the input number. Depending on lowDec and highDec values, the function returns
	/// decimal or exponential representation.

Foundation_API void floatToFixedStr(char* buffer,
	int bufferSize,
	float value,
	int precision);
	/// Converts a float value to string. Converted string must be shorter than bufferSize.
	/// Computes a decimal representation with a fixed number of digits after the
  	/// decimal point.


Foundation_API std::string& floatToStr(std::string& str,
	float value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);
	/// Converts a float value, assigns it to the supplied string and returns the reference.
	/// This function calls floatToStr(char*, int, float, int, int) and formats the result according to
	/// precision (total number of digits after the decimal point, -1 means ignore precision argument)
	/// and width (total length of formatted string).


Foundation_API std::string floatToStr(float value,
  int precision = -1,
  int width = 0,
  char thSep = 0,
  char decSep = 0);
  /// Converts a float value, returns it to the supplied string.
  /// This function calls floatToStr(char*, int, float, int, int) and formats the result according to
  /// precision (total number of digits after the decimal point, -1 means ignore precision argument)
  /// and width (total length of formatted string).


Foundation_API std::string& floatToFixedStr(std::string& str,
	float value,
	int precision,
	int width = 0,
	char thSep = 0,
	char decSep = 0);
	/// Converts a float value, assigns it to the supplied string and returns the reference.
	/// This function calls floatToFixedStr(char*, int, float, int) and formats the result according to
	/// precision (total number of digits after the decimal point) and width (total length of formatted string).


Foundation_API std::string floatToFixedStr(float value,
  int precision,
  int width = 0,
  char thSep = 0,
  char decSep = 0);
  /// Converts a float value, returns it to the supplied string.
  /// This function calls floatToFixedStr(char*, int, float, int) and formats the result according to
  /// precision (total number of digits after the decimal point) and width (total length of formatted string).


Foundation_API void doubleToStr(char* buffer,
	int bufferSize,
	double value,
	int lowDec = -std::numeric_limits<double>::digits10,
	int highDec = std::numeric_limits<double>::digits10);
	/// Converts a double value to string. Converted string must be shorter than bufferSize.
	/// Conversion is done by computing the shortest string of digits that correctly represents
	/// the input number. Depending on lowDec and highDec values, the function returns
	/// decimal or exponential representation.


Foundation_API void doubleToFixedStr(char* buffer,
	int bufferSize,
	double value,
	int precision);
	/// Converts a double value to string. Converted string must be shorter than bufferSize.
	/// Computes a decimal representation with a fixed number of digits after the
  	/// decimal point.


Foundation_API std::string& doubleToStr(std::string& str,
	double value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);
	/// Converts a double value, assigns it to the supplied string and returns the reference.
	/// This function calls doubleToStr(char*, int, double, int, int) and formats the result according to
	/// precision (total number of digits after the decimal point, -1 means ignore precision argument)
	/// and width (total length of formatted string).


Foundation_API std::string doubleToStr(double value,
  int precision = -1,
  int width = 0,
  char thSep = 0,
  char decSep = 0);
  /// Converts a double value, returns it to the supplied string.
  /// This function calls doubleToStr(char*, int, double, int, int) and formats the result according to
  /// precision (total number of digits after the decimal point, -1 means ignore precision argument)
  /// and width (total length of formatted string).


Foundation_API std::string& doubleToFixedStr(std::string& str,
	double value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);
	/// Converts a double value, assigns it to the supplied string and returns the reference.
	/// This function calls doubleToFixedStr(char*, int, double, int) and formats the result according to
	/// precision (total number of digits after the decimal point) and width (total length of formatted string).


Foundation_API std::string doubleToFixedStr(double value,
  int precision = -1,
  int width = 0,
  char thSep = 0,
  char decSep = 0);
  /// Converts a double value, returns it to the supplied string.
  /// This function calls doubleToFixedStr(char*, int, double, int) and formats the result according to
  /// precision (total number of digits after the decimal point) and width (total length of formatted string).


Foundation_API float strToFloat(const char* str,
	const char* inf = POCO_FLT_INF, const char* nan = POCO_FLT_NAN);
	/// Converts the string of characters into single-precision floating point number.
	/// Function uses double_convesrion::DoubleToStringConverter to do the conversion.


Foundation_API bool strToFloat(const std::string&, float& result,
	char decSep = '.', char thSep = ',',
	const char* inf = POCO_FLT_INF, const char* nan = POCO_FLT_NAN);
	/// Converts the string of characters into single-precision floating point number.
	/// The conversion result is assigned to the result parameter.
	/// If decimal separator and/or thousand separator are different from defaults, they should be
	/// supplied to ensure proper conversion.
	///
	/// Returns true if successful, false otherwise.


Foundation_API double strToDouble(const char* str,
	const char* inf = POCO_FLT_INF, const char* nan = POCO_FLT_NAN);
	/// Converts the string of characters into double-precision floating point number.


Foundation_API bool strToDouble(const std::string& str, double& result,
	char decSep = '.', char thSep = ',',
	const char* inf = POCO_FLT_INF, const char* nan = POCO_FLT_NAN);
	/// Converts the string of characters into double-precision floating point number.
	/// The conversion result is assigned to the result parameter.
	/// If decimal separator and/or thousand separator are different from defaults, they should be
	/// supplied to ensure proper conversion.
	///
	/// Returns true if successful, false otherwise.


} // namespace Poco


#ifdef POCO_COMPILER_MSVC
#pragma warning(pop)
#endif // POCO_COMPILER_MSVC


#endif // Foundation_NumericString_INCLUDED
