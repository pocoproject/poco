//
// Metric.h
//
// Library: Prometheus
// Package: Core
// Module:  Metric
//
// Definition of the Metric class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Metric_INCLUDED
#define Prometheus_Metric_INCLUDED


#include "Poco/Prometheus/Collector.h"
#include "Poco/Prometheus/Registry.h"


namespace Poco {
namespace Prometheus {


class Prometheus_API Metric: public Collector
	/// This is the base class for all metrics, such as
	/// Counter, Gauge and Histogram.
{
public:
	enum class Type
	{
		COUNTER,
		GAUGE,
		HISTOGRAM,
		SUMMARY,
		UNTYPED
	};

	Type type() const;
		/// Returns the metric's type.

	const std::string& help() const;
		/// Returns the metric's help text.

	~Metric() = default;
		/// Destroys the Metric.

protected:
	Metric(Type type, const std::string& name);
		/// Creates a Metric with the given type and name,
		/// and registers it with the default Registry.
		///
		/// The name must be a valid metric name and match the regular
		/// expression [a-zA-Z_:][a-zA-Z0-9_:]*.

	Metric(Type type, const std::string& name, Registry* pRegistry);
		/// Creates a Metric with the given type and name,
		/// and registers it with the given Registry (if not nullptr).
		///
		/// The name must be a valid metric name and match the regular
		/// expression [a-zA-Z_:][a-zA-Z0-9_:]*.

	void setHelp(const std::string& help);
		/// Sets the metric's help text.

private:
	const Type _type;
	std::string _help;
};


//
// inlines
//


inline Metric::Metric(Type type, const std::string& name):
	Collector(name),
	_type(type)
{
}


inline Metric::Metric(Type type, const std::string& name, Registry* pRegistry):
	Collector(name, pRegistry),
	_type(type)
{
}


inline Metric::Type Metric::type() const
{
	return _type;
}


inline const std::string& Metric::help() const
{
	return _help;
}


inline void Metric::setHelp(const std::string& help)
{
	_help = help;
}


} } // namespace Poco::Prometheus


#endif // Prometheus_Metric_INCLUDED
