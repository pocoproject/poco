//
// PTZPresetTourSpotDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourSpot_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourSpot_INCLUDED


#include "ONVIF/PTZPresetTourPresetDetailDeserializer.h"
#include "ONVIF/PTZPresetTourPresetDetailSerializer.h"
#include "ONVIF/PTZPresetTourSpot.h"
#include "ONVIF/PTZPresetTourSpotExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourSpotExtensionSerializer.h"
#include "ONVIF/PTZSpeedDeserializer.h"
#include "ONVIF/PTZSpeedSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourSpot>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourSpot& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourSpot& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetDetail"s,"Speed"s,"StayTime"s,"Extension"s};
		bool ret = false;
		ONVIF::PTZPresetTourPresetDetail gen_presetDetail;
		ret = TypeDeserializer<ONVIF::PTZPresetTourPresetDetail>::deserialize(REMOTING__NAMES[0], true, deser, gen_presetDetail);
		if (ret) value.setPresetDetail(gen_presetDetail);
		Poco::SharedPtr<ONVIF::PTZSpeed> gen_speed;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZSpeed>>::deserialize(REMOTING__NAMES[1], false, deser, gen_speed);
		if (ret) value.setSpeed(gen_speed);
		Poco::Optional<std::string> gen_stayTime;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_stayTime);
		if (ret) value.setStayTime(gen_stayTime);
		Poco::SharedPtr<ONVIF::PTZPresetTourSpotExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourSpotExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourSpot_INCLUDED

