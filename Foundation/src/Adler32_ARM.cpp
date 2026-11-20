//
// Adler32_ARM.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Adler-32 implementation with the NEON instructions of ARMv8.
//
// References:
// - RFC 1950, section 8.2: the Adler-32 algorithm.
// - Arm C Language Extensions (ACLE), https://arm-software.github.io/acle/:
//   NEON intrinsics.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Adler32.h"
#include <arm_neon.h>
#include <array>


namespace Poco {


namespace {


constexpr std::size_t STEP_SIZE = 32;
constexpr std::size_t HALF_VECTOR_SIZE = 8;

// The weight of byte i of a step is STEP_SIZE - i.
constexpr std::array<UInt8, STEP_SIZE> WEIGHTS = []
{
	std::array<UInt8, STEP_SIZE> weights{};
	for (std::size_t i = 0; i < STEP_SIZE; ++i)
		weights[i] = static_cast<UInt8>(STEP_SIZE - i);
	return weights;
}();


[[nodiscard]] inline UInt64 sum(uint32x4_t lanes)
	// the lanes added up in 64 bits
{
	return vaddvq_u64(vpaddlq_u32(lanes));
}


[[nodiscard]] Adler32::Sums sumSteps32(std::span<const unsigned char> steps)
	// Each step adds bytes into 16-bit lanes, each of which collects four bytes
	// (two of each 16-byte half), and pairs of those into the 32-bit lanes of
	// the byte sums, eight bytes per lane. The weighted sums
	// multiply each byte with its weight into a 16-bit lane that collects four
	// bytes (at most 255 * (32 + 24 + 16 + 8)), then add pairs of those into 32 bits.
{
	const uint8x8_t weights0 = vld1_u8(WEIGHTS.data());
	const uint8x8_t weights1 = vld1_u8(WEIGHTS.data() + HALF_VECTOR_SIZE);
	const uint8x8_t weights2 = vld1_u8(WEIGHTS.data() + 2 * HALF_VECTOR_SIZE);
	const uint8x8_t weights3 = vld1_u8(WEIGHTS.data() + 3 * HALF_VECTOR_SIZE);
	uint32x4_t byteSums = vdupq_n_u32(0);
	uint32x4_t prefixSums = vdupq_n_u32(0);
	uint32x4_t weightedSums = vdupq_n_u32(0);
	const unsigned char* pData = steps.data();
	const std::size_t count = steps.size() / STEP_SIZE;
	for (std::size_t i = 0; i < count; ++i, pData += STEP_SIZE)
	{
		const uint8x16_t first = vld1q_u8(pData);
		const uint8x16_t second = vld1q_u8(pData + STEP_SIZE / 2);
		prefixSums = vaddq_u32(prefixSums, byteSums);
		byteSums = vpadalq_u16(byteSums, vpadalq_u8(vpaddlq_u8(first), second));
		uint16x8_t weighted = vmull_u8(vget_low_u8(first), weights0);
		weighted = vmlal_u8(weighted, vget_high_u8(first), weights1);
		weighted = vmlal_u8(weighted, vget_low_u8(second), weights2);
		weighted = vmlal_u8(weighted, vget_high_u8(second), weights3);
		weightedSums = vpadalq_u16(weightedSums, weighted);
	}
	return {sum(byteSums), STEP_SIZE * sum(prefixSums) + sum(weightedSums)};
}


} // namespace


void Adler32::addInstructionImplementations(std::vector<Implementation>& implementations)
{
	// NEON is part of the ARMv8-A base architecture
	implementations.push_back({"NEON", &adler32NEON});
}


UInt32 Adler32::adler32NEON(UInt32 adler, std::span<const unsigned char> data)
{
	return updateInSteps(adler, data, STEP_SIZE, &sumSteps32);
}


} // namespace Poco
