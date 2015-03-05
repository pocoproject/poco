//
// ICMPClientRaw.h
//
// $Id: //poco/1.4/NetEx/include/Poco/Net/ICMPClientRaw.h#1 $
//
// Library: NetEx
// Package: ICMP
// Module:  ICMPClientRaw
//
// Definition of the ICMPClientRaw class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetEx_ICMPClientRaw_INCLUDED
#define NetEx_ICMPClientRaw_INCLUDED


#include "Poco/Net/NetEx.h"
#include "Poco/Net/ICMPClientImpl.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace Net {


class NetEx_API ICMPClientRaw : public ICMPClientImpl
	/// This class provides ICMP Ping functionality using raw sockets.
{
public:
	explicit ICMPClientRaw(IPAddress::Family family);
		/// Creates an ICMP client.

	~ICMPClientRaw();
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

private:
	mutable IPAddress::Family _family;
};


} } // namespace Poco::Net


#endif // NetEx_ICMPClientRaw_INCLUDED
