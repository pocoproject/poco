//
// Decimal128.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  Decimal128
//
// Definition of the Decimal128 BSON value type.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Decimal128_INCLUDED
#define MongoDB_Decimal128_INCLUDED


#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/MongoDB/Element.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace Poco::MongoDB {


// BSON Decimal128 specification:
//   https://github.com/mongodb/specifications/blob/master/source/bson-decimal128/decimal128.md
//
// Encoding (IEEE 754-2008 BID format):
//   bit 127        : sign
//   bits 126..0    : combination field + biased exponent + coefficient
//
//   If bits 126..125 are not both 1:
//     biased exponent = bits 126..113 (14 bits)
//     coefficient     = bits 112..0   (113-bit unsigned integer)
//   If bits 126..125 are both 1 and bits 124..123 are not both 1:
//     biased exponent = bits 124..111 (14 bits)
//     coefficient     = 0b100 || bits 110..0 (114 bits, but limited to <= 10^34 - 1)
//   If bits 126..123 are all 1:
//     bit 122 = 1 -> NaN; bit 122 = 0 -> Infinity
//
// Exponent bias = 6176. Maximum 34 significant decimal digits.


class MongoDB_API Decimal128
	/// Represents an IEEE 754-2008 decimal128 floating-point value (BSON
	/// TypeId 0x13, available since MongoDB 3.4). Used wherever exact decimal
	/// representation matters -- currency, ledger entries, scientific data --
	/// since binary double cannot represent 0.1 exactly.
	///
	/// Storage is two Poco::UInt64 halves matching the BSON wire format
	/// (16 bytes, little-endian, low half first then high half). The class
	/// supports BSON serialization, canonical string conversion per
	/// IEEE 754-2008 Section 5.12.4, parsing simple decimal literals, and
	/// equality comparison. It does not implement decimal arithmetic; users
	/// who need to operate on values are expected to round-trip through
	/// std::string or use an external decimal-arithmetic library.
{
public:
	using Ptr = Poco::SharedPtr<Decimal128>;

	Decimal128();
		/// Creates a Decimal128 representing positive zero (0E0).

	Decimal128(Poco::UInt64 low, Poco::UInt64 high);
		/// Creates a Decimal128 from the raw 128 bits, expressed as low and
		/// high 64-bit halves matching the on-wire layout.

	~Decimal128() = default;

	[[nodiscard]] Poco::UInt64 low() const noexcept;
		/// Returns the low 64 bits.

	[[nodiscard]] Poco::UInt64 high() const noexcept;
		/// Returns the high 64 bits.

	[[nodiscard]] bool isNaN() const noexcept;
		/// True if the value is NaN (any payload).

	[[nodiscard]] bool isInfinite() const noexcept;
		/// True if the value is +Infinity or -Infinity.

	[[nodiscard]] bool isNegative() const noexcept;
		/// True if the sign bit is set, including -0, -Inf and -NaN.

	[[nodiscard]] std::string toString() const;
		/// Returns the canonical decimal string per IEEE 754-2008 Section
		/// 5.12.4: special values render as "NaN", "Infinity", "-Infinity";
		/// finite values render in scientific or non-scientific notation
		/// depending on the adjusted exponent.

	bool operator == (const Decimal128& other) const noexcept;
	bool operator != (const Decimal128& other) const noexcept;

	static Decimal128 fromString(const std::string& s);
		/// Parses a decimal string into a Decimal128. Accepts the canonical
		/// forms produced by toString(): "NaN" (case insensitive), "Infinity"
		/// / "-Infinity" / "Inf" / "-Inf", and finite decimal literals with
		/// optional sign, optional fractional part, and optional exponent
		/// ("E"/"e" prefix). Throws Poco::SyntaxException on malformed input
		/// or Poco::RangeException when the coefficient exceeds 34 decimal
		/// digits or the exponent is out of the decimal128 range.

	static Decimal128 positiveZero();
	static Decimal128 negativeZero();
	static Decimal128 positiveInfinity();
	static Decimal128 negativeInfinity();
	static Decimal128 nan();
		/// Convenience constructors for the standard special values.

private:
	Poco::UInt64 _low;
	Poco::UInt64 _high;
};


//
// inlines
//
inline Poco::UInt64 Decimal128::low() const noexcept
{
	return _low;
}


inline Poco::UInt64 Decimal128::high() const noexcept
{
	return _high;
}


inline bool Decimal128::operator == (const Decimal128& other) const noexcept
{
	return _low == other._low && _high == other._high;
}


inline bool Decimal128::operator != (const Decimal128& other) const noexcept
{
	return !(*this == other);
}


template<>
struct ElementTraits<Decimal128::Ptr>
{
	enum { TypeId = 0x13 };

	static std::string toString(const Decimal128::Ptr& value, int /*indent*/ = 0)
	{
		if (!value) return "null";
		std::string result;
		result.reserve(48);
		result += "{\"$numberDecimal\": \"";
		result += value->toString();
		result += "\"}";
		return result;
	}
};


template<>
inline void BSONReader::read<Decimal128::Ptr>(Decimal128::Ptr& to)
{
	Poco::UInt64 low;
	Poco::UInt64 high;
	_reader >> low >> high;
	to = new Decimal128(low, high);
}


template<>
inline void BSONWriter::write<Decimal128::Ptr>(const Decimal128::Ptr& from)
{
	_writer << from->low() << from->high();
}


} // namespace Poco::MongoDB


#endif // MongoDB_Decimal128_INCLUDED
