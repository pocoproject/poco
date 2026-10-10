//
// ForecastDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_Forecast_INCLUDED
#define TypeDeserializer_WeatherWS_Forecast_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/Forecast.h"
#include "WeatherWS/POPDeserializer.h"
#include "WeatherWS/POPSerializer.h"
#include "WeatherWS/TempDeserializer.h"
#include "WeatherWS/TempSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::Forecast>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::Forecast& value)
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::Forecast& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Date"s,"WeatherID"s,"Desciption"s,"Temperatures"s,"ProbabilityOfPrecipiation"s};
		bool ret = false;
		Poco::DateTime gen_date;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[0], true, deser, gen_date);
		if (ret) value.setDate(gen_date);
		Poco::Int16 gen_weatherID;
		ret = TypeDeserializer<Poco::Int16>::deserialize(REMOTING__NAMES[1], true, deser, gen_weatherID);
		if (ret) value.setWeatherID(gen_weatherID);
		Poco::Optional<std::string> gen_desciption;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_desciption);
		if (ret) value.setDesciption(gen_desciption);
		WeatherWS::Temp gen_temperatures;
		ret = TypeDeserializer<WeatherWS::Temp>::deserialize(REMOTING__NAMES[3], true, deser, gen_temperatures);
		if (ret) value.setTemperatures(gen_temperatures);
		WeatherWS::POP gen_probabilityOfPrecipiation;
		ret = TypeDeserializer<WeatherWS::POP>::deserialize(REMOTING__NAMES[4], true, deser, gen_probabilityOfPrecipiation);
		if (ret) value.setProbabilityOfPrecipiation(gen_probabilityOfPrecipiation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_Forecast_INCLUDED

