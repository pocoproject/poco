//
// PTZConfigurationExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZConfigurationExtension_INCLUDED
#define TypeSerializer_ONVIF_PTZConfigurationExtension_INCLUDED


#include "ONVIF/PTControlDirectionDeserializer.h"
#include "ONVIF/PTControlDirectionSerializer.h"
#include "ONVIF/PTZConfigurationExtension.h"
#include "ONVIF/PTZConfigurationExtension2Deserializer.h"
#include "ONVIF/PTZConfigurationExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZConfigurationExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZConfigurationExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZConfigurationExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTControlDirection"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::PTControlDirection>>::serialize(REMOTING__NAMES[0], value.getPTControlDirection(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZConfigurationExtension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZConfigurationExtension_INCLUDED

