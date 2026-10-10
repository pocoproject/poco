//
// AudioOutputConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AudioOutputConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_AudioOutputConfigurationOptions_INCLUDED


#include "ONVIF/AudioOutputConfigurationOptions.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AudioOutputConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AudioOutputConfigurationOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AudioOutputConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"OutputTokensAvailable"s,"SendPrimacyOptions"s,"OutputLevelRange"s};
		bool ret = false;
		std::vector<std::string> gen_outputTokensAvailable;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, gen_outputTokensAvailable);
		if (ret) value.setOutputTokensAvailable(gen_outputTokensAvailable);
		std::vector<Poco::URI> gen_sendPrimacyOptions;
		ret = TypeDeserializer<std::vector<Poco::URI>>::deserialize(REMOTING__NAMES[1], false, deser, gen_sendPrimacyOptions);
		if (ret) value.setSendPrimacyOptions(gen_sendPrimacyOptions);
		ONVIF::IntRange gen_outputLevelRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[2], true, deser, gen_outputLevelRange);
		if (ret) value.setOutputLevelRange(gen_outputLevelRange);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AudioOutputConfigurationOptions_INCLUDED

