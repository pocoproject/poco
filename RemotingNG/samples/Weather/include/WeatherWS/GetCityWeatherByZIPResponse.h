// 
// GetCityWeatherByZIPResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_GetCityWeatherByZIPResponse_INCLUDED
#define WeatherWS_GetCityWeatherByZIPResponse_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "WeatherWS/WeatherReturn.h"


namespace WeatherWS {


//@ name="#GetCityWeatherByZIPResponse"
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class GetCityWeatherByZIPResponse
{
public:
	GetCityWeatherByZIPResponse();

	GetCityWeatherByZIPResponse(const WeatherReturn& getCityWeatherByZIPResult);

	virtual ~GetCityWeatherByZIPResponse();

	const WeatherReturn& getGetCityWeatherByZIPResult() const;

	void setGetCityWeatherByZIPResult(const WeatherReturn& val);

private:
	//@ name=GetCityWeatherByZIPResult
	//@ order=0
	WeatherReturn _getCityWeatherByZIPResult;

};


inline const WeatherReturn& GetCityWeatherByZIPResponse::getGetCityWeatherByZIPResult() const
{
	return _getCityWeatherByZIPResult;
}


inline void GetCityWeatherByZIPResponse::setGetCityWeatherByZIPResult(const WeatherReturn& val)
{
	_getCityWeatherByZIPResult = val;
}


} // WeatherWS


#endif // WeatherWS_GetCityWeatherByZIPResponse_INCLUDED
