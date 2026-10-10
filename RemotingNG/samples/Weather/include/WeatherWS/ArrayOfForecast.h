// 
// ArrayOfForecast.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_ArrayOfForecast_INCLUDED
#define WeatherWS_ArrayOfForecast_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace WeatherWS {
class Forecast;
} 


namespace WeatherWS {


//@ name=ArrayOfForecast
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class ArrayOfForecast
{
public:
	ArrayOfForecast();

	ArrayOfForecast(const std::vector<Poco::SharedPtr<Forecast>>& forecast);

	virtual ~ArrayOfForecast();

	const std::vector<Poco::SharedPtr<Forecast>>& getForecast() const;

	std::vector<Poco::SharedPtr<Forecast>>& getForecast();

	void setForecast(const std::vector<Poco::SharedPtr<Forecast>>& val);

private:
	//@ mandatory=false
	//@ name=Forecast
	//@ order=0
	std::vector<Poco::SharedPtr<Forecast>> _forecast;

};


inline const std::vector<Poco::SharedPtr<Forecast>>& ArrayOfForecast::getForecast() const
{
	return _forecast;
}


inline std::vector<Poco::SharedPtr<Forecast>>& ArrayOfForecast::getForecast()
{
	return _forecast;
}


inline void ArrayOfForecast::setForecast(const std::vector<Poco::SharedPtr<Forecast>>& val)
{
	_forecast = val;
}


} // WeatherWS


// The following headers are required for template instantiation.
#include "WeatherWS/Forecast.h"


#endif // WeatherWS_ArrayOfForecast_INCLUDED
