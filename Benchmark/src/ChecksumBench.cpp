//
// ChecksumBench.cpp
//
// Benchmarks for Checksum
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <benchmark/benchmark.h>
#include "Poco/Checksum.h"
#include "Adler32.h"
#include "CRC.h"
#include <cstdint>
#include <iterator>
#include <span>
#include <string>
#include <type_traits>


using Poco::Adler32;
using Poco::Checksum;
using Poco::CRC;


namespace {


constexpr std::size_t DATA_SIZE = 64 * 1024 * 1024;
// a large buffer, one that fits into the first-level cache, and a short message
constexpr std::int64_t BUFFER_SIZES[] = {DATA_SIZE, 4 * 1024, 64};


[[nodiscard]] const std::string& benchmarkData()
{
	static const std::string data = []
	{
		std::string result(DATA_SIZE, '\0');
		for (std::size_t i = 0; i < result.size(); ++i)
			result[i] = static_cast<char>((i * 2654435761u) >> 24);
		return result;
	}();
	return data;
}


void setThroughput(benchmark::State& state)
	// the buffer size is the argument of the benchmark
{
	state.SetBytesProcessed(static_cast<std::int64_t>(state.iterations()) * state.range(0));
}


void checksumThroughput(benchmark::State& state, Checksum::Type type)
	// Checksum with the implementation it selects
{
	const std::string& data = benchmarkData();
	const auto size = static_cast<std::size_t>(state.range(0));
	for (auto _ : state)
	{
		Checksum checksum(type);
		checksum.update(data.data(), size);
		Poco::UInt64 value = (type == Checksum::TYPE_CRC64_XZ || type == Checksum::TYPE_CRC64_ECMA182) ?
			checksum.checksum64() : checksum.checksum();
		benchmark::DoNotOptimize(value);
	}
	setThroughput(state);
}


template <typename Update>
void implementationThroughput(benchmark::State& state, Update update)
	// one CRC or Adler-32 implementation, through the private interface of Foundation
{
	static_assert(std::is_same_v<Update, CRC::Update> || std::is_same_v<Update, Adler32::Update>,
		"update must be a CRC::Update or an Adler32::Update");
	const std::string& data = benchmarkData();
	const std::span<const unsigned char> bytes(reinterpret_cast<const unsigned char*>(data.data()), static_cast<std::size_t>(state.range(0)));
	for (auto _ : state)
	{
		Poco::UInt64 value = update(0, bytes);
		benchmark::DoNotOptimize(value);
	}
	setThroughput(state);
}


template <typename Implementation>
void registerImplementation(const std::string& name, const Implementation& impl)
{
	static_assert(std::is_same_v<Implementation, CRC::Implementation> || std::is_same_v<Implementation, Adler32::Implementation>,
		"impl must be a CRC::Implementation or an Adler32::Implementation");
	const auto update = impl.update;
	benchmark::RegisterBenchmark(name + "/" + impl.name, [update](benchmark::State& state) { implementationThroughput(state, update); })
		->ArgsProduct({{std::begin(BUFFER_SIZES), std::end(BUFFER_SIZES)}})
		->Unit(benchmark::kMicrosecond);
}


bool registerChecksumBenchmarks()
{
	constexpr struct
	{
		const char* name;
		Checksum::Type type;
	} types[] = {
		{"CRC32", Checksum::TYPE_CRC32},
		{"Adler32", Checksum::TYPE_ADLER32},
		{"CRC64_XZ", Checksum::TYPE_CRC64_XZ},
		{"CRC64_ECMA182", Checksum::TYPE_CRC64_ECMA182}
	};
	for (const auto& t: types)
	{
		const Checksum::Type type = t.type;
		const std::string name = std::string("Checksum_") + t.name;
		benchmark::RegisterBenchmark(name, [type](benchmark::State& state) { checksumThroughput(state, type); })
			->ArgsProduct({{std::begin(BUFFER_SIZES), std::end(BUFFER_SIZES)}})
			->Unit(benchmark::kMicrosecond);
		if (type == Checksum::TYPE_ADLER32)
		{
			for (const auto& impl: Adler32::implementations())
				registerImplementation(name, impl);
		}
		else
		{
			for (const auto& impl: CRC::implementations(type))
				registerImplementation(name, impl);
		}
	}
	return true;
}


[[maybe_unused]] const bool CHECKSUM_BENCHMARKS_REGISTERED = registerChecksumBenchmarks();


} // namespace
