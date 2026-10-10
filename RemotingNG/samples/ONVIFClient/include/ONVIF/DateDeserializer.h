//
// DateDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Date_INCLUDED
#define TypeDeserializer_ONVIF_Date_INCLUDED


#include "ONVIF/Date.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Date>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Date& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Date& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Year"s,"Month"s,"Day"s};
		bool ret = false;
		int gen_year;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_year);
		if (ret) value.setYear(gen_year);
		int gen_month;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_month);
		if (ret) value.setMonth(gen_month);
		int gen_day;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_day);
		if (ret) value.setDay(gen_day);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Date_INCLUDED

