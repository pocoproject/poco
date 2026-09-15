//
// RelativeFocusDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RelativeFocus_INCLUDED
#define TypeDeserializer_ONVIF_RelativeFocus_INCLUDED


#include "ONVIF/RelativeFocus.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RelativeFocus>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RelativeFocus& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RelativeFocus& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Distance"s,"Speed"s};
		bool ret = false;
		float gen_distance;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_distance);
		if (ret) value.setDistance(gen_distance);
		Poco::Optional<float> gen_speed;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_speed);
		if (ret) value.setSpeed(gen_speed);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RelativeFocus_INCLUDED

