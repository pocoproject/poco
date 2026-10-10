//
// POPDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_POP_INCLUDED
#define TypeDeserializer_WeatherWS_POP_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/POP.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::POP>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::POP& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://ws.cdyne.com/WeatherWS/"s);
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::POP& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Nighttime"s,"Daytime"s};
		bool ret = false;
		Poco::Optional<std::string> gen_nighttime;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_nighttime);
		if (ret) value.setNighttime(gen_nighttime);
		Poco::Optional<std::string> gen_daytime;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_daytime);
		if (ret) value.setDaytime(gen_daytime);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_POP_INCLUDED

