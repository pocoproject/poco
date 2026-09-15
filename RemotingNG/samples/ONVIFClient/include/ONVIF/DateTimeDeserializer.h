//
// DateTimeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DateTime_INCLUDED
#define TypeDeserializer_ONVIF_DateTime_INCLUDED


#include "ONVIF/DateDeserializer.h"
#include "ONVIF/DateSerializer.h"
#include "ONVIF/DateTime.h"
#include "ONVIF/TimeDeserializer.h"
#include "ONVIF/TimeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DateTime>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DateTime& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DateTime& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Time"s,"Date"s};
		bool ret = false;
		ONVIF::Time gen_time;
		ret = TypeDeserializer<ONVIF::Time>::deserialize(REMOTING__NAMES[0], true, deser, gen_time);
		if (ret) value.setTime(gen_time);
		ONVIF::Date gen_date;
		ret = TypeDeserializer<ONVIF::Date>::deserialize(REMOTING__NAMES[1], true, deser, gen_date);
		if (ret) value.setDate(gen_date);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DateTime_INCLUDED

