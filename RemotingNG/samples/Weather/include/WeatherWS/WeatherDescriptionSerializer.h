//
// WeatherDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_WeatherWS_WeatherDescription_INCLUDED
#define TypeSerializer_WeatherWS_WeatherDescription_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "WeatherWS/WeatherDescription.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<WeatherWS::WeatherDescription>
{
public:
	static void serialize(const std::string& name, const WeatherWS::WeatherDescription& value, Serializer& ser)
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

	static void serializeImpl(const WeatherWS::WeatherDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"WeatherID"s,"Description"s,"PictureURL"s,""s};
		TypeSerializer<Poco::Int16>::serialize(REMOTING__NAMES[0], value.getWeatherID(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getDescription(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getPictureURL(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_WeatherWS_WeatherDescription_INCLUDED

