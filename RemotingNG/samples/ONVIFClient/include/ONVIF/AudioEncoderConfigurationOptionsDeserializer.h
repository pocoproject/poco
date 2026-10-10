//
// AudioEncoderConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AudioEncoderConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_AudioEncoderConfigurationOptions_INCLUDED


#include "ONVIF/AudioEncoderConfigurationOptionDeserializer.h"
#include "ONVIF/AudioEncoderConfigurationOptionSerializer.h"
#include "ONVIF/AudioEncoderConfigurationOptions.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AudioEncoderConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AudioEncoderConfigurationOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AudioEncoderConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Options"s};
		bool ret = false;
		std::vector<ONVIF::AudioEncoderConfigurationOption> gen_options;
		ret = TypeDeserializer<std::vector<ONVIF::AudioEncoderConfigurationOption>>::deserialize(REMOTING__NAMES[0], false, deser, gen_options);
		if (ret) value.setOptions(gen_options);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AudioEncoderConfigurationOptions_INCLUDED

