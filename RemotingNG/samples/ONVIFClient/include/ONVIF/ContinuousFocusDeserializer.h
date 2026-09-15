//
// ContinuousFocusDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ContinuousFocus_INCLUDED
#define TypeDeserializer_ONVIF_ContinuousFocus_INCLUDED


#include "ONVIF/ContinuousFocus.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ContinuousFocus>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ContinuousFocus& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ContinuousFocus& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Speed"s};
		bool ret = false;
		float gen_speed;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_speed);
		if (ret) value.setSpeed(gen_speed);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ContinuousFocus_INCLUDED

