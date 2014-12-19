//
// SNMPClientRawFactory.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/SNMPClientRawFactory.h#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientRawFactory
//
// Definition of the SNMPClientRawFactory class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SNMPClientRawFactory_INCLUDED
#define Net_SNMPClientRawFactory_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/ASN1FactoryDefault.h"


namespace Poco {
namespace Net {


class Net_API SNMPClientRawFactory : public ASN1FactoryDefault
	/// This class provides an ASN.1 factory for SNMP objects.
{
public:

	ASN1::Ptr create(ASN1Type type);		
		/// Creates the SNMP or Universal ASN1 type.

};


} } // namespace Poco::Net


#endif // Net_SNMPClientRawFactory_INCLUDED
