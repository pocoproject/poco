//
// ProcessCollector.h
//
// Library: Prometheus
// Package: Collectors
// Module:  ProcessCollector
//
// Definition of the ProcessCollector class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_ProcessCollector_INCLUDED
#define Prometheus_ProcessCollector_INCLUDED


#include "Poco/Prometheus/Collector.h"
#include "Poco/Prometheus/CallbackMetric.h"
#include "Poco/Timestamp.h"
#include <memory>
#include <vector>


namespace Poco {
namespace Prometheus {


class Prometheus_API ProcessCollector: public Collector
	/// This Collector provides process-specific metrics:
	///   - process_cpu_seconds_total: Total user and system CPU time spent in seconds.
	///   - process_max_fds: Maximum number of open file descriptors.
	///   - process_start_time_seconds: Start time of the process since unix epoch in seconds
	///     (actually, the time the Prometheus library was loaded).
	///   - process_up_time_seconds: Up time of the process in seconds
	///     (actually, time since the Prometheus library was loaded).
{
public:
	ProcessCollector();
		/// Creates a default ProcessCollector.

	ProcessCollector(const std::string& name, Registry* pRegistry);
		/// Creates a custom ProcessCollector with the given name (prefix)
		/// and registers it with the given Registry.

	~ProcessCollector() = default;
		/// Destroys the ProcessCollector.

	static Poco::Timestamp startTime();
		/// Returns the process start time.

	// Collector
	void exportTo(Exporter& exporter) const override;
		/// Writes the given Metric to the Exporter.

protected:
	void buildMetrics();

private:
	using MetricPtr = std::unique_ptr<Metric>;
	std::vector<MetricPtr> _metrics;
	static Poco::Timestamp _startTime;
};


//
// inlines
//


inline Poco::Timestamp ProcessCollector::startTime()
{
	return _startTime;
}


} } // namespace Poco::Prometheus


#endif // Prometheus_ProcessCollector_INCLUDED
