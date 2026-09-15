//
// PTZPresetTourOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourOptions_INCLUDED


#include "ONVIF/PTZPresetTourOptions.h"
#include "ONVIF/PTZPresetTourSpotOptionsDeserializer.h"
#include "ONVIF/PTZPresetTourSpotOptionsSerializer.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoStart"s,"StartingCondition"s,"TourSpot"s};
		bool ret = false;
		bool gen_autoStart;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_autoStart);
		if (ret) value.setAutoStart(gen_autoStart);
		ONVIF::PTZPresetTourStartingConditionOptions gen_startingCondition;
		ret = TypeDeserializer<ONVIF::PTZPresetTourStartingConditionOptions>::deserialize(REMOTING__NAMES[1], true, deser, gen_startingCondition);
		if (ret) value.setStartingCondition(gen_startingCondition);
		ONVIF::PTZPresetTourSpotOptions gen_tourSpot;
		ret = TypeDeserializer<ONVIF::PTZPresetTourSpotOptions>::deserialize(REMOTING__NAMES[2], true, deser, gen_tourSpot);
		if (ret) value.setTourSpot(gen_tourSpot);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourOptions_INCLUDED

