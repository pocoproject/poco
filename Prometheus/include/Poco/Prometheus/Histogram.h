//
// Histogram.h
//
// Library: Prometheus
// Package: Core
// Module:  Histogram
//
// Definition of the Histogram class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Histogram_INCLUDED
#define Prometheus_Histogram_INCLUDED


#include "Poco/Prometheus/LabeledMetricImpl.h"
#include "Poco/Clock.h"
#include "Poco/Mutex.h"
#include <vector>


namespace Poco {
namespace Prometheus {


struct Prometheus_API HistogramData
{
	std::vector<Poco::UInt64> bucketCounts;
		/// Counter for each bucket.

	double sum;
		/// Sum of all observations.

	Poco::UInt64 count;
		/// Total number of observations.
};


class Prometheus_API HistogramSample
{
public:
	explicit HistogramSample(const std::vector<double>& bucketBounds);
		/// Creates the HistogramSample.

	~HistogramSample() = default;
		/// Destroys the HistogramSample.

	void observe(double value);
		/// Observes the given amount, by increasing the count
		/// in the respective bucket.

	void observe(Poco::Clock::ClockVal v);
		/// Converts the given Clock time in microseconds to
		/// seconds and increases the count in the respective
		/// bucket.
		///
		/// Can only be used if no labels have been defined.

	HistogramData data() const;
		/// Returns the histogram's data.

	const std::vector<double>& bucketBounds() const;
		/// Returns the buckets upper bounds;

private:
	const std::vector<double>& _bucketBounds;
	std::vector<Poco::UInt64> _bucketCounts;
	Poco::UInt64 _count = 0;
	double _sum = 0.0;
	mutable Poco::FastMutex _mutex;

	HistogramSample() = delete;
	HistogramSample(const HistogramSample&) = delete;
	HistogramSample(HistogramSample&&) = delete;
	HistogramSample& operator = (const HistogramSample&) = delete;
	HistogramSample& operator = (HistogramSample&&) = delete;
};


class Prometheus_API Histogram: public LabeledMetricImpl<HistogramSample>
	/// A histogram with a configurable number of buckets.
	///
	/// To create a Histogram with labels and three buckets, and register it
	/// with the default Registry:
	///     Histogram sampleHistogram("sample_histogram"s, {
	///         .help = "A sample histogram"s,
	///         .labelNames = {"label1"s, "label2"s},
	///         .buckets = {0.5, 1.0, 5.0}
	///     });
	///
	/// To observe a value with a Histogram (with labels):
	///     sampleHistogram.labels({"value1"s, "value2"}).observe(1.5);
	///
	/// To create a Histogram without labels and register it
	/// with the default Registry:
	///     Histogram simpleHistogram("simple_histogram"s, {
	///         .help = "A simple histogram"s,
	///         .buckets = {0.5, 1.0, 5.0}
	///     });
	///
	/// To observe a value with a Histogram (without labels):
	///     simpleHistogram.observe(1.5);
{
public:
	struct Params
	{
		std::string help;
		std::vector<std::string> labelNames;
		std::vector<double> buckets;
	};

	explicit Histogram(const std::string& name);
		/// Creates a Histogram with the given name and
		/// registers it with the default registry.

	Histogram(const std::string& name, const Params& params);
		/// Creates a Histogram with the given name and params, and
		/// registers it with the default registry.

	Histogram(const std::string& name, Registry* pRegistry);
		/// Creates a Histogram with the given name and
		/// registers it with the given registry (if not nullptr).

	Histogram(const std::string& name, const Params& params, Registry* pRegistry);
		/// Creates a Histogram with the given name and params, and
		/// registers it with the given registry (if not nullptr).

	~Histogram() = default;
		/// Destroys the Histogram.

	using Metric::help;
	Histogram& help(const std::string& text);
		/// Sets the Histogram's help text.
		///
		/// Must only be set once, immediately after creating
		/// the Histogram.

	using LabeledMetric::labelNames;
	Histogram& labelNames(const std::vector<std::string>& labelNames);
		/// Sets the Histogram's label names.
		///
		/// Must only be set once, immediately after creating
		/// the Gauge. The label name "le" must not be used
		/// as it is reserved for the histogram.

	Histogram& buckets(const std::vector<double>& bucketBounds);
		/// Sets the Histogram's bucket upper bounds.
		///
		/// Upper bounds must be strictly ordered from lowest to highest
		/// value. The final infinite bucket must not be included.
		///
		/// Must only be set once, immediately after creating
		/// the Histogram.

	const std::vector<double> buckets() const;
		/// Returns the configured bucket upper bounds.

	void observe(double value);
		/// Observes the given amount, by increasing the count
		/// in the respective bucket.

	void observe(Poco::Clock::ClockVal v);
		/// Converts the given Clock time in microseconds to
		/// seconds and increases the count in the respective
		/// bucket.
		///
		/// Can only be used if no labels have been defined.

	HistogramData data() const;
		/// Returns the histogram's data.

	// LabeledMetricImpl
	std::unique_ptr<HistogramSample> createSample() const override;

	// Collector
	void exportTo(Exporter& exporter) const override;

private:
	std::vector<double> _bucketBounds;
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//


inline const std::vector<double>& HistogramSample::bucketBounds() const
{
	return _bucketBounds;
}


inline HistogramData HistogramSample::data() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	HistogramData data;
	data.bucketCounts = _bucketCounts;
	data.count = _count;
	data.sum = _sum;

	return data;
}


inline Histogram& Histogram::help(const std::string& text)
{
	setHelp(text);
	return *this;
}


inline Histogram& Histogram::labelNames(const std::vector<std::string>& labelNames)
{
	setLabelNames(labelNames);
	return *this;
}


inline const std::vector<double> Histogram::buckets() const
{
	return _bucketBounds;
}


} } // namespace Poco::Prometheus


#endif // Prometheus_Gauge_INCLUDED
