//
// ArrayOfForecastSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_WeatherWS_ArrayOfForecast_INCLUDED
#define TypeSerializer_WeatherWS_ArrayOfForecast_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "WeatherWS/ArrayOfForecast.h"
#include "WeatherWS/ForecastDeserializer.h"
#include "WeatherWS/ForecastSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<WeatherWS::ArrayOfForecast>
{
public:
	static void serialize(const std::string& name, const WeatherWS::ArrayOfForecast& value, Serializer& ser)
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

	static void serializeImpl(const WeatherWS::ArrayOfForecast& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Forecast"s,""s};
		TypeSerializer<std::vector<Poco::SharedPtr<WeatherWS::Forecast>>>::serialize(REMOTING__NAMES[0], value.getForecast(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_WeatherWS_ArrayOfForecast_INCLUDED

