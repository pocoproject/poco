//
// AsyncChannel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  AsyncChannel
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AsyncChannel.h"
#include "Poco/Notification.h"
#include "Poco/Message.h"
#include "Poco/Formatter.h"
#include "Poco/AutoPtr.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/NumberParser.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/Format.h"


namespace Poco {


class MessageNotification: public Notification
{
public:
	MessageNotification(const Message& msg):
		_msg(msg)
	{
	}

	~MessageNotification()
	{
	}

	const Message& message() const
	{
		return _msg;
	}

private:
	Message _msg;
};


AsyncChannel::AsyncChannel(Channel::Ptr pChannel, Thread::Priority prio):
	_pChannel(pChannel),
	_thread("AsyncChannel")
{
	_thread.setPriority(prio);
}


AsyncChannel::~AsyncChannel()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void AsyncChannel::setChannel(Channel::Ptr pChannel)
{
	FastMutex::ScopedLock lock(_channelMutex);

	_pChannel = pChannel;
}


Channel::Ptr AsyncChannel::getChannel() const
{
	return _pChannel;
}


void AsyncChannel::open()
{
	FastMutex::ScopedLock lock(_threadMutex);

	if (!_thread.isRunning()) _thread.start(*this);
}


void AsyncChannel::close()
{
	if (_thread.isRunning())
	{
		while (!_queue.empty()) Thread::sleep(100);

		do
		{
			_queue.wakeUpAll();
		}
		while (!_thread.tryJoin(100));
	}
}


void AsyncChannel::log(const Message& msg)
{
	if (_queueSize != 0 && _queue.size() >= _queueSize)
	{
		++_dropCount;
		return;
	}

	if (_dropCount != 0)
	{
		_queue.enqueueNotification(new MessageNotification(Message(msg, Poco::format("Dropped %z messages.", _dropCount))));
		_dropCount = 0;
	}

	open();

	_queue.enqueueNotification(new MessageNotification(msg));
}


void AsyncChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name == "channel")
	{
		setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
	}
	else if (name == "priority")
	{
		setPriority(value);
	}
	else if (name == "queueSize")
	{
		if (Poco::icompare(value, "none") == 0 || Poco::icompare(value, "unlimited") == 0 || value.empty())
			_queueSize = 0;
		else
			_queueSize = Poco::NumberParser::parseUnsigned(value);
	}
	else
	{
		Channel::setProperty(name, value);
	}
}


void AsyncChannel::run()
{
	AutoPtr<Notification> nf = _queue.waitDequeueNotification();
	while (nf)
	{
		MessageNotification* pNf = dynamic_cast<MessageNotification*>(nf.get());
		{
			FastMutex::ScopedLock lock(_channelMutex);

			if (pNf && _pChannel) _pChannel->log(pNf->message());
		}
		nf = _queue.waitDequeueNotification();
	}
}


void AsyncChannel::setPriority(const std::string& value)
{
	Thread::Priority prio = Thread::PRIO_NORMAL;

	if (value == "lowest")
		prio = Thread::PRIO_LOWEST;
	else if (value == "low")
		prio = Thread::PRIO_LOW;
	else if (value == "normal")
		prio = Thread::PRIO_NORMAL;
	else if (value == "high")
		prio = Thread::PRIO_HIGH;
	else if (value == "highest")
		prio = Thread::PRIO_HIGHEST;
	else
		throw InvalidArgumentException("thread priority", value);

	_thread.setPriority(prio);
}


} // namespace Poco
