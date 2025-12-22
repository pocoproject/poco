//
// TextExporter.h
//
// Library: Prometheus
// Package: Core
// Module:  TextExporter
//
// Definition of the TextExporter class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_TextExporter_INCLUDED
#define Prometheus_TextExporter_INCLUDED


#include "Poco/Prometheus/Exporter.h"
#include "Poco/Prometheus/Metric.h"
#include <ostream>


namespace Poco {
namespace Prometheus {


class Prometheus_API TextExporter: public Exporter
	/// Exporter implementation for the Prometheus text format.
	///
	/// See https://github.com/prometheus/docs/blob/main/content/docs/instrumenting/exposition_formats.md
	/// for the specification of the Prometheus text exposition format.
{
public:
	explicit TextExporter(std::ostream& ostr);
		/// Creates the TextExporter for the given output stream.

	TextExporter() = delete;
	TextExporter(const TextExporter&) = delete;
	TextExporter& operator = (const TextExporter&) = delete;

	~TextExporter() = default;

	// Exporter
	void writeHeader(const Metric& metric) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, float value, const Poco::Timestamp& timestamp = 0) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, double value, const Poco::Timestamp& timestamp = 0) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::UInt32 value, const Poco::Timestamp& timestamp = 0) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::Int32 value, const Poco::Timestamp& timestamp = 0) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::UInt64 value, const Poco::Timestamp& timestamp = 0) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::Int64 value, const Poco::Timestamp& timestamp = 0) override;
	void writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, const std::string& value, const Poco::Timestamp& timestamp = 0) override;

protected:
	static const std::string& typeToString(Metric::Type type);

	static const std::string COUNTER;
	static const std::string GAUGE;
	static const std::string HISTOGRAM;
	static const std::string SUMMARY;
	static const std::string UNTYPED;

private:
	std::ostream& _stream;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_TextExporter_INCLUDED
