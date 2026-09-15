//
// CodingCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_CodingCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_CodingCapabilities_INCLUDED


#include "ONVIF/AudioDecoderConfigurationOptionsDeserializer.h"
#include "ONVIF/AudioDecoderConfigurationOptionsSerializer.h"
#include "ONVIF/AudioEncoderConfigurationOptionsDeserializer.h"
#include "ONVIF/AudioEncoderConfigurationOptionsSerializer.h"
#include "ONVIF/CodingCapabilities.h"
#include "ONVIF/VideoDecoderConfigurationOptionsDeserializer.h"
#include "ONVIF/VideoDecoderConfigurationOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::CodingCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::CodingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::CodingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioEncodingCapabilities"s,"AudioDecodingCapabilities"s,"VideoDecodingCapabilities"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AudioEncoderConfigurationOptions> gen_audioEncodingCapabilities;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioEncoderConfigurationOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_audioEncodingCapabilities);
		if (ret) value.setAudioEncodingCapabilities(gen_audioEncodingCapabilities);
		Poco::SharedPtr<ONVIF::AudioDecoderConfigurationOptions> gen_audioDecodingCapabilities;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioDecoderConfigurationOptions>>::deserialize(REMOTING__NAMES[1], false, deser, gen_audioDecodingCapabilities);
		if (ret) value.setAudioDecodingCapabilities(gen_audioDecodingCapabilities);
		ONVIF::VideoDecoderConfigurationOptions gen_videoDecodingCapabilities;
		ret = TypeDeserializer<ONVIF::VideoDecoderConfigurationOptions>::deserialize(REMOTING__NAMES[2], true, deser, gen_videoDecodingCapabilities);
		if (ret) value.setVideoDecodingCapabilities(gen_videoDecodingCapabilities);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_CodingCapabilities_INCLUDED

