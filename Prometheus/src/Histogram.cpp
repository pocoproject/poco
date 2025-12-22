//
// Histogram.cpp
//
// Library: Prometheus
// Package: Core
// Module:  Histogram
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/Histogram.h"
#include "Poco/Prometheus/Gauge.h"
#include "Poco/Prometheus/Exporter.h"
#include "Poco/NumberFormatter.h"

using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


HistogramSample::HistogramSample(const std::vector<double>& bucketBounds):
	_bucketBounds(bucketBounds),
	_bucketCounts(bucketBounds.size(), 0)
{
}


void HistogramSample::observe(double value)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	const std::size_t n = _bucketBounds.size();
	for (std::size_t i = 0; i < n; i++)
	{
		if (value <= _bucketBounds[i])
		{
			_bucketCounts[i]++;
		}
	}
	_sum += value;
	_count++;
}


void HistogramSample::observe(Poco::Clock::ClockVal v)
{
	observe(double(v)/Poco::Clock::resolution());
}


Histogram::Histogram(const std::string& name):
	LabeledMetricImpl(Metric::Type::HISTOGRAM, name)
{
}


Histogram::Histogram(const std::string& name, const Params& params):
	LabeledMetricImpl(Metric::Type::HISTOGRAM, name),
	_bucketBounds(params.buckets)
{
	setHelp(params.help);
	setLabelNames(params.labelNames);
}


Histogram::Histogram(const std::string& name, Registry* pRegistry):
	LabeledMetricImpl(Metric::Type::HISTOGRAM, name, pRegistry)
{
}


Histogram::Histogram(const std::string& name, const Params& params, Registry* pRegistry):
	LabeledMetricImpl(Metric::Type::HISTOGRAM, name, pRegistry),
	_bucketBounds(params.buckets)
{
	setHelp(params.help);
	setLabelNames(params.labelNames);
}


Histogram& Histogram::buckets(const std::vector<double>& bucketBounds)
{
	_bucketBounds = bucketBounds;
	return *this;
}


void Histogram::observe(double value)
{
	labels(EMPTY_LABEL).observe(value);
}


void Histogram::observe(Poco::Clock::ClockVal v)
{
	labels(EMPTY_LABEL).observe(double(v)/Poco::Clock::resolution());
}


HistogramData Histogram::data() const
{
	return labels(EMPTY_LABEL).data();
}


std::unique_ptr<HistogramSample> Histogram::createSample() const
{
	return std::make_unique<HistogramSample>(_bucketBounds);
}


void Histogram::exportTo(Exporter& exporter) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Gauge bucket(name() + "_bucket"s, nullptr);
	Gauge sum(name() + "_sum"s, nullptr);
	Gauge count(name() + "_count"s, nullptr);

	exporter.writeHeader(*this);
	std::vector<std::string> bucketLabels = labelNames();
	bucketLabels.push_back("le"s);
	const std::size_t n = _bucketBounds.size();
	forEach<HistogramSample>(
		[&](const std::vector<std::string>& labelValues, const HistogramSample& sample)
		{
			std::vector<std::string> bucketLabelValues = labelValues;
			bucketLabelValues.push_back(""s);
			const HistogramData data = sample.data();
			for (std::size_t i = 0; i < n; i++)
			{
				bucketLabelValues.back() = Poco::NumberFormatter::format(_bucketBounds[i]);

				exporter.writeSample(bucket, bucketLabels, bucketLabelValues, data.bucketCounts[i]);
			}
			bucketLabelValues.back() = "+Inf"s;
			exporter.writeSample(bucket, bucketLabels, bucketLabelValues, data.count);
			exporter.writeSample(sum, labelNames(), labelValues, data.sum);
			exporter.writeSample(count, labelNames(), labelValues, data.count);
		}
	);
}


} } // namespace Poco::Prometheus
