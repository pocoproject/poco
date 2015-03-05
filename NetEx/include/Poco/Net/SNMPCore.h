//
// SNMPCore.h
//
// $Id: //poco/1.4/NetEx/include/Poco/Net/SNMPCore.h#1 $
//
// Library: NetEx
// Package: SNMP
// Module:  SNMPCore
//
// SNMP core definitions.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetEx_SNMPCore_INCLUDED
#define NetEx_SNMPCore_INCLUDED


#include "Poco/Net/NetEx.h"


namespace Poco {
namespace Net {


class NetEx_API SNMPVersion
	/// SNMP version constants.
{
public:
	enum Version {
		SNMPv1				= 0,
		SNMPv2c				= 1
	};
};


class NetEx_API SNMP_ASN1
	/// SNMP ASN1 type constants.
{
public:
	enum BERType {
		IPAddress			= 0x40,
		Counter32			= 0x41,
		Gauge32				= 0x42,
		TimeTicks			= 0x43,
		Opaque				= 0x44,
		NetworkAddress		= 0x45,
		Counter64			= 0x46,
		Uinteger32			= 0x47,

		GetRequestPDU		= 0xa0,
		GetNextRequestPDU	= 0xa1,
		GetResponsePDU		= 0xa2,
		SetRequestPDU		= 0xa3,
		GetBulkRequestPDU	= 0xa5
	};
};


} } // namespace Poco::Net


#endif // NetEx_SNMPCore_INCLUDED
