//
// AsyncNotificationCenter.h
//
// Library: Foundation
// Package: Notifications
// Module:  AsyncNotificationCenter
//
// Definition of the AsyncNotificationCenter class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AsyncNotificationCenter_INCLUDED
#define Foundation_AsyncNotificationCenter_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "Poco/Debugger.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Format.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/NotificationQueue.h"


namespace Poco {


class Foundation_API AsyncNotificationCenter: public NotificationCenter
	/// AsyncNotificationCenter decouples posting of notifications
	/// from notifying subscribers by calling observers' notification
	/// handler in a dedicated thread.
{
public:
	AsyncNotificationCenter();
		/// Creates the AsyncNotificationCenter and starts the notifying thread.

	~AsyncNotificationCenter();
		/// Stops the notifying thread and destroys the AsyncNotificationCenter.

	AsyncNotificationCenter& operator = (const AsyncNotificationCenter&) = delete;
	AsyncNotificationCenter(const AsyncNotificationCenter&) = delete;
	AsyncNotificationCenter& operator = (AsyncNotificationCenter&&) = delete;
	AsyncNotificationCenter(AsyncNotificationCenter&&) = delete;

	virtual void postNotification(Notification::Ptr pNotification);
		/// Enqueues notification into the notification queue.

	virtual int backlog() const;
		/// Returns the numbner of notifications in the notification queue.

private:
	void start();
	void stop();
	void dequeue();

	using Adapter = RunnableAdapter<AsyncNotificationCenter>;

	Thread _thread;
	NotificationQueue _nq;
	Adapter _ra;
	std::atomic<bool> _started;
	std::atomic<bool> _done;
};


} // namespace Poco


#endif // Foundation_AsyncNotificationCenter_INCLUDED
