//
// NetworkInterfaceExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkInterfaceExtension_INCLUDED
#define TypeSerializer_ONVIF_NetworkInterfaceExtension_INCLUDED


#include "ONVIF/Dot11ConfigurationDeserializer.h"
#include "ONVIF/Dot11ConfigurationSerializer.h"
#include "ONVIF/Dot3ConfigurationDeserializer.h"
#include "ONVIF/Dot3ConfigurationSerializer.h"
#include "ONVIF/NetworkInterfaceExtension.h"
#include "ONVIF/NetworkInterfaceExtension2Deserializer.h"
#include "ONVIF/NetworkInterfaceExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkInterfaceExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkInterfaceExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkInterfaceExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InterfaceType"s,"Dot3"s,"Dot11"s,"Extension"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getInterfaceType(), ser);
		TypeSerializer<std::vector<ONVIF::Dot3Configuration>>::serialize(REMOTING__NAMES[1], value.getDot3(), ser);
		TypeSerializer<std::vector<ONVIF::Dot11Configuration>>::serialize(REMOTING__NAMES[2], value.getDot11(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkInterfaceExtension2>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkInterfaceExtension_INCLUDED

