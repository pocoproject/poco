//
// FastLoggerConfigurator.cpp
//
// Library: Util
// Package: Configuration
// Module:  FastLoggerConfigurator
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"


#ifdef POCO_ENABLE_FASTLOGGER


#include "Poco/Util/FastLoggerConfigurator.h"
#include "Poco/FastLogger.h"
#include "Poco/AutoPtr.h"
#include <map>

using namespace std::string_literals;


namespace Poco {
namespace Util {


void FastLoggerConfigurator::configure(AbstractConfiguration::Ptr pConfig)
{
	poco_check_ptr(pConfig);

	// First configure sinks (file outputs, etc.)
	AbstractConfiguration::Ptr pSinksConfig(pConfig->createView("logging.fastloggers.sinks"s));
	configureSinks(pSinksConfig);

	// Then configure the loggers
	AbstractConfiguration::Ptr pLoggersConfig(pConfig->createView("logging.fastloggers"s));
	configureFastLoggers(pLoggersConfig);
}


void FastLoggerConfigurator::configureSinks(AbstractConfiguration::Ptr pConfig)
{
	// Start the backend first
	FastLogger::startBackend();

	// Check for file sink configuration
	if (pConfig->hasProperty("file"s))
	{
		std::string filename = pConfig->getString("file"s);
		if (!filename.empty())
		{
			FastLogger::addFileSink(filename);
		}
	}

	// Pattern configuration (for future use)
	if (pConfig->hasProperty("pattern"s))
	{
		std::string pattern = pConfig->getString("pattern"s);
		FastLogger::setPattern(pattern);
	}
}


void FastLoggerConfigurator::configureFastLoggers(AbstractConfiguration::Ptr pConfig)
{
	using LoggerMap = std::map<std::string, AutoPtr<AbstractConfiguration>>;

	// Use a map to sort loggers by name, ensuring parents are initialized before children
	LoggerMap loggerMap;
	for (const auto& l : pConfig->keys())
	{
		// Skip the sinks section
		if (l == "sinks"s) continue;

		AutoPtr<AbstractConfiguration> pLoggerConfig(pConfig->createView(l));
		loggerMap[pLoggerConfig->getString("name"s, ""s)] = pLoggerConfig;
	}

	for (const auto& p : loggerMap)
	{
		configureFastLogger(p.second);
	}
}


void FastLoggerConfigurator::configureFastLogger(AbstractConfiguration::Ptr pConfig)
{
	std::string name = pConfig->getString("name"s, ""s);
	FastLogger& logger = FastLogger::get(name);

	for (const auto& p : pConfig->keys())
	{
		if (p != "name"s)
		{
			if (p == "level"s)
			{
				logger.setLevel(pConfig->getString(p));
			}
			else
			{
				// Forward unknown properties to setProperty
				logger.setProperty(p, pConfig->getString(p));
			}
		}
	}
}


} } // namespace Poco::Util


#endif // POCO_ENABLE_FASTLOGGER
