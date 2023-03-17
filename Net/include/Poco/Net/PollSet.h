//
// PollSet.h
//
// Library: Net
// Package: Sockets
// Module:  PollSet
//
// Definition of the PollSet class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_PollSet_INCLUDED
#define Net_PollSet_INCLUDED


#include "Poco/Net/Socket.h"
#include <map>


namespace Poco {
namespace Net {


class PollSetImpl;


class Net_API PollSet
	/// A set of sockets that can be efficiently polled as a whole.
	///
	/// If supported, PollSet is implemented using epoll (Linux) or
	/// poll (BSD) APIs. A fallback implementation using select()
	/// is also provided.
{
public:
	enum Mode
	{
		POLL_READ  = Socket::SELECT_READ,
		POLL_WRITE = Socket::SELECT_WRITE,
		POLL_ERROR = Socket::SELECT_ERROR
	};

	using SocketModeMap = std::map<Poco::Net::Socket, int>;

	PollSet();
		/// Creates an empty PollSet.

	~PollSet();
		/// Destroys the PollSet.

	void add(const Poco::Net::Socket& socket, int mode);
		/// Adds the given socket to the set, for polling with
		/// the given mode, which can be an OR'd combination of
		/// POLL_READ, POLL_WRITE and POLL_ERROR.
		/// Subsequent socket additions to the PollSet are mode-cumulative,
		/// so the following code:
		///
		/// StreamSocket ss;
		/// PollSet ps;
		/// ps.add(ss, PollSet::POLL_READ);
		/// ps.add(ss, PollSet::POLL_WRITE);
		///
		/// shall result in the socket being monitored for read and write,
		/// equivalent to this:
		///
		/// ps.update(ss, PollSet::POLL_READ | PollSet::POLL_WRITE);

	void remove(const Poco::Net::Socket& socket);
		/// Removes the given socket from the set.

	void update(const Poco::Net::Socket& socket, int mode);
		/// Updates the mode of the given socket. If socket does
		/// not exist in the PollSet, it is silently added. For
		/// an existing socket, any prior mode is overwritten.
		/// Updating socket is non-mode-cumulative.
		///
		/// The following code:
		///
		/// StreamSocket ss;
		/// PollSet ps;
		/// ps.update(ss, PollSet::POLL_READ);
		/// ps.update(ss, PollSet::POLL_WRITE);
		///
		/// shall result in the socket being monitored for write only.

	bool has(const Socket& socket) const;
		/// Returns true if socket is registered for polling.

	bool empty() const;
		/// Returns true if no socket is registered for polling.

	void clear();
		/// Removes all sockets from the PollSet.

	SocketModeMap poll(const Poco::Timespan& timeout);
		/// Waits until the state of at least one of the PollSet's sockets
		/// changes accordingly to its mode, or the timeout expires.
		/// Returns a PollMap containing the sockets that have had
		/// their state changed.

	int count() const;
		/// Returns the number of sockets monitored.

	void wakeUp();
		/// Wakes up a waiting PollSet.
		/// Any errors that occur during this call are ignored.
		/// On platforms/implementations where this functionality
		/// is not available, it does nothing.
private:
	PollSetImpl* _pImpl;

	PollSet(const PollSet&);
	PollSet& operator = (const PollSet&);
};


} } // namespace Poco::Net


#endif // Net_PollSet_INCLUDED
