//
// CpuFeatures_X86.cpp
//
// Library: Foundation
// Package: Core
// Module:  CpuFeatures
//
// Detection of the x86-64 features.
//
// References:
// - Intel 64 and IA-32 Architectures Software Developer's Manual, Volume 2A,
//   CPUID: leaf 1 and leaf 7 subleaf 0 feature flags; Volume 1, chapter 13
//   "Managing State Using the XSAVE Feature Set": XCR0 and XGETBV.
// - AMD64 Architecture Programmer's Manual, Volume 3, appendix E "Obtaining
//   Processor Information Via the CPUID Instruction".
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CpuFeatures.h"
#if defined(POCO_COMPILER_MSVC)
	#include <intrin.h>
	#include <immintrin.h>
#elif defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
	#include <cpuid.h>
	#include <immintrin.h>
#endif


namespace Poco {


namespace {


struct CpuidRegisters
{
	unsigned eax = 0;
	unsigned ebx = 0;
	unsigned ecx = 0;
	unsigned edx = 0;
};


[[nodiscard]] CpuidRegisters cpuid([[maybe_unused]] unsigned leaf)
	// CPUID with subleaf 0; the registers are zero when the CPU does not have the leaf
{
	CpuidRegisters registers;
#if defined(POCO_COMPILER_MSVC)
	int values[4] = {};
	__cpuid(values, 0);
	if (static_cast<unsigned>(values[0]) < leaf) return registers;
	__cpuidex(values, static_cast<int>(leaf), 0);
	registers = {static_cast<unsigned>(values[0]), static_cast<unsigned>(values[1]), static_cast<unsigned>(values[2]), static_cast<unsigned>(values[3])};
#elif defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
	::__get_cpuid_count(leaf, 0, &registers.eax, &registers.ebx, &registers.ecx, &registers.edx);
#else
	// other compilers have no CPUID access
#endif
	return registers;
}


[[nodiscard]]
#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
__attribute__((target("xsave")))
#endif
UInt64 enabledRegisterState()
	// XCR0: the register state that the operating system saves on a context
	// switch; XGETBV exists only where CPUID leaf 1 reports OSXSAVE
{
#if defined(POCO_COMPILER_MSVC) || defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)
	return _xgetbv(0);
#else
	return 0;
#endif
}


[[nodiscard]] CpuFeatures detectCpuFeatures()
{
	CpuFeatures features;
	// CPUID leaf 1, ECX
	constexpr unsigned ECX_PCLMULQDQ = 1u << 1;
	[[maybe_unused]] constexpr unsigned ECX_SSSE3 = 1u << 9;
	[[maybe_unused]] constexpr unsigned ECX_SSE41 = 1u << 19;
	constexpr unsigned ECX_OSXSAVE = 1u << 27;
	// CPUID leaf 7, subleaf 0, EBX and ECX
	constexpr unsigned EBX_AVX2 = 1u << 5;
	constexpr unsigned EBX_AVX512F = 1u << 16;
	constexpr unsigned EBX_AVX512BW = 1u << 30;
	constexpr unsigned EBX_AVX512VL = 1u << 31;
	constexpr unsigned ECX_VPCLMULQDQ = 1u << 10;
	// XCR0: SSE (bit 1) and upper halves of the YMM registers (bit 2); opmask
	// registers (bit 5), upper halves of ZMM0-15 (bit 6) and ZMM16-31 (bit 7)
	constexpr UInt64 XCR0_YMM = 0x06;
	constexpr UInt64 XCR0_ZMM = 0xE6;

	const CpuidRegisters leaf1 = cpuid(1);
	// the CPU is always asked for PCLMULQDQ: nothing in the build uses it without a
	// check, so a build that enables it does not show that the CPU has it
	features.pclmulqdq = (leaf1.ecx & ECX_PCLMULQDQ) != 0;
	// with __SSSE3__ and __SSE4_1__ the build requires SSSE3 and SSE4.1: the compiler
	// and the bundled zlib use them without a check, so asking the CPU adds nothing
#if defined(__SSSE3__)
	features.ssse3 = true;
#else
	features.ssse3 = (leaf1.ecx & ECX_SSSE3) != 0;
#endif
#if defined(__SSE4_1__)
	features.sse41 = true;
#else
	features.sse41 = (leaf1.ecx & ECX_SSE41) != 0;
#endif

	// AVX2 and AVX-512 need the operating system to save their registers
	if ((leaf1.ecx & ECX_OSXSAVE) == 0) return features;
	const CpuidRegisters leaf7 = cpuid(7);
	const UInt64 state = enabledRegisterState();
	const bool ymmState = (state & XCR0_YMM) == XCR0_YMM;
	const bool zmmState = (state & XCR0_ZMM) == XCR0_ZMM;
	features.avx2 = ymmState && (leaf7.ebx & EBX_AVX2) != 0;
	features.avx512 = zmmState && (leaf7.ebx & EBX_AVX512F) != 0 && (leaf7.ebx & EBX_AVX512VL) != 0;
	features.avx512bw = zmmState && (leaf7.ebx & EBX_AVX512BW) != 0;
	features.vpclmulqdq = (leaf7.ecx & ECX_VPCLMULQDQ) != 0;
	return features;
}


} // namespace


} // namespace Poco
