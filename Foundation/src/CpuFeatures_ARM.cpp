//
// CpuFeatures_ARM.cpp
//
// Library: Foundation
// Package: Core
// Module:  CpuFeatures
//
// Detection of the ARMv8 features.
//
// References:
// - Linux getauxval(3) and the AArch64 HWCAP flags.
// - Apple, "Determining Instruction Set Characteristics" (sysctl hw.optional).
// - Microsoft, IsProcessorFeaturePresent function.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CpuFeatures.h"
#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
	#include <sys/auxv.h>
	#ifndef HWCAP_CRC32
		#include <asm/hwcap.h>
	#endif
#elif POCO_OS == POCO_OS_MAC_OS_X
	#include <sys/sysctl.h>
	#include <cstring>
#elif defined(POCO_OS_FAMILY_WINDOWS)
	#include "Poco/UnWindows.h"
#endif


namespace Poco {


namespace {


// How the operating system reports a feature of the CPU. The CRC32 query is
// unused when the build enables the CRC32 instructions.
#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID


// the kernel reports the CPU features in the auxiliary vector
using Feature = unsigned long;
[[maybe_unused]] constexpr Feature CRC32_FEATURE = HWCAP_CRC32;
constexpr Feature PMULL_FEATURE = HWCAP_PMULL;


[[nodiscard]] bool cpuHas(Feature feature)
{
	return (::getauxval(AT_HWCAP) & feature) != 0;
}


#elif POCO_OS == POCO_OS_MAC_OS_X


// the sysctl values hw.optional.* are 1 for a feature the CPU has
using Feature = const char*;
[[maybe_unused]] constexpr Feature CRC32_FEATURE = "hw.optional.armv8_crc32";
constexpr Feature PMULL_FEATURE = "hw.optional.arm.FEAT_PMULL";


[[nodiscard]] bool cpuHas(Feature feature)
{
	int value = 0;
	std::size_t size = sizeof(value);
	if (::sysctlbyname(feature, &value, &size, nullptr, 0) != 0)
	{
		// hw.optional.arm.FEAT_PMULL exists since macOS 12 and iOS 15; every Apple arm64 CPU has PMULL
		return std::strcmp(feature, PMULL_FEATURE) == 0;
	}
	return value != 0;
}


#elif defined(POCO_OS_FAMILY_WINDOWS)


// PMULL is part of the crypto extension that Windows reports as one feature
using Feature = DWORD;
[[maybe_unused]] constexpr Feature CRC32_FEATURE = PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE;
constexpr Feature PMULL_FEATURE = PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE;


[[nodiscard]] bool cpuHas(Feature feature)
{
	return ::IsProcessorFeaturePresent(feature) != 0;
}


#else


// other operating systems are not asked
using Feature = int;
[[maybe_unused]] constexpr Feature CRC32_FEATURE = 0;
constexpr Feature PMULL_FEATURE = 0;


[[nodiscard]] bool cpuHas(Feature)
{
	return false;
}


#endif


[[nodiscard]] CpuFeatures detectCpuFeatures()
{
	CpuFeatures features;
	// with __ARM_FEATURE_CRC32 the build requires the CRC32 instructions: the
	// compiler and the bundled zlib use them without a check, so asking the CPU
	// adds nothing
#if defined(__ARM_FEATURE_CRC32)
	features.armCrc32 = true;
#else
	features.armCrc32 = cpuHas(CRC32_FEATURE);
#endif
	// the CPU is always asked for PMULL: nothing in the build uses it without a
	// check, so a build that enables it does not show that the CPU has it
	features.armPmull = cpuHas(PMULL_FEATURE);
	return features;
}


} // namespace


} // namespace Poco
