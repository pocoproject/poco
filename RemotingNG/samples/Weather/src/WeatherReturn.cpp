// 
// WeatherReturn.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/WeatherReturn.h"


namespace WeatherWS {


WeatherReturn::WeatherReturn():
	_success(),
	_responseText(),
	_state(),
	_city(),
	_weatherStationCity(),
	_weatherID(),
	_description(),
	_temperature(),
	_relativeHumidity(),
	_wind(),
	_pressure(),
	_visibility(),
	_windChill(),
	_remarks()
{
}


WeatherReturn::WeatherReturn(bool success, const Poco::Optional<std::string>& responseText, const Poco::Optional<std::string>& state, const Poco::Optional<std::string>& city, const Poco::Optional<std::string>& weatherStationCity, Poco::Int16 weatherID, const Poco::Optional<std::string>& description, const Poco::Optional<std::string>& temperature, const Poco::Optional<std::string>& relativeHumidity, const Poco::Optional<std::string>& wind, const Poco::Optional<std::string>& pressure, const Poco::Optional<std::string>& visibility, const Poco::Optional<std::string>& windChill, const Poco::Optional<std::string>& remarks):
	_success(success),
	_responseText(responseText),
	_state(state),
	_city(city),
	_weatherStationCity(weatherStationCity),
	_weatherID(weatherID),
	_description(description),
	_temperature(temperature),
	_relativeHumidity(relativeHumidity),
	_wind(wind),
	_pressure(pressure),
	_visibility(visibility),
	_windChill(windChill),
	_remarks(remarks)
{
}


WeatherReturn::~WeatherReturn()
{
}


} // WeatherWS


