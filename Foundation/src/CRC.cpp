//
// CRC.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Portable CRC implementations, the Barrett reduction of the folding
// implementations and the selection of an implementation.
//
// References:
// - M. E. Kounavis, F. L. Berry, "A Systematic Approach to Building High
//   Performance Software-Based CRC Generators", IEEE ISCC 2005: slicing-by-8.
// - R. N. Williams, "A Painless Guide to CRC Error Detection Algorithms",
//   1993: reflected and non-reflected table-driven CRCs.
// - V. Gopal et al., "Fast CRC Computation for Generic Polynomials Using
//   PCLMULQDQ Instruction", Intel, 2009: Barrett reduction.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CRC.h"
#include "Poco/ByteOrder.h"
#include "Poco/Exception.h"
#include <zlib.h>
#include <array>
#include <cstring>


#if ZLIB_VERNUM < 0x1290
	#error "zlib 1.2.9 or later is required"
#endif


namespace Poco {


namespace {


constexpr std::size_t TABLE_COUNT = 8;
constexpr std::size_t BYTE_VALUES = 256;

using Tables = std::array<std::array<UInt64, BYTE_VALUES>, TABLE_COUNT>;


template <bool reflected>
[[nodiscard]] consteval Tables makeTables(UInt64 polynomial)
	// Builds the tables of slicing-by-8 (Kounavis and Berry): tables[k][b] is the
	// CRC-64 register, starting from zero, after byte b followed by k zero bytes.
	// A reflected CRC shifts the register right and takes the reflected
	// polynomial (Williams); a normal CRC shifts it left.
{
	const UInt64 reflectedPolynomial = CRC::reflect(polynomial);
	Tables tables{};
	for (std::size_t b = 0; b < BYTE_VALUES; ++b)
	{
		// table 0: one byte, bit by bit; the byte enters at the end of the register that is shifted out
		UInt64 r = reflected ? b : static_cast<UInt64>(b) << 56;
		for (int bit = 0; bit < 8; ++bit)
		{
			if constexpr (reflected)
				r = (r >> 1) ^ ((r & 1) != 0 ? reflectedPolynomial : 0);
			else
				r = (r << 1) ^ ((r >> 63) != 0 ? polynomial : 0);
		}
		tables[0][b] = r;
	}
	// table k: the entry of table k - 1, followed by one more zero byte
	for (std::size_t k = 1; k < TABLE_COUNT; ++k)
	{
		for (std::size_t b = 0; b < BYTE_VALUES; ++b)
		{
			const UInt64 r = tables[k - 1][b];
			if constexpr (reflected)
				tables[k][b] = (r >> 8) ^ tables[0][r & 0xFF];
			else
				tables[k][b] = (r << 8) ^ tables[0][r >> 56];
		}
	}
	return tables;
}


constexpr Tables CRC64_XZ_TABLES = makeTables<true>(CRC::CRC64_POLYNOMIAL);
constexpr Tables CRC64_ECMA182_TABLES = makeTables<false>(CRC::CRC64_POLYNOMIAL);


template <bool reflected>
[[nodiscard]] UInt64 updateWithTables(UInt64 crc, std::span<const unsigned char> data, const Tables& t)
	// Updates the CRC-64 register with slicing-by-8: eight bytes per step,
	// then one byte per step for the rest.
{
	// the loops of the implementations run on a pointer and a count, because
	// hardened standard libraries check every access to a span
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	while (length >= TABLE_COUNT)
	{
		// The eight bytes are added to the register, the first byte where a byte
		// enters it (the low byte of a reflected register, the high byte of a
		// normal one). Each byte of the sum is then looked up in the table for the
		// number of bytes that follow it, and the results are added.
		UInt64 word = 0;
		std::memcpy(&word, pData, sizeof(word));
		if constexpr (reflected)
		{
			crc ^= ByteOrder::fromLittleEndian(word);
			crc = t[7][crc & 0xFF] ^ t[6][(crc >> 8) & 0xFF] ^ t[5][(crc >> 16) & 0xFF] ^ t[4][(crc >> 24) & 0xFF] ^
				t[3][(crc >> 32) & 0xFF] ^ t[2][(crc >> 40) & 0xFF] ^ t[1][(crc >> 48) & 0xFF] ^ t[0][crc >> 56];
		}
		else
		{
			crc ^= ByteOrder::fromBigEndian(word);
			crc = t[7][crc >> 56] ^ t[6][(crc >> 48) & 0xFF] ^ t[5][(crc >> 40) & 0xFF] ^ t[4][(crc >> 32) & 0xFF] ^
				t[3][(crc >> 24) & 0xFF] ^ t[2][(crc >> 16) & 0xFF] ^ t[1][(crc >> 8) & 0xFF] ^ t[0][crc & 0xFF];
		}
		pData += TABLE_COUNT;
		length -= TABLE_COUNT;
	}
	// the rest: one byte per step with table 0
	for (; length > 0; --length)
	{
		if constexpr (reflected)
			crc = t[0][(crc ^ *pData++) & 0xFF] ^ (crc >> 8);
		else
			crc = t[0][(crc >> 56) ^ *pData++] ^ (crc << 8);
	}
	return crc;
}


} // namespace


CRC::Update CRC::implementation(Checksum::Type type)
{
	// one selection per CRC type, made on the first use (function-local statics are initialized thread-safely)
	using enum Checksum::Type;
	switch (type)
	{
	case TYPE_CRC32:
		{
			static const Update fastest = implementations(type).back().update;
			return fastest;
		}
	case TYPE_CRC64_XZ:
		{
			static const Update fastest = implementations(type).back().update;
			return fastest;
		}
	case TYPE_CRC64_ECMA182:
		{
			static const Update fastest = implementations(type).back().update;
			return fastest;
		}
	default:
		throw InvalidArgumentException("not a CRC checksum type");
	}
}


std::vector<CRC::Implementation> CRC::implementations(Checksum::Type type)
{
	std::vector<Implementation> result;
	using enum Checksum::Type;
	switch (type)
	{
	case TYPE_CRC32:
		result.push_back({"zlib", &crc32Zlib});
		break;
	case TYPE_CRC64_XZ:
		result.push_back({"tables", &crc64XZPortable});
		break;
	case TYPE_CRC64_ECMA182:
		result.push_back({"tables", &crc64ECMA182Portable});
		break;
	default:
		throw InvalidArgumentException("not a CRC checksum type");
	}
	// the implementations with instruction set extensions come from CRC_X86.cpp or CRC_ARM.cpp
	addInstructionImplementations(type, result);
	return result;
}


#if defined(POCO_CRC_X86_64) || defined(POCO_CRC_ARMV8)


UInt64 CRC::reduceReflected64(const Product& lane, const FoldConstants& factors, Multiply multiply)
{
	// The CRC register is lane * x^64 mod P. In a reflected lane the low 64 bits
	// hold the upper half U and the high 64 bits the lower half L:
	// lane * x^64 = U * x^128 + L * x^64 = U * (x^128 mod P) + L * x^64 (mod P),
	// which fits into 128 bits: upper half T1, lower half T0.
	const Product product = multiply(lane.low, factors.reduce[0]);
	const UInt64 upper = product.low ^ lane.high;
	const UInt64 lower = product.high;
	// Barrett reduction (Gopal et al.) of T1 * x^64 + T0:
	// quotient q = T1 + floor(T1 * mu' / x^64), where mu = x^64 + mu';
	// a reflected product carries one factor x, so its upper half is shifted left by one bit.
	const UInt64 quotient = upper ^ (multiply(upper, factors.mu).low << 1);
	// remainder = T0 + (q * P' mod x^64), where P = x^64 + P'; the bits of q * P'
	// that make up its lower 64 bits are bits 63 to 126 of the reflected product.
	const Product multiple = multiply(quotient, factors.polynomial);
	return lower ^ (multiple.high << 1) ^ (multiple.low >> 63);
}


UInt64 CRC::reduceNormal64(const Product& lane, const FoldConstants& factors, Multiply multiply)
{
	// The CRC register is lane * x^64 mod P. In a normal lane the high 64 bits
	// hold the upper half U and the low 64 bits the lower half L:
	// lane * x^64 = U * (x^128 mod P) + L * x^64 (mod P): upper half T1, lower half T0.
	const Product product = multiply(lane.high, factors.reduce[0]);
	const UInt64 upper = product.high ^ lane.low;
	const UInt64 lower = product.low;
	// Barrett reduction (Gopal et al.): q = T1 + floor(T1 * mu' / x^64),
	// remainder = T0 + (q * P' mod x^64), with mu = x^64 + mu' and P = x^64 + P'.
	const UInt64 quotient = upper ^ multiply(upper, factors.mu).high;
	return lower ^ multiply(quotient, factors.polynomial).low;
}


#endif


UInt64 CRC::crc32Zlib(UInt64 crc, std::span<const unsigned char> data)
{
	// an empty update may come with a null buffer, for which zlib returns its
	// initial value, 0, instead of the given one
	if (data.empty()) return crc;
	// zlib's crc32_z() takes and returns the CRC after the final XOR: it inverts
	// the register before and after the update, so the register is inverted here
	// on the way in and on the way out
	const uLong value = ::crc32_z(static_cast<uLong>(~crc & CRC32_MASK), data.data(), data.size());
	return ~static_cast<UInt64>(value) & CRC32_MASK;
}


UInt64 CRC::crc64XZPortable(UInt64 crc, std::span<const unsigned char> data)
{
	// reflected: the first byte enters the low byte of the register
	return updateWithTables<true>(crc, data, CRC64_XZ_TABLES);
}


UInt64 CRC::crc64ECMA182Portable(UInt64 crc, std::span<const unsigned char> data)
{
	// normal: the first byte enters the high byte of the register
	return updateWithTables<false>(crc, data, CRC64_ECMA182_TABLES);
}


} // namespace Poco


#if defined(POCO_CRC_X86_64)
#include "CRC_X86.cpp"
#elif defined(POCO_CRC_ARMV8)
#include "CRC_ARM.cpp"
#else


namespace Poco {


void CRC::addInstructionImplementations(Checksum::Type, std::vector<Implementation>&)
	// other architectures use the portable implementations only
{
}


} // namespace Poco


#endif
