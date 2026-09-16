//
// FloatRangeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FloatRange_INCLUDED
#define TypeDeserializer_ONVIF_FloatRange_INCLUDED


#include "ONVIF/FloatRange.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FloatRange>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FloatRange& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FloatRange& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Min"s,"Max"s};
		bool ret = false;
		float gen_min;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_min);
		if (ret) value.setMin(gen_min);
		float gen_max;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_max);
		if (ret) value.setMax(gen_max);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FloatRange_INCLUDED

