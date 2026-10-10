//
// H264OptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_H264Options_INCLUDED
#define TypeDeserializer_ONVIF_H264Options_INCLUDED


#include "ONVIF/H264Options.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::H264Options>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::H264Options& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::H264Options& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ResolutionsAvailable"s,"GovLengthRange"s,"FrameRateRange"s,"EncodingIntervalRange"s,"H264ProfilesSupported"s};
		bool ret = false;
		std::vector<ONVIF::VideoResolution> gen_resolutionsAvailable;
		ret = TypeDeserializer<std::vector<ONVIF::VideoResolution>>::deserialize(REMOTING__NAMES[0], true, deser, gen_resolutionsAvailable);
		if (ret) value.setResolutionsAvailable(gen_resolutionsAvailable);
		ONVIF::IntRange gen_govLengthRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_govLengthRange);
		if (ret) value.setGovLengthRange(gen_govLengthRange);
		ONVIF::IntRange gen_frameRateRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[2], true, deser, gen_frameRateRange);
		if (ret) value.setFrameRateRange(gen_frameRateRange);
		ONVIF::IntRange gen_encodingIntervalRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[3], true, deser, gen_encodingIntervalRange);
		if (ret) value.setEncodingIntervalRange(gen_encodingIntervalRange);
		std::vector<std::string> gen_h264ProfilesSupported;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[4], true, deser, gen_h264ProfilesSupported);
		if (ret) value.setH264ProfilesSupported(gen_h264ProfilesSupported);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_H264Options_INCLUDED

