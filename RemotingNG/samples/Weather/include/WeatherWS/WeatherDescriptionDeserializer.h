//
// WeatherDescriptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_WeatherDescription_INCLUDED
#define TypeDeserializer_WeatherWS_WeatherDescription_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/WeatherDescription.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::WeatherDescription>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::WeatherDescription& value)
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::WeatherDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"WeatherID"s,"Description"s,"PictureURL"s};
		bool ret = false;
		Poco::Int16 gen_weatherID;
		ret = TypeDeserializer<Poco::Int16>::deserialize(REMOTING__NAMES[0], true, deser, gen_weatherID);
		if (ret) value.setWeatherID(gen_weatherID);
		Poco::Optional<std::string> gen_description;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_description);
		if (ret) value.setDescription(gen_description);
		Poco::Optional<std::string> gen_pictureURL;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_pictureURL);
		if (ret) value.setPictureURL(gen_pictureURL);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_WeatherDescription_INCLUDED

