//
// OSDConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDConfiguration_INCLUDED
#define TypeSerializer_ONVIF_OSDConfiguration_INCLUDED


#include "ONVIF/OSDConfiguration.h"
#include "ONVIF/OSDConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDConfigurationExtensionSerializer.h"
#include "ONVIF/OSDImgConfigurationDeserializer.h"
#include "ONVIF/OSDImgConfigurationSerializer.h"
#include "ONVIF/OSDPosConfigurationDeserializer.h"
#include "ONVIF/OSDPosConfigurationSerializer.h"
#include "ONVIF/OSDReferenceDeserializer.h"
#include "ONVIF/OSDReferenceSerializer.h"
#include "ONVIF/OSDTextConfigurationDeserializer.h"
#include "ONVIF/OSDTextConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDConfiguration>
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

	static void serialize(const std::string& name, const ONVIF::OSDConfiguration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::OSDConfiguration& value, Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::OSDConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"VideoSourceConfigurationToken"s,"Type"s,"Position"s,"TextString"s,"Image"s,"Extension"s,""s};
		TypeSerializer<ONVIF::OSDReference>::serialize(REMOTING__NAMES[0], value.getVideoSourceConfigurationToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getType(), ser);
		TypeSerializer<ONVIF::OSDPosConfiguration>::serialize(REMOTING__NAMES[2], value.getPosition(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDTextConfiguration>>::serialize(REMOTING__NAMES[3], value.getTextString(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDImgConfiguration>>::serialize(REMOTING__NAMES[4], value.getImage(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDConfigurationExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDConfiguration_INCLUDED

