//
// DateTimeRangeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DateTimeRange_INCLUDED
#define TypeDeserializer_ONVIF_DateTimeRange_INCLUDED


#include "ONVIF/DateTimeRange.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DateTimeRange>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DateTimeRange& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DateTimeRange& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"From"s,"Until"s};
		bool ret = false;
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_from;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[0], true, deser, gen_from);
		if (ret) value.setFrom(gen_from);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_until;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[1], true, deser, gen_until);
		if (ret) value.setUntil(gen_until);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DateTimeRange_INCLUDED

