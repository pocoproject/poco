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
	_pFormatter(0),
	_pChannel(0)
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter):
	_pFormatter(pFormatter),
	_pChannel(0)
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
	_pFormatter = pFormatter;
}


Formatter::Ptr FormattingChannel::getFormatter() const
{
	return _pFormatter;
}


void FormattingChannel::setChannel(Channel::Ptr pChannel)
{
	_pChannel = pChannel;
}


Channel::Ptr FormattingChannel::getChannel() const
{
	return _pChannel;
}


void FormattingChannel::log(const Message& msg)
{
	if (_pChannel)
	{
		if (_pFormatter)
		{
			std::string text;
			_pFormatter->format(msg, text);
			_pChannel->log(Message(msg, text));
		}
		else
		{
			_pChannel->log(msg);
		}
	}
}


void FormattingChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name == "channel")
		setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
	else if (name == "formatter")
		setFormatter(LoggingRegistry::defaultRegistry().formatterForName(value));
	else if (_pChannel)
		_pChannel->setProperty(name, value);
}


void FormattingChannel::open()
{
	if (_pChannel)
		_pChannel->open();
}


void FormattingChannel::close()
{
	if (_pChannel)
		_pChannel->close();
}


} // namespace Poco
