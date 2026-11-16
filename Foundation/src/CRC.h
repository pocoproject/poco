//
// CRC.h
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Definition of the CRC class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_CRC_INCLUDED
#define Foundation_CRC_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Checksum.h"
#include <cstddef>
#include <span>
#include <type_traits>
#include <vector>


#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG) || defined(POCO_COMPILER_MSVC)
	#if POCO_ARCH == POCO_ARCH_AMD64
		#define POCO_CRC_X86_64 1
	#elif POCO_ARCH == POCO_ARCH_AARCH64 && defined(POCO_ARCH_LITTLE_ENDIAN)
		#define POCO_CRC_ARMV8 1
	#endif
#endif


namespace Poco {


class Foundation_API CRC
	/// The CRC-32 and CRC-64 implementations behind Checksum.
	///
	/// An implementation updates a CRC register with data and returns the
	/// register. The register holds the CRC before the final XOR of its type;
	/// CRC-32 uses the low 32 bits, and its upper 32 bits must be zero. Checksum
	/// applies the initial value and the final XOR.
	///
	/// The portable implementations are zlib's crc32_z() for CRC-32 and tables
	/// for eight bytes at a time for CRC-64. The others use the CRC32
	/// instructions of ARMv8, or fold 128-bit lanes of the data with carry-less
	/// multiplication (PCLMULQDQ on x86-64, PMULL on ARMv8; with VPCLMULQDQ
	/// two or four lanes per register) and reduce the last lane with a
	/// Barrett reduction. cpuFeatures() decides which of them the
	/// CPU supports.
	///
	/// This class is internal to the Foundation library; it is exported because
	/// the test suite and the benchmark compare the implementations.
	///
	/// References:
	///   - V. Gopal, E. Ozturk, J. Guilford et al., "Fast CRC Computation for
	///     Generic Polynomials Using PCLMULQDQ Instruction", Intel, 2009:
	///     folding and Barrett reduction.
	///   - G. Cook, "Catalogue of parametrised CRC algorithms",
	///     https://reveng.sourceforge.io/crc-catalogue/: parameters and check
	///     values of CRC-32/ISO-HDLC, CRC-64/XZ and CRC-64/ECMA-182.
	///   - R. N. Williams, "A Painless Guide to CRC Error Detection Algorithms",
	///     1993: reflected and non-reflected table-driven CRCs.
{
public:
	using Update = UInt64 (*)(UInt64 crc, std::span<const unsigned char> data);
		/// Updates the CRC register crc with the data and returns it.
		/// For CRC-32, the upper 32 bits of crc must be zero.

	struct Implementation
		/// A named implementation of a CRC type.
	{
		const char* name;
		Update update;
	};

	struct Product
		/// The 128-bit carry-less product of two 64-bit values.
	{
		UInt64 low;
		UInt64 high;
	};

	using Multiply = Product (*)(UInt64 a, UInt64 b);
		/// Carry-less multiplication.

	struct FoldConstants
		/// The factors of the folding implementations for a polynomial P of the
		/// CRC's width w. A fold pair moves a lane n bits towards the end of the
		/// data, with x^(n + 64) mod P for the upper half of the lane and
		/// x^n mod P for the lower half. A pair holds the factors for the low and
		/// the high 64 bits of a 128-bit lane; for a normal CRC these are the lower
		/// and the upper half of the lane, for a reflected CRC the upper and the
		/// lower half.
	{
		UInt64 foldBlock[2];       /// n = 8 * FOLD_BLOCK_SIZE
		UInt64 foldLane[2];        /// n = 8 * LANE_SIZE
		UInt64 foldBlock256[2];    /// n = 8 * FOLD_BLOCK_SIZE_256
		UInt64 foldRegister256[2]; /// n = 256, the width of a register
		UInt64 foldBlock512[2];    /// n = 8 * FOLD_BLOCK_SIZE_512
		UInt64 foldRegister512[2]; /// n = 512, the width of a register
		UInt64 reduce[2];
			/// x^(64 + w) mod P moves the upper half of the last lane for
			/// lane * x^w; x^(2 * w) mod P folds the 32 bits above x^64 (CRC-32 only).
		UInt64 mu;                 /// floor(x^(2 * w) / P) without its leading term x^w, for the Barrett reduction
		UInt64 polynomial;         /// P without its leading term x^w
	};

	static constexpr UInt64 CRC32_POLYNOMIAL = 0x04C11DB7;
	static constexpr UInt64 CRC64_POLYNOMIAL = 0x42F0E1EBA9EA3693;
	static constexpr UInt64 CRC32_MASK = 0xFFFFFFFF;
	static constexpr std::size_t LANE_SIZE = 16;
	static constexpr std::size_t FOLD_LANES = 8;
	static constexpr std::size_t FOLD_BLOCK_SIZE = FOLD_LANES * LANE_SIZE;
	static constexpr std::size_t FOLD_MIN_LENGTH = 64;
		/// The folding implementations hand shorter data to the portable ones.
	static constexpr std::size_t FOLD_REGISTERS = 4;
		/// The VPCLMULQDQ implementations fold this many 256-bit or 512-bit
		/// registers, each holding two or four lanes, per iteration.
	static constexpr std::size_t FOLD_BLOCK_SIZE_256 = FOLD_REGISTERS * 2 * LANE_SIZE;
	static constexpr std::size_t FOLD_BLOCK_SIZE_512 = FOLD_REGISTERS * 4 * LANE_SIZE;
		/// FOLD_BLOCK_SIZE_256 and FOLD_BLOCK_SIZE_512: the VPCLMULQDQ
		/// implementations with 256-bit and 512-bit registers hand shorter data
		/// to the PCLMULQDQ ones.

	[[nodiscard]] static Update implementation(Checksum::Type type);
		/// Returns the fastest implementation of the given CRC type that the CPU
		/// supports. Throws an InvalidArgumentException for TYPE_ADLER32.

	[[nodiscard]] static std::vector<Implementation> implementations(Checksum::Type type);
		/// Returns the implementations of the given CRC type that the CPU supports,
		/// the portable one first and the fastest one last.
		/// Throws an InvalidArgumentException for TYPE_ADLER32.

	[[nodiscard]] static consteval UInt64 reflect(UInt64 value);
		/// Returns the value with the order of its 64 bits reversed.

	[[nodiscard]] static consteval UInt64 xPowerModP(unsigned exponent, UInt64 polynomial, int width);
		/// Returns x^exponent mod P for the polynomial P of the given width,
		/// given without its leading term.

	[[nodiscard]] static consteval UInt64 barrettMu(UInt64 polynomial, int width);
		/// Returns floor(x^(2 * width) / P) without its leading term x^width.

	[[nodiscard]] static consteval FoldConstants foldConstants(UInt64 polynomial, int width, bool reflected);
		/// Returns the factors of the folding implementations for a CRC with the
		/// given polynomial and width. The factors of a reflected CRC are
		/// reflected in 64 bits; its fold and reduce factors also include the
		/// factor 1/x that the carry-less multiplication of reflected values
		/// introduces.

	CRC() = delete;

private:
#if defined(POCO_CRC_X86_64) || defined(POCO_CRC_ARMV8)
	template <auto foldBlocks, auto reduce, auto multiply, const FoldConstants& factors,
		std::size_t blockSize, std::size_t minLength, auto tail>
	[[nodiscard]] static UInt64 folded(UInt64 crc, std::span<const unsigned char> data);
		/// Folds the whole blocks of blockSize bytes in the data with foldBlocks and
		/// reduces them to the CRC register with reduce. Data shorter than minLength
		/// and the bytes after the last whole block go to tail, which continues from
		/// the register.

	[[nodiscard]] static UInt64 reduceReflected64(const Product& lane, const FoldConstants& factors, Multiply multiply);
		/// Returns the CRC-64 register for the last lane of a reflected CRC.

	[[nodiscard]] static UInt64 reduceNormal64(const Product& lane, const FoldConstants& factors, Multiply multiply);
		/// Returns the CRC-64 register for the last lane of a CRC that is not reflected.
#endif

	static void addInstructionImplementations(Checksum::Type type, std::vector<Implementation>& implementations);
		/// Appends the implementations of the given CRC type that use instruction
		/// set extensions of the CPU, the fastest one last.

	[[nodiscard]] static UInt64 crc32Zlib(UInt64 crc, std::span<const unsigned char> data);
		/// CRC-32 (reflected) with zlib's crc32_z(); always available.

	[[nodiscard]] static UInt64 crc64XZPortable(UInt64 crc, std::span<const unsigned char> data);
		/// CRC-64/XZ (reflected) with slicing-by-8; always available.

	[[nodiscard]] static UInt64 crc64ECMA182Portable(UInt64 crc, std::span<const unsigned char> data);
		/// CRC-64/ECMA-182 (normal bit order) with slicing-by-8; always available.

#if defined(POCO_CRC_X86_64)
	[[nodiscard]] static UInt64 reduceReflected32(const Product& lane, const FoldConstants& factors, Multiply multiply);
		/// Returns the CRC-32 register for the last lane of a reflected CRC.

	template <bool reflected, auto reduce, const FoldConstants& factors, auto portable>
	static void addFolding(std::vector<Implementation>& implementations, bool vpclmul256, bool vpclmul512);
		/// Appends the implementation of a CRC type that folds with PCLMULQDQ and,
		/// where vpclmul256 or vpclmul512 is true, the ones that fold 256-bit or
		/// 512-bit registers with VPCLMULQDQ. The PCLMULQDQ implementation hands
		/// shorter data and the rest to portable.
#endif
};


//
// inlines
//
consteval UInt64 CRC::reflect(UInt64 value)
{
	UInt64 result = 0;
	for (int i = 0; i < 64; ++i)
	{
		result = (result << 1) | (value & 1);
		value >>= 1;
	}
	return result;
}


consteval UInt64 CRC::xPowerModP(unsigned exponent, UInt64 polynomial, int width)
{
	// multiplies 1 by x exponent times; whenever the term x^width appears, it is
	// replaced by the rest of P (x^width = P' mod P, with P = x^width + P')
	const UInt64 topBit = static_cast<UInt64>(1) << (width - 1);
	const UInt64 mask = topBit | (topBit - 1);
	UInt64 result = 1;
	for (unsigned i = 0; i < exponent; ++i)
	{
		const bool carry = (result & topBit) != 0;
		result = (result << 1) & mask;
		if (carry) result ^= polynomial;
	}
	return result;
}


consteval UInt64 CRC::barrettMu(UInt64 polynomial, int width)
{
	// long division of x^(2 * width) by P, one quotient bit per step from
	// x^width down to x^0; remainder holds the width bits below the current
	// leading term, which is set where P has to be subtracted (added in GF(2))
	const UInt64 topBit = static_cast<UInt64>(1) << (width - 1);
	const UInt64 mask = topBit | (topBit - 1);
	UInt64 mu = 0;
	UInt64 remainder = 0;
	bool leading = true;
	for (int i = width; i >= 0; --i)
	{
		if (leading)
		{
			if (i < width) mu |= static_cast<UInt64>(1) << i;
			remainder ^= polynomial;
		}
		leading = (remainder & topBit) != 0;
		remainder = (remainder << 1) & mask;
	}
	return mu;
}


consteval CRC::FoldConstants CRC::foldConstants(UInt64 polynomial, int width, bool reflected)
{
	constexpr unsigned BLOCK_BITS = FOLD_BLOCK_SIZE * 8;
	constexpr unsigned LANE_BITS = LANE_SIZE * 8;
	constexpr unsigned BLOCK_BITS_256 = FOLD_BLOCK_SIZE_256 * 8;
	constexpr unsigned BLOCK_BITS_512 = FOLD_BLOCK_SIZE_512 * 8;
	constexpr unsigned REGISTER_BITS_256 = 2 * LANE_BITS;
	constexpr unsigned REGISTER_BITS_512 = 4 * LANE_BITS;
	constexpr unsigned HALF_BITS = LANE_BITS / 2;
	const unsigned crcBits = static_cast<unsigned>(width);
	// A lane holds 128 bits; moving it n bits towards the end of the data
	// multiplies it by x^n: its upper 64 bits by x^(n + 64) mod P, its lower
	// 64 bits by x^n mod P.
	if (reflected)
	{
		// A reflected value holds its highest power in bit 0. The carry-less
		// product of two reflected 64-bit values is the product of the
		// polynomials times x, so each factor is x^(n - 1) mod P, reflected.
		// The upper half of a reflected lane is in its low 64 bits.
		auto factor = [polynomial, width](unsigned exponent) consteval
		{
			return reflect(xPowerModP(exponent - 1, polynomial, width));
		};
		return {
			.foldBlock = {factor(BLOCK_BITS + HALF_BITS), factor(BLOCK_BITS)},
			.foldLane = {factor(LANE_BITS + HALF_BITS), factor(LANE_BITS)},
			.foldBlock256 = {factor(BLOCK_BITS_256 + HALF_BITS), factor(BLOCK_BITS_256)},
			.foldRegister256 = {factor(REGISTER_BITS_256 + HALF_BITS), factor(REGISTER_BITS_256)},
			.foldBlock512 = {factor(BLOCK_BITS_512 + HALF_BITS), factor(BLOCK_BITS_512)},
			.foldRegister512 = {factor(REGISTER_BITS_512 + HALF_BITS), factor(REGISTER_BITS_512)},
			.reduce = {factor(HALF_BITS + crcBits), factor(2 * crcBits)},
			.mu = reflect(barrettMu(polynomial, width)),
			.polynomial = reflect(polynomial)
		};
	}
	else
	{
		// a normal value holds its highest power in the most significant bit;
		// the upper half of a lane is in its high 64 bits
		auto factor = [polynomial, width](unsigned exponent) consteval
		{
			return xPowerModP(exponent, polynomial, width);
		};
		return {
			.foldBlock = {factor(BLOCK_BITS), factor(BLOCK_BITS + HALF_BITS)},
			.foldLane = {factor(LANE_BITS), factor(LANE_BITS + HALF_BITS)},
			.foldBlock256 = {factor(BLOCK_BITS_256), factor(BLOCK_BITS_256 + HALF_BITS)},
			.foldRegister256 = {factor(REGISTER_BITS_256), factor(REGISTER_BITS_256 + HALF_BITS)},
			.foldBlock512 = {factor(BLOCK_BITS_512), factor(BLOCK_BITS_512 + HALF_BITS)},
			.foldRegister512 = {factor(REGISTER_BITS_512), factor(REGISTER_BITS_512 + HALF_BITS)},
			.reduce = {factor(HALF_BITS + crcBits), factor(2 * crcBits)},
			.mu = barrettMu(polynomial, width),
			.polynomial = polynomial
		};
	}
}


#if defined(POCO_CRC_X86_64) || defined(POCO_CRC_ARMV8)


template <auto foldBlocks, auto reduce, auto multiply, const CRC::FoldConstants& factors,
	std::size_t blockSize, std::size_t minLength, auto tail>
UInt64 CRC::folded(UInt64 crc, std::span<const unsigned char> data)
{
	static_assert(std::is_invocable_r_v<Product, decltype(foldBlocks), UInt64, std::span<const unsigned char>, const FoldConstants&>,
		"foldBlocks must return a CRC::Product for a register, a span and the fold constants");
	static_assert(std::is_invocable_r_v<UInt64, decltype(reduce), const Product&, const FoldConstants&, Multiply>,
		"reduce must return a register for a CRC::Product, the fold constants and a CRC::Multiply");
	static_assert(std::is_convertible_v<decltype(multiply), Multiply>, "multiply must be a CRC::Multiply");
	static_assert(std::is_invocable_r_v<UInt64, decltype(tail), UInt64, std::span<const unsigned char>>,
		"tail must be callable as a CRC update function");
	static_assert(blockSize > 0 && blockSize % LANE_SIZE == 0, "blockSize must be a whole number of lanes");
	static_assert(minLength >= blockSize, "minLength must be at least one block");
	if (data.size() < minLength) return tail(crc, data);
	const std::size_t whole = data.size() - data.size() % blockSize;
	crc = reduce(foldBlocks(crc, data.first(whole), factors), factors, multiply);
	return tail(crc, data.subspan(whole));
}


#endif


} // namespace Poco


#endif // Foundation_CRC_INCLUDED
