//
// LoggingConfigurator.cpp
//
// Library: Util
// Package: Configuration
// Module:  LoggingConfigurator
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/AutoPtr.h"
#include "Poco/Channel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Formatter.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Logger.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/LoggingFactory.h"
#include <map>


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


LoggingConfigurator::LoggingConfigurator()
{
}


LoggingConfigurator::~LoggingConfigurator()
{
}


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
	AbstractConfiguration::Keys formatters;
	pConfig->keys(formatters);
	for (const auto& f: formatters)
	{
		AutoPtr<AbstractConfiguration> pFormatterConfig(pConfig->createView(f));
		AutoPtr<Formatter> pFormatter(createFormatter(pFormatterConfig));
		LoggingRegistry::defaultRegistry().registerFormatter(f, pFormatter);
	}
}


void LoggingConfigurator::configureChannels(AbstractConfiguration::Ptr pConfig)
{
	AbstractConfiguration::Keys channels;
	pConfig->keys(channels);
	for (const auto& c: channels)
	{
		AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(c));
		AutoPtr<Channel> pChannel = createChannel(pChannelConfig);
		LoggingRegistry::defaultRegistry().registerChannel(c, pChannel);
	}
	for (const auto& c: channels)
	{
		AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(c));
		Channel::Ptr pChannel = LoggingRegistry::defaultRegistry().channelForName(c);
		configureChannel(pChannel, pChannelConfig);
	}
}


void LoggingConfigurator::configureLoggers(AbstractConfiguration::Ptr pConfig)
{
	using LoggerMap = std::map<std::string, AutoPtr<AbstractConfiguration>>;

	AbstractConfiguration::Keys loggers;
	pConfig->keys(loggers);
	// use a map to sort loggers by their name, ensuring initialization in correct order (parents before children)
	LoggerMap loggerMap;
	for (const auto& l: loggers)
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
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
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
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p == "pattern"s)
		{
			AutoPtr<Formatter> pPatternFormatter(new PatternFormatter(pConfig->getString(p)));
			pWrapper = new FormattingChannel(pPatternFormatter, pChannel);
		}
		else if (p == "formatter"s)
		{
			AutoPtr<FormattingChannel> pFormattingChannel(new FormattingChannel(0, pChannel));
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
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p != "pattern"s && p != "formatter"s && p != "class"s)
		{
			pChannel->setProperty(p, pConfig->getString(p));
		}
	}
}


void LoggingConfigurator::configureLogger(AbstractConfiguration::Ptr pConfig)
{
	Logger& logger = Logger::get(pConfig->getString("name"s, ""s));
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p == "channel"s && pConfig->hasProperty("channel.class"s))
		{
			AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(p));
			AutoPtr<Channel> pChannel(createChannel(pChannelConfig));
			configureChannel(pChannel, pChannelConfig);
			Logger::setChannel(logger.name(), pChannel);
		}
		else if (p != "name"s)
		{
			Logger::setProperty(logger.name(), p, pConfig->getString(p));
		}
	}
}


} } // namespace Poco::Util
