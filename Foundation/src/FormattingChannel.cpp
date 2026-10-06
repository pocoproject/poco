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
#include "DeferredRelease.h"
#include "Poco/Message.h"
#include "Poco/LoggingRegistry.h"


namespace Poco {


FormattingChannel::Parts::Parts(Formatter::Ptr pFormatter, Channel::Ptr pChannel):
	pFormatter(std::move(pFormatter)),
	pChannel(std::move(pChannel))
{
}


FormattingChannel::FormattingChannel():
	_pParts(new Parts(nullptr, nullptr)),
	_pCurrentParts(_pParts.get())
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter):
	_pParts(new Parts(std::move(pFormatter), nullptr)),
	_pCurrentParts(_pParts.get())
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter, Channel::Ptr pChannel):
	_pParts(new Parts(std::move(pFormatter), std::move(pChannel))),
	_pCurrentParts(_pParts.get())
{
}


FormattingChannel::~FormattingChannel()
{
}


void FormattingChannel::setFormatter(Formatter::Ptr pFormatter)
{
	// The new formatter and the current channel make a new pair. The pair
	// that is replaced is let go of when the mutex is free again, since a
	// destructor may log, and not before the threads that log are done
	// with it.
	PartsPtr pParts = new Parts(std::move(pFormatter), nullptr);
	{
		FastMutex::ScopedLock lock(_mutex);
		pParts->pChannel = _pParts->pChannel;
		_pParts.swap(pParts);
		_pCurrentParts.store(_pParts.get());
	}
	DeferredRelease::release(pParts);
}


Formatter::Ptr FormattingChannel::getFormatter() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _pParts->pFormatter;
}


void FormattingChannel::setChannel(Channel::Ptr pChannel)
{
	// The current formatter and the new channel make a new pair; the one
	// replaced is let go of as in setFormatter().
	PartsPtr pParts = new Parts(nullptr, std::move(pChannel));
	{
		FastMutex::ScopedLock lock(_mutex);
		pParts->pFormatter = _pParts->pFormatter;
		_pParts.swap(pParts);
		_pCurrentParts.store(_pParts.get());
	}
	DeferredRelease::release(pParts);
}


Channel::Ptr FormattingChannel::getChannel() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _pParts->pChannel;
}


void FormattingChannel::log(const Message& msg)
{
	// The formatter and the channel are loaded as one pair and kept for
	// the time of the reader: a thread that logs takes no mutex and counts
	// no reference. Within the reader of a Logger, this one costs next to
	// nothing.
	DeferredRelease::Reader reader;
	Parts& parts = *_pCurrentParts.load();
	if (parts.pChannel)
	{
		if (parts.pFormatter)
		{
			std::string text;
			parts.pFormatter->format(msg, text);
			parts.pChannel->log(Message(msg, text));
		}
		else
		{
			parts.pChannel->log(msg);
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
