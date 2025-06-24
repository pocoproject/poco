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
#include "Poco/RunnableAdapter.h"
#include "Poco/NotificationQueue.h"

#if (POCO_HAVE_CPP20_COMPILER)
	#if !(POCO_HAVE_JTHREAD)
		#pragma message ("NOTE: std::jthread is expected but is not available. Please check your compiler version and settings.")
	#endif
#endif

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <list>
#include <map>
#include <optional>

namespace Poco {


class Foundation_API AsyncNotificationCenter: public NotificationCenter
	/// AsyncNotificationCenter decouples posting of notifications
	/// from notifying subscribers by calling observers' notification
	/// handler in a dedicated thread.
	///
	/// It supports multiple modes of operation:
	///
	/// - ENQUEUE: Notifications are added to a queue, separate single thread
	///            asynchronously dispatches them to observers sequentially
	///
	/// - NOTIFY: Notifications are added to a list for each observer, multiple
	///           worker threads process notifications in parallel
	///
	/// - BOTH: Combination of both modes, notifications are enqueued and worker
	///         threads dispatch them to observers in parallel.
	///
	/// NOTIFY and BOTH mode:
	///
	/// These modes are only available if the compiler supports C++20 std::jthread.
	///
	/// Notifications can be delivered to observers in a different order than they
	/// were posted, as they are processed by multiple worker threads. Observer
	/// handlers must also be thread-safe as multiple notifications can be dispatched
	/// to the same observer in parallel.
	///
	/// Note about using AsyncObserver
	///
	/// Although it is possible to use them with AsyncNotificationCenter,
	/// it is more efficient to use NObserver.
{
public:

	enum class AsyncMode { ENQUEUE,	NOTIFY, BOTH };
		/// ENQUEUE: Notifications are enqueued in a separate thread.
		/// NOTIFY: Notifications are dispatched to observers from worker threads
		/// BOTH: Notifications are enqueued and dispatched to observers in worker threads.

#if (POCO_HAVE_JTHREAD)

	AsyncNotificationCenter(AsyncMode mode = AsyncMode::ENQUEUE, std::size_t workersCount = AsyncNotificationCenter::DEFAULT_WORKERS_COUNT);
		/// Creates the AsyncNotificationCenter and starts the notifying thread and workers.
#else

	AsyncNotificationCenter();
		/// Creates the AsyncNotificationCenter and starts the notifying thread.

#endif

	~AsyncNotificationCenter() override;
		/// Stops the notifying thread and destroys the AsyncNotificationCenter.

	AsyncNotificationCenter& operator = (const AsyncNotificationCenter&) = delete;
	AsyncNotificationCenter(const AsyncNotificationCenter&) = delete;
	AsyncNotificationCenter& operator = (AsyncNotificationCenter&&) = delete;
	AsyncNotificationCenter(AsyncNotificationCenter&&) = delete;

	void postNotification(Notification::Ptr pNotification) override;
		/// Enqueues notification into the notification queue.

	int backlog() const override;
		/// Returns the number of notifications in the notification queue.

	std::vector<NotificationResult> synchronousDispatch(Notification::Ptr pNotification);
		/// Dispatches the notification synchronously to all observers that have a function
		/// for synchronous notification processing and accept the notification.
		/// This method blocks until the notification is processed by
		/// all observers. Returns results from all observers that accepted the notification.

protected:

	void notifyObservers(Notification::Ptr& pNotification) override;

private:
	void start();
	void stop();
	void dequeue();

	using Adapter = RunnableAdapter<AsyncNotificationCenter>;

	const AsyncMode _mode { AsyncMode::ENQUEUE };

	// Async enqueue for notifications
	Thread _enqueueThread;
	NotificationQueue _nq;
	Adapter _ra;
	std::atomic<bool> _enqueueThreadStarted;
	std::atomic<bool> _enqueueThreadDone;

#if (POCO_HAVE_JTHREAD)
	// Async notification dispatching

	using NotificationList = std::list<Notification::Ptr>;
	using ObserversMap = std::map<AbstractObserverPtr, NotificationList>;
	using NotificationTuple = std::tuple<AbstractObserverPtr, Notification::Ptr>;

	std::optional<NotificationTuple> nextNotification();

	void dispatchNotifications(std::stop_token& stopToken, int workerId);
		/// Dispatching function executed by each worker thread.

	constexpr static std::size_t DEFAULT_WORKERS_COUNT { 5 };
		/// Default number of worker threads to process notifications.
		/// This can be configured to a different value if needed.

	const std::size_t _workersCount { DEFAULT_WORKERS_COUNT };
		/// Number of worker threads to process notifications.
		/// This can be configured to a different value if needed.

	std::vector<std::jthread> _workers;
		/// Workers pop notifications from the lists and call the observers' notification handlers.
		/// If an observer is not registered anymore (hasObserver), it is removed from the map.
		/// Workers pick observers in a round robin fashion.

	ObserversMap _lists;
		/// Each observer has its own list of pending notifications.
		/// Observers are identifed by their pointers.
		/// When adding to the queue, observersToNotify is used to get the observers
		/// that are registered for such notification.

	ObserversMap::iterator _workerIterator;
		/// Iterator to the current observer list being processed by the worker threads.
		/// It is used to ensure that workers process observers in a round robin fashion.

	std::mutex _listsMutex;
		/// Mutex to protect access to the lists of notifications.
		/// It is used to ensure that workers can safely access the lists.
	bool _listsEmpty { true };
	std::condition_variable	_listsEmptyCondition;
		// Condition variable to notify workers when new notifications are added to lists.

#endif
};


} // namespace Poco


#endif // Foundation_AsyncNotificationCenter_INCLUDED
