// 
// ArrayOfForecast.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/ArrayOfForecast.h"


namespace WeatherWS {


ArrayOfForecast::ArrayOfForecast():
	_forecast()
{
}


ArrayOfForecast::ArrayOfForecast(const std::vector<Poco::SharedPtr<Forecast>>& forecast):
	_forecast(forecast)
{
}


ArrayOfForecast::~ArrayOfForecast()
{
}


} // WeatherWS


