//
// SNMPClientRawTypes.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/SNMPClientRawTypes.h#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientRawTypes
//
// Definition of the SNMP ASN1 types.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SNMPClientRawTypes_INCLUDED
#define Net_SNMPClientRawTypes_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/ASN1Types.h"


using namespace Poco::ASN1Types;


namespace Poco {
namespace Net {
namespace ASN1Types {


class Net_API IPAddress : public OctetString
	/// SNMP IP address ASN1 type.
{
public:

	IPAddress();

	IPAddress(const std::string &value);

	IPAddress(ASN1Type type, const std::string &value = "");

	std::string toString() const;

	std::string typeName() const;

};


class Net_API NetworkAddress : public IPAddress
	/// SNMP network address ASN1 type.
{
public:

	NetworkAddress();

	NetworkAddress(const std::string &value);

	std::string typeName() const;

};


class Net_API Counter32 : public Integer
	/// SNMP Counter32 ASN1 type.
{
public:

	Counter32();

	Counter32(Poco::UInt32 value);

	std::string typeName() const;

};


class Net_API Gauge32 : public Integer
	/// SNMP Gauge32 ASN1 type.
{
public:

	Gauge32();

	Gauge32(Poco::UInt32 value);

	std::string typeName() const;

};

#ifdef POCO_HAVE_INT64

class Net_API Counter64 : public Integer64
	/// SNMP Counter64 ASN1 type.
{
public:

	Counter64();

	Counter64(Poco::UInt64 value);

	std::string typeName() const;

};

#endif // POCO_HAVE_INT64

class Net_API TimeTicks : public Integer
	/// SNMP TimeTicks ASN1 type.
{
public:

	TimeTicks();

	TimeTicks(Poco::UInt32 value);

	std::string typeName() const;

};


class Net_API Opaque : public Unknown
	/// SNMP Opaque ASN1 type.
{
public:

	Opaque();

	Opaque(const std::string &value);

	std::string toString() const;

	std::string typeName() const;

};


} } } // namespace Poco::Net::ASN1Types


#endif // Net_SNMPClientRawTypes_INCLUDED
