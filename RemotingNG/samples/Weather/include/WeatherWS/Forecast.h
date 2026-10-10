// 
// Forecast.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_Forecast_INCLUDED
#define WeatherWS_Forecast_INCLUDED


#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include "WeatherWS/POP.h"
#include "WeatherWS/Temp.h"
#include <string>


namespace WeatherWS {


//@ name=Forecast
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class Forecast
{
public:
	Forecast();

	Forecast(
		const Poco::DateTime& date, 
		Poco::Int16 weatherID, 
		const Poco::Optional<std::string>& desciption, 
		const Temp& temperatures, 
		const POP& probabilityOfPrecipiation);

	virtual ~Forecast();

	const Poco::DateTime& getDate() const;

	const Poco::Optional<std::string>& getDesciption() const;

	const POP& getProbabilityOfPrecipiation() const;

	const Temp& getTemperatures() const;

	Poco::Int16 getWeatherID() const;

	void setDate(const Poco::DateTime& val);

	void setDesciption(const Poco::Optional<std::string>& val);

	void setProbabilityOfPrecipiation(const POP& val);

	void setTemperatures(const Temp& val);

	void setWeatherID(Poco::Int16 val);

private:
	//@ name=Date
	//@ order=0
	Poco::DateTime _date;

	//@ name=WeatherID
	//@ order=1
	Poco::Int16 _weatherID;

	//@ mandatory=false
	//@ name=Desciption
	//@ order=2
	Poco::Optional<std::string> _desciption;

	//@ name=Temperatures
	//@ order=3
	Temp _temperatures;

	//@ name=ProbabilityOfPrecipiation
	//@ order=4
	POP _probabilityOfPrecipiation;

};


inline const Poco::DateTime& Forecast::getDate() const
{
	return _date;
}


inline const Poco::Optional<std::string>& Forecast::getDesciption() const
{
	return _desciption;
}


inline const POP& Forecast::getProbabilityOfPrecipiation() const
{
	return _probabilityOfPrecipiation;
}


inline const Temp& Forecast::getTemperatures() const
{
	return _temperatures;
}


inline Poco::Int16 Forecast::getWeatherID() const
{
	return _weatherID;
}


inline void Forecast::setDate(const Poco::DateTime& val)
{
	_date = val;
}


inline void Forecast::setDesciption(const Poco::Optional<std::string>& val)
{
	_desciption = val;
}


inline void Forecast::setProbabilityOfPrecipiation(const POP& val)
{
	_probabilityOfPrecipiation = val;
}


inline void Forecast::setTemperatures(const Temp& val)
{
	_temperatures = val;
}


inline void Forecast::setWeatherID(Poco::Int16 val)
{
	_weatherID = val;
}


} // WeatherWS


#endif // WeatherWS_Forecast_INCLUDED
