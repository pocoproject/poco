//
// AudioAttributesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AudioAttributes_INCLUDED
#define TypeDeserializer_ONVIF_AudioAttributes_INCLUDED


#include "ONVIF/AudioAttributes.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AudioAttributes>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AudioAttributes& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AudioAttributes& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Bitrate"s,"Encoding"s,"Samplerate"s};
		bool ret = false;
		Poco::Optional<int> gen_bitrate;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_bitrate);
		if (ret) value.setBitrate(gen_bitrate);
		std::string gen_encoding;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_encoding);
		if (ret) value.setEncoding(gen_encoding);
		int gen_samplerate;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_samplerate);
		if (ret) value.setSamplerate(gen_samplerate);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AudioAttributes_INCLUDED

