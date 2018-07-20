//
// NTPClient.h
//
// Library: Net
// Package: NTP
// Module:  NTPClient
//
// Definition of the NTPClient class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NTPServer_INCLUDED
#define NTPServer_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Thread.h"

namespace Poco {
namespace Net {

class Net_API NTPServer: public Poco::Runnable
	/// A simple sequential UDP echo server.
{
public:
	NTPServer();
		/// Creates the NTPServer.

	NTPServer(const Poco::Net::SocketAddress& sa);
		/// Creates the NTPServer and binds it to
		/// the given address.

	~NTPServer();
		/// Destroys the NTPServer.
		
	void run();
		/// Does the work.

	void stop();

private:
	Poco::Net::DatagramSocket _socket;
	Poco::Thread _thread;
	bool         _stop;
};

} } // namespace Poco::Net

#endif // NTPServer_INCLUDED
