//
// ThreadPoolCollector.cpp
//
// Library: Prometheus
// Package: Collectors
// Module:  ThreadPoolCollector
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/ThreadPoolCollector.h"
#include "Poco/Ascii.h"


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


const std::string ThreadPoolCollector::NAME_PREFIX{"poco_threadpool"s};


ThreadPoolCollector::ThreadPoolCollector():
	Collector(NAME_PREFIX),
	_threadPoolName("default"s),
	_threadPool(Poco::ThreadPool::defaultPool())
{
	buildMetrics();
}


ThreadPoolCollector::ThreadPoolCollector(const Poco::ThreadPool& threadPool):
	Collector(collectorName(threadPool.name())),
	_threadPoolName(threadPool.name()),
	_threadPool(threadPool)
{
	buildMetrics();
}


ThreadPoolCollector::ThreadPoolCollector(const std::string& name, const Poco::ThreadPool& threadPool, Registry* pRegistry):
	Collector(collectorName(name), pRegistry),
	_threadPoolName(name),
	_threadPool(threadPool)
{
	buildMetrics();
}


void ThreadPoolCollector::exportTo(Exporter& exporter) const
{
	const std::vector<std::string> labelNames{"name"s};
	const std::vector<std::string> labelValues{_threadPoolName};

	for (const auto& p: _metrics)
	{
		exporter.writeHeader(*p);
		exporter.writeSample(*p, labelNames, labelValues, p->value(), 0);
	}
}


void ThreadPoolCollector::buildMetrics()
{
	const Poco::ThreadPool& threadPool = _threadPool;

	_metrics.push_back(std::make_unique<CallbackIntGauge>(
		NAME_PREFIX + "_max_threads"s,
		"Maximum number of threads available in the thread pool (capacity)"s,
		nullptr,
		[&threadPool]()
		{
			return threadPool.capacity();
		}));

	_metrics.push_back(std::make_unique<CallbackIntGauge>(
		NAME_PREFIX + "_used_threads"s,
		"Number of currently used threads"s,
		nullptr,
		[&threadPool]()
		{
			return threadPool.used();
		}));

	_metrics.push_back(std::make_unique<CallbackIntGauge>(
		NAME_PREFIX + "_allocated_threads"s,
		"Number of currently allocated threads"s,
		nullptr,
		[&threadPool]()
		{
			return threadPool.allocated();
		}));
}


std::string ThreadPoolCollector::collectorName(const std::string& threadPoolName)
{
	std::string result(NAME_PREFIX);
	if (!threadPoolName.empty())
	{
		result += '_';
		for (const char c: threadPoolName)
		{
			if (Poco::Ascii::isAlphaNumeric(c) || c == '_')
				result += c;
			else
				result += '_';
		}
	}
	return result;
}


} } // namespace Poco::Prometheus
