//
// TempDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_Temp_INCLUDED
#define TypeDeserializer_WeatherWS_Temp_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/Temp.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::Temp>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::Temp& value)
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::Temp& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MorningLow"s,"DaytimeHigh"s};
		bool ret = false;
		Poco::Optional<std::string> gen_morningLow;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_morningLow);
		if (ret) value.setMorningLow(gen_morningLow);
		Poco::Optional<std::string> gen_daytimeHigh;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_daytimeHigh);
		if (ret) value.setDaytimeHigh(gen_daytimeHigh);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_Temp_INCLUDED

