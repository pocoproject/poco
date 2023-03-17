//
// MultiSocketPoller.h
//
// Library: Net
// Package: UDP
// Module:  MultiSocketPoller
//
// Definition of the MultiSocketPoller class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_MultiSocketPoller_INCLUDED
#define Net_MultiSocketPoller_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/UDPHandler.h"


namespace Poco {
namespace Net {


template <std::size_t S = POCO_UDP_BUF_SIZE>
class MultiSocketPoller
	/// MultiSocketPoller, as its name indicates, repeatedly polls a set of
	/// sockets for readability and/or error. If socket is readable or in error
	/// state, the reading/error handling actions are delegated to the reader.
{
public:
	MultiSocketPoller(typename UDPHandlerImpl<S>::List& handlers, const Poco::Net::SocketAddress& sa, int nSockets = 10, Poco::Timespan timeout = 250000):
		_address(sa),
		_timeout(timeout),
		_reader(handlers)
		/// Creates the MutiSocketPoller.
	{
		poco_assert (_address.port() > 0 && _address.host().toString() != "0.0.0.0");
		addSockets(nSockets);
	}

	MultiSocketPoller(typename UDPHandlerImpl<S>::List& handlers, const UDPServerParams& serverParams):
		_address(serverParams.address()),
		_timeout(serverParams.timeout()),
		_reader(handlers)
		/// Creates the MutiSocketPoller.
	{
		poco_assert (_address.port() > 0 && _address.host().toString() != "0.0.0.0");
		addSockets(serverParams.numberOfSockets());
	}

	~MultiSocketPoller()
		/// Destroys MutiSocketPoller
	{
	}

	Poco::UInt16 port() const
		/// Returns the port the socket is
		/// listening on.
	{
		return _address.port();
	}

	Poco::Net::SocketAddress address() const
		/// Returns the address of the server.
	{
		return _address;
	}

	void poll()
	{
		if (_reader.handlerStopped()) return;
		PollSet::SocketModeMap sm;
		PollSet::SocketModeMap::iterator it;
		PollSet::SocketModeMap::iterator end;
		sm = _pollSet.poll(_timeout);
		it = sm.begin();
		end = sm.end();
		for (; it != end; ++it)
		{
			if (it->second & PollSet::POLL_READ)
			{
				DatagramSocket ds(it->first);
				_reader.read(ds);
			}
			else if (it->second & PollSet::POLL_ERROR)
			{
				_reader.setError(it->first.impl()->sockfd());
			}
		}
	}

	void stop()
	{
		_reader.stopHandler();
	}

	bool done() const
	{
		return _reader.handlerDone();
	}

private:
	void addSockets(int nSockets)
	{
		for (int i = 0; i < nSockets; ++i)
		{
			DatagramSocket ds; ds.bind(_address, true, true);
			_pollSet.add(ds, PollSet::POLL_READ | PollSet::POLL_ERROR);
		}
	}

	PollSet            _pollSet;
	SocketAddress      _address;
	Poco::Timespan     _timeout;
	UDPSocketReader<S> _reader;
};


} } // namespace Poco::Net


#endif // Net_MultiSocketPoller_INCLUDED
