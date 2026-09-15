//
// OSDImgConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDImgConfiguration_INCLUDED
#define TypeSerializer_ONVIF_OSDImgConfiguration_INCLUDED


#include "ONVIF/OSDImgConfiguration.h"
#include "ONVIF/OSDImgConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDImgConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDImgConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::OSDImgConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::OSDImgConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImgPath"s,"Extension"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getImgPath(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDImgConfigurationExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDImgConfiguration_INCLUDED

