//
// TextExporter.cpp
//
// Library: Prometheus
// Package: Core
// Module:  TextExporter
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/TextExporter.h"
#include "Poco/NumberFormatter.h"
#include <vector>
#include <ostream>
#include <cmath>


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


const std::string TextExporter::COUNTER{"counter"s};
const std::string TextExporter::GAUGE{"gauge"s};
const std::string TextExporter::HISTOGRAM{"histogram"s};
const std::string TextExporter::SUMMARY{"summary"s};
const std::string TextExporter::UNTYPED{"untyped"s};


namespace
{
	std::string escape(const std::string& str)
	{
		std::string result;
		for (char c: str)
		{
			switch (c)
			{
			case '\\':
				result += "\\\\";
				break;
			case '\n':
				result += "\\n";
				break;
			case '"':
				result += "\\\"";
				break;
			default:
				result += c;
			}
		}
		return result;
	}

	template <typename T>
	void writeSampleImpl(std::ostream& stream, const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, T&& value, const Poco::Timestamp& timestamp)
	{
		poco_assert_dbg (labelNames.size() == labelValues.size());

		stream << metric.name();
		if (!labelNames.empty())
		{
			stream << '{';
			for (std::size_t i = 0; i < labelNames.size(); i++)
			{
				if (i > 0) stream << ',';
				stream << labelNames[i] << "=\"" << escape(labelValues[i]) << '"';
			}
			stream << '}';
		}
		stream << ' ' << value;
		if (timestamp != 0)
		{
			stream << ' ' << timestamp.epochMicroseconds()/1000;
		}
		stream << '\n';
	}
}


TextExporter::TextExporter(std::ostream& ostr):
	_stream(ostr)
{
}


void TextExporter::writeHeader(const Metric& metric)
{
	const std::string& help = metric.help();
	const std::string& type = typeToString(metric.type());

	if (!help.empty())
	{
		_stream << "# HELP " << metric.name() << ' ' << help << '\n';
	}
	_stream << "# TYPE " << metric.name() << ' ' << type << '\n';
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, float value, const Poco::Timestamp& timestamp)
{
	writeSample(metric, labelNames, labelValues, static_cast<double>(value), timestamp);
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, double value, const Poco::Timestamp& timestamp)
{
	std::string valueString;
	if (std::isinf(value))
	{
		if (value > 0)
			valueString = "+Inf";
		else
			valueString = "-Inf";
	}
	else if (std::isnan(value))
	{
		valueString = "NaN";
	}
	else
	{
		valueString = Poco::NumberFormatter::format(value);
	}
	writeSampleImpl(_stream, metric, labelNames, labelValues, valueString, timestamp);
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::UInt32 value, const Poco::Timestamp& timestamp)
{
	writeSampleImpl(_stream, metric, labelNames, labelValues, value, timestamp);
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::Int32 value, const Poco::Timestamp& timestamp)
{
	writeSampleImpl(_stream, metric, labelNames, labelValues, value, timestamp);
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::UInt64 value, const Poco::Timestamp& timestamp)
{
	writeSampleImpl(_stream, metric, labelNames, labelValues, value, timestamp);
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, Poco::Int64 value, const Poco::Timestamp& timestamp)
{
	writeSampleImpl(_stream, metric, labelNames, labelValues, value, timestamp);
}


void TextExporter::writeSample(const Metric& metric, const std::vector<std::string>& labelNames, const std::vector<std::string>& labelValues, const std::string& value, const Poco::Timestamp& timestamp)
{
	writeSampleImpl(_stream, metric, labelNames, labelValues, value, timestamp);
}


const std::string& TextExporter::typeToString(Metric::Type type)
{
	switch (type)
	{
	case Metric::Type::COUNTER:
		return COUNTER;
	case Metric::Type::GAUGE:
		return GAUGE;
	case Metric::Type::HISTOGRAM:
		return HISTOGRAM;
	case Metric::Type::SUMMARY:
		return SUMMARY;
	case Metric::Type::UNTYPED:
		return UNTYPED;
	default:
		poco_bugcheck();
		return UNTYPED;
	}
}


} } // namespace Poco::Prometheus
