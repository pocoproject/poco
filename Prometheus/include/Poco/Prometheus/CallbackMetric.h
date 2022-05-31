//
// CallbackMetric.h
//
// Library: Prometheus
// Package: Core
// Module:  CallbackMetric
//
// Definition of the CallbackMetric class template.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_CallbackMetric_INCLUDED
#define Prometheus_CallbackMetric_INCLUDED


#include "Poco/Prometheus/Metric.h"
#include "Poco/Prometheus/Exporter.h"
#include <functional>
#include <vector>


namespace Poco {
namespace Prometheus {


template <typename T, Metric::Type metricType>
class CallbackMetric: public Metric
	/// A generic Metric implementation where the sample is
	/// obtained via a callback function or lambda.
	///
	/// The callback is invoked whenever the value
	/// of the metric is requested. This is typically when
	/// the metric is written to an Exporter.
	///
	/// This is implemented as a template to support both counter and gauge
	/// metrics with different underlying types.
	///
	/// Note that only metric types COUNTER and GAUGE are supported.
	///
	/// Labels are not supported.
	///
	/// Example usage:
	///     CallbackMetric<Poco::UInt64, Metric::Type::COUNTER> sampleCount(
	///         "sample_count"s,
	///         []()
	///         {
	///             return 42;
	///         }
	///     );
	///
	/// There are also pre-instantiated types available:
	///    - CallbackIntCounter
	///    - CallbackIntGauge
	///    - CallbackCounter
	///    - CallbackGauge
	///
	/// The following sample types are supported:
	///    - Poco::Int32
	///    - Poco::UInt32
	///    - Poco::Int64
	///    - Poco::UInt64
	///    - double
{
public:
	using Sample = T;
	using Callback = std::function<Sample()>;

	CallbackMetric(const std::string& name, Callback callback):
		Metric(metricType, name),
		_callback(callback)
		/// Creates a CallbackMetric with the given type and name and
		/// registers it with the default registry.
	{
	}

	CallbackMetric(const std::string& name, const std::string& help, Callback callback):
		Metric(metricType, name),
		_callback(callback)
		/// Creates a CallbackMetric with the given type, name and help text, and
		/// registers it with the default registry.
	{
		setHelp(help);
	}

	CallbackMetric(const std::string& name, Registry* pRegistry, Callback callback):
		Metric(metricType, name, pRegistry),
		_callback(callback)
		/// Creates a CallbackMetric with the given type and name and
		/// registers it with the given registry (if not nullptr).
	{
	}

	CallbackMetric(const std::string& name, const std::string& help, Registry* pRegistry, Callback callback):
		Metric(metricType, name, pRegistry),
		_callback(callback)
		/// Creates a CallbackMetric with the given type, name and help text, and
		/// registers it with the given registry (if not nullptr).
	{
		setHelp(help);
	}

	~CallbackMetric() = default;
		/// Destroys the CallbackMetric.

	using Metric::help;
	CallbackMetric& help(const std::string& text)
		/// Sets the CallbackMetric's help text.
		///
		/// Must only be set once, immediately after creating
		/// the CallbackMetric.
	{
		setHelp(text);
		return *this;
	}

	Sample value() const
		/// Invokes the callback function and returns the
		/// value returned by it.
	{
		return _callback();
	}

	// Collector
	void exportTo(Exporter& exporter) const override
	{
		const std::vector<std::string> EMPTY_VEC;

		exporter.writeHeader(*this);
		exporter.writeSample(*this, EMPTY_VEC, EMPTY_VEC, _callback(), 0);
	}

private:
	Callback _callback;
};


using CallbackIntCounter = CallbackMetric<Poco::UInt64, Metric::Type::COUNTER>;
using CallbackIntGauge = CallbackMetric<Poco::Int64, Metric::Type::GAUGE>;
using CallbackCounter = CallbackMetric<double, Metric::Type::COUNTER>;
using CallbackGauge = CallbackMetric<double, Metric::Type::GAUGE>;


} } // namespace Poco::Prometheus


#endif // Prometheus_CallbackMetric_INCLUDED
