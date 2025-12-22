//
// ThreadPoolCollector.h
//
// Library: Prometheus
// Package: Collectors
// Module:  ThreadPoolCollector
//
// Definition of the ThreadPoolCollector class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_ThreadPoolCollector_INCLUDED
#define Prometheus_ThreadPoolCollector_INCLUDED


#include "Poco/Prometheus/Collector.h"
#include "Poco/Prometheus/CallbackMetric.h"
#include "Poco/ThreadPool.h"
#include <memory>
#include <vector>


namespace Poco {
namespace Prometheus {


class Prometheus_API ThreadPoolCollector: public Collector
	/// This Collector provides Poco::ThreadPool specific metrics:
	///   - poco_threadpool_max_threads: Maximum number of threads available in the thread pool (capacity).
	///   - poco_threadpool_used_threads: Number of currently used threads.
	///   - poco_threadpool_allocated_threads: Number of currently allocated threads.
	///
	/// Metrics have a single label "name" identifying the thread pool.
	/// The name of the default thread pool is "default".
{
public:
	ThreadPoolCollector();
		/// Creates a default ThreadPoolCollector for the default Poco::ThreadPool.

	explicit ThreadPoolCollector(const Poco::ThreadPool& threadPool);
		/// Creates a default ThreadPoolCollector for the given Poco::ThreadPool.
		/// The Poco::ThreadPool must have a non-empty name and the name must
		/// be unique across all ThreadPoolCollector instances.

	ThreadPoolCollector(const std::string& name, const Poco::ThreadPool& threadPool, Registry* pRegistry);
		/// Creates a custom ThreadPoolCollector with the given name
		/// and registers it with the given Registry.

	~ThreadPoolCollector() = default;
		/// Destroys the ThreadPoolCollector.

	// Collector
	void exportTo(Exporter& exporter) const override;
		/// Writes the given Metric to the Exporter.

protected:
	void buildMetrics();

	static std::string collectorName(const std::string& threadPoolName);

	static const std::string NAME_PREFIX;

private:
	using MetricPtr = std::unique_ptr<CallbackIntGauge>;
	const std::string _threadPoolName;
	const Poco::ThreadPool& _threadPool;
	std::vector<MetricPtr> _metrics;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_ThreadPoolCollector_INCLUDED
