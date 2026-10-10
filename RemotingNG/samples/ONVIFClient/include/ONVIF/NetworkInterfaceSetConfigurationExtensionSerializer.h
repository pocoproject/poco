//
// NetworkInterfaceSetConfigurationExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED
#define TypeSerializer_ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED


#include "ONVIF/Dot11ConfigurationDeserializer.h"
#include "ONVIF/Dot11ConfigurationSerializer.h"
#include "ONVIF/Dot3ConfigurationDeserializer.h"
#include "ONVIF/Dot3ConfigurationSerializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension2Deserializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkInterfaceSetConfigurationExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkInterfaceSetConfigurationExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkInterfaceSetConfigurationExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot3"s,"Dot11"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::Dot3Configuration>>::serialize(REMOTING__NAMES[0], value.getDot3(), ser);
		TypeSerializer<std::vector<ONVIF::Dot11Configuration>>::serialize(REMOTING__NAMES[1], value.getDot11(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceSetConfigurationExtension2>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED

