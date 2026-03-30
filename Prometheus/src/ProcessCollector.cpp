//
// ProcessCollector.cpp
//
// Library: Prometheus
// Package: Collectors
// Module:  ProcessCollector
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/ProcessCollector.h"
#include "Poco/Process.h"
#ifdef POCO_OS_FAMILY_UNIX
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#endif
#if POCO_OS == POCO_OS_LINUX
#include <dirent.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#endif


using namespace std::string_literals;


namespace Poco::Prometheus {


Poco::Timestamp ProcessCollector::_startTime;


ProcessCollector::ProcessCollector():
	Collector("process"s)
{
	buildMetrics();
}


ProcessCollector::ProcessCollector(const std::string& name, Registry* pRegistry):
	Collector(name, pRegistry)
{
	buildMetrics();
}


void ProcessCollector::exportTo(Exporter& exporter) const
{
	for (const auto& p: _metrics)
	{
		p->exportTo(exporter);
	}
#if POCO_OS == POCO_OS_LINUX
	exportThreadCPU(exporter);
#endif
}


#if POCO_OS == POCO_OS_LINUX
namespace {

double readProcStatusKb(const char* field)
{
	std::ifstream in("/proc/self/status");
	if (!in) return 0.0;
	std::string line;
	while (std::getline(in, line))
	{
		if (line.compare(0, std::strlen(field), field) == 0)
		{
			// Line format: "VmRSS:    12345 kB"
			std::size_t fieldLen = std::strlen(field);
			if (line.size() <= fieldLen) return 0.0;
			const char* start = line.c_str() + fieldLen;
			char* end = nullptr;
			double val = std::strtod(start, &end);
			return (end != start) ? val : 0.0;
		}
	}
	return 0.0;
}

long clockTicksPerSecond()
{
	static long ticks = sysconf(_SC_CLK_TCK);
	return ticks;
}

} // anonymous namespace
#endif


void ProcessCollector::buildMetrics()
{
	_metrics.push_back(std::make_unique<CallbackGauge>(
		name() + "_cpu_seconds_total"s,
		"Total user and system CPU time spent in seconds"s,
		nullptr,
		[]()
		{
			Poco::Int64 user;
			Poco::Int64 system;
			Poco::Process::timesMicroseconds(user, system);
			return static_cast<double>(user/1000 + system/1000)/1000.0;
		}));

#ifdef POCO_OS_FAMILY_UNIX
	_metrics.push_back(std::make_unique<CallbackIntGauge>(
		name() + "_max_fds"s,
		"Maximum number of open file descriptors"s,
		nullptr,
		[]()
		{
			return sysconf(_SC_OPEN_MAX);
		}));
#endif

#if POCO_OS == POCO_OS_LINUX
	_metrics.push_back(std::make_unique<CallbackGauge>(
		name() + "_resident_memory_bytes"s,
		"Resident memory size in bytes"s,
		nullptr,
		[]()
		{
			return readProcStatusKb("VmRSS:") * 1024.0;
		}));

	_metrics.push_back(std::make_unique<CallbackGauge>(
		name() + "_virtual_memory_bytes"s,
		"Virtual memory size in bytes"s,
		nullptr,
		[]()
		{
			return readProcStatusKb("VmSize:") * 1024.0;
		}));

	_pThreadCPU = std::make_unique<Gauge>(
		name() + "_thread_cpu_seconds_total"s,
		Gauge::Params{
			"Per-thread total CPU time in seconds (user + system)"s,
			{"tid"s, "name"s}
		},
		nullptr);
#endif

	_metrics.push_back(std::make_unique<CallbackGauge>(
		name() + "_start_time_seconds"s,
		"Start time of the process since unix epoch in seconds"s,
		nullptr,
		[]()
		{
			return static_cast<double>(ProcessCollector::startTime().epochMicroseconds()/1000)/1000.0;
		}));

	_metrics.push_back(std::make_unique<CallbackGauge>(
		name() + "_up_time_seconds"s,
		"Time in seconds the process has been up and running"s,
		nullptr,
		[]()
		{
			return static_cast<double>(ProcessCollector::startTime().elapsed()/1000)/1000.0;
		}));
}


#if POCO_OS == POCO_OS_LINUX
void ProcessCollector::exportThreadCPU(Exporter& exporter) const
{
	if (!_pThreadCPU) return;

	const std::vector<std::string> labelNames{"tid"s, "name"s};
	const double ticksPerSec = static_cast<double>(clockTicksPerSecond());
	if (ticksPerSec <= 0.0) return;
	bool headerWritten = false;

	struct DirGuard
	{
		DIR* d;
		explicit DirGuard(DIR* p): d(p) {}
		~DirGuard() { if (d) closedir(d); }
	};

	DIR* dir = opendir("/proc/self/task");
	if (!dir) return;
	DirGuard guard(dir);

	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr)
	{
		if (entry->d_name[0] < '0' || entry->d_name[0] > '9')
			continue;

		std::string tid(entry->d_name);
		std::string path = "/proc/self/task/"s + tid + "/stat"s;
		std::ifstream in(path);
		if (!in) continue;

		std::string line;
		if (!std::getline(in, line)) continue;

		// Parse comm: find first '(' and last ')' to handle spaces/parens in name
		auto openParen = line.find('(');
		auto closeParen = line.rfind(')');
		if (openParen == std::string::npos || closeParen == std::string::npos)
			continue;

		std::string comm = line.substr(openParen + 1, closeParen - openParen - 1);

		// Fields after ')': state(3) ppid(4) ... utime(14) stime(15)
		// Skip state field (single char), then parse numeric fields.
		// utime is the 11th numeric token after state, stime is the 12th.
		const char* rest = line.c_str() + closeParen + 2;
		while (*rest == ' ') ++rest;
		if (*rest) ++rest; // skip state character
		long long utime = 0, stime = 0;
		char* end = nullptr;
		for (int i = 1; i <= 12; ++i)
		{
			while (*rest == ' ') ++rest;
			if (!*rest) break;
			long long val = std::strtoll(rest, &end, 10);
			if (end == rest) break;
			if (i == 11) utime = val;
			if (i == 12) stime = val;
			rest = end;
		}

		double seconds = static_cast<double>(utime + stime) / ticksPerSec;

		if (!headerWritten)
		{
			exporter.writeHeader(*_pThreadCPU);
			headerWritten = true;
		}
		exporter.writeSample(*_pThreadCPU, labelNames, {tid, comm}, seconds, 0);
	}
}
#endif


} // namespace Poco::Prometheus
