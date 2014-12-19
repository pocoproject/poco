//
// SNMPRawEventArgs.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/SNMPRawEventArgs.h#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPRawEventArgs
//
// Definition of SNMPRawEventArgs.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SNMPRawEventArgs_INCLUDED
#define Net_SNMPRawEventArgs_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/ASN1.h"


namespace Poco {
namespace Net {


class Net_API SNMPRawEventArgs
	/// Argument to the SNMP client raw events.
{
public:
	SNMPRawEventArgs(const SocketAddress& address);
		/// Creates SNMPRawEventArgs.

	virtual ~SNMPRawEventArgs();
		/// Destroys SNMPRawEventArgs.

	std::string hostName() const;
		/// Tries to resolve the target IP address into host name.
		/// If unsuccessful, all exceptions are silently ignored and 
		///	the IP address is returned.

	std::string hostAddress() const;
		/// Returns the target IP address.

	ASN1::Ptr response() const;
		/// Returns the parsed ASN1 response.

	const std::string &error() const;
		/// Returns error if any.

private:
	SNMPRawEventArgs();

	void setResponse(ASN1::Ptr response);
	void setError(const std::string &error);

	SocketAddress _address;
	int _requestid;
	ASN1::Ptr _response;
	std::string _error;

	friend class SNMPClientRaw;
};


} } // namespace Poco::Net


#endif // Net_SNMPRawEventArgs_INCLUDED
