//
// Collector.h
//
// Library: Prometheus
// Package: Core
// Module:  Collector
//
// Definition of the Collector class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Collector_INCLUDED
#define Prometheus_Collector_INCLUDED


#include "Poco/Prometheus/Prometheus.h"
#include "Poco/Prometheus/Registry.h"


namespace Poco {
namespace Prometheus {


class Exporter;
class Registry;


class Prometheus_API Collector
	/// This is the base class for all metrics, such as
	/// Counter, Gauge and Histogram.
	///
	/// Collector objects automatically register themselves
	/// with the default Registry (unless a nullptr is passed to the
	/// constructor's pRegistry parameter), but do not automatically
	/// unregister. This brings the danger of dangling pointers if
	/// Collector instances are destroyed without being explicitly
	/// unregistered first. In most practical cases this is not an
	/// issue as Collector instances will be alive for the entire
	/// run time of the process. However, it can be an issue if
	/// Collector instances are created and destroyed dynamically
	/// during the run time of the process.
{
public:
	const std::string& name() const;
		/// Returns the metric's name.

	virtual void exportTo(Exporter& exporter) const = 0;
		/// Writes the given Metric to the Exporter.

	virtual ~Collector() = default;
		/// Destroys the Collector.

protected:
	Collector(const std::string& name);
		/// Creates a Collector with the given name
		/// and registers it with the default Registry.
		///
		/// The name must be a valid metric name and match the regular
		/// expression [a-zA-Z_:][a-zA-Z0-9_:]*.

	Collector(const std::string& name, Registry* pRegistry);
		/// Creates a Collector with the given name
		/// and registers it with the given Registry (if not nullptr).
		///
		/// The name must be a valid metric name and match the regular
		/// expression [a-zA-Z_:][a-zA-Z0-9_:]*.

	static const std::string& validateName(const std::string& name);

private:
	const std::string _name;

	Collector() = delete;
	Collector(const Collector&) = delete;
	Collector(Collector&&) = delete;
	Collector& operator = (const Collector&) = delete;
	Collector& operator = (Collector&&) = delete;
};


//
// inlines
//


inline Collector::Collector(const std::string& name):
	_name(validateName(name))
{
	Registry::defaultRegistry().registerCollector(this);
}


inline Collector::Collector(const std::string& name, Registry* pRegistry):
	_name(validateName(name))
{
	if (pRegistry) pRegistry->registerCollector(this);
}


inline const std::string& Collector::name() const
{
	return _name;
}


} } // namespace Poco::Prometheus


#endif // Prometheus_Collector_INCLUDED
