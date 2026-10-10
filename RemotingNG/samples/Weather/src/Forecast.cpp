// 
// Forecast.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/Forecast.h"


namespace WeatherWS {


Forecast::Forecast():
	_date(),
	_weatherID(),
	_desciption(),
	_temperatures(),
	_probabilityOfPrecipiation()
{
}


Forecast::Forecast(const Poco::DateTime& date, Poco::Int16 weatherID, const Poco::Optional<std::string>& desciption, const Temp& temperatures, const POP& probabilityOfPrecipiation):
	_date(date),
	_weatherID(weatherID),
	_desciption(desciption),
	_temperatures(temperatures),
	_probabilityOfPrecipiation(probabilityOfPrecipiation)
{
}


Forecast::~Forecast()
{
}


} // WeatherWS


