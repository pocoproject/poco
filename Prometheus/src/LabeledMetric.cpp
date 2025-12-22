//
// LabeledMetric.cpp
//
// Library: Prometheus
// Package: Core
// Module:  LabeledMetric
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/LabeledMetric.h"
#include "Poco/RegularExpression.h"
#include "Poco/Exception.h"


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


const std::vector<std::string> LabeledMetric::EMPTY_LABEL;


const std::string& LabeledMetric::validateLabelName(const std::string& name)
{
	static Poco::RegularExpression nameExpr{"^[a-zA-Z0-9_]*$"s};

	if (!nameExpr.match(name)) throw Poco::SyntaxException("Not a valid label name"s, name);

	return name;
}


void LabeledMetric::setLabelNames(const std::vector<std::string>& labelNames)
{
	if (!_labelNames.empty()) throw Poco::IllegalStateException("Label names have already been set for metric"s, name());

	_labelNames.reserve(labelNames.size());
	for (const auto& labelName: labelNames)
	{
		_labelNames.push_back(validateLabelName(labelName));
	}
}


} } // namespace Poco::Prometheus
