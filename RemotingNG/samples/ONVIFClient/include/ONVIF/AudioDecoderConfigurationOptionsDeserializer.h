//
// AudioDecoderConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AudioDecoderConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_AudioDecoderConfigurationOptions_INCLUDED


#include "ONVIF/AACDecOptionsDeserializer.h"
#include "ONVIF/AACDecOptionsSerializer.h"
#include "ONVIF/AudioDecoderConfigurationOptions.h"
#include "ONVIF/AudioDecoderConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/AudioDecoderConfigurationOptionsExtensionSerializer.h"
#include "ONVIF/G711DecOptionsDeserializer.h"
#include "ONVIF/G711DecOptionsSerializer.h"
#include "ONVIF/G726DecOptionsDeserializer.h"
#include "ONVIF/G726DecOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AudioDecoderConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AudioDecoderConfigurationOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AudioDecoderConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AACDecOptions"s,"G711DecOptions"s,"G726DecOptions"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AACDecOptions> gen_aACDecOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AACDecOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_aACDecOptions);
		if (ret) value.setAACDecOptions(gen_aACDecOptions);
		Poco::SharedPtr<ONVIF::G711DecOptions> gen_g711DecOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::G711DecOptions>>::deserialize(REMOTING__NAMES[1], false, deser, gen_g711DecOptions);
		if (ret) value.setG711DecOptions(gen_g711DecOptions);
		Poco::SharedPtr<ONVIF::G726DecOptions> gen_g726DecOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::G726DecOptions>>::deserialize(REMOTING__NAMES[2], false, deser, gen_g726DecOptions);
		if (ret) value.setG726DecOptions(gen_g726DecOptions);
		Poco::SharedPtr<ONVIF::AudioDecoderConfigurationOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioDecoderConfigurationOptionsExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AudioDecoderConfigurationOptions_INCLUDED

