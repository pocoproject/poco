//
// IPv4NetworkInterfaceSetConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED
#define TypeSerializer_ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/IPv4NetworkInterfaceSetConfiguration.h"
#include "ONVIF/PrefixedIPv4AddressDeserializer.h"
#include "ONVIF/PrefixedIPv4AddressSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IPv4NetworkInterfaceSetConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::IPv4NetworkInterfaceSetConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IPv4NetworkInterfaceSetConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Manual"s,"DHCP"s,""s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getEnabled(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv4Address>>::serialize(REMOTING__NAMES[1], value.getManual(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getDHCP(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED

