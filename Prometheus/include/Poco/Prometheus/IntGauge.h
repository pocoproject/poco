//
// IntGauge.h
//
// Library: Prometheus
// Package: Core
// Module:  IntGauge
//
// Definition of the IntGauge class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_IntGauge_INCLUDED
#define Prometheus_IntGauge_INCLUDED


#include "Poco/Prometheus/Metric.h"
#include <atomic>


namespace Poco {
namespace Prometheus {


class Registry;


class Prometheus_API IntGauge: public Metric
	/// IntGauge is a very low overhead implementation of
	/// a Gauge, supporting 64-bit integer values only,
	/// using std::atomic.
	///
	/// Labels are not supported.
{
public:
	using Sample = Poco::Int64;

	struct Params
	{
		std::string help;
	};

	explicit IntGauge(const std::string& name);
		/// Creates a IntGauge with the given name and
		/// registers it with the default registry.

	IntGauge(const std::string& name, const Params& params);
		/// Creates a IntGauge with the given name and help text (via params), and
		/// registers it with the default registry.

	IntGauge(const std::string& name, Registry* pRegistry);
		/// Creates a IntGauge with the given name and
		/// registers it with the given registry (if not nullptr).

	IntGauge(const std::string& name, const Params& params, Registry* pRegistry);
		/// Creates a IntGauge with the given name and help text (via params), and
		/// registers it with the given registry (if not nullptr).

	~IntGauge() = default;
		/// Destroys the IntGauge.

	using Metric::help;
	IntGauge& help(const std::string& text);
		/// Sets the IntGauge's help text.
		///
		/// Must only be set once, immediately after creating
		/// the IntGauge.

	Sample value() const;
		/// Returns the IntGauge's current value.

	void inc();
		/// Increments the IntGauge's current value by one.

	void dec();
		/// Decrements the IntGauge's current value by one.

	void inc(Sample v);
		/// Increments the IntGauge's current value by the given value.

	void dec(Sample v);
		/// Increments the IntGauge's current value by the given value.

	void set(Sample v);
		/// Sets the IntGauge's current value.

	void setToCurrentTime();
		/// Sets the Gauge's current value to the current epoch time
		/// (seconds since midnight January 1st 1970).

	// Collector
	void exportTo(Exporter& exporter) const override;

private:
	std::atomic<Sample> _value{0};
};


//
// inlines
//


inline IntGauge::IntGauge(const std::string& name):
	Metric(Metric::Type::GAUGE, name)
{
}


inline IntGauge::IntGauge(const std::string& name, const Params& params):
	Metric(Metric::Type::GAUGE, name)
{
	setHelp(params.help);
}


inline IntGauge::IntGauge(const std::string& name, Registry* pRegistry):
	Metric(Metric::Type::GAUGE, name, pRegistry)
{
}


inline IntGauge::IntGauge(const std::string& name, const Params& params, Registry* pRegistry):
	Metric(Metric::Type::GAUGE, name, pRegistry)
{
	setHelp(params.help);
}


inline IntGauge& IntGauge::help(const std::string& text)
{
	setHelp(text);
	return *this;
}


inline IntGauge::Sample IntGauge::value() const
{
	return _value.load(std::memory_order_relaxed);
}


inline void IntGauge::inc()
{
	_value.fetch_add(1, std::memory_order_relaxed);
}


inline void IntGauge::dec()
{
	_value.fetch_sub(1, std::memory_order_relaxed);
}


inline void IntGauge::inc(IntGauge::Sample v)
{
	_value.fetch_add(v, std::memory_order_relaxed);
}


inline void IntGauge::dec(IntGauge::Sample v)
{
	_value.fetch_sub(v, std::memory_order_relaxed);
}


inline void IntGauge::set(IntGauge::Sample v)
{
	_value.exchange(v, std::memory_order_relaxed);
}


inline void IntGauge::setToCurrentTime()
{
	set(Poco::Timestamp().epochTime());
}


} } // namespace Poco::Prometheus


#endif // Prometheus_IntGauge_INCLUDED
