//
// SNMPClientRaw.h
//
// $Id: //poco/1.4/NetEx/include/Poco/Net/SNMPClientRaw.h#1 $
//
// Library: NetEx
// Package: SNMP
// Module:  SNMPClientRaw
//
// Definition of the SNMPClientRaw class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetEx_SNMPClientRaw_INCLUDED
#define NetEx_SNMPClientRaw_INCLUDED


#include "Poco/Net/NetEx.h"
#include "Poco/Net/SNMPRawEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/ASN/ASN1.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace Net {


class NetEx_API SNMPClientRaw
	/// This class provides SNMP querying functionality.
	///
	/// This version returns the raw ASN1 types received.
	///	For a more friendly result version, use the SNMPClient class.
{
public:
	mutable Poco::BasicEvent<SNMPRawEventArgs> snmpReply;
	mutable Poco::BasicEvent<SNMPRawEventArgs> snmpError;

	SNMPClientRaw();

	explicit SNMPClientRaw(const SocketAddress& listenAddr);
		/// Creates an SNMP raw client.

	~SNMPClientRaw();
		/// Destroys the SNMP raw client.

	ASN::ASN1::Ptr query(SocketAddress& address, ASN::ASN1::Ptr query);
		/// Queries the specified address for a single result.
		/// Notifications are posted for events.
		/// 
		/// Returns the number parsed ASN1 object.

	ASN::ASN1::Ptr query(const std::string& address, ASN::ASN1::Ptr query);
		/// Calls SNMPClientRaw::query(SocketAddress&, int) and
		/// returns the result.

	void send(SocketAddress& address, ASN::ASN1::Ptr query);
		/// Queries the specified address.
		/// Notifications are posted for events.

	void send(const std::string& address, ASN::ASN1::Ptr query);
		/// Queries the specified address.
		/// Notifications are posted for events.

private:
	Poco::Net::DatagramSocket _socket;
};


} } // namespace Poco::Net


#endif // NetEx_ICMPClient_INCLUDED
