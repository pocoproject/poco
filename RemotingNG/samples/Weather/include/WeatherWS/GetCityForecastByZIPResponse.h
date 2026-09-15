// 
// GetCityForecastByZIPResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_GetCityForecastByZIPResponse_INCLUDED
#define WeatherWS_GetCityForecastByZIPResponse_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "WeatherWS/ForecastReturn.h"


namespace WeatherWS {


//@ name="#GetCityForecastByZIPResponse"
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class GetCityForecastByZIPResponse
{
public:
	GetCityForecastByZIPResponse();

	GetCityForecastByZIPResponse(const Poco::Optional<ForecastReturn>& getCityForecastByZIPResult);

	virtual ~GetCityForecastByZIPResponse();

	const Poco::Optional<ForecastReturn>& getGetCityForecastByZIPResult() const;

	void setGetCityForecastByZIPResult(const Poco::Optional<ForecastReturn>& val);

private:
	//@ mandatory=false
	//@ name=GetCityForecastByZIPResult
	//@ order=0
	Poco::Optional<ForecastReturn> _getCityForecastByZIPResult;

};


inline const Poco::Optional<ForecastReturn>& GetCityForecastByZIPResponse::getGetCityForecastByZIPResult() const
{
	return _getCityForecastByZIPResult;
}


inline void GetCityForecastByZIPResponse::setGetCityForecastByZIPResult(const Poco::Optional<ForecastReturn>& val)
{
	_getCityForecastByZIPResult = val;
}


} // WeatherWS


#endif // WeatherWS_GetCityForecastByZIPResponse_INCLUDED
