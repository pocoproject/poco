//
// ArrayOfWeatherDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_WeatherWS_ArrayOfWeatherDescription_INCLUDED
#define TypeSerializer_WeatherWS_ArrayOfWeatherDescription_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "WeatherWS/ArrayOfWeatherDescription.h"
#include "WeatherWS/WeatherDescriptionDeserializer.h"
#include "WeatherWS/WeatherDescriptionSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<WeatherWS::ArrayOfWeatherDescription>
{
public:
	static void serialize(const std::string& name, const WeatherWS::ArrayOfWeatherDescription& value, Serializer& ser)
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

	static void serializeImpl(const WeatherWS::ArrayOfWeatherDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"WeatherDescription"s,""s};
		TypeSerializer<std::vector<WeatherWS::WeatherDescription>>::serialize(REMOTING__NAMES[0], value.getWeatherDescription(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_WeatherWS_ArrayOfWeatherDescription_INCLUDED

