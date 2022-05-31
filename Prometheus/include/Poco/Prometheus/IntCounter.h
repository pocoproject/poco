//
// IntCounter.h
//
// Library: Prometheus
// Package: Core
// Module:  IntCounter
//
// Definition of the IntCounter class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_IntCounter_INCLUDED
#define Prometheus_IntCounter_INCLUDED


#include "Poco/Prometheus/Metric.h"
#include <atomic>


namespace Poco {
namespace Prometheus {


class Registry;


class Prometheus_API IntCounter: public Metric
	/// IntCounter is a very low overhead implementation of
	/// a Counter, supporting 64-bit unsigned integer values
	/// only, using std::atomic
	///
	/// Labels are not supported.
{
public:
	using Sample = Poco::UInt64;

	struct Params
	{
		std::string help;
	};

	explicit IntCounter(const std::string& name);
		/// Creates a IntCounter with the given name and
		/// registers it with the default registry.

	IntCounter(const std::string& name, const Params& params);
		/// Creates a IntCounter with the given name and help text (via params), and
		/// registers it with the default registry.

	IntCounter(const std::string& name, Registry* pRegistry);
		/// Creates a IntCounter with the given name and
		/// registers it with the given registry (if not nullptr).

	IntCounter(const std::string& name, const Params& params, Registry* pRegistry);
		/// Creates a IntCounter with the given name and help text (via params), and
		/// registers it with the given registry (if not nullptr).

	~IntCounter() = default;
		/// Destroys the IntCounter.

	using Metric::help;
	IntCounter& help(const std::string& text);
		/// Sets the IntCounter's help text.
		///
		/// Must only be set once, immediately after creating
		/// the IntCounter.

	Sample value() const;
		/// Returns the IntCounter's current value.

	void inc();
		/// Increments the IntCounter's current value by one.

	void inc(Sample v);
		/// Increments the IntCounter's current value by the given value.

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


inline IntCounter::IntCounter(const std::string& name):
	Metric(Metric::Type::COUNTER, name)
{
}


inline IntCounter::IntCounter(const std::string& name, const Params& params):
	Metric(Metric::Type::COUNTER, name)
{
	setHelp(params.help);
}


inline IntCounter::IntCounter(const std::string& name, Registry* pRegistry):
	Metric(Metric::Type::COUNTER, name, pRegistry)
{
}


inline IntCounter::IntCounter(const std::string& name, const Params& params, Registry* pRegistry):
	Metric(Metric::Type::COUNTER, name, pRegistry)
{
	setHelp(params.help);
}


inline IntCounter& IntCounter::help(const std::string& text)
{
	setHelp(text);
	return *this;
}


inline IntCounter::Sample IntCounter::value() const
{
	return _value.load(std::memory_order_relaxed);
}


inline void IntCounter::inc()
{
	_value.fetch_add(1, std::memory_order_relaxed);
}


inline void IntCounter::inc(IntCounter::Sample v)
{
	_value.fetch_add(v, std::memory_order_relaxed);
}


} } // namespace Poco::Prometheus


#endif // Prometheus_IntCounter_INCLUDED
