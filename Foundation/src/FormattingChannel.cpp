//
// FormattingChannel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  Formatter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/FormattingChannel.h"
#include "Poco/Message.h"
#include "Poco/LoggingRegistry.h"


namespace Poco {


FormattingChannel::FormattingChannel():
	_pFormatter(nullptr),
	_pChannel(nullptr)
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter):
	_pFormatter(pFormatter),
	_pChannel(nullptr)
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter, Channel::Ptr pChannel):
	_pFormatter(pFormatter),
	_pChannel(pChannel)
{
}


FormattingChannel::~FormattingChannel()
{
}


void FormattingChannel::setFormatter(Formatter::Ptr pFormatter)
{
	// The formatter that is replaced is released when the mutex is free
	// again: its destructor may log.
	{
		FastMutex::ScopedLock lock(_mutex);
		_pFormatter.swap(pFormatter);
	}
}


Formatter::Ptr FormattingChannel::getFormatter() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _pFormatter;
}


void FormattingChannel::setChannel(Channel::Ptr pChannel)
{
	// The channel that is replaced is released when the mutex is free
	// again: its destructor may log.
	{
		FastMutex::ScopedLock lock(_mutex);
		_pChannel.swap(pChannel);
	}
}


Channel::Ptr FormattingChannel::getChannel() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _pChannel;
}


void FormattingChannel::log(const Message& msg)
{
	Channel::Ptr pChannel = getChannel();
	if (pChannel)
	{
		Formatter::Ptr pFormatter = getFormatter();
		if (pFormatter)
		{
			std::string text;
			pFormatter->format(msg, text);
			pChannel->log(Message(msg, text));
		}
		else
		{
			pChannel->log(msg);
		}
	}
}


void FormattingChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name == "channel")
		setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
	else if (name == "formatter")
		setFormatter(LoggingRegistry::defaultRegistry().formatterForName(value));
	else
	{
		Channel::Ptr pChannel = getChannel();
		if (pChannel)
			pChannel->setProperty(name, value);
	}
}


void FormattingChannel::open()
{
	Channel::Ptr pChannel = getChannel();
	if (pChannel)
		pChannel->open();
}


void FormattingChannel::close()
{
	Channel::Ptr pChannel = getChannel();
	if (pChannel)
		pChannel->close();
}


} // namespace Poco
