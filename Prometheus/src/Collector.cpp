//
// Collector.cpp
//
// Library: Prometheus
// Package: Core
// Module:  Collector
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/Collector.h"
#include "Poco/RegularExpression.h"
#include "Poco/Exception.h"


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


const std::string& Collector::validateName(const std::string& name)
{
	static Poco::RegularExpression nameExpr{"^[a-zA-Z_:][a-zA-Z0-9_:]*$"s};

	if (!nameExpr.match(name)) throw Poco::SyntaxException("Not a valid collector or metric name"s, name);

	return name;
}


} } // namespace Poco::Prometheus
