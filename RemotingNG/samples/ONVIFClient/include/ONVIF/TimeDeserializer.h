//
// TimeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Time_INCLUDED
#define TypeDeserializer_ONVIF_Time_INCLUDED


#include "ONVIF/Time.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Time>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Time& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Time& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Hour"s,"Minute"s,"Second"s};
		bool ret = false;
		int gen_hour;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_hour);
		if (ret) value.setHour(gen_hour);
		int gen_minute;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_minute);
		if (ret) value.setMinute(gen_minute);
		int gen_second;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_second);
		if (ret) value.setSecond(gen_second);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Time_INCLUDED

