//
// Decimal128.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Decimal128
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Decimal128.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cctype>
#include <cstring>


namespace Poco::MongoDB {


namespace
{
	constexpr Poco::Int32  DECIMAL128_BIAS                = 6176;
	constexpr Poco::Int32  DECIMAL128_EXPONENT_MAX        =  6144;
	constexpr Poco::Int32  DECIMAL128_EXPONENT_MIN        = -6143;
	constexpr Poco::Int32  DECIMAL128_MAX_DIGITS          = 34;

	constexpr Poco::UInt64 SIGN_MASK                      = 0x8000000000000000ULL;
	constexpr Poco::UInt64 SPECIAL_MASK                   = 0x7C00000000000000ULL;
	constexpr Poco::UInt64 INF_VALUE                      = 0x7800000000000000ULL;
	constexpr Poco::UInt64 NAN_VALUE                      = 0x7C00000000000000ULL;
	constexpr Poco::UInt64 LARGE_COEFF_MASK               = 0x6000000000000000ULL;
	constexpr Poco::UInt64 LARGE_COEFF_VALUE              = 0x6000000000000000ULL;

	struct Coefficient
		/// 113-bit unsigned integer stored as four little-endian UInt32 limbs.
		/// Used for in-place decimal-by-10 division when stringifying.
	{
		Poco::UInt32 limbs[4];

		void clear() noexcept
		{
			limbs[0] = limbs[1] = limbs[2] = limbs[3] = 0;
		}

		void fromHighLow(Poco::UInt64 coeffHigh, Poco::UInt64 coeffLow) noexcept
		{
			limbs[0] = static_cast<Poco::UInt32>(coeffLow);
			limbs[1] = static_cast<Poco::UInt32>(coeffLow >> 32);
			limbs[2] = static_cast<Poco::UInt32>(coeffHigh);
			limbs[3] = static_cast<Poco::UInt32>(coeffHigh >> 32);
		}

		[[nodiscard]] bool isZero() const noexcept
		{
			return limbs[0] == 0 && limbs[1] == 0 && limbs[2] == 0 && limbs[3] == 0;
		}

		unsigned divmod10() noexcept
		{
			Poco::UInt64 r = 0;
			for (int i = 3; i >= 0; --i)
			{
				Poco::UInt64 v = (r << 32) | limbs[i];
				limbs[i] = static_cast<Poco::UInt32>(v / 10);
				r = v % 10;
			}
			return static_cast<unsigned>(r);
		}

		void mul10Add(unsigned digit) noexcept
		{
			Poco::UInt64 carry = digit;
			for (int i = 0; i < 4; ++i)
			{
				Poco::UInt64 v = static_cast<Poco::UInt64>(limbs[i]) * 10 + carry;
				limbs[i] = static_cast<Poco::UInt32>(v);
				carry = v >> 32;
			}
		}

		void packHighLow(Poco::UInt64& coeffHigh, Poco::UInt64& coeffLow) const noexcept
		{
			coeffLow  = (static_cast<Poco::UInt64>(limbs[1]) << 32) | limbs[0];
			coeffHigh = (static_cast<Poco::UInt64>(limbs[3]) << 32) | limbs[2];
		}
	};
} // namespace


Decimal128::Decimal128():
	_low(0),
	_high(0x3040000000000000ULL) // exponent = 0, coefficient = 0
{
}


Decimal128::Decimal128(Poco::UInt64 low, Poco::UInt64 high):
	_low(low),
	_high(high)
{
}


bool Decimal128::isNaN() const noexcept
{
	return (_high & SPECIAL_MASK) == NAN_VALUE;
}


bool Decimal128::isInfinite() const noexcept
{
	return (_high & SPECIAL_MASK) == INF_VALUE;
}


bool Decimal128::isNegative() const noexcept
{
	return (_high & SIGN_MASK) != 0;
}


Decimal128 Decimal128::positiveZero()
{
	return Decimal128(0, 0x3040000000000000ULL);
}


Decimal128 Decimal128::negativeZero()
{
	return Decimal128(0, 0xB040000000000000ULL);
}


Decimal128 Decimal128::positiveInfinity()
{
	return Decimal128(0, 0x7800000000000000ULL);
}


Decimal128 Decimal128::negativeInfinity()
{
	return Decimal128(0, 0xF800000000000000ULL);
}


Decimal128 Decimal128::nan()
{
	return Decimal128(0, 0x7C00000000000000ULL);
}


std::string Decimal128::toString() const
{
	const bool negative = isNegative();

	if (isNaN()) return "NaN";
	if (isInfinite()) return negative ? "-Infinity" : "Infinity";

	Poco::Int32 biasedExponent;
	Coefficient coeff;
	coeff.clear();

	if ((_high & LARGE_COEFF_MASK) == LARGE_COEFF_VALUE)
	{
		// Large-coefficient encoding: implicit leading "100" makes the
		// coefficient >= 10^33 and therefore representable, but the
		// resulting value is non-canonical. Per the BSON spec the value
		// renders as zero with the encoded exponent.
		biasedExponent = static_cast<Poco::Int32>((_high >> 47) & 0x3FFF);
	}
	else
	{
		// Standard encoding: bits 126..113 = biased exponent, bits 112..0 =
		// coefficient.
		biasedExponent = static_cast<Poco::Int32>((_high >> 49) & 0x3FFF);
		const Poco::UInt64 coeffHigh = _high & 0x0001FFFFFFFFFFFFULL;
		coeff.fromHighLow(coeffHigh, _low);
	}

	const Poco::Int32 exponent = biasedExponent - DECIMAL128_BIAS;

	std::string mantissa;
	mantissa.reserve(DECIMAL128_MAX_DIGITS);
	if (coeff.isZero())
	{
		mantissa.push_back('0');
	}
	else
	{
		while (!coeff.isZero())
		{
			mantissa.push_back(static_cast<char>('0' + coeff.divmod10()));
		}
		std::reverse(mantissa.begin(), mantissa.end());
	}

	const int significantDigits = static_cast<int>(mantissa.size());
	const Poco::Int32 adjusted = exponent + significantDigits - 1;

	std::string out;
	out.reserve(48);
	if (negative) out += '-';

	// Canonical form (IEEE 754-2008 Section 5.12.4): non-scientific notation
	// when exponent <= 0 AND adjusted >= -6, otherwise one-digit-then-E.
	if (exponent <= 0 && adjusted >= -6)
	{
		if (exponent == 0)
		{
			out += mantissa;
		}
		else
		{
			const int integerDigits = significantDigits + exponent; // exponent < 0
			if (integerDigits <= 0)
			{
				out += "0.";
				out.append(-integerDigits, '0');
				out += mantissa;
			}
			else
			{
				out.append(mantissa, 0, integerDigits);
				out += '.';
				out.append(mantissa, integerDigits, std::string::npos);
			}
		}
	}
	else
	{
		out += mantissa[0];
		if (significantDigits > 1)
		{
			out += '.';
			out.append(mantissa, 1, std::string::npos);
		}
		out += 'E';
		if (adjusted >= 0) out += '+';
		out += std::to_string(adjusted);
	}

	return out;
}


Decimal128 Decimal128::fromString(const std::string& s)
{
	if (s.empty())
		throw Poco::SyntaxException("Decimal128::fromString: empty string");

	std::size_t i = 0;
	bool negative = false;
	if (s[i] == '+' || s[i] == '-')
	{
		negative = (s[i] == '-');
		++i;
	}

	if (i >= s.size())
		throw Poco::SyntaxException("Decimal128::fromString: missing digits in '" + s + "'");

	// Match NaN / Infinity (case-insensitive).
	auto matchKeyword = [&s, &i](const char* kw)
	{
		std::size_t k = 0;
		std::size_t j = i;
		while (kw[k] != '\0' && j < s.size())
		{
			if (std::tolower(static_cast<unsigned char>(s[j])) !=
			    std::tolower(static_cast<unsigned char>(kw[k])))
			{
				return false;
			}
			++j;
			++k;
		}
		if (kw[k] != '\0') return false;
		i = j;
		return true;
	};

	if (matchKeyword("nan"))
	{
		if (i != s.size())
			throw Poco::SyntaxException("Decimal128::fromString: trailing junk after NaN in '" + s + "'");
		Decimal128 v = Decimal128::nan();
		if (negative) return Decimal128(v.low(), v.high() | SIGN_MASK);
		return v;
	}
	if (matchKeyword("infinity") || matchKeyword("inf"))
	{
		if (i != s.size())
			throw Poco::SyntaxException("Decimal128::fromString: trailing junk after Infinity in '" + s + "'");
		return negative ? Decimal128::negativeInfinity() : Decimal128::positiveInfinity();
	}

	// Finite literal: integer-part [ "." fractional-part ] [ ("E"|"e") [+-]? exp-digits ]
	std::string digits;
	digits.reserve(DECIMAL128_MAX_DIGITS + 4);
	Poco::Int32 implicitExponent = 0;
	bool sawDigit = false;
	bool sawDecimalPoint = false;
	bool sawExponent = false;

	for (; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == '.')
		{
			if (sawDecimalPoint)
				throw Poco::SyntaxException("Decimal128::fromString: multiple decimal points in '" + s + "'");
			sawDecimalPoint = true;
		}
		else if (c >= '0' && c <= '9')
		{
			sawDigit = true;
			digits += c;
			if (sawDecimalPoint) --implicitExponent;
		}
		else if (c == 'E' || c == 'e')
		{
			sawExponent = true;
			break;
		}
		else
		{
			throw Poco::SyntaxException("Decimal128::fromString: invalid character in '" + s + "'");
		}
	}

	if (!sawDigit)
		throw Poco::SyntaxException("Decimal128::fromString: no digits in '" + s + "'");

	Poco::Int32 explicitExponent = 0;
	if (sawExponent)
	{
		std::size_t expStart = i + 1; // skip past 'E' or 'e'
		bool expNegative = false;
		if (expStart < s.size() && (s[expStart] == '+' || s[expStart] == '-'))
		{
			expNegative = (s[expStart] == '-');
			++expStart;
		}
		if (expStart >= s.size())
			throw Poco::SyntaxException("Decimal128::fromString: missing exponent digits in '" + s + "'");

		Poco::Int64 acc = 0;
		for (std::size_t k = expStart; k < s.size(); ++k)
		{
			char c = s[k];
			if (c < '0' || c > '9')
				throw Poco::SyntaxException("Decimal128::fromString: invalid exponent in '" + s + "'");
			acc = acc * 10 + (c - '0');
			if (acc > 1000000000LL)
				throw Poco::RangeException("Decimal128::fromString: exponent out of range in '" + s + "'");
		}
		explicitExponent = static_cast<Poco::Int32>(expNegative ? -acc : acc);
	}

	Poco::Int32 exponent = implicitExponent + explicitExponent;

	// Strip leading zeros; trailing zeros are part of the canonical
	// (coefficient, exponent) pair and must be preserved.
	std::size_t firstNonZero = 0;
	while (firstNonZero + 1 < digits.size() && digits[firstNonZero] == '0')
		++firstNonZero;
	digits.erase(0, firstNonZero);

	if (digits.size() > static_cast<std::size_t>(DECIMAL128_MAX_DIGITS))
		throw Poco::RangeException("Decimal128::fromString: more than 34 significant digits in '" + s + "'");

	if (exponent > DECIMAL128_EXPONENT_MAX || exponent < DECIMAL128_EXPONENT_MIN)
		throw Poco::RangeException("Decimal128::fromString: exponent out of range in '" + s + "'");

	Coefficient coeff;
	coeff.clear();
	for (char c: digits)
	{
		coeff.mul10Add(static_cast<unsigned>(c - '0'));
	}

	Poco::UInt64 coeffHigh = 0;
	Poco::UInt64 coeffLow  = 0;
	coeff.packHighLow(coeffHigh, coeffLow);

	const Poco::UInt64 biasedExponent = static_cast<Poco::UInt64>(exponent + DECIMAL128_BIAS) & 0x3FFFULL;
	Poco::UInt64 high = (biasedExponent << 49) | (coeffHigh & 0x0001FFFFFFFFFFFFULL);
	if (negative) high |= SIGN_MASK;

	return Decimal128(coeffLow, high);
}


} // namespace Poco::MongoDB
