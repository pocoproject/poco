//
// Adler32_X86.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Adler-32 implementations with the SSSE3, AVX2 and AVX-512BW instructions
// of x86-64.
//
// References:
// - RFC 1950, section 8.2: the Adler-32 algorithm.
// - Intel Intrinsics Guide,
//   https://www.intel.com/content/www/us/en/docs/intrinsics-guide/
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Adler32.h"
#include "CpuFeatures.h"
#include <immintrin.h>
#include <array>
#include <iterator>
#include <numeric>


// GCC and Clang emit the instructions only in functions compiled for them;
// MSVC accepts the intrinsics everywhere.
#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
	#define POCO_ADLER32_TARGET_SSSE3 __attribute__((target("ssse3")))
	#define POCO_ADLER32_TARGET_AVX2 __attribute__((target("avx2")))
	#define POCO_ADLER32_TARGET_AVX512 __attribute__((target("avx512f,avx512bw")))
#else
	#define POCO_ADLER32_TARGET_SSSE3
	#define POCO_ADLER32_TARGET_AVX2
	#define POCO_ADLER32_TARGET_AVX512
#endif


namespace Poco {


namespace {


constexpr std::size_t MAX_STEP_SIZE = 64;

// The weight of byte i of a step of n bytes is n - i: the last n entries.
constexpr std::array<signed char, MAX_STEP_SIZE> WEIGHTS = []
{
	std::array<signed char, MAX_STEP_SIZE> weights{};
	for (std::size_t i = 0; i < MAX_STEP_SIZE; ++i)
		weights[i] = static_cast<signed char>(MAX_STEP_SIZE - i);
	return weights;
}();


template <std::size_t lanes>
[[nodiscard]] Adler32::Sums stepSums(const UInt32 (&byteSums)[lanes], const UInt32 (&prefixSums)[lanes], const UInt32 (&weightedSums)[lanes], std::size_t stepSize)
	// the lanes added up in 64 bits
{
	const auto sum = [](const UInt32 (&values)[lanes])
	{
		return std::accumulate(std::begin(values), std::end(values), static_cast<UInt64>(0));
	};
	return {sum(byteSums), stepSize * sum(prefixSums) + sum(weightedSums)};
}


// Each step: PSADBW gives a 16-bit sum per eight bytes, so the byte sums are
// kept in every other 32-bit lane; PMADDUBSW multiplies the bytes with their weights and adds
// pairs into 16 bits (at most 255 * (64 + 63), no saturation); PMADDWD adds
// pairs of those into the 32-bit lanes of the weighted sums.


[[nodiscard]] POCO_ADLER32_TARGET_SSSE3 Adler32::Sums sumSteps16(std::span<const unsigned char> steps)
{
	constexpr std::size_t STEP_SIZE = sizeof(__m128i);
	const __m128i zero = _mm_setzero_si128();
	const __m128i ones = _mm_set1_epi16(1);
	const __m128i weights = _mm_loadu_si128(reinterpret_cast<const __m128i*>(WEIGHTS.data() + MAX_STEP_SIZE - STEP_SIZE));
	__m128i byteSums = zero;
	__m128i prefixSums = zero;
	__m128i weightedSums = zero;
	const unsigned char* pData = steps.data();
	const std::size_t count = steps.size() / STEP_SIZE;
	for (std::size_t i = 0; i < count; ++i, pData += STEP_SIZE)
	{
		const __m128i bytes = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pData));
		prefixSums = _mm_add_epi32(prefixSums, byteSums);
		byteSums = _mm_add_epi32(byteSums, _mm_sad_epu8(bytes, zero));
		weightedSums = _mm_add_epi32(weightedSums, _mm_madd_epi16(_mm_maddubs_epi16(bytes, weights), ones));
	}
	UInt32 lanes[3][STEP_SIZE / sizeof(UInt32)];
	_mm_storeu_si128(reinterpret_cast<__m128i*>(lanes[0]), byteSums);
	_mm_storeu_si128(reinterpret_cast<__m128i*>(lanes[1]), prefixSums);
	_mm_storeu_si128(reinterpret_cast<__m128i*>(lanes[2]), weightedSums);
	return stepSums(lanes[0], lanes[1], lanes[2], STEP_SIZE);
}


[[nodiscard]] POCO_ADLER32_TARGET_AVX2 Adler32::Sums sumSteps32(std::span<const unsigned char> steps)
{
	constexpr std::size_t STEP_SIZE = sizeof(__m256i);
	const __m256i zero = _mm256_setzero_si256();
	const __m256i ones = _mm256_set1_epi16(1);
	const __m256i weights = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(WEIGHTS.data() + MAX_STEP_SIZE - STEP_SIZE));
	__m256i byteSums = zero;
	__m256i prefixSums = zero;
	__m256i weightedSums = zero;
	const unsigned char* pData = steps.data();
	const std::size_t count = steps.size() / STEP_SIZE;
	for (std::size_t i = 0; i < count; ++i, pData += STEP_SIZE)
	{
		const __m256i bytes = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pData));
		prefixSums = _mm256_add_epi32(prefixSums, byteSums);
		byteSums = _mm256_add_epi32(byteSums, _mm256_sad_epu8(bytes, zero));
		weightedSums = _mm256_add_epi32(weightedSums, _mm256_madd_epi16(_mm256_maddubs_epi16(bytes, weights), ones));
	}
	UInt32 lanes[3][STEP_SIZE / sizeof(UInt32)];
	_mm256_storeu_si256(reinterpret_cast<__m256i*>(lanes[0]), byteSums);
	_mm256_storeu_si256(reinterpret_cast<__m256i*>(lanes[1]), prefixSums);
	_mm256_storeu_si256(reinterpret_cast<__m256i*>(lanes[2]), weightedSums);
	return stepSums(lanes[0], lanes[1], lanes[2], STEP_SIZE);
}


[[nodiscard]] POCO_ADLER32_TARGET_AVX512 Adler32::Sums sumSteps64(std::span<const unsigned char> steps)
{
	constexpr std::size_t STEP_SIZE = sizeof(__m512i);
	const __m512i zero = _mm512_setzero_si512();
	const __m512i ones = _mm512_set1_epi16(1);
	const __m512i weights = _mm512_loadu_si512(WEIGHTS.data());
	__m512i byteSums = zero;
	__m512i prefixSums = zero;
	__m512i weightedSums = zero;
	const unsigned char* pData = steps.data();
	const std::size_t count = steps.size() / STEP_SIZE;
	for (std::size_t i = 0; i < count; ++i, pData += STEP_SIZE)
	{
		const __m512i bytes = _mm512_loadu_si512(pData);
		prefixSums = _mm512_add_epi32(prefixSums, byteSums);
		byteSums = _mm512_add_epi32(byteSums, _mm512_sad_epu8(bytes, zero));
		weightedSums = _mm512_add_epi32(weightedSums, _mm512_madd_epi16(_mm512_maddubs_epi16(bytes, weights), ones));
	}
	UInt32 lanes[3][STEP_SIZE / sizeof(UInt32)];
	_mm512_storeu_si512(lanes[0], byteSums);
	_mm512_storeu_si512(lanes[1], prefixSums);
	_mm512_storeu_si512(lanes[2], weightedSums);
	return stepSums(lanes[0], lanes[1], lanes[2], STEP_SIZE);
}


} // namespace


void Adler32::addInstructionImplementations(std::vector<Implementation>& implementations)
{
	const CpuFeatures& features = cpuFeatures();
	if (features.ssse3) implementations.push_back({"SSSE3", &adler32SSSE3});
	if (features.avx2) implementations.push_back({"AVX2", &adler32AVX2});
	if (features.avx512 && features.avx512bw) implementations.push_back({"AVX-512BW", &adler32AVX512});
}


UInt32 Adler32::adler32SSSE3(UInt32 adler, std::span<const unsigned char> data)
{
	return updateInSteps(adler, data, sizeof(__m128i), &sumSteps16);
}


UInt32 Adler32::adler32AVX2(UInt32 adler, std::span<const unsigned char> data)
{
	return updateInSteps(adler, data, sizeof(__m256i), &sumSteps32);
}


UInt32 Adler32::adler32AVX512(UInt32 adler, std::span<const unsigned char> data)
{
	return updateInSteps(adler, data, sizeof(__m512i), &sumSteps64);
}


} // namespace Poco
