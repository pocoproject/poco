//
// OSDPosConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDPosConfiguration_INCLUDED
#define TypeSerializer_ONVIF_OSDPosConfiguration_INCLUDED


#include "ONVIF/OSDPosConfiguration.h"
#include "ONVIF/OSDPosConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDPosConfigurationExtensionSerializer.h"
#include "ONVIF/VectorDeserializer.h"
#include "ONVIF/VectorSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDPosConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::OSDPosConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::OSDPosConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"Pos"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getType(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Vector>>::serialize(REMOTING__NAMES[1], value.getPos(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDPosConfigurationExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDPosConfiguration_INCLUDED

