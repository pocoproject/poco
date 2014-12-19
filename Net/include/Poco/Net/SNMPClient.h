//
// SNMPClient.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/SNMPClient.h#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClient
//
// Definition of the SNMPClient class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SNMPClient_INCLUDED
#define Net_SNMPClient_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SNMPCore.h"
#include "Poco/Net/SNMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SNMPClientTypes.h"
#include "Poco/ASN1.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace Net {


class Net_API SNMPClient
	/// This class provides SNMP querying functionality.
{
public:
	mutable Poco::BasicEvent<SNMPEventArgs> snmpBegin;
	mutable Poco::BasicEvent<SNMPEventArgs> snmpReply;
	mutable Poco::BasicEvent<SNMPEventArgs> snmpError;
	mutable Poco::BasicEvent<SNMPEventArgs> snmpEnd;

	SNMPClient();
		/// Constructs the SNMP client.

	~SNMPClient();
		/// Destroys the SNMP client.

	void send(SocketAddress& address, SNMPTypes::SNMPMessage::Ptr message);
		/// Sends the SNMP message to the specified address.
		/// Notifications are posted for events.

	void send(const std::string& address, SNMPTypes::SNMPMessage::Ptr message);
		/// Calls SNMPClient::send(SocketAddress&, int).

	void get(SocketAddress& address, const std::string &oid, int requestId = 0, 
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv1);
		/// Sends a SNMP GET request for a single value.
		/// Notifications are posted for events.

	void get(const std::string& address, const std::string &oid, int requestId = 0,
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv1);
		/// Calls SNMPClient::get(SocketAddress&, const std::string&, int, const std::string&, SNMPVersion::Version).

	void set(SocketAddress& address, const std::string &oid, ASN1::Ptr value, int requestId = 0, 
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv1);
		/// Sends a SNMP SET request for a single value.
		/// Notifications are posted for events.

	void set(const std::string& address, const std::string &oid, ASN1::Ptr value, int requestId = 0,
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv1);
		/// Calls SNMPClient::set(SocketAddress&, const std::string&, ASN1::Ptr value, int, const std::string&, SNMPVersion::Version).

	void walk(SocketAddress& address, const std::string &oid, int requestId = 0,
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv1);
		/// Sends a SNMP GETNEXT request for a list of values, starting from the passed OID.
		/// Notifications are posted for events.

	void walk(const std::string& address, const std::string &oid, int requestId = 0,
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv1);
		/// Calls SNMPClient::walk(SocketAddress&, const std::string&, int, const std::string&, SNMPVersion::Version).

	void getBulk(SocketAddress& address, const std::string &oid, int requestId = 0,
		int nonRepeaters = 0, int maxRepetitions = 10,
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv2c);
		/// Sends a SNMP GETBULK request for a list of values, starting from the passed OID.
		/// Notifications are posted for events.

	void getBulk(const std::string& address, const std::string &oid, int requestId = 0,
		int nonRepeaters = 0, int maxRepetitions = 10,
		const std::string &community = "public",
		SNMPVersion::Version version = SNMPVersion::SNMPv2c);
		/// Calls SNMPClient::getBulk(SocketAddress&, const std::string&, int, int, int const std::string&, SNMPVersion::Version).
};


} } // namespace Poco::Net


#endif // Net_SNMPClient_INCLUDED
