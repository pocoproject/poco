//
// GetVideoSourceModesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetVideoSourceModesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetVideoSourceModesResponse_INCLUDED


#include "ONVIF/Media/GetVideoSourceModesResponse.h"
#include "ONVIF/Media/VideoSourceModeDeserializer.h"
#include "ONVIF/Media/VideoSourceModeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetVideoSourceModesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetVideoSourceModesResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetVideoSourceModesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoSourceModes"s};
		bool ret = false;
		std::vector<ONVIF::Media::VideoSourceMode> gen_videoSourceModes;
		ret = TypeDeserializer<std::vector<ONVIF::Media::VideoSourceMode>>::deserialize(REMOTING__NAMES[0], true, deser, gen_videoSourceModes);
		if (ret) value.setVideoSourceModes(gen_videoSourceModes);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetVideoSourceModesResponse_INCLUDED

