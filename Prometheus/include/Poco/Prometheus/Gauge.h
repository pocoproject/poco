//
// Gauge.h
//
// Library: Prometheus
// Package: Core
// Module:  Gauge
//
// Definition of the Gauge class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Gauge_INCLUDED
#define Prometheus_Gauge_INCLUDED


#include "Poco/Prometheus/LabeledMetricImpl.h"
#include "Poco/Prometheus/AtomicFloat.h"
#include "Poco/Clock.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace Prometheus {


class Prometheus_API GaugeSample
{
public:
	GaugeSample() = default;
		/// Creates the GaugeSample.

	~GaugeSample() = default;
		/// Destroys the GaugeSample.

	double value() const;
		/// Returns the Gauge's current value.

	Poco::Timestamp timestamp() const;
		/// Returns the sample's timestamp, which in this
		/// implementation is always 0.

	void inc(double v = 1.0);
		/// Increments the Gauge's current value.

	void dec(double v = 1.0);
		/// Decrements the Gauge's current value.

	void set(double v);
		/// Sets the Gauge's current value.

	void set(Poco::Clock::ClockVal v);
		/// Sets the Gauge's current value to the
		/// given Clock value, converted to seconds.

	void setToCurrentTime();
		/// Sets the Gauge's current value to the current epoch time
		/// (seconds since midnight January 1st 1970).

private:
	AtomicFloat<double> _value;

	GaugeSample(const GaugeSample&) = delete;
	GaugeSample(GaugeSample&&) = delete;
	GaugeSample& operator = (const GaugeSample&) = delete;
	GaugeSample& operator = (GaugeSample&&) = delete;
};


class Prometheus_API Gauge: public LabeledMetricImpl<GaugeSample>
	/// A generic gauge supporting labels.
	///
	///
	/// To create a Gauge with labels and register it
	/// with the default Registry:
	///     Gauge sampleGauge("sample_gauge"s, {
	///         .help = "A sample gauge"s,
	///         .labelNames = {"label1"s, "label2"s}
	///     });
	///
	/// To increment a Gauge (with labels):
	///     sampleGauge.labels({"value1"s, "value2"}).inc();
	///
	/// To create a Gauge without labels and register it
	/// with the default Registry:
	///     Gauge simpleGauge("simple_gauge"s);
	///     simpleGauge.help("A simple gauge"s);
	///
	/// To set the value of a Gauge (without labels):
	///     simpleGauge.set(42);
{
public:
	struct Params
	{
		std::string help;
		std::vector<std::string> labelNames;
	};

	explicit Gauge(const std::string& name);
		/// Creates a Gauge with the given name and
		/// registers it with the default registry.

	Gauge(const std::string& name, const Params& params);
		/// Creates a Gauge with the given name and params and
		/// registers it with the default registry.

	Gauge(const std::string& name, Registry* pRegistry);
		/// Creates a Gauge with the given name and
		/// registers it with the given registry (if not nullptr).

	Gauge(const std::string& name, const Params& params, Registry* pRegistry);
		/// Creates a Gauge with the given name and params, and
		/// registers it with the given registry (if not nullptr).

	~Gauge() = default;
		/// Destroys the Gauge.

	using LabeledMetric::help;
	Gauge& help(const std::string& text);
		/// Sets the Gauge's help text.
		///
		/// Must only be set once, immediately after creating
		/// the Gauge.

	using LabeledMetric::labelNames;
	Gauge& labelNames(const std::vector<std::string>& labelNames);
		/// Sets the Gauge's label names.
		///
		/// Must only be set once, immediately after creating
		/// the Gauge.

	double value() const;
		/// Returns the Gauge's current value.
		///
		/// Can only be used if no labels have been defined.

	void inc(double v = 1.0);
		/// Increments the Gauge's current value.
		///
		/// Can only be used if no labels have been defined.

	void dec(double v = 1.0);
		/// Decrements the Gauge's current value.
		///
		/// Can only be used if no labels have been defined.

	void set(double v);
		/// Sets the Gauge's current value.
		///
		/// Can only be used if no labels have been defined.

	void set(Poco::Clock::ClockVal v);
		/// Sets the Gauge's current value to the
		/// given Clock value, converted to seconds.
		///
		/// Can only be used if no labels have been defined.

	void setToCurrentTime();
		/// Sets the Gauge's current value to the current epoch time
		/// (seconds since midnight January 1st 1970).

	// Collector
	void exportTo(Exporter& exporter) const override;

protected:
	// LabeledMetricImpl
	std::unique_ptr<GaugeSample> createSample() const override;
	void writeSample(Exporter& exporter, const std::vector<std::string>& labelValues, const GaugeSample& sample) const override;

private:
	GaugeSample _sample;
};


//
// inlines
//


inline double GaugeSample::value() const
{
	return _value.value();
}


inline Poco::Timestamp GaugeSample::timestamp() const
{
	return 0;
}


inline void GaugeSample::inc(double v)
{
	_value += v;
}


inline void GaugeSample::dec(double v)
{
	_value -= v;
}


inline void GaugeSample::set(double v)
{
	_value = v;
}


inline void GaugeSample::set(Poco::Clock::ClockVal v)
{
	_value = static_cast<double>(v)/Poco::Clock::resolution();
}


inline void GaugeSample::setToCurrentTime()
{
	_value = static_cast<double>(Poco::Timestamp().epochMicroseconds()/1000)/1000.0;
}


inline Gauge& Gauge::help(const std::string& text)
{
	setHelp(text);
	return *this;
}


inline Gauge& Gauge::labelNames(const std::vector<std::string>& labelNames)
{
	setLabelNames(labelNames);
	return *this;
}


inline void Gauge::inc(double v)
{
	_sample.inc(v);
}


inline void Gauge::dec(double v)
{
	_sample.dec(v);
}


inline void Gauge::set(double v)
{
	_sample.set(v);
}


inline void Gauge::set(Poco::Clock::ClockVal v)
{
	_sample.set(v);
}


inline void Gauge::setToCurrentTime()
{
	_sample.setToCurrentTime();
}


} } // namespace Poco::Prometheus


#endif // Prometheus_Gauge_INCLUDED
