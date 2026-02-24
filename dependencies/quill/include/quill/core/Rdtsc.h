/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include <cstdint>

#include "quill/core/Attributes.h"
#include <cstdint>

#if defined(__ARM_ARCH) || defined(_M_ARM) || defined(_M_ARM64) || defined(__PPC64__)
  // ARM or PowerPC — use ChronoTimeUtils for timestamping
  #include "quill/core/ChronoTimeUtils.h"
  #include <chrono>

#elif defined(__riscv) || defined(__s390x__) || defined(__loongarch64)
  // RISC-V, IBM Z (s390x), or LoongArch — no special intrinsics required

#else
  // Assume x86 or x86-64 platform
  #if __has_include(<intrin.h>) && defined(_WIN32)
    // Use Windows-specific intrinsics
    #include <intrin.h>
  #elif __has_include(<x86gprintrin.h>) && !defined(__INTEL_COMPILER)
    // Use x86 general-purpose intrinsics if available and not using Intel compiler
    #include <x86gprintrin.h>
  #else
    // Fallback to standard x86 intrinsics
    #include <x86intrin.h>
  #endif

#endif

QUILL_BEGIN_NAMESPACE

namespace detail
{
#if defined(__aarch64__)
// arm64
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept
{
  // System timer of ARMv8 runs at a different frequency than the CPU's.
  // The frequency is fixed, typically in the range 1-50MHz.  It can be
  // read at CNTFRQ special register.  We assume the OS has set up the virtual timer properly.
  int64_t virtual_timer_value;
  __asm__ volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
  return static_cast<uint64_t>(virtual_timer_value);
}
#elif (defined(__ARM_ARCH) && !defined(_MSC_VER))
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept
{
  #if (__ARM_ARCH >= 6)
  // V6 is the earliest arch that has a standard cyclecount
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;

  __asm__ volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1)
  {
    __asm__ volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul)
    {
      __asm__ volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      return (static_cast<uint64_t>(pmccntr)) * 64u;
    }
  }
  #endif

  // soft failover
  return detail::get_timestamp_ns<std::chrono::steady_clock>();
}
#elif defined(__riscv)
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept
{
  uint64_t tsc;
  __asm__ volatile("rdtime %0" : "=r"(tsc));
  return tsc;
}
#elif defined(__loongarch64)
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept
{
  uint64_t tsc;
  __asm__ volatile("rdtime.d %0,$r0" : "=r"(tsc));
  return tsc;
}
#elif defined(__s390x__)
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept
{
  uint64_t tsc;
  __asm__ volatile("stck %0" : "=Q"(tsc) : : "cc");
  return tsc;
}
#elif (defined(_M_ARM) || defined(_M_ARM64) || defined(__PPC64__))
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept
{
  // soft failover
  return detail::get_timestamp_ns<std::chrono::steady_clock>();
}
#else
/**
 * Get the TSC counter
 * @return rdtsc timestamp
 */
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline uint64_t rdtsc() noexcept { return __rdtsc(); }
#endif

} // namespace detail

QUILL_END_NAMESPACE