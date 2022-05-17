//
// Counter.h
//
// Library: Prometheus
// Package: Core
// Module:  Counter
//
// Definition of the Counter class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Counter_INCLUDED
#define Prometheus_Counter_INCLUDED


#include "Poco/Prometheus/LabeledMetricImpl.h"
#include "Poco/Prometheus/AtomicFloat.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace Prometheus {


class Prometheus_API CounterSample
{
public:
	CounterSample() = default;
		/// Creates the CounterSample.

	~CounterSample() = default;
		/// Destroys the CounterSample.

	double value() const;
		/// Returns the counter's current value.

	Poco::Timestamp timestamp() const;
		/// Returns the sample's timestamp, which in this
		/// implementation is always 0.

	void inc(double v = 1.0);
		/// Increments the counter's current value.

private:
	AtomicFloat<double> _value;

	CounterSample(const CounterSample&) = delete;
	CounterSample(CounterSample&&) = delete;
	CounterSample& operator = (const CounterSample&) = delete;
	CounterSample& operator = (CounterSample&&) = delete;
};


class Prometheus_API Counter: public LabeledMetricImpl<CounterSample>
	/// A generic counter supporting labels.
	///
	/// To create a Counter with labels and register it
	/// with the default Registry:
	///     Counter sampleCounter("sample_counter"s, {
	///         .help = "A sample counter"s,
	///         .labelNames = {"label1"s, "label2"s}
	///     });
	///
	/// To increment a Counter (with labels):
	///     sampleCounter.labels({"value1"s, "value2"}).inc();
	///
	/// To create a Counter without labels and register it
	/// with the default Registry:
	///     Counter simpleCounter("simple_counter"s);
	///     simpleCounter.help("A simple counter"s);
	///
	/// To increment the value of a Counter (without labels):
	///     simpleCounter.inc(42);
{
public:
	struct Params
	{
		std::string help;
		std::vector<std::string> labelNames;
	};

	explicit Counter(const std::string& name);
		/// Creates a Counter with the given name and
		/// registers it with the default registry.

	Counter(const std::string& name, const Params& params);
		/// Creates a Counter with the given name and parameters
		/// registers it with the default registry.

	Counter(const std::string& name, Registry* pRegistry);
		/// Creates a Counter with the given name and
		/// registers it with the given registry (if not nullptr).

	Counter(const std::string& name, const Params& params, Registry* pRegistry);
		/// Creates a Counter with the given name and params, and
		/// registers it with the given registry (if not nullptr).

	~Counter() = default;
		/// Destroys the Counter.

	using LabeledMetric::help;
	Counter& help(const std::string& text);
		/// Sets the Counter's help text.
		///
		/// Must only be set once, immediately after creating
		/// the Counter.

	using LabeledMetric::labelNames;
	Counter& labelNames(const std::vector<std::string>& labelNames);
		/// Sets the Counter's label names.
		///
		/// Must only be set once, immediately after creating
		/// the Counter.

	double value() const;
		/// Returns the counter's current value.
		///
		/// Can only be used if no labels have been defined.

	void inc(double v = 1.0);
		/// Increments the counter's current value.
		///
		/// Can only be used if no labels have been defined.

	// Collector
	void exportTo(Exporter& exporter) const override;

protected:
	// LabeledMetricImpl
	std::unique_ptr<CounterSample> createSample() const override;
	void writeSample(Exporter& exporter, const std::vector<std::string>& labelValues, const CounterSample& sample) const override;

private:
	CounterSample _sample;
};


//
// inlines
//


inline double CounterSample::value() const
{
	return _value.value();
}


inline Poco::Timestamp CounterSample::timestamp() const
{
	return 0;
}


inline void CounterSample::inc(double v)
{
	poco_assert_dbg (v >= 0.0);

	_value += v;
}


inline Counter& Counter::help(const std::string& text)
{
	setHelp(text);
	return *this;
}


inline Counter& Counter::labelNames(const std::vector<std::string>& labelNames)
{
	setLabelNames(labelNames);
	return *this;
}


inline void Counter::inc(double v)
{
	_sample.inc(v);
}


} } // namespace Poco::Prometheus


#endif // Prometheus_Counter_INCLUDED
