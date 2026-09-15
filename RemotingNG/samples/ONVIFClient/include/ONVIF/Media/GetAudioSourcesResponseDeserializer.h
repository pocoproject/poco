//
// GetAudioSourcesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetAudioSourcesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetAudioSourcesResponse_INCLUDED


#include "ONVIF/AudioSourceDeserializer.h"
#include "ONVIF/AudioSourceSerializer.h"
#include "ONVIF/Media/GetAudioSourcesResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetAudioSourcesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetAudioSourcesResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetAudioSourcesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioSources"s};
		bool ret = false;
		std::vector<ONVIF::AudioSource> gen_audioSources;
		ret = TypeDeserializer<std::vector<ONVIF::AudioSource>>::deserialize(REMOTING__NAMES[0], false, deser, gen_audioSources);
		if (ret) value.setAudioSources(gen_audioSources);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetAudioSourcesResponse_INCLUDED

