//
// SNMPClientRawTypes.cpp
//
// $Id: //poco/1.4/Net/src/SNMPClientRawTypes.cpp#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientRawTypes
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SNMPClientRawTypes.h"
#include "Poco/Net/SNMPCore.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"


namespace Poco {
namespace Net {
namespace ASN1Types {


///
/// IPAddress
///


IPAddress::IPAddress() : Poco::ASN1Types::OctetString(SNMP_ASN1::IPAddress)
{

}


IPAddress::IPAddress(const std::string &value) : Poco::ASN1Types::OctetString(SNMP_ASN1::IPAddress, value)
{

}


IPAddress::IPAddress(ASN1Type type, const std::string &value) : Poco::ASN1Types::OctetString(type, value)
{

}


std::string IPAddress::toString() const
{
	if (getValue().size() == 4)
	{
		return Poco::format("%?d.%?d.%?d.%?d", Poco::UInt8(getValue()[0]), Poco::UInt8(getValue()[1]), Poco::UInt8(getValue()[2]), Poco::UInt8(getValue()[3]));
	}
	else if (getValue().size() > 0)
	{
		return "[INVALID IP ADDRESS]";
	}
	return "";
}


std::string IPAddress::typeName() const
{
	return "IPADDRESS";
}


///
/// NetworkAddress
///


NetworkAddress::NetworkAddress() : IPAddress(SNMP_ASN1::NetworkAddress)
{

}


NetworkAddress::NetworkAddress(const std::string &value) : IPAddress(SNMP_ASN1::NetworkAddress, value)
{

}


std::string NetworkAddress::typeName() const
{
	return "NETWORKADDRESS";
}


///
/// Counter32
///


Counter32::Counter32() : Integer(SNMP_ASN1::Counter32)
{

}


Counter32::Counter32(Poco::UInt32 value) : Integer(SNMP_ASN1::Counter32, value)
{

}


std::string Counter32::typeName() const
{
	return "COUNTER32";
}


///
/// Gauge32
///


Gauge32::Gauge32() : Integer(SNMP_ASN1::Gauge32)
{

}


Gauge32::Gauge32(Poco::UInt32 value) : Integer(SNMP_ASN1::Gauge32, value)
{

}


std::string Gauge32::typeName() const
{
	return "GAUGE32";
}


#ifdef POCO_HAVE_INT64


///
/// Counter64
///


Counter64::Counter64() : Integer64(SNMP_ASN1::Counter64)
{

}


Counter64::Counter64(Poco::UInt64 value) : Integer64(SNMP_ASN1::Counter64, value)
{

}


std::string Counter64::typeName() const
{
	return "COUNTER64";
}


#endif POCO_HAVE_INT64


///
/// TimeTicks
///


TimeTicks::TimeTicks() : Integer(SNMP_ASN1::TimeTicks)
{

}


TimeTicks::TimeTicks(Poco::UInt32 value) : Integer(SNMP_ASN1::TimeTicks, value)
{

}


std::string TimeTicks::typeName() const
{
	return "TIMETICKS";
}


///
/// Opaque
///


Opaque::Opaque() : Unknown(SNMP_ASN1::Opaque)
{

}


Opaque::Opaque(const std::string &value) : Unknown(SNMP_ASN1::Opaque, value)
{

}


std::string Opaque::toString() const
{
	return Poco::format("[opaque type 0x%s size %?d]", Poco::NumberFormatter::formatHex(getType().rawValue()), getValue().size());
}


std::string Opaque::typeName() const
{
	return "OPAQUE";
}


} } } // namespace Poco::Net::ASN1Types
