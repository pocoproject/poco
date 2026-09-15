//
// NetworkInterfaceSetConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkInterfaceSetConfiguration_INCLUDED
#define TypeSerializer_ONVIF_NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/IPv4NetworkInterfaceSetConfigurationDeserializer.h"
#include "ONVIF/IPv4NetworkInterfaceSetConfigurationSerializer.h"
#include "ONVIF/IPv6NetworkInterfaceSetConfigurationDeserializer.h"
#include "ONVIF/IPv6NetworkInterfaceSetConfigurationSerializer.h"
#include "ONVIF/NetworkInterfaceConnectionSettingDeserializer.h"
#include "ONVIF/NetworkInterfaceConnectionSettingSerializer.h"
#include "ONVIF/NetworkInterfaceSetConfiguration.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtensionDeserializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkInterfaceSetConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkInterfaceSetConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkInterfaceSetConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Link"s,"MTU"s,"IPv4"s,"IPv6"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getEnabled(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceConnectionSetting>>::serialize(REMOTING__NAMES[1], value.getLink(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getMTU(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPv4NetworkInterfaceSetConfiguration>>::serialize(REMOTING__NAMES[3], value.getIPv4(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPv6NetworkInterfaceSetConfiguration>>::serialize(REMOTING__NAMES[4], value.getIPv6(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceSetConfigurationExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkInterfaceSetConfiguration_INCLUDED

