//
// AudioSourceConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AudioSourceConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_AudioSourceConfigurationOptions_INCLUDED


#include "ONVIF/AudioSourceConfigurationOptions.h"
#include "ONVIF/AudioSourceOptionsExtensionDeserializer.h"
#include "ONVIF/AudioSourceOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AudioSourceConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::AudioSourceConfigurationOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AudioSourceConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InputTokensAvailable"s,"Extension"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getInputTokensAvailable(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioSourceOptionsExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AudioSourceConfigurationOptions_INCLUDED

