//
// LoggingRegistry.cpp
//
// Library: Foundation
// Package: Logging
// Module:  LoggingRegistry
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/LoggingRegistry.h"


namespace Poco {


LoggingRegistry::LoggingRegistry()
{
}


LoggingRegistry::~LoggingRegistry()
{
}


Channel::Ptr LoggingRegistry::channelForName(const std::string& name) const
{
	FastMutex::ScopedLock lock(_mutex);

	ChannelMap::const_iterator it = _channelMap.find(name);
	if (it != _channelMap.end())
		return it->second;
	else
		throw NotFoundException("logging channel", name);
}


Formatter::Ptr LoggingRegistry::formatterForName(const std::string& name) const
{
	FastMutex::ScopedLock lock(_mutex);

	FormatterMap::const_iterator it = _formatterMap.find(name);
	if (it != _formatterMap.end())
		return it->second;
	else
		throw NotFoundException("logging formatter", name);
}


void LoggingRegistry::registerChannel(const std::string& name, Channel::Ptr pChannel)
{
	FastMutex::ScopedLock lock(_mutex);

	_channelMap[name] = ChannelPtr(pChannel, true);
}


void LoggingRegistry::registerFormatter(const std::string& name, Formatter::Ptr pFormatter)
{
	FastMutex::ScopedLock lock(_mutex);

	_formatterMap[name] = FormatterPtr(pFormatter, true);
}


void LoggingRegistry::unregisterChannel(const std::string& name)
{
	FastMutex::ScopedLock lock(_mutex);

	ChannelMap::iterator it = _channelMap.find(name);
	if (it != _channelMap.end())
		_channelMap.erase(it);
	else
		throw NotFoundException("logging channel", name);
}


void LoggingRegistry::unregisterFormatter(const std::string& name)
{
	FastMutex::ScopedLock lock(_mutex);

	FormatterMap::iterator it = _formatterMap.find(name);
	if (it != _formatterMap.end())
		_formatterMap.erase(it);
	else
		throw NotFoundException("logging formatter", name);
}


void LoggingRegistry::clear()
{
	FastMutex::ScopedLock lock(_mutex);

	_channelMap.clear();
	_formatterMap.clear();
}


LoggingRegistry& LoggingRegistry::defaultRegistry()
{
	static LoggingRegistry lr;
	return lr;
}


} // namespace Poco
