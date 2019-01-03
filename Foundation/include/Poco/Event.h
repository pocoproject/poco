//
// Event.h
//
// Library: Foundation
// Package: Threading
// Module:  Event
//
// Definition of the Event class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Event_INCLUDED
#define Foundation_Event_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <condition_variable>
#include <mutex>
#include <atomic>


namespace Poco {


class Foundation_API Event
	/// An Event is a synchronization object that
	/// allows one thread to signal one or more
	/// other threads that a certain event
	/// has happened.
	/// Usually, one thread signals an event,
	/// while one or more other threads wait
	/// for an event to become signalled.
{
public:
	enum EventType
	{
		EVENT_MANUALRESET, /// Manual reset event
		EVENT_AUTORESET    /// Auto-reset event
	};

	explicit Event(EventType type = EVENT_AUTORESET);
		/// Creates the event. If type is EVENT_AUTORESET,
		/// the event is automatically reset after
		/// a wait() successfully returns.

	//@ deprecated
	explicit Event(bool autoReset);
		/// Please use Event::Event(EventType) instead.

	~Event();
		/// Destroys the event.

	void set();
		/// Signals the event. If autoReset is true,
		/// only one thread waiting for the event
		/// can resume execution.
		/// If autoReset is false, all waiting threads
		/// can resume execution.

	void wait();
		/// Waits for the event to become signalled.

	void wait(long milliseconds);
		/// Waits for the event to become signalled.
		/// Throws a TimeoutException if the event
		/// does not become signalled within the specified
		/// time interval.

	bool tryWait(long milliseconds);
		/// Waits for the event to become signalled.
		/// Returns true if the event
		/// became signalled within the specified
		/// time interval, false otherwise.

	void reset();
		/// Resets the event to unsignalled state.

private:
	Event(const Event&);
	Event& operator = (const Event&);

	bool waitImpl(long milliseconds);

	std::atomic<bool>       _state;
	bool                    _autoreset;
	mutable std::mutex      _mutex;
	std::condition_variable _cond;
};


//
// inlines
//


inline void Event::wait(long milliseconds)
{
	if (!waitImpl(milliseconds)) throw TimeoutException();
}


inline bool Event::tryWait(long milliseconds)
{
	return waitImpl(milliseconds);
}


} // namespace Poco


#endif // Foundation_Event_INCLUDED
