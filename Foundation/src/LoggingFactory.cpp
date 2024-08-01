//
// LoggingFactory.cpp
//
// Library: Foundation
// Package: Logging
// Module:  LoggingFactory
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/LoggingFactory.h"
#include "Poco/AsyncChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/NullChannel.h"
#include "Poco/EventChannel.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOGCHANNEL)
#include "Poco/SyslogChannel.h"
#endif
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/EventLogChannel.h"
#include "Poco/WindowsConsoleChannel.h"
#endif
#include "Poco/PatternFormatter.h"


using namespace std::string_literals;


namespace Poco {


LoggingFactory::LoggingFactory()
{
	registerBuiltins();
}


LoggingFactory::~LoggingFactory()
{
}


void LoggingFactory::registerChannelClass(const std::string& className, ChannelInstantiator* pFactory)
{
	_channelFactory.registerClass(className, pFactory);
}


void LoggingFactory::registerFormatterClass(const std::string& className, FormatterFactory* pFactory)
{
	_formatterFactory.registerClass(className, pFactory);
}


Channel::Ptr LoggingFactory::createChannel(const std::string& className) const
{
	return _channelFactory.createInstance(className);
}


Formatter::Ptr LoggingFactory::createFormatter(const std::string& className) const
{
	return _formatterFactory.createInstance(className);
}


LoggingFactory& LoggingFactory::defaultFactory()
{
	static LoggingFactory lf;
	return lf;
}


void LoggingFactory::registerBuiltins()
{
	_channelFactory.registerClass("AsyncChannel"s, new Instantiator<AsyncChannel, Channel>);
#if defined(POCO_OS_FAMILY_WINDOWS)
	_channelFactory.registerClass("ConsoleChannel"s, new Instantiator<WindowsConsoleChannel, Channel>);
	_channelFactory.registerClass("ColorConsoleChannel"s, new Instantiator<WindowsColorConsoleChannel, Channel>);
#else
	_channelFactory.registerClass("ConsoleChannel"s, new Instantiator<ConsoleChannel, Channel>);
	_channelFactory.registerClass("ColorConsoleChannel"s, new Instantiator<ColorConsoleChannel, Channel>);
#endif

#ifndef POCO_NO_FILECHANNEL
	_channelFactory.registerClass("FileChannel"s, new Instantiator<FileChannel, Channel>);
	_channelFactory.registerClass("SimpleFileChannel"s, new Instantiator<SimpleFileChannel, Channel>);
#endif
	_channelFactory.registerClass("FormattingChannel"s, new Instantiator<FormattingChannel, Channel>);
#ifndef POCO_NO_SPLITTERCHANNEL
	_channelFactory.registerClass("SplitterChannel"s, new Instantiator<SplitterChannel, Channel>);
#endif
	_channelFactory.registerClass("NullChannel"s, new Instantiator<NullChannel, Channel>);
	_channelFactory.registerClass("EventChannel"s, new Instantiator<EventChannel, Channel>);

#if defined(POCO_OS_FAMILY_UNIX)
#ifndef POCO_NO_SYSLOGCHANNEL
	_channelFactory.registerClass("SyslogChannel"s, new Instantiator<SyslogChannel, Channel>);
#endif
#endif

#if defined(POCO_OS_FAMILY_WINDOWS)
	_channelFactory.registerClass("EventLogChannel"s, new Instantiator<EventLogChannel, Channel>);
#endif

	_formatterFactory.registerClass("PatternFormatter"s, new Instantiator<PatternFormatter, Formatter>);
}


} // namespace Poco
