//
// AsyncChannel.h
//
// Library: Foundation
// Package: Logging
// Module:  AsyncChannel
//
// Definition of the AsyncChannel class.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AsyncChannel_INCLUDED
#define Foundation_AsyncChannel_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Channel.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include "Poco/AutoPtr.h"
#include "Poco/NotificationQueue.h"
#include <atomic>


namespace Poco {


class Foundation_API AsyncChannel: public Channel, public Runnable
	/// A channel uses a separate thread for logging.
	///
	/// Using this channel can help to improve the performance of
	/// applications that produce huge amounts of log messages or
	/// that write log messages to multiple channels simultaneously.
	///
	/// All log messages are put into a queue and this queue is
	/// then processed by a separate thread.
{
public:
	using Ptr = AutoPtr<AsyncChannel>;

	AsyncChannel(Channel::Ptr pChannel = 0, Thread::Priority prio = Thread::PRIO_NORMAL);
		/// Creates the AsyncChannel and connects it to
		/// the given channel.

	void setChannel(Channel::Ptr pChannel);
		/// Connects the AsyncChannel to the given target channel.
		/// All messages will be forwarded to this channel.

	Channel::Ptr getChannel() const;
		/// Returns the target channel.

	void open();
		/// Opens the channel and creates the
		/// background logging thread.

	void close();
		/// Closes the channel and stops the background
		/// logging thread.

	void log(const Message& msg);
		/// Queues the message for processing by the
		/// background thread.

	void setProperty(const std::string& name, const std::string& value);
		/// Sets or changes a configuration property.
		///
		/// The "channel" property allows setting the target
		/// channel via the LoggingRegistry.
		/// The "channel" property is set-only.
		///
		/// The "priority" property allows setting the thread
		/// priority. The following values are supported:
		///    * lowest
		///    * low
		///    * normal (default)
		///    * high
		///    * highest
		///
		/// The "priority" property is set-only.
		///
		/// The "queueSize" property allows to limit the number
		/// of messages in the queue. If the queue is full and
		/// new messages are logged, these are dropped until the
		/// queue has free capacity again. The number of dropped
		/// messages is recorded, and a log message indicating the
		/// number of dropped messages will be generated when the
		/// queue has free capacity again.
		/// In addition to an unsigned integer specifying the size,
		/// this property can have the values "none" or "unlimited",
		/// which disable the queue size limit. A size of 0 also
		/// removes the limit.
		///
		/// The "queueSize" property is set-only.

protected:
	~AsyncChannel();
	void run();
	void setPriority(const std::string& value);

private:
	Channel::Ptr _pChannel;
	Thread    _thread;
	FastMutex _threadMutex;
	FastMutex _channelMutex;
	NotificationQueue _queue;
	std::size_t _queueSize = 0;
	std::size_t _dropCount = 0;
	std::atomic<bool> _closed;
};


} // namespace Poco


#endif // Foundation_AsyncChannel_INCLUDED
