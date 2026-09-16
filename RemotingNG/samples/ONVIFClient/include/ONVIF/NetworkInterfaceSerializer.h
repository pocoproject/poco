//
// NetworkInterfaceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkInterface_INCLUDED
#define TypeSerializer_ONVIF_NetworkInterface_INCLUDED


#include "ONVIF/IPv4NetworkInterfaceDeserializer.h"
#include "ONVIF/IPv4NetworkInterfaceSerializer.h"
#include "ONVIF/IPv6NetworkInterfaceDeserializer.h"
#include "ONVIF/IPv6NetworkInterfaceSerializer.h"
#include "ONVIF/NetworkInterface.h"
#include "ONVIF/NetworkInterfaceExtensionDeserializer.h"
#include "ONVIF/NetworkInterfaceExtensionSerializer.h"
#include "ONVIF/NetworkInterfaceInfoDeserializer.h"
#include "ONVIF/NetworkInterfaceInfoSerializer.h"
#include "ONVIF/NetworkInterfaceLinkDeserializer.h"
#include "ONVIF/NetworkInterfaceLinkSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkInterface>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0]);
		}
		
	}

	static void serialize(const std::string& name, const ONVIF::NetworkInterface& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::NetworkInterface& value, Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::NetworkInterface& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Info"s,"Link"s,"IPv4"s,"IPv6"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getEnabled(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceInfo>>::serialize(REMOTING__NAMES[1], value.getInfo(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceLink>>::serialize(REMOTING__NAMES[2], value.getLink(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPv4NetworkInterface>>::serialize(REMOTING__NAMES[3], value.getIPv4(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPv6NetworkInterface>>::serialize(REMOTING__NAMES[4], value.getIPv6(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkInterface_INCLUDED

