//
// AudioOutputConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AudioOutputConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_AudioOutputConfigurationOptions_INCLUDED


#include "ONVIF/AudioOutputConfigurationOptions.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AudioOutputConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::AudioOutputConfigurationOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AudioOutputConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"OutputTokensAvailable"s,"SendPrimacyOptions"s,"OutputLevelRange"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getOutputTokensAvailable(), ser);
		TypeSerializer<std::vector<Poco::URI>>::serialize(REMOTING__NAMES[1], value.getSendPrimacyOptions(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[2], value.getOutputLevelRange(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AudioOutputConfigurationOptions_INCLUDED

