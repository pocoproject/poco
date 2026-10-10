//
// PTZPresetTourSpotOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourSpotOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourSpotOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsDeserializer.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsSerializer.h"
#include "ONVIF/PTZPresetTourSpotOptions.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourSpotOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourSpotOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourSpotOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetDetail"s,"StayTime"s};
		bool ret = false;
		ONVIF::PTZPresetTourPresetDetailOptions gen_presetDetail;
		ret = TypeDeserializer<ONVIF::PTZPresetTourPresetDetailOptions>::deserialize(REMOTING__NAMES[0], true, deser, gen_presetDetail);
		if (ret) value.setPresetDetail(gen_presetDetail);
		ONVIF::DurationRange gen_stayTime;
		ret = TypeDeserializer<ONVIF::DurationRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_stayTime);
		if (ret) value.setStayTime(gen_stayTime);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourSpotOptions_INCLUDED

