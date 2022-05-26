//
// IntGauge.cpp
//
// Library: Prometheus
// Package: Core
// Module:  IntGauge
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/IntGauge.h"
#include "Poco/Prometheus/Exporter.h"
#include <vector>


namespace Poco {
namespace Prometheus {


void IntGauge::exportTo(Exporter& exporter) const
{
	const std::vector<std::string> EMPTY_VEC;

	exporter.writeHeader(*this);
	exporter.writeSample(*this, EMPTY_VEC, EMPTY_VEC, _value, 0);
}


} } // namespace Poco::Prometheus
