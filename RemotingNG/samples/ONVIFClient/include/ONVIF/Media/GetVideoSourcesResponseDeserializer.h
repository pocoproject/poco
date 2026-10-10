//
// GetVideoSourcesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetVideoSourcesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetVideoSourcesResponse_INCLUDED


#include "ONVIF/Media/GetVideoSourcesResponse.h"
#include "ONVIF/VideoSourceDeserializer.h"
#include "ONVIF/VideoSourceSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetVideoSourcesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetVideoSourcesResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetVideoSourcesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoSources"s};
		bool ret = false;
		std::vector<ONVIF::VideoSource> gen_videoSources;
		ret = TypeDeserializer<std::vector<ONVIF::VideoSource>>::deserialize(REMOTING__NAMES[0], false, deser, gen_videoSources);
		if (ret) value.setVideoSources(gen_videoSources);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetVideoSourcesResponse_INCLUDED

