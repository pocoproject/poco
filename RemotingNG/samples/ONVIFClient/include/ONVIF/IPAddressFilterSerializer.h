//
// IPAddressFilterSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IPAddressFilter_INCLUDED
#define TypeSerializer_ONVIF_IPAddressFilter_INCLUDED


#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/IPAddressFilterDeserializer.h"
#include "ONVIF/IPAddressFilterExtensionDeserializer.h"
#include "ONVIF/IPAddressFilterExtensionSerializer.h"
#include "ONVIF/IPAddressFilterSerializer.h"
#include "ONVIF/PrefixedIPv4AddressDeserializer.h"
#include "ONVIF/PrefixedIPv4AddressSerializer.h"
#include "ONVIF/PrefixedIPv6AddressDeserializer.h"
#include "ONVIF/PrefixedIPv6AddressSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IPAddressFilter>
{
public:
	static void serialize(const std::string& name, const ONVIF::IPAddressFilter& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::IPAddressFilter& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"IPv4Address"s,"IPv6Address"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::IPAddressFilter>>::serialize(REMOTING__NAMES[0], value.getType(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv4Address>>::serialize(REMOTING__NAMES[1], value.getIPv4Address(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv6Address>>::serialize(REMOTING__NAMES[2], value.getIPv6Address(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPAddressFilterExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IPAddressFilter_INCLUDED

