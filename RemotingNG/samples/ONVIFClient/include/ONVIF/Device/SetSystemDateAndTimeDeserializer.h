//
// SetSystemDateAndTimeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetSystemDateAndTime_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetSystemDateAndTime_INCLUDED


#include "ONVIF/DateTimeDeserializer.h"
#include "ONVIF/DateTimeSerializer.h"
#include "ONVIF/Device/SetSystemDateAndTime.h"
#include "ONVIF/TimeZoneDeserializer.h"
#include "ONVIF/TimeZoneSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetSystemDateAndTime>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetSystemDateAndTime& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetSystemDateAndTime& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DateTimeType"s,"DaylightSavings"s,"TimeZone"s,"UTCDateTime"s};
		bool ret = false;
		std::string gen_dateTimeType;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_dateTimeType);
		if (ret) value.setDateTimeType(gen_dateTimeType);
		bool gen_daylightSavings;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_daylightSavings);
		if (ret) value.setDaylightSavings(gen_daylightSavings);
		Poco::SharedPtr<ONVIF::TimeZone> gen_timeZone;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::TimeZone>>::deserialize(REMOTING__NAMES[2], false, deser, gen_timeZone);
		if (ret) value.setTimeZone(gen_timeZone);
		Poco::SharedPtr<ONVIF::DateTime> gen_uTCDateTime;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DateTime>>::deserialize(REMOTING__NAMES[3], false, deser, gen_uTCDateTime);
		if (ret) value.setUTCDateTime(gen_uTCDateTime);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetSystemDateAndTime_INCLUDED

