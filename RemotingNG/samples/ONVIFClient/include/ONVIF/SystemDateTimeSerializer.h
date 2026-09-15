//
// SystemDateTimeSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SystemDateTime_INCLUDED
#define TypeSerializer_ONVIF_SystemDateTime_INCLUDED


#include "ONVIF/DateTimeDeserializer.h"
#include "ONVIF/DateTimeSerializer.h"
#include "ONVIF/SystemDateTime.h"
#include "ONVIF/SystemDateTimeExtensionDeserializer.h"
#include "ONVIF/SystemDateTimeExtensionSerializer.h"
#include "ONVIF/TimeZoneDeserializer.h"
#include "ONVIF/TimeZoneSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SystemDateTime>
{
public:
	static void serialize(const std::string& name, const ONVIF::SystemDateTime& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::SystemDateTime& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DateTimeType"s,"DaylightSavings"s,"TimeZone"s,"UTCDateTime"s,"LocalDateTime"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getDateTimeType(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getDaylightSavings(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::TimeZone>>::serialize(REMOTING__NAMES[2], value.getTimeZone(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DateTime>>::serialize(REMOTING__NAMES[3], value.getUTCDateTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DateTime>>::serialize(REMOTING__NAMES[4], value.getLocalDateTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SystemDateTimeExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SystemDateTime_INCLUDED

