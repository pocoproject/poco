//
// Gauge.cpp
//
// Library: Prometheus
// Package: Core
// Module:  Gauge
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/Gauge.h"


namespace Poco {
namespace Prometheus {


Gauge::Gauge(const std::string& name):
	LabeledMetricImpl<GaugeSample>(Metric::Type::GAUGE, name)
{
}


Gauge::Gauge(const std::string& name, const Params& params):
	LabeledMetricImpl<GaugeSample>(Metric::Type::GAUGE, name)
{
	setHelp(params.help);
	setLabelNames(params.labelNames);
}


Gauge::Gauge(const std::string& name, Registry* pRegistry):
	LabeledMetricImpl<GaugeSample>(Metric::Type::GAUGE, name, pRegistry)
{
}


Gauge::Gauge(const std::string& name, const Params& params, Registry* pRegistry):
	LabeledMetricImpl<GaugeSample>(Metric::Type::GAUGE, name, pRegistry)
{
	setHelp(params.help);
	setLabelNames(params.labelNames);
}


double Gauge::value() const
{
	return _sample.value();
}


void Gauge::exportTo(Exporter& exporter) const
{
	if (labelNames().empty())
	{
		exporter.writeHeader(*this);
		exporter.writeSample(*this, {}, {}, _sample.value(), _sample.timestamp());
	}
	else
	{
		LabeledMetricImpl<GaugeSample>::exportTo(exporter);
	}
}


std::unique_ptr<GaugeSample> Gauge::createSample() const
{
	return std::make_unique<GaugeSample>();
}


void Gauge::writeSample(Exporter& exporter, const std::vector<std::string>& labelValues, const GaugeSample& sample) const
{
	exporter.writeSample(*this, labelNames(), labelValues, sample.value(), sample.timestamp());
}


} } // namespace Poco::Prometheus
