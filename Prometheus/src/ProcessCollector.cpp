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


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


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
}


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


} } // namespace Poco::Prometheus
