//
// PTZPresetTourStartingConditionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourStartingCondition_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourStartingCondition_INCLUDED


#include "ONVIF/PTZPresetTourStartingCondition.h"
#include "ONVIF/PTZPresetTourStartingConditionExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourStartingCondition>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RandomPresetOrder"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::PTZPresetTourStartingCondition& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::PTZPresetTourStartingCondition& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RandomPresetOrder"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getRandomPresetOrder(), ser);
	}

	static void serializeImpl(const ONVIF::PTZPresetTourStartingCondition& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecurringTime"s,"RecurringDuration"s,"Direction"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getRecurringTime(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getRecurringDuration(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getDirection(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourStartingConditionExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourStartingCondition_INCLUDED

