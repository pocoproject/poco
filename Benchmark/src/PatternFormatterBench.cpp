//
// PatternFormatterBench.cpp
//
// Benchmarks for PatternFormatter
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <benchmark/benchmark.h>
#include "Poco/PatternFormatter.h"
#include "Poco/Message.h"


using Poco::PatternFormatter;
using Poco::Message;


namespace {


//
// Simple pattern: just message text
//

static void BM_PatternFormatter_TextOnly(benchmark::State& state)
{
	PatternFormatter formatter("%t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_TextOnly);


//
// Source and text
//

static void BM_PatternFormatter_SourceText(benchmark::State& state)
{
	PatternFormatter formatter("%s: %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_SourceText);


//
// Date and time (common logging pattern)
//

static void BM_PatternFormatter_DateTime(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_DateTime);


//
// Date, time with milliseconds
//

static void BM_PatternFormatter_DateTimeMillis(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S.%i");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_DateTimeMillis);


//
// Typical production logging pattern
//

static void BM_PatternFormatter_Typical(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_Typical);


//
// Full pattern with thread info
//

static void BM_PatternFormatter_Full(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S.%i [%p] [%T:%I] %s: %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_Full);


//
// With source file and line (debug pattern)
//

static void BM_PatternFormatter_Debug(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S.%i [%p] %s(%U:%u): %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION,
		__FILE__, __LINE__);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_Debug);


//
// With source file basename (using %O)
//

static void BM_PatternFormatter_Basename(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S.%i [%p] %s(%O:%u): %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION,
		"/very/long/path/to/source/file/TestFile.cpp", 42);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_Basename);


//
// With node name
//

static void BM_PatternFormatter_NodeName(benchmark::State& state)
{
	PatternFormatter formatter("%Y-%m-%d %H:%M:%S.%i [%N] [%p] %s: %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_NodeName);


//
// Local time conversion
//

static void BM_PatternFormatter_LocalTime(benchmark::State& state)
{
	PatternFormatter formatter("%L%Y-%m-%d %H:%M:%S.%i [%p] %s: %t");
	Message msg("TestSource", "This is a test log message", Message::PRIO_INFORMATION);
	std::string result;

	for (auto _ : state)
	{
		result.clear();
		formatter.format(msg, result);
		benchmark::DoNotOptimize(result.data());
	}
	state.SetBytesProcessed(state.iterations() * result.size());
}
BENCHMARK(BM_PatternFormatter_LocalTime);


} // namespace
