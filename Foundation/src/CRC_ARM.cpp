//
// CRC_ARM.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// CRC implementations with the CRC32 and PMULL instructions of ARMv8.
//
// References:
// - V. Gopal et al., "Fast CRC Computation for Generic Polynomials Using
//   PCLMULQDQ Instruction", Intel, 2009: folding, applied to PMULL.
// - Arm C Language Extensions (ACLE), https://arm-software.github.io/acle/:
//   CRC32 and PMULL intrinsics.
// - zlib, crc32.c: crc32_combine() and x2nmodp() (M. Adler): the CRC of
//   concatenated data from the CRCs of its parts, by multiplication with x^(8n) mod P.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CRC.h"
#include "CpuFeatures.h"
#include "Poco/ByteOrder.h"
#if defined(POCO_COMPILER_MSVC)
	#include <intrin.h>
#else
	#include <arm_acle.h>
#endif
#include <arm_neon.h>
#include <array>
#include <cstring>


// GCC and Clang emit the instructions only in functions compiled for them;
// MSVC accepts the intrinsics everywhere.
#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
	#define POCO_CRC_TARGET_CRC32 __attribute__((target("+crc")))
	// PMULL (64 x 64 -> 128-bit carry-less multiplication) belongs to the AES extension
	#define POCO_CRC_TARGET_PMULL __attribute__((target("+crypto")))
#else
	#define POCO_CRC_TARGET_CRC32
	#define POCO_CRC_TARGET_PMULL
#endif


namespace Poco {


namespace {


// the factors of the CRC-64 folding, computed by the compiler from the polynomial
constexpr CRC::FoldConstants CRC64_XZ_FACTORS = CRC::foldConstants(CRC::CRC64_POLYNOMIAL, 64, true);
constexpr CRC::FoldConstants CRC64_ECMA182_FACTORS = CRC::foldConstants(CRC::CRC64_POLYNOMIAL, 64, false);

// CRC-32 runs three independent streams of this many bytes, so that the
// CRC32 instructions overlap, and combines their registers.
constexpr std::size_t CRC32_STREAM_SIZE = 1024;
constexpr std::size_t CRC32_STREAMS = 3;


[[nodiscard]] inline UInt64 load64(const unsigned char* data)
{
	UInt64 word = 0;
	std::memcpy(&word, data, sizeof(word));
	return ByteOrder::fromLittleEndian(word);
}


[[nodiscard]] POCO_CRC_TARGET_PMULL inline uint64x2_t multiplyLow(uint64x2_t a, uint64x2_t b)
	// carry-less product of the low 64 bits of a and b
{
#if defined(POCO_COMPILER_MSVC)
	// MSVC has no poly64_t and poly128_t; its vmull_p64() takes uint64x1_t
	return vmull_p64(vget_low_u64(a), vget_low_u64(b));
#else
	return vreinterpretq_u64_p128(vmull_p64(vgetq_lane_u64(a, 0), vgetq_lane_u64(b, 0)));
#endif
}


[[nodiscard]] POCO_CRC_TARGET_PMULL inline uint64x2_t multiplyHigh(uint64x2_t a, uint64x2_t b)
	// carry-less product of the high 64 bits of a and b
{
#if defined(POCO_COMPILER_MSVC)
	return vmull_high_p64(a, b);
#else
	return vreinterpretq_u64_p128(vmull_high_p64(vreinterpretq_p64_u64(a), vreinterpretq_p64_u64(b)));
#endif
}


[[nodiscard]] POCO_CRC_TARGET_CRC32 UInt32 crc32Instructions(UInt32 crc, std::span<const unsigned char> data)
	// CRC-32 register update with the CRC32 instructions (reflected, no inversion):
	// eight bytes per instruction, then one byte per instruction
{
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	for (; length >= 8; length -= 8, pData += 8)
		crc = __crc32d(crc, load64(pData));
	for (; length > 0; --length)
		crc = __crc32b(crc, *pData++);
	return crc;
}


constexpr UInt32 CRC32_REFLECTED_POLYNOMIAL = static_cast<UInt32>(CRC::reflect(CRC::CRC32_POLYNOMIAL) >> 32);
constexpr std::size_t NIBBLES = 8;
constexpr std::size_t NIBBLE_VALUES = 16;

using ShiftTable = std::array<std::array<UInt32, NIBBLE_VALUES>, NIBBLES>;


[[nodiscard]] consteval UInt32 multiplyModP(UInt32 a, UInt32 b)
	// product of two reflected CRC-32 polynomials mod P, one bit of a per step
{
	UInt32 product = 0;
	for (int i = 0; i < 32; ++i)
	{
		if (((a >> (31 - i)) & 1) != 0) product ^= b;
		b = (b >> 1) ^ ((b & 1) != 0 ? CRC32_REFLECTED_POLYNOMIAL : 0);
	}
	return product;
}


[[nodiscard]] consteval ShiftTable makeShiftTable(std::size_t bytes)
	// table[j][v] is nibble v at bits 4j to 4j + 3 of a register times x^(8 * bytes) mod P
{
	const UInt32 factor = static_cast<UInt32>(CRC::reflect(CRC::xPowerModP(static_cast<unsigned>(8 * bytes), CRC::CRC32_POLYNOMIAL, 32)) >> 32);
	ShiftTable table{};
	for (std::size_t j = 0; j < NIBBLES; ++j)
	{
		for (std::size_t v = 0; v < NIBBLE_VALUES; ++v)
			table[j][v] = multiplyModP(static_cast<UInt32>(v << (4 * j)), factor);
	}
	return table;
}


constexpr ShiftTable CRC32_SHIFT_ONE = makeShiftTable(CRC32_STREAM_SIZE);
constexpr ShiftTable CRC32_SHIFT_TWO = makeShiftTable(2 * CRC32_STREAM_SIZE);


[[nodiscard]] inline UInt32 shiftWithTable(UInt32 crc, const ShiftTable& table)
{
	UInt32 result = 0;
	for (std::size_t j = 0; j < NIBBLES; ++j)
		result ^= table[j][(crc >> (4 * j)) & 0xF];
	return result;
}


[[nodiscard]] POCO_CRC_TARGET_CRC32 UInt64 crc32Streams(UInt64 crc, std::span<const unsigned char> data)
	// CRC-32 register update in three streams: the CRC32 instruction has a
	// latency of several cycles, so three independent registers run over three
	// consecutive streams at once. The CRC of three streams of n bytes is
	// crc0 * x^(16 * n) + crc1 * x^(8 * n) + crc2 (mod P), with crc1 and crc2
	// started from zero; the shifts by x^(8 * n) and x^(16 * n) take eight
	// table lookups each. Shorter data and the rest after the last three streams
	// go through one register.
{
	auto crc0 = static_cast<UInt32>(crc);
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	for (; length >= CRC32_STREAMS * CRC32_STREAM_SIZE; length -= CRC32_STREAMS * CRC32_STREAM_SIZE)
	{
		UInt32 crc1 = 0;
		UInt32 crc2 = 0;
		for (std::size_t i = 0; i < CRC32_STREAM_SIZE; i += 8)
		{
			crc0 = __crc32d(crc0, load64(pData + i));
			crc1 = __crc32d(crc1, load64(pData + CRC32_STREAM_SIZE + i));
			crc2 = __crc32d(crc2, load64(pData + 2 * CRC32_STREAM_SIZE + i));
		}
		crc0 = shiftWithTable(crc0, CRC32_SHIFT_TWO) ^ shiftWithTable(crc1, CRC32_SHIFT_ONE) ^ crc2;
		pData += CRC32_STREAMS * CRC32_STREAM_SIZE;
	}
	return crc32Instructions(crc0, data.last(length));
}


[[nodiscard]] POCO_CRC_TARGET_PMULL inline uint64x2_t fold(uint64x2_t lane, uint64x2_t factors, uint64x2_t data)
	// One fold step: low half of the lane times the low factor plus high half
	// times the high factor, a 128-bit value congruent to the lane moved by the
	// distance of the factors, plus the data at that distance. Adding the data
	// between the two products lets CPUs that fuse PMULL with the following EOR
	// do so for both products.
{
	return veorq_u64(veorq_u64(multiplyLow(lane, factors), data), multiplyHigh(lane, factors));
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET_PMULL inline uint64x2_t loadLane(const unsigned char* data)
{
	const uint8x16_t bytes = vld1q_u8(data);
	if constexpr (reflected)
	{
		// a reflected CRC takes bit 0 of the first byte as the highest power,
		// which a little-endian load puts into bit 0 of the lane
		return vreinterpretq_u64_u8(bytes);
	}
	else
	{
		// a normal CRC takes the first byte as the highest power: reverse the 16
		// bytes (within each half, then swap the halves), so that the first byte
		// becomes the most significant one
		const uint8x16_t reversed = vrev64q_u8(bytes);
		return vreinterpretq_u64_u8(vextq_u8(reversed, reversed, 8));
	}
}


// TODO: this loop is the same in CRC_X86.cpp and CRC_ARM.cpp; share it through a template over a lane type.
template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET_PMULL CRC::Product foldData(UInt64 crc, std::span<const unsigned char> data, const CRC::FoldConstants& factors)
	// Folds the data, a multiple of LANE_SIZE and at least FOLD_MIN_LENGTH
	// bytes, into one lane (Gopal et al., "folding").
{
	// the register is added to the first 64 bits of the data: the low 64 bits of
	// a reflected lane, the high 64 bits of a normal lane
	const uint64x2_t firstBits = vsetq_lane_u64(crc, vdupq_n_u64(0), reflected ? 0 : 1);
	const uint64x2_t foldLane = vld1q_u64(factors.foldLane);
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	uint64x2_t lane;
	if (length >= CRC::FOLD_BLOCK_SIZE)
	{
		// the FOLD_LANES lanes of a block are folded independently, each
		// FOLD_BLOCK_SIZE bytes forward onto its lane of the next block, so that
		// several multiplications are in flight
		uint64x2_t lanes[CRC::FOLD_LANES];
		for (std::size_t i = 0; i < CRC::FOLD_LANES; ++i)
			lanes[i] = loadLane<reflected>(pData + i * CRC::LANE_SIZE);
		lanes[0] = veorq_u64(lanes[0], firstBits);
		pData += CRC::FOLD_BLOCK_SIZE;
		length -= CRC::FOLD_BLOCK_SIZE;

		const uint64x2_t foldBlock = vld1q_u64(factors.foldBlock);
		for (; length >= CRC::FOLD_BLOCK_SIZE; length -= CRC::FOLD_BLOCK_SIZE, pData += CRC::FOLD_BLOCK_SIZE)
		{
			for (std::size_t i = 0; i < CRC::FOLD_LANES; ++i)
				lanes[i] = fold(lanes[i], foldBlock, loadLane<reflected>(pData + i * CRC::LANE_SIZE));
		}
		// combine the lanes: fold each one LANE_SIZE bytes onto the next
		lane = lanes[0];
		for (std::size_t i = 1; i < CRC::FOLD_LANES; ++i)
			lane = fold(lane, foldLane, lanes[i]);
	}
	else
	{
		// less than one block: a single lane
		lane = veorq_u64(loadLane<reflected>(pData), firstBits);
		pData += CRC::LANE_SIZE;
		length -= CRC::LANE_SIZE;
	}
	// the remaining lanes, one at a time
	for (; length >= CRC::LANE_SIZE; length -= CRC::LANE_SIZE, pData += CRC::LANE_SIZE)
		lane = fold(lane, foldLane, loadLane<reflected>(pData));
	return {vgetq_lane_u64(lane, 0), vgetq_lane_u64(lane, 1)};
}


[[nodiscard]] POCO_CRC_TARGET_PMULL CRC::Product multiply(UInt64 a, UInt64 b)
	// carry-less product of two 64-bit values, for the reduction
{
	const uint64x2_t product = multiplyLow(vdupq_n_u64(a), vdupq_n_u64(b));
	return {vgetq_lane_u64(product, 0), vgetq_lane_u64(product, 1)};
}


} // namespace


void CRC::addInstructionImplementations(Checksum::Type type, std::vector<Implementation>& implementations)
{
	const bool crc32 = cpuFeatures().armCrc32;
	const bool pmull = cpuFeatures().armPmull;
	using enum Checksum::Type;
	switch (type)
	{
	case TYPE_CRC32:
		if (crc32) implementations.push_back({"ARMv8 CRC32", &crc32Streams});
		break;
	case TYPE_CRC64_XZ:
		if (pmull) implementations.push_back({"PMULL", &folded<&foldData<true>, &reduceReflected64, &multiply, CRC64_XZ_FACTORS, LANE_SIZE, FOLD_MIN_LENGTH, &crc64XZPortable>});
		break;
	case TYPE_CRC64_ECMA182:
		if (pmull) implementations.push_back({"PMULL", &folded<&foldData<false>, &reduceNormal64, &multiply, CRC64_ECMA182_FACTORS, LANE_SIZE, FOLD_MIN_LENGTH, &crc64ECMA182Portable>});
		break;
	default:
		break;
	}
}


} // namespace Poco
