//
// H264DecOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_H264DecOptions_INCLUDED
#define TypeDeserializer_ONVIF_H264DecOptions_INCLUDED


#include "ONVIF/H264DecOptions.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::H264DecOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::H264DecOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::H264DecOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ResolutionsAvailable"s,"SupportedH264Profiles"s,"SupportedInputBitrate"s,"SupportedFrameRate"s};
		bool ret = false;
		std::vector<ONVIF::VideoResolution> gen_resolutionsAvailable;
		ret = TypeDeserializer<std::vector<ONVIF::VideoResolution>>::deserialize(REMOTING__NAMES[0], true, deser, gen_resolutionsAvailable);
		if (ret) value.setResolutionsAvailable(gen_resolutionsAvailable);
		std::vector<std::string> gen_supportedH264Profiles;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], true, deser, gen_supportedH264Profiles);
		if (ret) value.setSupportedH264Profiles(gen_supportedH264Profiles);
		ONVIF::IntRange gen_supportedInputBitrate;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[2], true, deser, gen_supportedInputBitrate);
		if (ret) value.setSupportedInputBitrate(gen_supportedInputBitrate);
		ONVIF::IntRange gen_supportedFrameRate;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[3], true, deser, gen_supportedFrameRate);
		if (ret) value.setSupportedFrameRate(gen_supportedFrameRate);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_H264DecOptions_INCLUDED

