//
// SNMPClientRawFactory.h
//
// $Id: //poco/1.4/NetEx/include/Poco/Net/SNMPClientRawFactory.h#1 $
//
// Library: NetEx
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


#ifndef NetEx_SNMPClientRawFactory_INCLUDED
#define NetEx_SNMPClientRawFactory_INCLUDED


#include "Poco/Net/NetEx.h"
#include "Poco/ASN/ASN1FactoryDefault.h"


namespace Poco {
namespace Net {


class NetEx_API SNMPClientRawFactory : public ASN::ASN1FactoryDefault
	/// This class provides an ASN.1 factory for SNMP objects.
{
public:

	ASN::ASN1::Ptr create(ASN::ASN1Type type);		
		/// Creates the SNMP or Universal ASN1 type.

};


} } // namespace Poco::Net


#endif // NetEx_SNMPClientRawFactory_INCLUDED
