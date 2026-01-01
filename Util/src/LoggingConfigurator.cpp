//
// LoggingConfigurator.cpp
//
// Library: Util
// Package: Configuration
// Module:  LoggingConfigurator
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Channel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Formatter.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Logger.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/LoggingFactory.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#ifdef POCO_ENABLE_FASTLOGGER
#include "Poco/FastLogger.h"
#endif
#include <map>
#include <sstream>


using Poco::AutoPtr;
using Poco::Formatter;
using Poco::PatternFormatter;
using Poco::Channel;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::LoggingRegistry;
using Poco::LoggingFactory;
using namespace std::string_literals;


namespace Poco {
namespace Util {


void LoggingConfigurator::configure(AbstractConfiguration::Ptr pConfig)
{
	poco_check_ptr (pConfig);

	AbstractConfiguration::Ptr pFormattersConfig(pConfig->createView("logging.formatters"s));
	configureFormatters(pFormattersConfig);

	AbstractConfiguration::Ptr pChannelsConfig(pConfig->createView("logging.channels"s));
	configureChannels(pChannelsConfig);

	AbstractConfiguration::Ptr pLoggersConfig(pConfig->createView("logging.loggers"s));
	configureLoggers(pLoggersConfig);
}


void LoggingConfigurator::configureFormatters(AbstractConfiguration::Ptr pConfig)
{
	for (const auto& key: pConfig->keys())
	{
		AutoPtr<AbstractConfiguration> pFormatterConfig(pConfig->createView(key));
		AutoPtr<Formatter> pFormatter(createFormatter(pFormatterConfig));
		LoggingRegistry::defaultRegistry().registerFormatter(key, pFormatter);
	}
}


void LoggingConfigurator::configureChannels(AbstractConfiguration::Ptr pConfig)
{
    auto cKeys = pConfig->keys();
	for (const auto& c: cKeys)
	{
		AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(c));
		AutoPtr<Channel> pChannel = createChannel(pChannelConfig);
		LoggingRegistry::defaultRegistry().registerChannel(c, pChannel);
	}
	for (const auto& c: cKeys)
	{
		AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(c));
		Channel::Ptr pChannel = LoggingRegistry::defaultRegistry().channelForName(c);
		configureChannel(pChannel, pChannelConfig);
	}
}


void LoggingConfigurator::configureLoggers(AbstractConfiguration::Ptr pConfig)
{
	using LoggerMap = std::map<std::string, AutoPtr<AbstractConfiguration>>;

	// use a map to sort loggers by their name, ensuring initialization in correct order (parents before children)
	LoggerMap loggerMap;
	for (const auto& l: pConfig->keys())
	{
		AutoPtr<AbstractConfiguration> pLoggerConfig(pConfig->createView(l));
		loggerMap[pLoggerConfig->getString("name"s, ""s)] = pLoggerConfig;
	}
	for (const auto& p: loggerMap)
	{
		configureLogger(p.second);
	}
}


Formatter::Ptr LoggingConfigurator::createFormatter(AbstractConfiguration::Ptr pConfig)
{
	Formatter::Ptr pFormatter(LoggingFactory::defaultFactory().createFormatter(pConfig->getString("class"s)));
	for (const auto& p: pConfig->keys())
	{
		if (p != "class"s)
			pFormatter->setProperty(p, pConfig->getString(p));
	}
	return pFormatter;
}


Channel::Ptr LoggingConfigurator::createChannel(AbstractConfiguration::Ptr pConfig)
{
	Channel::Ptr pChannel(LoggingFactory::defaultFactory().createChannel(pConfig->getString("class"s)));
	Channel::Ptr pWrapper(pChannel);
	for (const auto& p: pConfig->keys())
	{
		if (p == "pattern"s)
		{
			AutoPtr<Formatter> pPatternFormatter(new PatternFormatter(pConfig->getString(p)));
			pWrapper = new FormattingChannel(pPatternFormatter, pChannel);
		}
		else if (p == "formatter"s)
		{
			AutoPtr<FormattingChannel> pFormattingChannel(new FormattingChannel(nullptr, pChannel));
			if (pConfig->hasProperty("formatter.class"s))
			{
				AutoPtr<AbstractConfiguration> pFormatterConfig(pConfig->createView(p));
				AutoPtr<Formatter> pFormatter(createFormatter(pFormatterConfig));
				pFormattingChannel->setFormatter(pFormatter);
			}
			else pFormattingChannel->setProperty(p, pConfig->getString(p));
			pWrapper = pFormattingChannel;
		}
	}
	return pWrapper;
}


void LoggingConfigurator::configureChannel(Channel::Ptr pChannel, AbstractConfiguration::Ptr pConfig)
{
	for (const auto& p: pConfig->keys())
	{
		if (p != "pattern"s && p != "formatter"s && p != "class"s)
		{
			pChannel->setProperty(p, pConfig->getString(p));
		}
	}
}


void LoggingConfigurator::configureLogger(AbstractConfiguration::Ptr pConfig)
{
	const std::string loggerName = pConfig->getString("name"s, ""s);
	const std::string loggerType = pConfig->getString("type"s, ""s);
	const bool useFastLogger = icompare(loggerType, "fast"s) == 0;

	auto props = pConfig->keys();

	if (useFastLogger)
	{
#ifdef POCO_ENABLE_FASTLOGGER
		// Process quill.* backend options BEFORE creating the logger,
		// since FastLogger::get() starts the backend thread
		for (const auto& p: props)
		{
			if (p == "quill"s)
			{
				AutoPtr<AbstractConfiguration> pQuillConfig(pConfig->createView(p));
				for (const auto& q: pQuillConfig->keys())
				{
					FastLogger::setBackendOption(q, pQuillConfig->getString(q));
				}
			}
		}

		// Now create/get the logger (this starts the backend with the options set above)
		FastLogger& logger = FastLogger::get(loggerName);
		for (const auto& p: props)
		{
			if (p == "channel"s && pConfig->hasProperty("channel.class"s))
			{
				AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(p));
				AutoPtr<Channel> pChannel(createChannel(pChannelConfig));
				configureChannel(pChannel, pChannelConfig);
				FastLogger::setChannel(logger.name(), pChannel);
			}
			else if (p != "name"s && p != "type"s && p != "quill"s)
			{
				FastLogger::setProperty(logger.name(), p, pConfig->getString(p));
			}
		}
#else
		throw Poco::InvalidAccessException("FastLogger is not available (POCO_ENABLE_FASTLOGGER is not defined)");
#endif
	}
	else
	{
		Logger& logger = Logger::get(loggerName);
		for (const auto& p: props)
		{
			if (p == "channel"s && pConfig->hasProperty("channel.class"s))
			{
				AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(p));
				AutoPtr<Channel> pChannel(createChannel(pChannelConfig));
				configureChannel(pChannel, pChannelConfig);
				Logger::setChannel(logger.name(), pChannel);
			}
			else if (p == "quill"s)
			{
				// Warn about quill.* options on non-fast loggers
				AutoPtr<AbstractConfiguration> pQuillConfig(pConfig->createView(p));
				for (const auto& q: pQuillConfig->keys())
				{
					Logger::get("LoggingConfigurator"s).warning(
						"Ignoring quill.%s property on logger '%s' - quill options only apply to type=fast loggers"s,
						q, loggerName.empty() ? "(root)"s : loggerName);
				}
			}
			else if (p != "name"s && p != "type"s)
			{
				Logger::setProperty(logger.name(), p, pConfig->getString(p));
			}
		}
	}
}


void LoggingConfigurator::configure(
	const std::string& level,
	const std::string& pattern,
	const std::string& configTemplate)
{
	std::string config = Poco::format(configTemplate, level, pattern);

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig);
}


} } // namespace Poco::Util
