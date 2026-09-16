//
// ProfileExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ProfileExtension_INCLUDED
#define TypeSerializer_ONVIF_ProfileExtension_INCLUDED


#include "ONVIF/AudioDecoderConfigurationDeserializer.h"
#include "ONVIF/AudioDecoderConfigurationSerializer.h"
#include "ONVIF/AudioOutputConfigurationDeserializer.h"
#include "ONVIF/AudioOutputConfigurationSerializer.h"
#include "ONVIF/ProfileExtension.h"
#include "ONVIF/ProfileExtension2Deserializer.h"
#include "ONVIF/ProfileExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ProfileExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::ProfileExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ProfileExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioOutputConfiguration"s,"AudioDecoderConfiguration"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioOutputConfiguration>>::serialize(REMOTING__NAMES[0], value.getAudioOutputConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioDecoderConfiguration>>::serialize(REMOTING__NAMES[1], value.getAudioDecoderConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ProfileExtension2>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ProfileExtension_INCLUDED

