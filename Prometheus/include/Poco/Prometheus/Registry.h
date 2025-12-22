//
// Registry.h
//
// Library: Prometheus
// Package: Core
// Module:  Registry
//
// Definition of the Registry class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Registry_INCLUDED
#define Prometheus_Registry_INCLUDED


#include "Poco/Prometheus/Prometheus.h"
#include "Poco/Mutex.h"
#include <map>


namespace Poco {
namespace Prometheus {


class Collector;
class Exporter;


class Prometheus_API Registry
	/// The metric registry is a collection of all active
	/// Collector instances.
	///
	/// The Registry internally stores plain pointers to Collector
	/// objects. Collector objects automatically register themselves
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
	Registry() = default;
		/// Creates the Registry.

	~Registry() = default;
		/// Destroys the Registry.

	void registerCollector(Collector* pCollector);
		/// Registers a Collector with the Registry.
		///
		/// An attempt to register a collector with the name
		/// of an already registered collector will fail with
		/// a Poco::ExistsException.

	void unregisterCollector(Collector* pCollector);
		/// Removes a Collector from the Registry.

	void unregisterCollector(const std::string& collectorName);
		/// Removes the Collector with the given name from the Registry.
		///
		/// Does nothing if no collector with the given name has been registered.

	Collector* findCollector(const std::string& collectorName) const;
		/// Looks up the Collector with the given name.
		///
		/// Returns a pointer to the collector instance if found,
		/// otherwise returns a nullptr.

	void clear();
		/// Removes all Collector instances from the Registry.

	void exportTo(Exporter& exporter) const;
		/// Exports all registered collector's metrics through the given Exporter.

	static Registry& defaultRegistry();
		/// Returns the default Registry.

private:
	std::map<std::string, Collector*> _collectors;
	mutable Poco::FastMutex _mutex;

	Registry(const Registry&) = delete;
	Registry(Registry&&) = delete;
	Registry& operator = (const Registry&) = delete;
	Registry& operator = (Registry&&) = delete;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_Metric_INCLUDED
