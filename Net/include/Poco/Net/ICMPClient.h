//
// ICMPClient.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/ICMPClient.h#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPClient
//
// Definition of the ICMPClient class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_ICMPClient_INCLUDED
#define Net_ICMPClient_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/ICMPClientImpl.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace Net {


class ICMPClientImpl;


class Net_API ICMPClient
	/// This class provides ICMP Ping functionality.
	///
	/// The events are available when class is instantiated
	///	and non-static member functions are called.
	/// 
	///	A "lightweight" alternative is direct (without instantiation)
	///	use of static member functions.
{
public:
	mutable Poco::BasicEvent<ICMPEventArgs> pingBegin;
	mutable Poco::BasicEvent<ICMPEventArgs> pingReply;
	mutable Poco::BasicEvent<ICMPEventArgs> pingError;
	mutable Poco::BasicEvent<ICMPEventArgs> pingEnd;

	explicit ICMPClient(IPAddress::Family family, bool useRawSocket = true);
		/// Creates an ICMP client.
		///
		/// If using raw socket, most operating systems requires root access.
		/// Non-raw sockets are not available in all platforms, if not available
		/// NotImplementedException is thrown.

	~ICMPClient();
		/// Destroys the ICMP client.

	int ping(SocketAddress& address, int repeat = 1) const;
		/// Pings the specified address [repeat] times.
		/// Notifications are posted for events.
		/// 
		/// Returns the number of valid replies.

	int ping(const std::string& address, int repeat = 1) const;
		/// Calls ICMPClient::ping(SocketAddress&, int) and
		/// returns the result.
		/// 
		/// Returns the number of valid replies.

	static int ping(SocketAddress& address, IPAddress::Family family, int repeat = 1, bool useRawSocket = true);
		/// Pings the specified address [repeat] times.
		/// Notifications are not posted for events.
		/// 
		/// Returns the number of valid replies.

	static int pingIPv4(const std::string& address, int repeat = 1, bool useRawSocket = true);
		/// Calls ICMPClient::ping(SocketAddress&, int) and
		/// returns the result.
		/// 
		/// Returns the number of valid replies.

private:
	void tpbegin(const void *sender, Poco::Net::ICMPEventArgs &args);
	void tpreply(const void *sender, Poco::Net::ICMPEventArgs &args);
	void tperror(const void *sender, Poco::Net::ICMPEventArgs &args);
	void tpend(const void *sender, Poco::Net::ICMPEventArgs &args);

	ICMPClientImpl *_impl;
};


} } // namespace Poco::Net


#endif // Net_ICMPClient_INCLUDED
