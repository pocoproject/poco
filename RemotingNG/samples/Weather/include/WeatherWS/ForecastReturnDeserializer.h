//
// ForecastReturnDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_ForecastReturn_INCLUDED
#define TypeDeserializer_WeatherWS_ForecastReturn_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/ArrayOfForecastDeserializer.h"
#include "WeatherWS/ArrayOfForecastSerializer.h"
#include "WeatherWS/ForecastReturn.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::ForecastReturn>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::ForecastReturn& value)
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::ForecastReturn& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Success"s,"ResponseText"s,"State"s,"City"s,"WeatherStationCity"s,"ForecastResult"s};
		bool ret = false;
		bool gen_success;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_success);
		if (ret) value.setSuccess(gen_success);
		Poco::Optional<std::string> gen_responseText;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_responseText);
		if (ret) value.setResponseText(gen_responseText);
		Poco::Optional<std::string> gen_state;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_state);
		if (ret) value.setState(gen_state);
		Poco::Optional<std::string> gen_city;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_city);
		if (ret) value.setCity(gen_city);
		Poco::Optional<std::string> gen_weatherStationCity;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_weatherStationCity);
		if (ret) value.setWeatherStationCity(gen_weatherStationCity);
		Poco::Optional<WeatherWS::ArrayOfForecast> gen_forecastResult;
		ret = TypeDeserializer<Poco::Optional<WeatherWS::ArrayOfForecast>>::deserialize(REMOTING__NAMES[5], false, deser, gen_forecastResult);
		if (ret) value.setForecastResult(gen_forecastResult);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_ForecastReturn_INCLUDED

