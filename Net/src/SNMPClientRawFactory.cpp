//
// SNMPClientRawFactory.cpp
//
// $Id: //poco/1.4/Net/src/SNMPClientRawFactory.cpp#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientRawFactory
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SNMPCore.h"
#include "Poco/Net/SNMPClientRawFactory.h"
#include "Poco/Net/SNMPClientRawTypes.h"


namespace Poco {
namespace Net {


ASN1::Ptr SNMPClientRawFactory::create(ASN1Type type)
{
	if (type.aclass() == ASN1Type::Application)
	{
		switch (type.rawValue())
		{
		case SNMP_ASN1::IPAddress: 
			return new ASN1Types::IPAddress;
		case SNMP_ASN1::NetworkAddress: 
			return new ASN1Types::NetworkAddress;
		case SNMP_ASN1::Counter32: 
		case SNMP_ASN1::Uinteger32: 
			return new ASN1Types::Counter32;
		case SNMP_ASN1::Gauge32: 
			return new ASN1Types::Gauge32;
		case SNMP_ASN1::Counter64: 
#ifdef POCO_HAVE_INT64
			return new ASN1Types::Counter64;
#else
			return new ASN1Types::Counter32;
#endif
		case SNMP_ASN1::TimeTicks: 
			return new ASN1Types::TimeTicks;
		case SNMP_ASN1::Opaque: 
			return new ASN1Types::Opaque;
		}
	}
	else if (type.aclass() == ASN1Type::ContextSpecific)
	{
		switch (type.rawValue())
		{
		case SNMP_ASN1::GetRequestPDU: 
		case SNMP_ASN1::GetNextRequestPDU:
		case SNMP_ASN1::GetResponsePDU:
			return new Poco::ASN1Types::Sequence(type);
		}
	}
	return ASN1FactoryDefault::create(type);
}


} } // namespace Poco::Net
