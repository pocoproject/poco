//
// AudioDecoderConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AudioDecoderConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_AudioDecoderConfigurationOptions_INCLUDED


#include "ONVIF/AACDecOptionsDeserializer.h"
#include "ONVIF/AACDecOptionsSerializer.h"
#include "ONVIF/AudioDecoderConfigurationOptions.h"
#include "ONVIF/AudioDecoderConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/AudioDecoderConfigurationOptionsExtensionSerializer.h"
#include "ONVIF/G711DecOptionsDeserializer.h"
#include "ONVIF/G711DecOptionsSerializer.h"
#include "ONVIF/G726DecOptionsDeserializer.h"
#include "ONVIF/G726DecOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AudioDecoderConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::AudioDecoderConfigurationOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AudioDecoderConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AACDecOptions"s,"G711DecOptions"s,"G726DecOptions"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::AACDecOptions>>::serialize(REMOTING__NAMES[0], value.getAACDecOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::G711DecOptions>>::serialize(REMOTING__NAMES[1], value.getG711DecOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::G726DecOptions>>::serialize(REMOTING__NAMES[2], value.getG726DecOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioDecoderConfigurationOptionsExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AudioDecoderConfigurationOptions_INCLUDED

