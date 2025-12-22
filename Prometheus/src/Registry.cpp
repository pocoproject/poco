//
// Registry.cpp
//
// Library: Prometheus
// Package: Core
// Module:  Registry
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/Collector.h"
#include "Poco/Exception.h"


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


void Registry::registerCollector(Collector* pCollector)
{
	poco_check_ptr (pCollector);

	Poco::FastMutex::ScopedLock lock(_mutex);

	const auto it = _collectors.find(pCollector->name());
	if (it == _collectors.end())
	{
		_collectors[pCollector->name()] = pCollector;
	}
	else
	{
		throw Poco::ExistsException("collector"s, pCollector->name());
	}
}


void Registry::unregisterCollector(Collector* pCollector)
{
	poco_check_ptr (pCollector);

	unregisterCollector(pCollector->name());
}


void Registry::unregisterCollector(const std::string& collectorName)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_collectors.erase(collectorName);
}


Collector* Registry::findCollector(const std::string& collectorName) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	const auto it = _collectors.find(collectorName);
	if (it != _collectors.end())
		return it->second;
	else
		return nullptr;
}


void Registry::clear()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_collectors.clear();
}


void Registry::exportTo(Exporter& exporter) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	for (const auto& p: _collectors)
	{
		p.second->exportTo(exporter);
	}
}


Registry& Registry::defaultRegistry()
{
	static Registry r;
	return r;
}


} } // namespace Poco::Prometheus
