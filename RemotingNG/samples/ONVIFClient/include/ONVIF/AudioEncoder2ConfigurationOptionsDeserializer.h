//
// AudioEncoder2ConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED


#include "ONVIF/AudioEncoder2ConfigurationOptions.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AudioEncoder2ConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AudioEncoder2ConfigurationOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AudioEncoder2ConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"BitrateList"s,"SampleRateList"s};
		bool ret = false;
		std::string gen_encoding;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_encoding);
		if (ret) value.setEncoding(gen_encoding);
		ONVIF::IntItems gen_bitrateList;
		ret = TypeDeserializer<ONVIF::IntItems>::deserialize(REMOTING__NAMES[1], true, deser, gen_bitrateList);
		if (ret) value.setBitrateList(gen_bitrateList);
		ONVIF::IntItems gen_sampleRateList;
		ret = TypeDeserializer<ONVIF::IntItems>::deserialize(REMOTING__NAMES[2], true, deser, gen_sampleRateList);
		if (ret) value.setSampleRateList(gen_sampleRateList);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED

