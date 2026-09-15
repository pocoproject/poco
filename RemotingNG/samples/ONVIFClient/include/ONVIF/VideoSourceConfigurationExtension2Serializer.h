//
// VideoSourceConfigurationExtension2Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoSourceConfigurationExtension2_INCLUDED
#define TypeSerializer_ONVIF_VideoSourceConfigurationExtension2_INCLUDED


#include "ONVIF/LensDescriptionDeserializer.h"
#include "ONVIF/LensDescriptionSerializer.h"
#include "ONVIF/SceneOrientationDeserializer.h"
#include "ONVIF/SceneOrientationSerializer.h"
#include "ONVIF/VideoSourceConfigurationExtension2.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoSourceConfigurationExtension2>
{
public:
	static void serialize(const std::string& name, const ONVIF::VideoSourceConfigurationExtension2& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::VideoSourceConfigurationExtension2& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"LensDescription"s,"SceneOrientation"s,""s};
		TypeSerializer<std::vector<ONVIF::LensDescription>>::serialize(REMOTING__NAMES[0], value.getLensDescription(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SceneOrientation>>::serialize(REMOTING__NAMES[1], value.getSceneOrientation(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoSourceConfigurationExtension2_INCLUDED

