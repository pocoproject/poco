//
// Benchmark.h
//
// Library: Benchmark
// Package: Benchmark
// Module:  Benchmark
//
// Common utilities and macros for Poco benchmarks.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Poco_Benchmark_Benchmark_INCLUDED
#define Poco_Benchmark_Benchmark_INCLUDED


#include <benchmark/benchmark.h>


//
// Convenience macros for common benchmark patterns
//


/// Prevent compiler from optimizing away a value
#define POCO_BENCHMARK_DO_NOT_OPTIMIZE(value) \
	benchmark::DoNotOptimize(value)


/// Mark a memory location as clobbered (forces memory barrier)
#define POCO_BENCHMARK_CLOBBER_MEMORY() \
	benchmark::ClobberMemory()


#endif // Poco_Benchmark_Benchmark_INCLUDED
