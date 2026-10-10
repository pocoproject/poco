// 
// GetWeatherInformationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_GetWeatherInformationResponse_INCLUDED
#define WeatherWS_GetWeatherInformationResponse_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "WeatherWS/ArrayOfWeatherDescription.h"


namespace WeatherWS {


//@ name="#GetWeatherInformationResponse"
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class GetWeatherInformationResponse
{
public:
	GetWeatherInformationResponse();

	GetWeatherInformationResponse(const Poco::Optional<ArrayOfWeatherDescription>& getWeatherInformationResult);

	virtual ~GetWeatherInformationResponse();

	const Poco::Optional<ArrayOfWeatherDescription>& getGetWeatherInformationResult() const;

	void setGetWeatherInformationResult(const Poco::Optional<ArrayOfWeatherDescription>& val);

private:
	//@ mandatory=false
	//@ name=GetWeatherInformationResult
	//@ order=0
	Poco::Optional<ArrayOfWeatherDescription> _getWeatherInformationResult;

};


inline const Poco::Optional<ArrayOfWeatherDescription>& GetWeatherInformationResponse::getGetWeatherInformationResult() const
{
	return _getWeatherInformationResult;
}


inline void GetWeatherInformationResponse::setGetWeatherInformationResult(const Poco::Optional<ArrayOfWeatherDescription>& val)
{
	_getWeatherInformationResult = val;
}


} // WeatherWS


#endif // WeatherWS_GetWeatherInformationResponse_INCLUDED
