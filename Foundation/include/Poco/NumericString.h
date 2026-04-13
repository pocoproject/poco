//
// NumericString.h
//
// Library: Foundation
// Package: Core
// Module:  NumericString
//
// Numeric string utility functions.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NumericString_INCLUDED
#define Foundation_NumericString_INCLUDED


#ifndef POCO_NUMERIC_STRING_PRIVATE
	#error "Do not include Poco/NumericString.h directly. Use Poco/NumberFormatter.h or Poco/NumberParser.h instead."
#endif


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/FPEnvironment.h"
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif
#include <cctype>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <limits>
#if !defined(POCO_NO_LOCALE)
	#include <locale>
#endif
#include <string>
#include <type_traits>

/// Maximum length of an integer formatted as a string.
/// 64 binary digits + sign + NUL + padding margin = 67.
inline constexpr int POCO_MAX_INT_STRING_LEN = 67;

/// Maximum length of a floating-point formatted string.
/// Matches double_conversion::kMaxSignificantDecimalDigits.
inline constexpr int POCO_MAX_FLT_STRING_LEN = 780;

inline constexpr char POCO_FLT_INF[] = "inf";
inline constexpr char POCO_FLT_NAN[] = "nan";
inline constexpr char POCO_FLT_EXP = 'e';


namespace Poco {


template<typename T>
inline bool isNegative(T x)
{
	if constexpr (std::is_signed_v<T>)
		return x < 0;
	else
		return false;
}


template<typename To, typename From>
inline bool isIntOverflow(From val)
{
	poco_assert_dbg (std::numeric_limits<From>::is_integer);
	poco_assert_dbg (std::numeric_limits<To>::is_integer);
	if constexpr (std::numeric_limits<To>::is_signed)
	{
		return (!std::numeric_limits<From>::is_signed &&
			  (uintmax_t)val > (uintmax_t)INTMAX_MAX) ||
			  (intmax_t)val  < (intmax_t)std::numeric_limits<To>::min() ||
			  (intmax_t)val  > (intmax_t)std::numeric_limits<To>::max();
	}
	else
	{
		return isNegative(val) ||
				(uintmax_t)val > (uintmax_t)std::numeric_limits<To>::max();
	}
}


template<typename R, typename F, typename S>
[[nodiscard]] bool safeMultiply(R& result, F f, S s)
{
	using CT = std::common_type_t<R, F, S>;
	auto cast = [](auto v) { return static_cast<CT>(v); };

	if ((f == 0) || (s==0))
	{
		result = 0;
		return true;
	}

	if constexpr (std::is_signed_v<F> || std::is_signed_v<S>)
	{
		if (f > 0)
		{
			if (s > 0)
			{
				if (cast(f) > (cast(std::numeric_limits<R>::max()) / cast(s)))
					return false;
			}
			else
			{
				if (cast(s) < (cast(std::numeric_limits<R>::min()) / cast(f)))
					return false;
			}
		}
		else
		{
			if (s > 0)
			{
				if (cast(f) < (cast(std::numeric_limits<R>::min()) / cast(s)))
					return false;
			}
			else
			{
				if (cast(s) < (cast(std::numeric_limits<R>::max()) / cast(f)))
					return false;
			}
		}
	}
	else
	{
		// Both f and s are unsigned (and non-zero at this point)
		if (cast(f) > (cast(std::numeric_limits<R>::max()) / cast(s)))
			return false;
	}
	result = f * s;
	return true;
}


template <typename To, typename From>
[[nodiscard]] inline bool isSafeIntCast(From from)
	/// Returns true if it is safe to cast
	/// integer from From to To.
{
	return !isIntOverflow<To, From>(from);
}


template <typename To, typename From>
inline To& safeIntCast(From from, To& to)
	/// Returns cast value if it is safe
	/// to cast integer from From to To,
	/// otherwise throws BadCastException.
{
	if (!isIntOverflow<To, From>(from))
	{
		to = static_cast<To>(from);
		return to;
	}
	throw BadCastException("safeIntCast: Integer overflow");
}

inline char decimalSeparator()
	/// Returns decimal separator from global locale or
	/// default '.' for platforms where locale is unavailable.
{
#if !defined(POCO_NO_LOCALE)
	return std::use_facet<std::numpunct<char>>(std::locale()).decimal_point();
#else
	return '.';
#endif
}


inline char thousandSeparator()
	/// Returns thousand separator from global locale or
	/// default ',' for platforms where locale is unavailable.
{
#if !defined(POCO_NO_LOCALE)
	return std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep();
#else
	return ',';
#endif
}


//
// String to Number Conversions
//

template <typename I>
[[nodiscard]] bool strToInt(const char* begin, const char* end, I& outResult, short base, char thSep = ',')
	/// Converts character range [begin, end) to integer number;
	/// begin must point past any leading whitespace.
	/// Thousand separators are recognized for base10 and current locale;
	/// they are silently skipped and not verified for correct positioning.
	/// It is not allowed to convert a negative number to anything except
	/// 10-base signed integer.
	/// For hexadecimal numbers, the case of the digits is not relevant.
	///
	/// Function returns true if successful. If parsing was unsuccessful,
	/// the return value is false with the result value undetermined.
{
	poco_assert_dbg (base == 2 || base == 8 || base == 10 || base == 16);

	if (begin >= end) return false;
	if ((*begin == '-') && ((base != 10) || std::is_unsigned_v<I>))
		return false;

	// Skip '+' sign (std::from_chars doesn't accept it)
	const char* start = begin;
	if (*start == '+') ++start;
	if (start >= end) return false; // reject bare sign without digits

	// Try std::from_chars -- handles sign, overflow, and all bases.
	const auto [ptr, ec] = std::from_chars(start, end, outResult, base);
	if (ec == std::errc() && ptr == end)
		return true;

	// If from_chars reported overflow, invalid input, or no thSep is configured,
	// there's nothing the slow path can do differently.
	if (ec != std::errc() || !thSep || base != 10)
		return false;

	// from_chars consumed some digits then stopped at a non-digit character.
	// The input may contain thousand separators -- strip them and retry.
	char cleaned[POCO_MAX_INT_STRING_LEN];
	char* dst = cleaned;
	for (const char* src = start; src < end; ++src)
	{
		if (*src != thSep)
		{
			if (static_cast<std::size_t>(dst - cleaned) >= sizeof(cleaned) - 1)
				return false; // input too long
			*dst++ = *src;
		}
	}

	if (dst == cleaned) return false; // nothing left after stripping

	const auto [ptr2, ec2] = std::from_chars(cleaned, dst, outResult, base);
	return ec2 == std::errc() && ptr2 == dst;
}


template <typename I>
[[nodiscard]] bool strToInt(const char* pStr, I& outResult, short base, char thSep = ',')
	/// Converts zero-terminated character array to integer number.
	/// Skips leading whitespace, then delegates to the range-based overload.
{
	if (!pStr) return false;
	while (std::isspace(static_cast<unsigned char>(*pStr))) ++pStr;
	return strToInt(pStr, pStr + std::strlen(pStr), outResult, base, thSep);
}


template <typename I>
[[nodiscard]] bool strToInt(const std::string& str, I& result, short base, char thSep = ',')
	/// Converts string to integer number.
	/// Avoids strlen by using the known string length directly.
{
	const char* const end = str.data() + str.size();
	const char* begin = str.data();
	while (begin < end && std::isspace(static_cast<unsigned char>(*begin))) ++begin;
	return strToInt(begin, end, result, base, thSep);
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

/// Digit-pairs lookup table for the "two-digits-at-a-time" integer-to-string
/// technique. Instead of extracting one digit per iteration (value / 10),
/// this approach extracts two digits at once (value / 100) and uses the
/// remainder (0-99) as an index into this 200-byte table, halving the
/// number of expensive integer divisions.
///
/// The technique is widely used in high-performance formatting libraries
/// including {fmt}, jemalloc, and Facebook Folly.
/// See also: https://github.com/miloyip/itoa-benchmark
inline constexpr char kDigitPairs[201] =
	"00010203040506070809"
	"10111213141516171819"
	"20212223242526272829"
	"30313233343536373839"
	"40414243444546474849"
	"50515253545556575859"
	"60616263646566676869"
	"70717273747576777879"
	"80818283848586878889"
	"90919293949596979899";

/// Direct digit tables for non-base-10 paths
inline constexpr char kDigitsUpper[] = "0123456789ABCDEF";
inline constexpr char kDigitsLower[] = "0123456789abcdef";

} // namespace Impl


template <typename T>
[[nodiscard]] bool intToStr(T value,
	unsigned short base,
	char* result,
	std::size_t& size,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0,
	bool lowercase = false)
	/// Converts integer to string. Standard numeric bases from binary to hexadecimal
	/// are supported.
	/// If width is non-zero, it pads the return value with fill character to the specified width.
	/// When padding is zero character ('0'), it is prepended to the number itself; all other
	/// paddings are prepended to the formatted result with minus sign or base prefix included
	/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal,
	/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
	/// Formatted string has at least [width] total length.
{
	if constexpr (std::is_signed_v<T>)
	{
		poco_assert_dbg(((value < 0) && (base == 10)) || (value >= 0));
	}

	if (base < 2 || base > 0x10)
	{
		*result = '\0';
		return false;
	}

	// size is used as buffer capacity (in) and written length (out).
	const std::size_t capacity = size;
	// Guard against buffer overflow in padding loops (phases 3-5).
	if (width >= static_cast<int>(capacity))
		throw RangeException();

	// --- Phase 1: extract sign, convert to unsigned ---
	// This avoids UB for T_MIN (e.g. -2^63) where -value overflows signed.
	using U = std::make_unsigned_t<T>;

	[[maybe_unused]] bool negative = false;
	U uval;
	if constexpr (std::is_signed_v<T>)
	{
		if (value < 0)
		{
			negative = true;
			// Two-step cast avoids signed overflow UB for T_MIN:
			// cast to unsigned first, then negate in unsigned arithmetic.
			uval = static_cast<U>(0) - static_cast<U>(value);
		}
		else
		{
			uval = static_cast<U>(value);
		}
	}
	else
	{
		uval = value;
	}

	// --- Phase 2: extract digits backwards into result buffer ---
	// We write directly into 'result' (backwards), then reverse at the end.
	char* ptr = result;

	if (base == 10)
	{
		// ---- Fast path: base 10, two digits at a time ----
		// This halves the number of expensive divisions.
		if (thSep) [[unlikely]]
		{
			// With thousand separators: track digit count for separator insertion.
			int thCount = 0;

			while (uval >= 100)
			{
				const U q = uval / 100;
				const unsigned r = static_cast<unsigned>(uval - q * 100);
				uval = q;

				// Lower digit (ones place of the pair)
				*ptr++ = Impl::kDigitPairs[r * 2 + 1];
				if (++thCount == 3)
				{
					*ptr++ = thSep;
					thCount = 0;
				}

				// Upper digit (tens place of the pair)
				*ptr++ = Impl::kDigitPairs[r * 2];
				if (++thCount == 3 && uval != 0)
				{
					*ptr++ = thSep;
					thCount = 0;
				}
			}
			// Remaining 1 or 2 digits
			if (uval >= 10)
			{
				const unsigned r = static_cast<unsigned>(uval);
				*ptr++ = Impl::kDigitPairs[r * 2 + 1];
				if (++thCount == 3)
				{
					*ptr++ = thSep;
					thCount = 0;
				}
				*ptr++ = Impl::kDigitPairs[r * 2];
			}
			else
			{
				*ptr++ = static_cast<char>('0' + uval);
			}
		}
		else
		{
			// No thousand separators: pure speed path.
			while (uval >= 100)
			{
				const U q = uval / 100;
				const unsigned r = static_cast<unsigned>(uval - q * 100);
				uval = q;
				*ptr++ = Impl::kDigitPairs[r * 2 + 1];
				*ptr++ = Impl::kDigitPairs[r * 2];
			}
			if (uval >= 10)
			{
				const unsigned r = static_cast<unsigned>(uval);
				*ptr++ = Impl::kDigitPairs[r * 2 + 1];
				*ptr++ = Impl::kDigitPairs[r * 2];
			}
			else
			{
				*ptr++ = static_cast<char>('0' + uval);
			}
		}
	}
	else
	{
		// ---- Generic path: base 2..9, 11..16 ----
		const char* digits = lowercase ? Impl::kDigitsLower : Impl::kDigitsUpper;

		// For power-of-two bases, use shift+mask instead of division.
		if ((base & (base - 1)) == 0)
		{
			unsigned shift;
			switch (base)
			{
				case 2:  shift = 1; break;
				case 4:  shift = 2; break;
				case 8:  shift = 3; break;
				case 16: shift = 4; break;
				default: poco_bugcheck(); shift = 0; break; // unreachable for valid power-of-2 bases
			}
			const U mask = (static_cast<U>(1) << shift) - 1;
			do
			{
				*ptr++ = digits[uval & mask];
				uval >>= shift;
			} while (uval);
		}
		else [[unlikely]]
		{
			// Non-power-of-two: base 3, 5, 6, 7, 9, 11..15
			do
			{
				const U q = uval / base;
				*ptr++ = digits[uval - q * base];
				uval = q;
			} while (uval);
		}
	}

	// --- Phase 3: zero-fill padding (inserted backwards, gets reversed) ---
	if (fill == '0')
	{
		int w = width;
		if (negative) --w;
		if (prefix && base == 010) --w;
		if (prefix && base == 0x10) w -= 2;
		while (static_cast<int>(ptr - result) < w) *ptr++ = '0';
	}

	// --- Phase 4: prefix and sign (appended backwards, reversed later) ---
	if (prefix && base == 010)
	{
		*ptr++ = '0';
	}
	else if (prefix && base == 0x10)
	{
		*ptr++ = 'x';
		*ptr++ = '0';
	}

	if (negative) *ptr++ = '-';

	// --- Phase 5: non-zero-fill padding ---
	if (fill != '0')
	{
		while (static_cast<int>(ptr - result) < width) *ptr++ = fill;
	}

	// --- Phase 6: finalize and reverse ---
	size = static_cast<std::size_t>(ptr - result);
	if (size >= capacity)
		throw RangeException();
	*ptr-- = '\0';

	char* lo = result;
	while (lo < ptr)
	{
		char tmp = *ptr;
		*ptr-- = *lo;
		*lo++ = tmp;
	}

	return true;
}


template <typename T>
POCO_DEPRECATED("use intToStr instead")
bool uIntToStr(T value,
	unsigned short base,
	char* result,
	std::size_t& size,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0,
	bool lowercase = false)
	/// Converts unsigned integer to string. Numeric bases from binary to hexadecimal are supported.
	/// If width is non-zero, it pads the return value with fill character to the specified width.
	/// When padding is zero character ('0'), it is prepended to the number itself; all other
	/// paddings are prepended to the formatted result with minus sign or base prefix included
	/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal,
	/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
	/// Formatted string has at least [width] total length.
	/// @deprecated Use intToStr instead.
{
	return intToStr(value, base, result, size, prefix, width, fill, thSep, lowercase);
}


template <typename T>
[[nodiscard]] bool intToStr(T number,
	unsigned short base,
	std::string& result,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0,
	bool lowercase = false)
	/// Converts integer to string; This is a wrapper function, for details see the
	/// bool intToStr(T, unsigned short, char*, int, int, char, char) implementation.
{
	char res[POCO_MAX_INT_STRING_LEN] = {0};
	std::size_t size = POCO_MAX_INT_STRING_LEN;
	const bool ret = intToStr(number, base, res, size, prefix, width, fill, thSep, lowercase);
	result.assign(res, size);
	return ret;
}


template <typename T>
POCO_DEPRECATED("use intToStr instead")
bool uIntToStr(T number,
	unsigned short base,
	std::string& result,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0,
	bool lowercase = false)
	/// Converts unsigned integer to string; This is a wrapper function, for details see the
	/// bool uIntToStr(T, unsigned short, char*, int, int, char, char) implementation.
	/// @deprecated Use intToStr instead.
{
	return intToStr(number, base, result, prefix, width, fill, thSep, lowercase);
}


//
// Floating-point to/from string conversions.
// Uses std::to_chars/from_chars when available (POCO_HAS_FLOAT_CHARCONV),
// otherwise falls back to the bundled double-conversion library
// (http://code.google.com/p/double-conversion/), which implements the
// algorithm described in Florian Loitsch's paper:
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


Foundation_API std::string& floatToFixedStr(std::string& str,
	float value,
	int precision,
	int width = 0,
	char thSep = 0,
	char decSep = 0);
	/// Converts a float value, assigns it to the supplied string and returns the reference.
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


Foundation_API std::string& doubleToFixedStr(std::string& str,
	double value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);
	/// Converts a double value, assigns it to the supplied string and returns the reference.
	/// This function calls doubleToFixedStr(char*, int, double, int) and formats the result according to
	/// precision (total number of digits after the decimal point) and width (total length of formatted string).


Foundation_API float strToFloat(const char* str,
	const char* inf = POCO_FLT_INF, const char* nan = POCO_FLT_NAN);
	/// Converts the string of characters into single-precision floating point number.
	/// Function uses double_conversion::DoubleToStringConverter to do the conversion.


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


#endif // Foundation_NumericString_INCLUDED
