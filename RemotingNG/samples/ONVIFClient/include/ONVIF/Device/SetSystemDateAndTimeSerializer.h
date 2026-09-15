//
// SetSystemDateAndTimeSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_SetSystemDateAndTime_INCLUDED
#define TypeSerializer_ONVIF_Device_SetSystemDateAndTime_INCLUDED


#include "ONVIF/DateTimeDeserializer.h"
#include "ONVIF/DateTimeSerializer.h"
#include "ONVIF/Device/SetSystemDateAndTime.h"
#include "ONVIF/TimeZoneDeserializer.h"
#include "ONVIF/TimeZoneSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::SetSystemDateAndTime>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::SetSystemDateAndTime& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::SetSystemDateAndTime& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DateTimeType"s,"DaylightSavings"s,"TimeZone"s,"UTCDateTime"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getDateTimeType(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getDaylightSavings(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::TimeZone>>::serialize(REMOTING__NAMES[2], value.getTimeZone(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DateTime>>::serialize(REMOTING__NAMES[3], value.getUTCDateTime(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_SetSystemDateAndTime_INCLUDED

