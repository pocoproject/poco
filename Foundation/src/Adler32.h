//
// Adler32.h
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Definition of the Adler32 class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Adler32_INCLUDED
#define Foundation_Adler32_INCLUDED


#include "Poco/Foundation.h"
#include <cstddef>
#include <span>
#include <vector>


#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG) || defined(POCO_COMPILER_MSVC)
	#if POCO_ARCH == POCO_ARCH_AMD64
		#define POCO_ADLER32_X86_64 1
	#elif POCO_ARCH == POCO_ARCH_AARCH64
		#define POCO_ADLER32_ARMV8 1
	#endif
#endif


namespace Poco {


class Foundation_API Adler32
	/// The Adler-32 implementations behind Checksum.
	///
	/// An implementation updates an Adler-32 checksum with data and returns
	/// it: the sum A of the bytes plus one in the low 16 bits, the sum B of
	/// the values of A after each byte in the high 16 bits, both modulo 65521.
	///
	/// The portable implementation is zlib's adler32_z(). The others use the
	/// vector instructions of the CPU: NEON on ARMv8, SSSE3, AVX2 or AVX-512BW
	/// on x86-64. cpuFeatures() decides which of them the CPU supports.
	///
	/// This class is internal to the Foundation library; it is exported because
	/// the test suite and the benchmark compare the implementations.
	///
	/// Reference: P. Deutsch, J-L. Gailly, "ZLIB Compressed Data Format
	/// Specification version 3.3", RFC 1950, section 8.2: the Adler-32 algorithm.
{
public:
	using Update = UInt32 (*)(UInt32 adler, std::span<const unsigned char> data);
		/// Updates the checksum adler with the data and returns it.

	struct Implementation
		/// A named implementation of Adler-32.
	{
		const char* name;
		Update update;
	};

	struct Sums
		/// The sums of a block of n bytes d[0..n-1] that update a checksum:
		/// A' = A + bytes, B' = B + n * A + weighted.
	{
		UInt64 bytes;    /// the sum of d[i]
		UInt64 weighted; /// the sum of (n - i) * d[i]
	};

	[[nodiscard]] static Update implementation();
		/// Returns the fastest implementation that the CPU supports.

	[[nodiscard]] static std::vector<Implementation> implementations();
		/// Returns the implementations that the CPU supports, the portable one
		/// first and the fastest one last.

	Adler32() = delete;

private:
	static constexpr std::size_t MAX_STEPS = 2052;
		/// The vector implementations sum at most this many steps in 32-bit
		/// lanes before they update the checksum. A step adds at most eight
		/// bytes to a lane of the byte sums, 8 * 255 = 2040, and the byte sums
		/// of the steps before it to a lane of the prefix sums; after k steps
		/// of bytes 0xFF, a prefix lane holds 2040 * k * (k - 1) / 2, which
		/// stays below 2^32 for k up to 2052. A lane of the weighted sums
		/// grows by at most 8 * 255 * 64 per step and stays far below 2^32.

	using SumSteps = Sums (*)(std::span<const unsigned char> steps);
		/// Returns the sums of the vector steps in steps, which holds whole
		/// steps. A byte is weighted with its distance to the end of its step,
		/// and with the step size for each later step: the prefix sums, which
		/// add the byte sums of the earlier steps in each step, count these.

	[[nodiscard]] static UInt32 updateInSteps(UInt32 adler, std::span<const unsigned char> data, std::size_t stepSize, SumSteps sumSteps);
		/// Updates the checksum with the whole steps of stepSize bytes in the
		/// data, at most MAX_STEPS at a time, and the rest with zlib.

	static void addInstructionImplementations(std::vector<Implementation>& implementations);
		/// Appends the implementations that use instruction set extensions of
		/// the CPU, the fastest one last.

	[[nodiscard]] static UInt32 adler32Zlib(UInt32 adler, std::span<const unsigned char> data);
		/// Adler-32 with zlib's adler32_z(); always available.

#if defined(POCO_ADLER32_X86_64)
	[[nodiscard]] static UInt32 adler32SSSE3(UInt32 adler, std::span<const unsigned char> data);
		/// 16 bytes per step with SSSE3; selected when the CPU has SSSE3.

	[[nodiscard]] static UInt32 adler32AVX2(UInt32 adler, std::span<const unsigned char> data);
		/// 32 bytes per step with AVX2; selected when the CPU has AVX2.

	[[nodiscard]] static UInt32 adler32AVX512(UInt32 adler, std::span<const unsigned char> data);
		/// 64 bytes per step with AVX-512BW; selected when the CPU has AVX-512 F, VL and BW.
#elif defined(POCO_ADLER32_ARMV8)
	[[nodiscard]] static UInt32 adler32NEON(UInt32 adler, std::span<const unsigned char> data);
		/// 32 bytes per step with NEON, which every ARMv8 CPU has.
#endif
};


} // namespace Poco


#endif // Foundation_Adler32_INCLUDED
