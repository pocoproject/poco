// 
// ForecastReturn.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/ForecastReturn.h"


namespace WeatherWS {


ForecastReturn::ForecastReturn():
	_success(),
	_responseText(),
	_state(),
	_city(),
	_weatherStationCity(),
	_forecastResult()
{
}


ForecastReturn::ForecastReturn(bool success, const Poco::Optional<std::string>& responseText, const Poco::Optional<std::string>& state, const Poco::Optional<std::string>& city, const Poco::Optional<std::string>& weatherStationCity, const Poco::Optional<ArrayOfForecast>& forecastResult):
	_success(success),
	_responseText(responseText),
	_state(state),
	_city(city),
	_weatherStationCity(weatherStationCity),
	_forecastResult(forecastResult)
{
}


ForecastReturn::~ForecastReturn()
{
}


} // WeatherWS


