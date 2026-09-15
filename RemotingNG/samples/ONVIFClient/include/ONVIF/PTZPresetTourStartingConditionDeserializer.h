//
// PTZPresetTourStartingConditionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourStartingCondition_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourStartingCondition_INCLUDED


#include "ONVIF/PTZPresetTourStartingCondition.h"
#include "ONVIF/PTZPresetTourStartingConditionExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourStartingCondition>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourStartingCondition& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::PTZPresetTourStartingCondition& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RandomPresetOrder"s};
		bool ret = false;
		Poco::Optional<bool> gen_randomPresetOrder;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_randomPresetOrder);
		if (ret) value.setRandomPresetOrder(gen_randomPresetOrder);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourStartingCondition& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecurringTime"s,"RecurringDuration"s,"Direction"s,"Extension"s};
		bool ret = false;
		Poco::Optional<int> gen_recurringTime;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_recurringTime);
		if (ret) value.setRecurringTime(gen_recurringTime);
		Poco::Optional<std::string> gen_recurringDuration;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_recurringDuration);
		if (ret) value.setRecurringDuration(gen_recurringDuration);
		Poco::Optional<std::string> gen_direction;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_direction);
		if (ret) value.setDirection(gen_direction);
		Poco::SharedPtr<ONVIF::PTZPresetTourStartingConditionExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourStartingConditionExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RandomPresetOrder"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourStartingCondition_INCLUDED

