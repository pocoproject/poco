//
// ForecastReturnSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_WeatherWS_ForecastReturn_INCLUDED
#define TypeSerializer_WeatherWS_ForecastReturn_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "WeatherWS/ArrayOfForecastDeserializer.h"
#include "WeatherWS/ArrayOfForecastSerializer.h"
#include "WeatherWS/ForecastReturn.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<WeatherWS::ForecastReturn>
{
public:
	static void serialize(const std::string& name, const WeatherWS::ForecastReturn& value, Serializer& ser)
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

	static void serializeImpl(const WeatherWS::ForecastReturn& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Success"s,"ResponseText"s,"State"s,"City"s,"WeatherStationCity"s,"ForecastResult"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getSuccess(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getResponseText(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getState(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[3], value.getCity(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[4], value.getWeatherStationCity(), ser);
		TypeSerializer<Poco::Optional<WeatherWS::ArrayOfForecast>>::serialize(REMOTING__NAMES[5], value.getForecastResult(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_WeatherWS_ForecastReturn_INCLUDED

