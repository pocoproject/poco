//
// GetAudioOutputConfigurationOptionsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetAudioOutputConfigurationOptionsResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetAudioOutputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioOutputConfigurationOptionsDeserializer.h"
#include "ONVIF/AudioOutputConfigurationOptionsSerializer.h"
#include "ONVIF/Media/GetAudioOutputConfigurationOptionsResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetAudioOutputConfigurationOptionsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetAudioOutputConfigurationOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetAudioOutputConfigurationOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Options"s};
		bool ret = false;
		ONVIF::AudioOutputConfigurationOptions gen_options;
		ret = TypeDeserializer<ONVIF::AudioOutputConfigurationOptions>::deserialize(REMOTING__NAMES[0], true, deser, gen_options);
		if (ret) value.setOptions(gen_options);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetAudioOutputConfigurationOptionsResponse_INCLUDED

