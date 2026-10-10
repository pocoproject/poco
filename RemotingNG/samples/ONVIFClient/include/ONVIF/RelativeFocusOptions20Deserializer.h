//
// RelativeFocusOptions20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RelativeFocusOptions20_INCLUDED
#define TypeDeserializer_ONVIF_RelativeFocusOptions20_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/RelativeFocusOptions20.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RelativeFocusOptions20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RelativeFocusOptions20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RelativeFocusOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Distance"s,"Speed"s};
		bool ret = false;
		ONVIF::FloatRange gen_distance;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[0], true, deser, gen_distance);
		if (ret) value.setDistance(gen_distance);
		Poco::SharedPtr<ONVIF::FloatRange> gen_speed;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_speed);
		if (ret) value.setSpeed(gen_speed);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RelativeFocusOptions20_INCLUDED

