//
// EchoServer.h
//
// Definition of the EchoServer class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef EchoServer_INCLUDED
#define EchoServer_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include <atomic>


class EchoServer: public Poco::Runnable
	/// A simple sequential echo server.
{
public:
	EchoServer();
		/// Creates the EchoServer.

	EchoServer(const Poco::Net::SocketAddress& address);
		/// Creates the EchoServer using the given address.

	EchoServer(const Poco::Net::ServerSocket& sock);
		/// Creates the EchoServer using the already created socket

	~EchoServer();
		/// Destroys the EchoServer.

	Poco::UInt16 port() const;
		/// Returns the port the echo server is
		/// listening on.

	void run();
		/// Does the work.

	void stop();
		/// Sets the stop flag.

	bool done();
		/// Retruns true if if server is done.

private:
	Poco::Net::ServerSocket _socket;
	Poco::Thread _thread;
	Poco::Event  _ready;
	std::atomic<bool> _stop;
	std::atomic<bool> _done;
};


#endif // EchoServer_INCLUDED
