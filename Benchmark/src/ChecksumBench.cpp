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
#include "CRC.h"
#include <cstdint>
#include <span>
#include <string>


using Poco::Checksum;
using Poco::CRC;


namespace {


constexpr std::size_t DATA_SIZE = 64 * 1024 * 1024;


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
{
	state.SetBytesProcessed(static_cast<std::int64_t>(state.iterations()) * static_cast<std::int64_t>(DATA_SIZE));
}


void checksumThroughput(benchmark::State& state, Checksum::Type type)
	// Checksum with the implementation it selects
{
	const std::string& data = benchmarkData();
	for (auto _ : state)
	{
		Checksum checksum(type);
		checksum.update(data);
		Poco::UInt64 value = (type == Checksum::TYPE_CRC64_XZ || type == Checksum::TYPE_CRC64_ECMA182) ?
			checksum.checksum64() : checksum.checksum();
		benchmark::DoNotOptimize(value);
	}
	setThroughput(state);
}


void implementationThroughput(benchmark::State& state, CRC::Update update)
	// one CRC implementation, through the private interface of Foundation
{
	const std::string& data = benchmarkData();
	const std::span<const unsigned char> bytes(reinterpret_cast<const unsigned char*>(data.data()), data.size());
	for (auto _ : state)
	{
		Poco::UInt64 value = update(0, bytes);
		benchmark::DoNotOptimize(value);
	}
	setThroughput(state);
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
		benchmark::RegisterBenchmark(std::string("Checksum_") + t.name, [type](benchmark::State& state) { checksumThroughput(state, type); })
			->Unit(benchmark::kMillisecond);
		if (type == Checksum::TYPE_ADLER32) continue;
		for (const auto& impl: CRC::implementations(type))
		{
			const CRC::Update update = impl.update;
			benchmark::RegisterBenchmark(std::string("Checksum_") + t.name + "/" + impl.name, [update](benchmark::State& state) { implementationThroughput(state, update); })
				->Unit(benchmark::kMillisecond);
		}
	}
	return true;
}


[[maybe_unused]] const bool CHECKSUM_BENCHMARKS_REGISTERED = registerChecksumBenchmarks();


} // namespace
