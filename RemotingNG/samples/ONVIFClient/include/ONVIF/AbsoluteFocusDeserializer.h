//
// AbsoluteFocusDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AbsoluteFocus_INCLUDED
#define TypeDeserializer_ONVIF_AbsoluteFocus_INCLUDED


#include "ONVIF/AbsoluteFocus.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AbsoluteFocus>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AbsoluteFocus& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AbsoluteFocus& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Position"s,"Speed"s};
		bool ret = false;
		float gen_position;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_position);
		if (ret) value.setPosition(gen_position);
		Poco::Optional<float> gen_speed;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_speed);
		if (ret) value.setSpeed(gen_speed);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AbsoluteFocus_INCLUDED

