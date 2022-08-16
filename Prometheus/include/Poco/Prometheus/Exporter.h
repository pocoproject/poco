//
// Exporter.h
//
// Library: Prometheus
// Package: Core
// Module:  Exporter
//
// Definition of the Exporter class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Exporter_INCLUDED
#define Prometheus_Exporter_INCLUDED


#include "Poco/Prometheus/Prometheus.h"
#include "Poco/Timestamp.h"
#include <vector>


namespace Poco {
namespace Prometheus {


class Metric;


class Prometheus_API Exporter
	/// The Exporter interface is used to format and write metrics
	/// to an output stream.
{
public:
	virtual void writeHeader(const Metric& metric) = 0;
		/// Writes the header (HELP and TYPE in text format).

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, float value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, double value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::UInt32 value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::Int32 value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::UInt64 value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::Int64 value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

	virtual void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, const std::string& value, const Poco::Timestamp& timestamp = 0) = 0;
		/// Writes a sample for the given metric and the given labels.

protected:
	Exporter() = default;
	virtual ~Exporter() = default;

	Exporter(const Exporter&) = delete;
	Exporter(Exporter&&) = delete;
	Exporter& operator = (const Exporter&) = delete;
	Exporter& operator = (Exporter&&) = delete;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_Exporter_INCLUDED
