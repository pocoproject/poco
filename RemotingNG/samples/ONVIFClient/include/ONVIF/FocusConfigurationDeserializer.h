//
// FocusConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FocusConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_FocusConfiguration_INCLUDED


#include "ONVIF/FocusConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FocusConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FocusConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FocusConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoFocusMode"s,"DefaultSpeed"s,"NearLimit"s,"FarLimit"s};
		bool ret = false;
		std::string gen_autoFocusMode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_autoFocusMode);
		if (ret) value.setAutoFocusMode(gen_autoFocusMode);
		float gen_defaultSpeed;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_defaultSpeed);
		if (ret) value.setDefaultSpeed(gen_defaultSpeed);
		float gen_nearLimit;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[2], true, deser, gen_nearLimit);
		if (ret) value.setNearLimit(gen_nearLimit);
		float gen_farLimit;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[3], true, deser, gen_farLimit);
		if (ret) value.setFarLimit(gen_farLimit);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FocusConfiguration_INCLUDED

