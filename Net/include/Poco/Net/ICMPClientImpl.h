//
// ICMPClientImpl.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/ICMPClientImpl.h#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPClientImpl
//
// Definition of the ICMPClientImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_ICMPClientImpl_INCLUDED
#define Net_ICMPClientImpl_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace Net {


class Net_API ICMPClientImpl
{
public:
	mutable Poco::BasicEvent<ICMPEventArgs> pingBegin;
	mutable Poco::BasicEvent<ICMPEventArgs> pingReply;
	mutable Poco::BasicEvent<ICMPEventArgs> pingError;
	mutable Poco::BasicEvent<ICMPEventArgs> pingEnd;

	virtual ~ICMPClientImpl() {}
		/// Destroys the ICMP client.

	virtual int ping(SocketAddress& address, int repeat = 1) const = 0;
		/// Pings the specified address [repeat] times.
		/// Notifications are posted for events.
		/// 
		/// Returns the number of valid replies.

	virtual int ping(const std::string& address, int repeat = 1) const = 0;
		/// Calls ICMPClient::ping(SocketAddress&, int) and
		/// returns the result.
		/// 
		/// Returns the number of valid replies.
};


} } // namespace Poco::Net


#endif // Net_ICMPClientImpl_INCLUDED
