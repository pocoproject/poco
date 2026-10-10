//
// ArrayOfWeatherDescriptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_ArrayOfWeatherDescription_INCLUDED
#define TypeDeserializer_WeatherWS_ArrayOfWeatherDescription_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/ArrayOfWeatherDescription.h"
#include "WeatherWS/WeatherDescriptionDeserializer.h"
#include "WeatherWS/WeatherDescriptionSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::ArrayOfWeatherDescription>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::ArrayOfWeatherDescription& value)
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::ArrayOfWeatherDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"WeatherDescription"s};
		bool ret = false;
		std::vector<WeatherWS::WeatherDescription> gen_weatherDescription;
		ret = TypeDeserializer<std::vector<WeatherWS::WeatherDescription>>::deserialize(REMOTING__NAMES[0], false, deser, gen_weatherDescription);
		if (ret) value.setWeatherDescription(gen_weatherDescription);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_ArrayOfWeatherDescription_INCLUDED

