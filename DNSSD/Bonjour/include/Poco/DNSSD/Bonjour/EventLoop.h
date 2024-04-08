//
// EventLoop.h
//
// $Id: //poco/1.7/DNSSD/Bonjour/include/Poco/DNSSD/Bonjour/EventLoop.h#1 $
//
// Library: DNSSD/Bonjour
// Package: Implementation
// Module:  EventLoop
//
// Definition of the EventLoop class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Bonjour_EventLoop_INCLUDED
#define DNSSD_Bonjour_EventLoop_INCLUDED


#include "Poco/DNSSD/Bonjour/Bonjour.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include "Poco/Net/Socket.h"
#include <map>
#include <set>
#include <dns_sd.h>


namespace Poco {
namespace DNSSD {
namespace Bonjour {


class DNSSD_Bonjour_API EventLoop: public Poco::Runnable
	/// The EventLoop class monitors all sockets used by Bonjour (for the connection
	/// between the client and the daemon) in a thread and drives the
	/// Bonjour machinery.
{
public:
	typedef Poco::ScopedLock<EventLoop> ScopedLock;

	enum
	{
		EVENTLOOP_TIMEOUT = 250
	};

	EventLoop();
		/// Creates the EventLoop.

	~EventLoop();
		/// Destroys the EventLoop.

	void add(DNSServiceRef sdRef);
		/// Adds the reference to the eventloop, monitoring it for data.

	void remove(DNSServiceRef sdRef);
		/// Removes and destroys the DNSServiceRef.

	void stop();
		/// Requests to stop event loop processing.

	void shutdown();
		/// Cleans up and frees all DNSServiceRefs.
		///
		/// Must be called after the event loop thread has been stopped.

	void lock();
		/// Locks the internal mutex.

	void unlock();
		/// Unlocks the internal mutex.

protected:
	void run() override;
	void removeImpl(DNSServiceRef sdRef);

private:
	using SockToRef = std::map<Poco::Net::Socket, DNSServiceRef>;
	using RefToSock = std::map<DNSServiceRef, Poco::Net::Socket>;
	using RefSet = std::set<DNSServiceRef>;

	RefToSock   _refs;
	SockToRef   _sockets;
	RefSet      _invalidatedRefs;
	bool        _stop;
	Poco::Event _refAdded;
	Poco::Mutex _mutex;
};


//
// inlines
//
inline void EventLoop::stop()
{
	_stop = true;
	_refAdded.set();
}


inline void EventLoop::lock()
{
	_mutex.lock();
}


inline void EventLoop::unlock()
{
	_mutex.unlock();
}


} } } // namespace Poco::DNSSD::Bonjour


#endif // DNSSD_Bonjour_EventLoop_INCLUDED
