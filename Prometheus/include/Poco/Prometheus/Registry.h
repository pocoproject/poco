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


namespace Poco::Prometheus {


class Collector;
class Exporter;


class Prometheus_API Registry
	/// The metric registry is a collection of all active
	/// Collector instances.
	///
	/// The Registry internally stores plain pointers to Collector
	/// objects. Collector objects automatically register themselves
	/// with the default Registry (unless a nullptr is passed to the
	/// constructor's pRegistry parameter), and unregister themselves
	/// when they are destroyed. A Registry that is destroyed before
	/// its Collectors removes them first, so that they do not
	/// refer to it afterwards.
{
public:
	Registry();
		/// Creates the Registry.

	~Registry();
		/// Removes all Collector instances and destroys the Registry.
		///
		/// The Registry must not be destroyed while a Collector
		/// registered with it is destroyed or used in another thread.

	void registerCollector(Collector* pCollector);
		/// Registers a Collector with the Registry.
		///
		/// An attempt to register a collector with the name
		/// of an already registered collector, or a collector
		/// that is registered with another Registry, will fail
		/// with a Poco::ExistsException.

	void unregisterCollector(Collector* pCollector);
		/// Removes a Collector from the Registry.
		///
		/// Does nothing if the given collector is not registered
		/// with the Registry.

	void unregisterCollector(const std::string& collectorName);
		/// Removes the Collector with the given name from the Registry.
		///
		/// Does nothing if no collector with the given name has been registered.

	[[nodiscard]] Collector* findCollector(const std::string& collectorName) const;
		/// Looks up the Collector with the given name.
		///
		/// Returns a pointer to the collector instance if found,
		/// otherwise returns a nullptr.

	void clear();
		/// Removes all Collector instances from the Registry.

	void exportTo(Exporter& exporter) const;
		/// Exports all registered collector's metrics through the given Exporter.
		///
		/// The Registry's mutex is held during the export. A callback that
		/// runs during the export, such as the callback of a CallbackMetric
		/// or a function of the Exporter, must not create, destroy, register
		/// or unregister a Collector of this Registry, nor call
		/// registerCollector(), unregisterCollector(), findCollector(),
		/// clear() or exportTo() of this Registry. The mutex is not recursive
		/// on every platform, so such a call can deadlock or change the
		/// Registry while it is exported.

	[[nodiscard]] static Registry& defaultRegistry();
		/// Returns the default Registry.

private:
	std::map<std::string, Collector*> _collectors;
	mutable Poco::FastMutex _mutex;

	Registry(const Registry&) = delete;
	Registry(Registry&&) = delete;
	Registry& operator = (const Registry&) = delete;
	Registry& operator = (Registry&&) = delete;
};


} // namespace Poco::Prometheus


#endif // Prometheus_Metric_INCLUDED
