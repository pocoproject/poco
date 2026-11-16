//
// CpuFeatures.h
//
// Library: Foundation
// Package: Core
// Module:  CpuFeatures
//
// Definition of the CpuFeatures struct and the cpuFeatures() function.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_CpuFeatures_INCLUDED
#define Foundation_CpuFeatures_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


struct CpuFeatures
	/// The instruction set extensions of the CPU that the program runs on,
	/// as far as the Foundation library uses them.
	///
	/// This struct is internal to the Foundation library.
{
	bool pclmulqdq = false;  /// x86-64: carry-less multiplication (PCLMULQDQ)
	bool sse41 = false;      /// x86-64: SSE4.1
	bool ssse3 = false;      /// x86-64: SSSE3
	bool avx2 = false;       /// x86-64: AVX2, with the YMM registers enabled by the operating system
	bool avx512 = false;     /// x86-64: AVX-512F and AVX-512VL, with the opmask and ZMM registers enabled by the operating system
	bool avx512bw = false;   /// x86-64: AVX-512BW, with the opmask and ZMM registers enabled by the operating system
	bool vpclmulqdq = false; /// x86-64: carry-less multiplication of 256-bit and 512-bit registers (VPCLMULQDQ)
	bool armCrc32 = false;   /// ARMv8: CRC32 instructions
	bool armPmull = false;   /// ARMv8: polynomial multiplication of 64-bit values (PMULL)
};


[[nodiscard]] const CpuFeatures& cpuFeatures();
	/// Returns the features of the CPU, detected on the first call.
	///
	/// SSSE3, SSE4.1 and the ARMv8 CRC32 instructions are set without asking the CPU
	/// when the compiler's feature macros show that the build requires them: the
	/// compiler and the bundled zlib then use them without a check, so asking the
	/// CPU adds nothing. PCLMULQDQ, PMULL, VPCLMULQDQ, AVX2 and AVX-512 are always
	/// asked for, AVX2 and AVX-512 together with the operating system's support for
	/// their registers. The CPU is asked on x86-64 with GCC, Clang and MSVC, and on
	/// ARMv8 under Linux, Android, Apple platforms and Windows; elsewhere only what
	/// the compiler's feature macros show is set.


} // namespace Poco


#endif // Foundation_CpuFeatures_INCLUDED
