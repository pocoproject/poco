// 
// Weather.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_Weather_INCLUDED
#define WeatherWS_Weather_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "WeatherWS/ArrayOfWeatherDescription.h"
#include "WeatherWS/ForecastReturn.h"
#include "WeatherWS/WeatherReturn.h"
#include <string>


namespace WeatherWS {


//@ name=Weather
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ remote
class Weather
{
public:
	virtual ~Weather();

	//@ action="http://ws.cdyne.com/WeatherWS/GetCityForecastByZIP"
	//@ name=GetCityForecastByZIP
	//@ replyName=GetCityForecastByZIPResponse
	//@ $zIP={direction=in, name=ZIP}
	//@ $getCityForecastByZIPResult={direction=out, name=GetCityForecastByZIPResult}
	virtual void getCityForecastByZIP(
		const Poco::Optional<std::string>& zIP, 
		Poco::Optional<ForecastReturn>& getCityForecastByZIPResult) = 0;

	//@ action="http://ws.cdyne.com/WeatherWS/GetCityWeatherByZIP"
	//@ name=GetCityWeatherByZIP
	//@ replyName=GetCityWeatherByZIPResponse
	//@ $zIP={direction=in, name=ZIP}
	//@ $getCityWeatherByZIPResult={direction=out, name=GetCityWeatherByZIPResult}
	virtual void getCityWeatherByZIP(
		const Poco::Optional<std::string>& zIP, 
		WeatherReturn& getCityWeatherByZIPResult) = 0;

	//@ action="http://ws.cdyne.com/WeatherWS/GetWeatherInformation"
	//@ name=GetWeatherInformation
	//@ replyName=GetWeatherInformationResponse
	//@ $getWeatherInformationResult={direction=out, name=GetWeatherInformationResult}
	virtual void getWeatherInformation(Poco::Optional<ArrayOfWeatherDescription>& getWeatherInformationResult) = 0;
};


} // WeatherWS


#endif // WeatherWS_Weather_INCLUDED
