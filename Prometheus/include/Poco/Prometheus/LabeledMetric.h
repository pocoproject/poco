//
// LabeledMetric.h
//
// Library: Prometheus
// Package: Core
// Module:  LabeledMetric
//
// Definition of the LabeledMetric class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_LabeledMetric_INCLUDED
#define Prometheus_LabeledMetric_INCLUDED


#include "Poco/Prometheus/Metric.h"
#include <vector>


namespace Poco {
namespace Prometheus {


class Prometheus_API LabeledMetric: public Metric
	/// This class adds support for labels to the
	/// basic Metric class.
{
public:
	const std::vector<std::string>& labelNames() const;
		/// Returns the metric's label names.

protected:
	LabeledMetric(Type type, const std::string& name);
		/// Creates a LabeledMetric with the given type and name,
		/// and registers it with the default Registry.
		///
		/// The name must be a valid metric name and match the regular
		/// expression [a-zA-Z_:][a-zA-Z0-9_:]*.

	LabeledMetric(Type type, const std::string& name, Registry* pRegistry);
		/// Creates a LabeledMetric with the given type and name,
		/// and registers it with the given Registry (if not nullptr).
		///
		/// The name must be a valid metric name and match the regular
		/// expression [a-zA-Z_:][a-zA-Z0-9_:]*.

	void setLabelNames(const std::vector<std::string>& labelNames);
		/// Sets the metric's label names.

	virtual ~LabeledMetric() = default;
		/// Destroys the LabeledMetric.

	static const std::string& validateLabelName(const std::string& name);
		/// Validates the given name against the regular expression
		/// defining a valid label name ([a-zA-Z_:][a-zA-Z0-9_:]*).

	static const std::vector<std::string> EMPTY_LABEL;

private:
	std::vector<std::string> _labelNames;
};


//
// inlines
//


inline LabeledMetric::LabeledMetric(Type type, const std::string& name):
	Metric(type, name)
{
}


inline LabeledMetric::LabeledMetric(Type type, const std::string& name, Registry* pRegistry):
	Metric(type, name, pRegistry)
{
}


inline const std::vector<std::string>& LabeledMetric::labelNames() const
{
	return _labelNames;
}


} } // namespace Poco::Prometheus


#endif // Prometheus_Metric_INCLUDED
