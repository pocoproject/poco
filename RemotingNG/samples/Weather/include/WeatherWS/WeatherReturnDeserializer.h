//
// WeatherReturnDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_WeatherWS_WeatherReturn_INCLUDED
#define TypeDeserializer_WeatherWS_WeatherReturn_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "WeatherWS/WeatherReturn.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<WeatherWS::WeatherReturn>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, WeatherWS::WeatherReturn& value)
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

	static void deserializeImpl(Deserializer& deser, WeatherWS::WeatherReturn& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Success"s,"ResponseText"s,"State"s,"City"s,"WeatherStationCity"s,"WeatherID"s,"Description"s,"Temperature"s,"RelativeHumidity"s,"Wind"s,"Pressure"s,"Visibility"s,"WindChill"s,"Remarks"s};
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
		Poco::Int16 gen_weatherID;
		ret = TypeDeserializer<Poco::Int16>::deserialize(REMOTING__NAMES[5], true, deser, gen_weatherID);
		if (ret) value.setWeatherID(gen_weatherID);
		Poco::Optional<std::string> gen_description;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[6], false, deser, gen_description);
		if (ret) value.setDescription(gen_description);
		Poco::Optional<std::string> gen_temperature;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[7], false, deser, gen_temperature);
		if (ret) value.setTemperature(gen_temperature);
		Poco::Optional<std::string> gen_relativeHumidity;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[8], false, deser, gen_relativeHumidity);
		if (ret) value.setRelativeHumidity(gen_relativeHumidity);
		Poco::Optional<std::string> gen_wind;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[9], false, deser, gen_wind);
		if (ret) value.setWind(gen_wind);
		Poco::Optional<std::string> gen_pressure;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[10], false, deser, gen_pressure);
		if (ret) value.setPressure(gen_pressure);
		Poco::Optional<std::string> gen_visibility;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[11], false, deser, gen_visibility);
		if (ret) value.setVisibility(gen_visibility);
		Poco::Optional<std::string> gen_windChill;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[12], false, deser, gen_windChill);
		if (ret) value.setWindChill(gen_windChill);
		Poco::Optional<std::string> gen_remarks;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[13], false, deser, gen_remarks);
		if (ret) value.setRemarks(gen_remarks);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_WeatherWS_WeatherReturn_INCLUDED

