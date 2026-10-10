//
// ForecastSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_WeatherWS_Forecast_INCLUDED
#define TypeSerializer_WeatherWS_Forecast_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "WeatherWS/Forecast.h"
#include "WeatherWS/POPDeserializer.h"
#include "WeatherWS/POPSerializer.h"
#include "WeatherWS/TempDeserializer.h"
#include "WeatherWS/TempSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<WeatherWS::Forecast>
{
public:
	static void serialize(const std::string& name, const WeatherWS::Forecast& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://ws.cdyne.com/WeatherWS/"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const WeatherWS::Forecast& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Date"s,"WeatherID"s,"Desciption"s,"Temperatures"s,"ProbabilityOfPrecipiation"s,""s};
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[0], value.getDate(), ser);
		TypeSerializer<Poco::Int16>::serialize(REMOTING__NAMES[1], value.getWeatherID(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getDesciption(), ser);
		TypeSerializer<WeatherWS::Temp>::serialize(REMOTING__NAMES[3], value.getTemperatures(), ser);
		TypeSerializer<WeatherWS::POP>::serialize(REMOTING__NAMES[4], value.getProbabilityOfPrecipiation(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_WeatherWS_Forecast_INCLUDED

