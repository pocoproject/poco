//
// UDPServer.h
//
// Library: Net
// Package: UDP
// Module:  UDPServer
//
// Definition of the UDPServer class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPServer_INCLUDED
#define Net_UDPServer_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Net/UDPHandler.h"
#include "Poco/Net/UDPServerParams.h"
#include "Poco/Net/UDPSocketReader.h"
#include "Poco/Net/SingleSocketPoller.h"
#include "Poco/Net/MultiSocketPoller.h"
#include <map>


namespace Poco {
namespace Net {


template <std::size_t S = POCO_UDP_BUF_SIZE,
		typename P = SingleSocketPoller<S>>
class UDPServerImpl: public Poco::Runnable
	/// UDP server, runs in its own thread and owns a poller, to which
	/// data arrival and discovery is delegated. See SingleSocketPoller and
	/// MultipleSocketPoller for more information.
{
public:
	UDPServerImpl(typename UDPHandlerImpl<S>::List& handlers, const Poco::Net::SocketAddress& sa):
		_poller(handlers, sa),
		_thread("UDPServer"),
		_stop(false)
		/// Creates the UDPServer and binds it to
		/// the given address.
	{
		_thread.start(*this);
	}

	UDPServerImpl(typename UDPHandlerImpl<S>::List& handlers, const UDPServerParams& params):
		_poller(handlers, params),
		_thread("UDPServer"),
		_stop(false)
		/// Creates the UDPServer and binds it to
		/// the given address.
	{
		_thread.start(*this);
	}

	~UDPServerImpl()
		/// Destroys the UDPServer.
	{
		_stop = true;
		_poller.stop();
		while (!_poller.done()) Thread::sleep(10);
		_thread.join();
	}

	Poco::UInt16 port() const
		/// Returns the port the server is
		/// listening on.
	{
		return _poller.port();
	}

	Poco::Net::SocketAddress address() const
		/// Returns the address of the server.
	{
		return _poller.address();
	}

	void run()
		/// Does the work.
	{
		while (!_stop) _poller.poll();
	}

private:
	P            _poller;
	Poco::Thread _thread;
	bool         _stop;
};


using UDPServer = UDPServerImpl<POCO_UDP_BUF_SIZE, SingleSocketPoller<POCO_UDP_BUF_SIZE>>;
using UDPMultiServer = UDPServerImpl<POCO_UDP_BUF_SIZE, MultiSocketPoller<POCO_UDP_BUF_SIZE>>;


} } // namespace Poco::Net


#endif // Net_UDPServer_INCLUDED
