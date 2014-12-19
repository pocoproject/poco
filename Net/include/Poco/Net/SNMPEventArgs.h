//
// SNMPEventArgs.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/SNMPEventArgs.h#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPEventArgs
//
// Definition of SNMPEventArgs.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SNMPEventArgs_INCLUDED
#define Net_SNMPEventArgs_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SNMPClientTypes.h"


namespace Poco {
namespace Net {


class Net_API SNMPEventArgs
	/// SNMP event arguments.
{
public:
	SNMPEventArgs(const SocketAddress& address);
		/// Creates SNMPEventArgs.

	virtual ~SNMPEventArgs();
		/// Destroys SNMPEventArgs.

	std::string hostName() const;
		/// Tries to resolve the target IP address into host name.
		/// If unsuccessful, all exceptions are silently ignored and 
		///	the IP address is returned.

	std::string hostAddress() const;
		/// Returns the target IP address.

	SNMPTypes::SNMPMessage::Ptr message() const;
		/// Returns the parsed SNMP message.

	Poco::ASN1::Ptr rawMessage() const;
		/// Returns the parsed ANS1 raw message.

	const std::string &error() const;
		/// Returns the error if any.

	bool abort() const;
		/// Abort process flag.

	void setAbort(bool value);
		/// Set to abort the current process.

private:
	SNMPEventArgs();

	void setMessage(SNMPTypes::SNMPMessage::Ptr message);
	void setRawMessage(Poco::ASN1::Ptr rawmessage);
	void setError(const std::string &error);

	SocketAddress _address;
	int _requestid;
	SNMPTypes::SNMPMessage::Ptr _message;
	Poco::ASN1::Ptr _rawmessage;
	std::string _error;
	bool _abort;

	friend class SNMPClient;
};


} } // namespace Poco::Net


#endif // Net_SNMPEventArgs_INCLUDED
