//
// DateTimeSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DateTime_INCLUDED
#define TypeSerializer_ONVIF_DateTime_INCLUDED


#include "ONVIF/DateDeserializer.h"
#include "ONVIF/DateSerializer.h"
#include "ONVIF/DateTime.h"
#include "ONVIF/TimeDeserializer.h"
#include "ONVIF/TimeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DateTime>
{
public:
	static void serialize(const std::string& name, const ONVIF::DateTime& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::DateTime& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Time"s,"Date"s,""s};
		TypeSerializer<ONVIF::Time>::serialize(REMOTING__NAMES[0], value.getTime(), ser);
		TypeSerializer<ONVIF::Date>::serialize(REMOTING__NAMES[1], value.getDate(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DateTime_INCLUDED

