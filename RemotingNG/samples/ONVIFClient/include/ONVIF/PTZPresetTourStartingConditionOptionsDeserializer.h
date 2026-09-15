//
// PTZPresetTourStartingConditionOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourStartingConditionOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourStartingConditionOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/PTZPresetTourStartingConditionOptions.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourStartingConditionOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourStartingConditionOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourStartingConditionOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecurringTime"s,"RecurringDuration"s,"Direction"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::IntRange> gen_recurringTime;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[0], false, deser, gen_recurringTime);
		if (ret) value.setRecurringTime(gen_recurringTime);
		Poco::SharedPtr<ONVIF::DurationRange> gen_recurringDuration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DurationRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_recurringDuration);
		if (ret) value.setRecurringDuration(gen_recurringDuration);
		std::vector<std::string> gen_direction;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_direction);
		if (ret) value.setDirection(gen_direction);
		Poco::SharedPtr<ONVIF::PTZPresetTourStartingConditionOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourStartingConditionOptionsExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourStartingConditionOptions_INCLUDED

