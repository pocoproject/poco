//
// PresetTourDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PresetTour_INCLUDED
#define TypeDeserializer_ONVIF_PresetTour_INCLUDED


#include "ONVIF/PTZPresetTourExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourExtensionSerializer.h"
#include "ONVIF/PTZPresetTourSpotDeserializer.h"
#include "ONVIF/PTZPresetTourSpotSerializer.h"
#include "ONVIF/PTZPresetTourStartingConditionDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionSerializer.h"
#include "ONVIF/PTZPresetTourStatusDeserializer.h"
#include "ONVIF/PTZPresetTourStatusSerializer.h"
#include "ONVIF/PresetTour.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PresetTour>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PresetTour& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::PresetTour& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s};
		bool ret = false;
		Poco::Optional<std::string> gen_token;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_token);
		if (ret) value.setToken(gen_token);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::PresetTour& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Status"s,"AutoStart"s,"StartingCondition"s,"TourSpot"s,"Extension"s};
		bool ret = false;
		Poco::Optional<std::string> gen_name;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_name);
		if (ret) value.setName(gen_name);
		ONVIF::PTZPresetTourStatus gen_status;
		ret = TypeDeserializer<ONVIF::PTZPresetTourStatus>::deserialize(REMOTING__NAMES[1], true, deser, gen_status);
		if (ret) value.setStatus(gen_status);
		bool gen_autoStart;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_autoStart);
		if (ret) value.setAutoStart(gen_autoStart);
		ONVIF::PTZPresetTourStartingCondition gen_startingCondition;
		ret = TypeDeserializer<ONVIF::PTZPresetTourStartingCondition>::deserialize(REMOTING__NAMES[3], true, deser, gen_startingCondition);
		if (ret) value.setStartingCondition(gen_startingCondition);
		std::vector<ONVIF::PTZPresetTourSpot> gen_tourSpot;
		ret = TypeDeserializer<std::vector<ONVIF::PTZPresetTourSpot>>::deserialize(REMOTING__NAMES[4], false, deser, gen_tourSpot);
		if (ret) value.setTourSpot(gen_tourSpot);
		Poco::SharedPtr<ONVIF::PTZPresetTourExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PresetTour_INCLUDED

