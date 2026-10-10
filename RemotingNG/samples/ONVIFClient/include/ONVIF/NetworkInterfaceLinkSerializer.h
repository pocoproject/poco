//
// NetworkInterfaceLinkSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkInterfaceLink_INCLUDED
#define TypeSerializer_ONVIF_NetworkInterfaceLink_INCLUDED


#include "ONVIF/NetworkInterfaceConnectionSettingDeserializer.h"
#include "ONVIF/NetworkInterfaceConnectionSettingSerializer.h"
#include "ONVIF/NetworkInterfaceLink.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkInterfaceLink>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkInterfaceLink& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkInterfaceLink& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AdminSettings"s,"OperSettings"s,"InterfaceType"s,""s};
		TypeSerializer<ONVIF::NetworkInterfaceConnectionSetting>::serialize(REMOTING__NAMES[0], value.getAdminSettings(), ser);
		TypeSerializer<ONVIF::NetworkInterfaceConnectionSetting>::serialize(REMOTING__NAMES[1], value.getOperSettings(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getInterfaceType(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkInterfaceLink_INCLUDED

