//
// CodingCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_CodingCapabilities_INCLUDED
#define TypeSerializer_ONVIF_CodingCapabilities_INCLUDED


#include "ONVIF/AudioDecoderConfigurationOptionsDeserializer.h"
#include "ONVIF/AudioDecoderConfigurationOptionsSerializer.h"
#include "ONVIF/AudioEncoderConfigurationOptionsDeserializer.h"
#include "ONVIF/AudioEncoderConfigurationOptionsSerializer.h"
#include "ONVIF/CodingCapabilities.h"
#include "ONVIF/VideoDecoderConfigurationOptionsDeserializer.h"
#include "ONVIF/VideoDecoderConfigurationOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::CodingCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::CodingCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::CodingCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioEncodingCapabilities"s,"AudioDecodingCapabilities"s,"VideoDecodingCapabilities"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioEncoderConfigurationOptions>>::serialize(REMOTING__NAMES[0], value.getAudioEncodingCapabilities(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioDecoderConfigurationOptions>>::serialize(REMOTING__NAMES[1], value.getAudioDecodingCapabilities(), ser);
		TypeSerializer<ONVIF::VideoDecoderConfigurationOptions>::serialize(REMOTING__NAMES[2], value.getVideoDecodingCapabilities(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_CodingCapabilities_INCLUDED

