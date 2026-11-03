//
// Counter.cpp
//
// Library: Prometheus
// Package: Core
// Module:  Counter
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/Counter.h"


namespace Poco::Prometheus {


CounterSample::CounterSample() = default;


CounterSample::~CounterSample() = default;


Counter::Counter(const std::string& name):
	LabeledMetricImpl<CounterSample>(Metric::Type::COUNTER, name, nullptr)
{
	Registry::defaultRegistry().registerCollector(this);
}


Counter::Counter(const std::string& name, const Params& params):
	LabeledMetricImpl<CounterSample>(Metric::Type::COUNTER, name, nullptr)
{
	setHelp(params.help);
	setLabelNames(params.labelNames);
	Registry::defaultRegistry().registerCollector(this);
}


Counter::Counter(const std::string& name, Registry* pRegistry):
	LabeledMetricImpl<CounterSample>(Metric::Type::COUNTER, name, nullptr)
{
	if (pRegistry != nullptr) pRegistry->registerCollector(this);
}


Counter::Counter(const std::string& name, const Params& params, Registry* pRegistry):
	LabeledMetricImpl<CounterSample>(Metric::Type::COUNTER, name, nullptr)
{
	setHelp(params.help);
	setLabelNames(params.labelNames);
	if (pRegistry != nullptr) pRegistry->registerCollector(this);
}


Counter::~Counter()
{
	unregister();
}


double Counter::value() const
{
	return _sample.value();
}


void Counter::exportTo(Exporter& exporter) const
{
	if (labelNames().empty())
	{
		exporter.writeHeader(*this);
		exporter.writeSample(*this, {}, {}, _sample.value(), _sample.timestamp());
	}
	else
	{
		LabeledMetricImpl<CounterSample>::exportTo(exporter);
	}
}


std::unique_ptr<CounterSample> Counter::createSample() const
{
	return std::make_unique<CounterSample>();
}


void Counter::writeSample(Exporter& exporter, const std::vector<std::string>& labelValues, const CounterSample& sample) const
{
	exporter.writeSample(*this, labelNames(), labelValues, sample.value(), sample.timestamp());
}


} // namespace Poco::Prometheus
