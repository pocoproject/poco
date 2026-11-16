//
// CRC_X86.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// CRC implementations with the PCLMULQDQ and VPCLMULQDQ instructions of x86-64.
//
// References:
// - V. Gopal et al., "Fast CRC Computation for Generic Polynomials Using
//   PCLMULQDQ Instruction", Intel, 2009: folding; VPCLMULQDQ applies the
//   same fold to each 128-bit lane of a 256-bit or 512-bit register.
// - Intel Intrinsics Guide,
//   https://www.intel.com/content/www/us/en/docs/intrinsics-guide/
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CRC.h"
#include "CpuFeatures.h"
#include <immintrin.h>


// GCC and Clang emit the instructions only in functions compiled for them;
// MSVC accepts the intrinsics everywhere.
#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
	#define POCO_CRC_TARGET __attribute__((target("pclmul,sse4.1")))
	#define POCO_CRC_TARGET_256 __attribute__((target("pclmul,vpclmulqdq,avx2")))
	#define POCO_CRC_TARGET_512 __attribute__((target("pclmul,vpclmulqdq,avx512f,avx512vl,avx512bw")))
#else
	#define POCO_CRC_TARGET
	#define POCO_CRC_TARGET_256
	#define POCO_CRC_TARGET_512
#endif


namespace Poco {


namespace {


// the factors of the three CRCs, computed by the compiler from the polynomials
constexpr CRC::FoldConstants CRC32_FACTORS = CRC::foldConstants(CRC::CRC32_POLYNOMIAL, 32, true);
constexpr CRC::FoldConstants CRC64_XZ_FACTORS = CRC::foldConstants(CRC::CRC64_POLYNOMIAL, 64, true);
constexpr CRC::FoldConstants CRC64_ECMA182_FACTORS = CRC::foldConstants(CRC::CRC64_POLYNOMIAL, 64, false);


[[nodiscard]] POCO_CRC_TARGET inline __m128i load(const unsigned char* data)
{
	return _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
}


[[nodiscard]] POCO_CRC_TARGET inline __m128i factorPair(const UInt64 (&factors)[2])
	// factors[0] in the low 64 bits, factors[1] in the high 64 bits
{
	return _mm_set_epi64x(static_cast<long long>(factors[1]), static_cast<long long>(factors[0]));
}


[[nodiscard]] POCO_CRC_TARGET inline __m128i fold(__m128i lane, __m128i factors)
	// One fold step: low half of the lane times the low factor plus high half
	// times the high factor, a 128-bit value congruent to the lane moved by the
	// distance of the factors. The caller adds the data at that distance.
{
	return _mm_xor_si128(_mm_clmulepi64_si128(lane, factors, 0x00), _mm_clmulepi64_si128(lane, factors, 0x11));
}


[[nodiscard]] POCO_CRC_TARGET inline __m128i byteReversal()
	// the shuffle that reverses the 16 bytes of a lane
{
	return _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET inline __m128i loadLane(const unsigned char* data)
{
	if constexpr (reflected)
	{
		// a reflected CRC takes bit 0 of the first byte as the highest power,
		// which a little-endian load puts into bit 0 of the lane
		return load(data);
	}
	else
	{
		// a normal CRC takes the first byte as the highest power: reverse the 16
		// bytes, so that the first byte becomes the most significant one
		return _mm_shuffle_epi8(load(data), byteReversal());
	}
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET inline __m128i initialLane(UInt64 crc)
{
	// the register is added to the first w bits of the data: the low bits of a
	// reflected lane, the high 64 bits of a normal lane (a 64-bit CRC)
	const __m128i initial = _mm_cvtsi64_si128(static_cast<long long>(crc));
	return reflected ? initial : _mm_slli_si128(initial, 8);
}


// TODO: this loop is the same in CRC_X86.cpp and CRC_ARM.cpp; share it through a template over a lane type.
template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET __m128i foldLanes(UInt64 crc, std::span<const unsigned char> data, const CRC::FoldConstants& factors)
	// Folds the data, a multiple of LANE_SIZE and at least FOLD_MIN_LENGTH
	// bytes, into one lane (Gopal et al., "folding").
{
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	const __m128i firstBits = initialLane<reflected>(crc);
	const __m128i foldLane = factorPair(factors.foldLane);
	__m128i lane;
	if (length >= CRC::FOLD_BLOCK_SIZE)
	{
		// the FOLD_LANES lanes of a block are folded independently, each
		// FOLD_BLOCK_SIZE bytes forward onto its lane of the next block, so that
		// several multiplications are in flight
		__m128i lanes[CRC::FOLD_LANES];
		for (std::size_t i = 0; i < CRC::FOLD_LANES; ++i)
			lanes[i] = loadLane<reflected>(pData + i * CRC::LANE_SIZE);
		lanes[0] = _mm_xor_si128(lanes[0], firstBits);
		pData += CRC::FOLD_BLOCK_SIZE;
		length -= CRC::FOLD_BLOCK_SIZE;

		const __m128i foldBlock = factorPair(factors.foldBlock);
		for (; length >= CRC::FOLD_BLOCK_SIZE; length -= CRC::FOLD_BLOCK_SIZE, pData += CRC::FOLD_BLOCK_SIZE)
		{
			for (std::size_t i = 0; i < CRC::FOLD_LANES; ++i)
				lanes[i] = _mm_xor_si128(fold(lanes[i], foldBlock), loadLane<reflected>(pData + i * CRC::LANE_SIZE));
		}
		// combine the lanes: fold each one LANE_SIZE bytes onto the next
		lane = lanes[0];
		for (std::size_t i = 1; i < CRC::FOLD_LANES; ++i)
			lane = _mm_xor_si128(fold(lane, foldLane), lanes[i]);
	}
	else
	{
		// less than one block: a single lane
		lane = _mm_xor_si128(loadLane<reflected>(pData), firstBits);
		pData += CRC::LANE_SIZE;
		length -= CRC::LANE_SIZE;
	}
	// the remaining lanes, one at a time
	for (; length >= CRC::LANE_SIZE; length -= CRC::LANE_SIZE, pData += CRC::LANE_SIZE)
		lane = _mm_xor_si128(fold(lane, foldLane), loadLane<reflected>(pData));
	return lane;
}


[[nodiscard]] POCO_CRC_TARGET inline CRC::Product halves(__m128i lane)
{
	return {static_cast<UInt64>(_mm_cvtsi128_si64(lane)), static_cast<UInt64>(_mm_extract_epi64(lane, 1))};
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET CRC::Product foldData(UInt64 crc, std::span<const unsigned char> data, const CRC::FoldConstants& factors)
	// the folded lane as two 64-bit halves for the reduction
{
	return halves(foldLanes<reflected>(crc, data, factors));
}


[[nodiscard]] POCO_CRC_TARGET CRC::Product multiply(UInt64 a, UInt64 b)
	// carry-less product of two 64-bit values, for the reduction
{
	return halves(_mm_clmulepi64_si128(_mm_cvtsi64_si128(static_cast<long long>(a)), _mm_cvtsi64_si128(static_cast<long long>(b)), 0x00));
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET_256 inline __m256i loadLanes256(const unsigned char* data)
	// two lanes, each loaded as loadLane() does
{
	const __m256i bytes = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data));
	if constexpr (reflected)
		return bytes;
	else
		return _mm256_shuffle_epi8(bytes, _mm256_broadcastsi128_si256(byteReversal()));
}


[[nodiscard]] POCO_CRC_TARGET_256 inline __m256i fold256(__m256i lanes, __m256i factors)
	// fold() for each lane of the register
{
	return _mm256_xor_si256(_mm256_clmulepi64_epi128(lanes, factors, 0x00), _mm256_clmulepi64_epi128(lanes, factors, 0x11));
}


[[nodiscard]] POCO_CRC_TARGET_256 inline __m128i combineLanes256(__m256i lanes, const CRC::FoldConstants& factors)
	// folds the first lane LANE_SIZE bytes onto the second
{
	return _mm_xor_si128(fold(_mm256_castsi256_si128(lanes), factorPair(factors.foldLane)), _mm256_extracti128_si256(lanes, 1));
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET_256 CRC::Product foldData256(UInt64 crc, std::span<const unsigned char> data, const CRC::FoldConstants& factors)
	// Folds the data, a multiple of FOLD_BLOCK_SIZE_256 bytes, into one lane as
	// foldLanes() does, with FOLD_REGISTERS registers of two lanes each.
{
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	__m256i registers[CRC::FOLD_REGISTERS];
	for (std::size_t i = 0; i < CRC::FOLD_REGISTERS; ++i)
		registers[i] = loadLanes256<reflected>(pData + i * sizeof(__m256i));
	registers[0] = _mm256_xor_si256(registers[0], _mm256_zextsi128_si256(initialLane<reflected>(crc)));
	pData += CRC::FOLD_BLOCK_SIZE_256;
	length -= CRC::FOLD_BLOCK_SIZE_256;

	const __m256i foldBlock = _mm256_broadcastsi128_si256(factorPair(factors.foldBlock256));
	for (; length >= CRC::FOLD_BLOCK_SIZE_256; length -= CRC::FOLD_BLOCK_SIZE_256, pData += CRC::FOLD_BLOCK_SIZE_256)
	{
		for (std::size_t i = 0; i < CRC::FOLD_REGISTERS; ++i)
			registers[i] = _mm256_xor_si256(fold256(registers[i], foldBlock), loadLanes256<reflected>(pData + i * sizeof(__m256i)));
	}
	// combine the registers: fold each one 32 bytes onto the next, then the two lanes of the last one
	const __m256i foldRegister = _mm256_broadcastsi128_si256(factorPair(factors.foldRegister256));
	__m256i lanes = registers[0];
	for (std::size_t i = 1; i < CRC::FOLD_REGISTERS; ++i)
		lanes = _mm256_xor_si256(fold256(lanes, foldRegister), registers[i]);
	return halves(combineLanes256(lanes, factors));
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET_512 inline __m512i loadLanes512(const unsigned char* data)
	// four lanes, each loaded as loadLane() does; the byte shuffle needs AVX-512BW
{
	const __m512i bytes = _mm512_loadu_si512(data);
	if constexpr (reflected)
		return bytes;
	else
		return _mm512_shuffle_epi8(bytes, _mm512_broadcast_i32x4(byteReversal()));
}


[[nodiscard]] POCO_CRC_TARGET_512 inline __m512i fold512(__m512i lanes, __m512i factors, __m512i data)
	// fold() for each lane of the register, plus the data at the distance of
	// the factors; one three-way XOR (truth table 0x96) adds the products and the data
{
	return _mm512_ternarylogic_epi64(_mm512_clmulepi64_epi128(lanes, factors, 0x00), _mm512_clmulepi64_epi128(lanes, factors, 0x11), data, 0x96);
}


template <bool reflected>
[[nodiscard]] POCO_CRC_TARGET_512 CRC::Product foldData512(UInt64 crc, std::span<const unsigned char> data, const CRC::FoldConstants& factors)
	// Folds the data, a multiple of FOLD_BLOCK_SIZE_512 bytes, into one lane as
	// foldLanes() does, with FOLD_REGISTERS registers of four lanes each.
{
	const unsigned char* pData = data.data();
	std::size_t length = data.size();
	__m512i registers[CRC::FOLD_REGISTERS];
	for (std::size_t i = 0; i < CRC::FOLD_REGISTERS; ++i)
		registers[i] = loadLanes512<reflected>(pData + i * sizeof(__m512i));
	registers[0] = _mm512_xor_si512(registers[0], _mm512_zextsi128_si512(initialLane<reflected>(crc)));
	pData += CRC::FOLD_BLOCK_SIZE_512;
	length -= CRC::FOLD_BLOCK_SIZE_512;

	const __m512i foldBlock = _mm512_broadcast_i32x4(factorPair(factors.foldBlock512));
	for (; length >= CRC::FOLD_BLOCK_SIZE_512; length -= CRC::FOLD_BLOCK_SIZE_512, pData += CRC::FOLD_BLOCK_SIZE_512)
	{
		for (std::size_t i = 0; i < CRC::FOLD_REGISTERS; ++i)
			registers[i] = fold512(registers[i], foldBlock, loadLanes512<reflected>(pData + i * sizeof(__m512i)));
	}
	// combine the registers: fold each one 64 bytes onto the next, then the
	// lower two lanes of the last one 32 bytes onto the upper two
	const __m512i foldRegister = _mm512_broadcast_i32x4(factorPair(factors.foldRegister512));
	__m512i lanes = registers[0];
	for (std::size_t i = 1; i < CRC::FOLD_REGISTERS; ++i)
		lanes = fold512(lanes, foldRegister, registers[i]);
	const __m256i foldRegister256 = _mm256_broadcastsi128_si256(factorPair(factors.foldRegister256));
	const __m256i upper = _mm512_extracti64x4_epi64(lanes, 1);
	return halves(combineLanes256(_mm256_xor_si256(fold256(_mm512_castsi512_si256(lanes), foldRegister256), upper), factors));
}


} // namespace


template <bool reflected, auto reduce, const CRC::FoldConstants& factors, auto portable>
void CRC::addFolding(std::vector<Implementation>& implementations, bool vpclmul256, bool vpclmul512)
{
	static_assert(std::is_invocable_r_v<UInt64, decltype(reduce), const Product&, const FoldConstants&, Multiply>,
		"reduce must return a register for a CRC::Product, the fold constants and a CRC::Multiply");
	static_assert(std::is_convertible_v<decltype(portable), Update>, "portable must be a CRC update function");
	constexpr Update clmul128 = &folded<&foldData<reflected>, reduce, &multiply, factors, LANE_SIZE, FOLD_MIN_LENGTH, portable>;
	constexpr Update clmul256 = &folded<&foldData256<reflected>, reduce, &multiply, factors, FOLD_BLOCK_SIZE_256, FOLD_BLOCK_SIZE_256, clmul128>;
	constexpr Update clmul512 = &folded<&foldData512<reflected>, reduce, &multiply, factors, FOLD_BLOCK_SIZE_512, FOLD_BLOCK_SIZE_512, clmul128>;
	implementations.push_back({"PCLMULQDQ", clmul128});
	if (vpclmul256) implementations.push_back({"VPCLMULQDQ AVX2", clmul256});
	if (vpclmul512) implementations.push_back({"VPCLMULQDQ AVX-512", clmul512});
}


void CRC::addInstructionImplementations(Checksum::Type type, std::vector<Implementation>& implementations)
{
	// all three CRCs fold with PCLMULQDQ; SSE4.1 for _mm_extract_epi64() and,
	// with SSSE3, which every CPU with SSE4.1 has, _mm_shuffle_epi8()
	const CpuFeatures& features = cpuFeatures();
	if (!features.pclmulqdq || !features.sse41) return;
	// the VPCLMULQDQ implementations hand shorter data and the rest to the PCLMULQDQ ones
	const bool vpclmul256 = features.vpclmulqdq && features.avx2;
	const bool vpclmul512 = features.vpclmulqdq && features.avx512 && features.avx512bw;
	using enum Checksum::Type;
	switch (type)
	{
	case TYPE_CRC32:
		addFolding<true, &reduceReflected32, CRC32_FACTORS, &crc32Zlib>(implementations, vpclmul256, vpclmul512);
		break;
	case TYPE_CRC64_XZ:
		addFolding<true, &reduceReflected64, CRC64_XZ_FACTORS, &crc64XZPortable>(implementations, vpclmul256, vpclmul512);
		break;
	case TYPE_CRC64_ECMA182:
		addFolding<false, &reduceNormal64, CRC64_ECMA182_FACTORS, &crc64ECMA182Portable>(implementations, vpclmul256, vpclmul512);
		break;
	default:
		break;
	}
}


UInt64 CRC::reduceReflected32(const Product& lane, const FoldConstants& factors, Multiply multiply)
{
	// The CRC register is lane * x^32 mod P. The low 64 bits of a reflected lane
	// hold its upper half U, the high 64 bits its lower half L:
	// lane * x^32 = U * (x^96 mod P) + L * x^32 (mod P), a 96-bit value whose
	// upper 32 bits are folded once more with x^64 mod P into a 64-bit value V.
	const Product product = multiply(lane.low, factors.reduce[0]);
	const UInt64 upper = product.low ^ (lane.high << 32);
	const UInt64 value = multiply(upper, factors.reduce[1]).high ^ product.high ^ (lane.high >> 32);
	// Barrett reduction (Gopal et al.) of V = V1 * x^32 + V0 with mu = floor(x^64 / P):
	// q = V1 + floor(V1 * mu' / x^32), remainder = V0 + (q * P' mod x^32), where
	// mu = x^32 + mu' and P = x^32 + P'. In the
	// reflected 64-bit value, V1 is in the low and V0 in the high 32 bits; the
	// shifts pick the bits of the reflected products that hold these terms.
	const UInt64 valueUpper = value & CRC32_MASK;
	const UInt64 quotient = valueUpper ^ ((multiply(valueUpper, factors.mu).low >> 31) & CRC32_MASK);
	const Product multiple = multiply(quotient, factors.polynomial);
	return (value >> 32) ^ (((multiple.high << 1) | (multiple.low >> 63)) & CRC32_MASK);
}


} // namespace Poco
