//
// VideoSourceConfigurationOptionsExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED
#define TypeSerializer_ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED


#include "ONVIF/RotateOptionsDeserializer.h"
#include "ONVIF/RotateOptionsSerializer.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension2Deserializer.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoSourceConfigurationOptionsExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::VideoSourceConfigurationOptionsExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::VideoSourceConfigurationOptionsExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Rotate"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::RotateOptions>>::serialize(REMOTING__NAMES[0], value.getRotate(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoSourceConfigurationOptionsExtension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED

