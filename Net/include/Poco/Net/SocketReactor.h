//
// SocketReactor.h
//
// Library: Net
// Package: Reactor
// Module:  SocketReactor
//
// Definition of the SocketReactor class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SocketReactor_INCLUDED
#define Net_SocketReactor_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketNotifier.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Runnable.h"
#include "Poco/Timespan.h"
#include "Poco/Observer.h"
#include "Poco/AutoPtr.h"
#include "Poco/Event.h"
#include <map>
#include <atomic>


namespace Poco {


class Thread;


namespace Net {


class Socket;


class Net_API SocketReactor: public Poco::Runnable
	/// This class, which is part of the Reactor pattern,
	/// implements the "Initiation Dispatcher".
	///
	/// The Reactor pattern has been described in the book
	/// "Pattern Languages of Program Design" by Jim Coplien
	/// and Douglas C. Schmidt (Addison Wesley, 1995).
	///
	/// The Reactor design pattern handles service requests that
	/// are delivered concurrently to an application by one or more
	/// clients. Each service in an application may consist of several
	/// methods and is represented by a separate event handler. The event
	/// handler is responsible for servicing service-specific requests.
	/// The SocketReactor dispatches the event handlers.
	///
	/// Event handlers (any class can be an event handler - there
	/// is no base class for event handlers) can be registered
	/// with the addEventHandler() method and deregistered with
	/// the removeEventHandler() method.
	///
	/// An event handler is always registered for a certain socket,
	/// which is given in the call to addEventHandler(). Any method
	/// of the event handler class can be registered to handle the
	/// event - the only requirement is that the method takes
	/// a pointer to an instance of SocketNotification (or a subclass of it)
	/// as argument.
	///
	/// Once started, the SocketReactor waits for events
	/// on the registered sockets, using PollSet:poll().
	/// If an event is detected, the corresponding event handler
	/// is invoked. There are five event types (and corresponding
	/// notification classes) defined: ReadableNotification, WritableNotification,
	/// ErrorNotification, TimeoutNotification and ShutdownNotification.
	///
	/// The ReadableNotification will be dispatched if a socket becomes
	/// readable. The WritableNotification will be dispatched if a socket
	/// becomes writable. The ErrorNotification will be dispatched if
	/// there is an error condition on a socket.
	///
	/// Timeout/sleep strategy operates as follows:
	///
	/// If the poll timeout expires and no event has occurred, a
	/// TimeoutNotification will be dispatched to all event handlers
	/// registered for it. This is done in the onTimeout() method
	/// which can be overridden by subclasses to perform custom
	/// timeout processing.
	///
	/// By default, the SocketReactor is configured to start sleeping
	/// when the poll timeout is zero and there are no socket events for
	/// a certain amount of time; sleep duration is progressive, up to
	/// the configured limit. This behavior can be disabled through
	/// configuration parameters.
	///
	/// When there are no registered handlers, the SocketRactor sleeps
	/// an incremental amount of milliseconds, up to the sleep limit.
	/// Increment step value and sleep limit are configurable.
	///
	/// Finally, when the SocketReactor is about to shut down (as a result
	/// of stop() being called), it dispatches a ShutdownNotification
	/// to all event handlers. This is done in the onShutdown() method
	/// which can be overridden by subclasses to perform custom
	/// shutdown processing.
	///
	/// The SocketReactor is implemented so that it can run in its own thread.
	/// Moreover, the thread affinity to a CPU core can optionally be set for the
	/// thread on platforms where that functionality is supported and implemented.
	/// It is also possible to run multiple SocketReactors in parallel, as long
	/// as they work on different sockets.
	///
	/// It is safe to call addEventHandler() and removeEventHandler() from another
	/// thread while the SocketReactor is running. Also, it is safe to call
	/// addEventHandler() and removeEventHandler() from event handlers.
	///
	/// SocketReactor uses NotificationCenter to notify observers. When a handler
	/// throws an exception, the NotificationCenter stops notifying the rest of
	/// the observers about that particular event instance and propagates the
	/// exception, which is eventually caught in the SocketReactor::run() method.
	/// This sequence of events is obviously not desirable and it is highly
	/// recommended that handlers wrap the code in try/catch and deal with all
	/// the exceptions internally, lest they disrupt the notification of the peers.
{
public:
	struct Params
		/// Reactor parameters.
		/// Default values should work well for most scenarios.
		///
		/// Note: the default behavior on zero poll timeout is to start
		/// incrementally sleeping after `idleThreshold` and no socket events.
		/// This prevents high CPU usage during periods without network
		/// activity. To disable it, set `throttle` to false.
	{
		Poco::Timespan pollTimeout = DEFAULT_TIMEOUT;
			/// Timeout for PolllSet::poll()

		long sleep = 0;
			/// Amount of milliseconds to sleep, progressively incremented,
			/// at `increment` step, up to the `sleepLimit`.

		long sleepLimit = DEFAULT_SLEEP_LIMIT;
			/// Max sleep duration in milliseconds
			/// This is the ceiling value in milliseconds for the sleep algorithm,
			/// which kicks in in two cases:
			///
			///   - when there are no subscribers and the reactor is just idle-spinning
			///   - when there are subscribers, but there was no socket events signalled
			///     for `sleepLimit` milliseconds and `throttle` is true

		int increment = 1;
			/// Increment value for the sleep backoff algorithm.

		long idleThreshold = DEFAULT_SLEEP_LIMIT;
			/// Indicates when to start sleeping (throttling) on zero poll timeout

		bool throttle = true;
			/// Indicates whether to start sleeping when poll timeout is zero and
			/// there's no socket events for a period longer than `idleThreshold`
	};

	SocketReactor();
		/// Creates the SocketReactor.

	explicit SocketReactor(const Poco::Timespan& pollTimeout, int threadAffinity = -1);
		/// Creates the SocketReactor, using the given poll timeout.
		///
		/// The threadAffinity argument, when non-negative, indicates on which CPU core should
		/// the run() method run. Nonexisting core or situation when this feature is not implemented
		/// are silently ignored and this argument has no effect in such scenarios.

	SocketReactor(const Params& params, int threadAffinity = -1);
		/// Creates the SocketReactor, using the given parameters.
		/// The threadAffinity argument, when non-negative, indicates on which CPU core should
		/// the run() method run. Nonexisting core or situation when this feature is not implemented
		/// are silently ignored and this argument has no effect in such scenarios.

	virtual ~SocketReactor();
		/// Destroys the SocketReactor.

	virtual void run();
		/// Runs the SocketReactor. The reactor will run
		/// until stop() is called (in a separate thread).
		/// Can be overriden by inheriting classes.

	void start();
		/// Starts the SocketReactor.

	void stop();
		/// Stops the SocketReactor.
		///
		/// The reactor will be stopped when the next event
		/// (including a timeout event) occurs.

	void wakeUp();
		/// Wakes up idle reactor.

	void setTimeout(const Poco::Timespan& timeout);
		/// Sets the timeout.
		///
		/// If no socket event occurs for the given timeout
		/// interval, a timeout event is sent to all event listeners.
		///
		/// The default timeout is 250 milliseconds;
		///
		/// The timeout is passed to the PollSet::poll() method.

	const Poco::Timespan& getTimeout() const;
		/// Returns the timeout.

	void addEventHandler(const Socket& socket, const Poco::AbstractObserver& observer);
		/// Registers an event handler with the SocketReactor.
		///
		/// Usage:
		///     Poco::Observer<MyEventHandler, SocketNotification> obs(*this, &MyEventHandler::handleMyEvent);
		///     reactor.addEventHandler(obs);

	bool hasEventHandler(const Socket& socket, const Poco::AbstractObserver& observer);
		/// Returns true if the observer is registered with SocketReactor for the given socket.

	void removeEventHandler(const Socket& socket, const Poco::AbstractObserver& observer);
		/// Unregisters an event handler with the SocketReactor.
		///
		/// Usage:
		///     Poco::Observer<MyEventHandler, SocketNotification> obs(*this, &MyEventHandler::handleMyEvent);
		///     reactor.removeEventHandler(obs);

	bool has(const Socket& socket) const;
		/// Returns true if socket is registered with this rector.

protected:
	using NotifierPtr = Poco::AutoPtr<SocketNotifier>;
	using NotificationPtr = Poco::AutoPtr<SocketNotification>;
	using EventHandlerMap = std::map<poco_socket_t, NotifierPtr>;
	using MutexType = Poco::FastMutex;
	using ScopedLock = MutexType::ScopedLock;

	virtual void onTimeout();
		/// Called if the timeout expires and no other events are available.
		///
		/// Can be overridden by subclasses. The default implementation
		/// dispatches the TimeoutNotification and thus should be called by overriding
		/// implementations.

	virtual void onShutdown();
		/// Called when the SocketReactor is about to terminate.
		///
		/// Can be overridden by subclasses. The default implementation
		/// dispatches the ShutdownNotification and thus should be called by overriding
		/// implementations.

	void onError(const Socket& socket, int code, const std::string& description);
		/// Notifies all subscribers when the reactor loop throws an exception.

	void onError(int code, const std::string& description);
		/// Notifies all subscribers when the reactor loop throws an exception.

	void dispatch(const Socket& socket, SocketNotification* pNotification);
		/// Dispatches the given notification to all observers
		/// registered for the given socket.

	void dispatch(SocketNotification* pNotification);
		/// Dispatches the given notification to all observers.

	bool hasSocketHandlers();

	const Params& getParams() const;
	int getThreadAffinity() const;
	const std::atomic<bool>& mustStop() const;
	const EventHandlerMap& getHandlers() const;
	const PollSet& getPollSet() const;
	Notification* getReadableNotification();
	Notification* getWritableNotification();
	Notification* getErrorNotification();
	Notification* getTimeoutNotification();
	Notification* getShutdownNotification();

private:

	void dispatch(NotifierPtr& pNotifier, SocketNotification* pNotification);
	NotifierPtr getNotifier(const Socket& socket, bool makeNew = false);

	void sleep();

	enum
	{
		DEFAULT_TIMEOUT = 250000,
			/// Default timeout for PollSet::poll()
		DEFAULT_SLEEP_LIMIT = 250
			/// Default limit for event-based sleeping
	};

	Params            _params;
	int               _threadAffinity = -1;
	std::atomic<bool> _stop;
	EventHandlerMap   _handlers;
	PollSet           _pollSet;
	NotificationPtr   _pReadableNotification;
	NotificationPtr   _pWritableNotification;
	NotificationPtr   _pErrorNotification;
	NotificationPtr   _pTimeoutNotification;
	NotificationPtr   _pShutdownNotification;
	MutexType         _mutex;
	Poco::Event       _event;

	friend class SocketNotifier;
};

//
// inlines
//


inline void SocketReactor::setTimeout(const Poco::Timespan& timeout)
{
	_params.pollTimeout = timeout;
}


inline const Poco::Timespan& SocketReactor::getTimeout() const
{
	return _params.pollTimeout;
}


inline bool SocketReactor::has(const Socket& socket) const
{
	return _pollSet.has(socket);
}


inline void SocketReactor::onError(const Socket& socket, int code, const std::string& description)
{
	dispatch(new ErrorNotification(this, socket, code, description));
}


inline void SocketReactor::onError(int code, const std::string& description)
{
	dispatch(new ErrorNotification(this, code, description));
}


inline void SocketReactor::dispatch(NotifierPtr& pNotifier, SocketNotification* pNotification)
{
	pNotifier->dispatch(pNotification);
}


inline const SocketReactor::Params& SocketReactor::getParams() const
{
	return _params;
}


inline int SocketReactor::getThreadAffinity() const
{
	return _threadAffinity;
}


inline const std::atomic<bool>& SocketReactor::mustStop() const
{
	return _stop;
}


inline const SocketReactor::EventHandlerMap& SocketReactor::getHandlers() const
{
	return _handlers;
}


inline const PollSet& SocketReactor::getPollSet() const
{
	return _pollSet;
}


inline Notification* SocketReactor::getReadableNotification()
{
	return _pReadableNotification;
}


inline Notification* SocketReactor::getWritableNotification()
{
	return _pWritableNotification;
}


inline Notification* SocketReactor::getErrorNotification()
{
	return _pErrorNotification;
}


inline Notification* SocketReactor::getTimeoutNotification()
{
	return _pTimeoutNotification;
}


inline Notification* SocketReactor::getShutdownNotification()
{
	return _pShutdownNotification;
}


} } // namespace Poco::Net


#endif // Net_SocketReactor_INCLUDED
