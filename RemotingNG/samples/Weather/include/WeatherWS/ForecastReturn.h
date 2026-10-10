// 
// ForecastReturn.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_ForecastReturn_INCLUDED
#define WeatherWS_ForecastReturn_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "WeatherWS/ArrayOfForecast.h"
#include <string>


namespace WeatherWS {


//@ name=ForecastReturn
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class ForecastReturn
{
public:
	ForecastReturn();

	ForecastReturn(
		bool success, 
		const Poco::Optional<std::string>& responseText, 
		const Poco::Optional<std::string>& state, 
		const Poco::Optional<std::string>& city, 
		const Poco::Optional<std::string>& weatherStationCity, 
		const Poco::Optional<ArrayOfForecast>& forecastResult);

	virtual ~ForecastReturn();

	const Poco::Optional<std::string>& getCity() const;

	const Poco::Optional<ArrayOfForecast>& getForecastResult() const;

	const Poco::Optional<std::string>& getResponseText() const;

	const Poco::Optional<std::string>& getState() const;

	bool getSuccess() const;

	const Poco::Optional<std::string>& getWeatherStationCity() const;

	void setCity(const Poco::Optional<std::string>& val);

	void setForecastResult(const Poco::Optional<ArrayOfForecast>& val);

	void setResponseText(const Poco::Optional<std::string>& val);

	void setState(const Poco::Optional<std::string>& val);

	void setSuccess(bool val);

	void setWeatherStationCity(const Poco::Optional<std::string>& val);

private:
	//@ name=Success
	//@ order=0
	bool _success;

	//@ mandatory=false
	//@ name=ResponseText
	//@ order=1
	Poco::Optional<std::string> _responseText;

	//@ mandatory=false
	//@ name=State
	//@ order=2
	Poco::Optional<std::string> _state;

	//@ mandatory=false
	//@ name=City
	//@ order=3
	Poco::Optional<std::string> _city;

	//@ mandatory=false
	//@ name=WeatherStationCity
	//@ order=4
	Poco::Optional<std::string> _weatherStationCity;

	//@ mandatory=false
	//@ name=ForecastResult
	//@ order=5
	Poco::Optional<ArrayOfForecast> _forecastResult;

};


inline const Poco::Optional<std::string>& ForecastReturn::getCity() const
{
	return _city;
}


inline const Poco::Optional<ArrayOfForecast>& ForecastReturn::getForecastResult() const
{
	return _forecastResult;
}


inline const Poco::Optional<std::string>& ForecastReturn::getResponseText() const
{
	return _responseText;
}


inline const Poco::Optional<std::string>& ForecastReturn::getState() const
{
	return _state;
}


inline bool ForecastReturn::getSuccess() const
{
	return _success;
}


inline const Poco::Optional<std::string>& ForecastReturn::getWeatherStationCity() const
{
	return _weatherStationCity;
}


inline void ForecastReturn::setCity(const Poco::Optional<std::string>& val)
{
	_city = val;
}


inline void ForecastReturn::setForecastResult(const Poco::Optional<ArrayOfForecast>& val)
{
	_forecastResult = val;
}


inline void ForecastReturn::setResponseText(const Poco::Optional<std::string>& val)
{
	_responseText = val;
}


inline void ForecastReturn::setState(const Poco::Optional<std::string>& val)
{
	_state = val;
}


inline void ForecastReturn::setSuccess(bool val)
{
	_success = val;
}


inline void ForecastReturn::setWeatherStationCity(const Poco::Optional<std::string>& val)
{
	_weatherStationCity = val;
}


} // WeatherWS


#endif // WeatherWS_ForecastReturn_INCLUDED
