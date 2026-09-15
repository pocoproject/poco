// 
// WeatherReturn.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_WeatherReturn_INCLUDED
#define WeatherWS_WeatherReturn_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>


namespace WeatherWS {


//@ name=WeatherReturn
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class WeatherReturn
{
public:
	WeatherReturn();

	WeatherReturn(
		bool success, 
		const Poco::Optional<std::string>& responseText, 
		const Poco::Optional<std::string>& state, 
		const Poco::Optional<std::string>& city, 
		const Poco::Optional<std::string>& weatherStationCity, 
		Poco::Int16 weatherID, 
		const Poco::Optional<std::string>& description, 
		const Poco::Optional<std::string>& temperature, 
		const Poco::Optional<std::string>& relativeHumidity, 
		const Poco::Optional<std::string>& wind, 
		const Poco::Optional<std::string>& pressure, 
		const Poco::Optional<std::string>& visibility, 
		const Poco::Optional<std::string>& windChill, 
		const Poco::Optional<std::string>& remarks);

	virtual ~WeatherReturn();

	const Poco::Optional<std::string>& getCity() const;

	const Poco::Optional<std::string>& getDescription() const;

	const Poco::Optional<std::string>& getPressure() const;

	const Poco::Optional<std::string>& getRelativeHumidity() const;

	const Poco::Optional<std::string>& getRemarks() const;

	const Poco::Optional<std::string>& getResponseText() const;

	const Poco::Optional<std::string>& getState() const;

	bool getSuccess() const;

	const Poco::Optional<std::string>& getTemperature() const;

	const Poco::Optional<std::string>& getVisibility() const;

	Poco::Int16 getWeatherID() const;

	const Poco::Optional<std::string>& getWeatherStationCity() const;

	const Poco::Optional<std::string>& getWind() const;

	const Poco::Optional<std::string>& getWindChill() const;

	void setCity(const Poco::Optional<std::string>& val);

	void setDescription(const Poco::Optional<std::string>& val);

	void setPressure(const Poco::Optional<std::string>& val);

	void setRelativeHumidity(const Poco::Optional<std::string>& val);

	void setRemarks(const Poco::Optional<std::string>& val);

	void setResponseText(const Poco::Optional<std::string>& val);

	void setState(const Poco::Optional<std::string>& val);

	void setSuccess(bool val);

	void setTemperature(const Poco::Optional<std::string>& val);

	void setVisibility(const Poco::Optional<std::string>& val);

	void setWeatherID(Poco::Int16 val);

	void setWeatherStationCity(const Poco::Optional<std::string>& val);

	void setWind(const Poco::Optional<std::string>& val);

	void setWindChill(const Poco::Optional<std::string>& val);

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

	//@ name=WeatherID
	//@ order=5
	Poco::Int16 _weatherID;

	//@ mandatory=false
	//@ name=Description
	//@ order=6
	Poco::Optional<std::string> _description;

	//@ mandatory=false
	//@ name=Temperature
	//@ order=7
	Poco::Optional<std::string> _temperature;

	//@ mandatory=false
	//@ name=RelativeHumidity
	//@ order=8
	Poco::Optional<std::string> _relativeHumidity;

	//@ mandatory=false
	//@ name=Wind
	//@ order=9
	Poco::Optional<std::string> _wind;

	//@ mandatory=false
	//@ name=Pressure
	//@ order=10
	Poco::Optional<std::string> _pressure;

	//@ mandatory=false
	//@ name=Visibility
	//@ order=11
	Poco::Optional<std::string> _visibility;

	//@ mandatory=false
	//@ name=WindChill
	//@ order=12
	Poco::Optional<std::string> _windChill;

	//@ mandatory=false
	//@ name=Remarks
	//@ order=13
	Poco::Optional<std::string> _remarks;

};


inline const Poco::Optional<std::string>& WeatherReturn::getCity() const
{
	return _city;
}


inline const Poco::Optional<std::string>& WeatherReturn::getDescription() const
{
	return _description;
}


inline const Poco::Optional<std::string>& WeatherReturn::getPressure() const
{
	return _pressure;
}


inline const Poco::Optional<std::string>& WeatherReturn::getRelativeHumidity() const
{
	return _relativeHumidity;
}


inline const Poco::Optional<std::string>& WeatherReturn::getRemarks() const
{
	return _remarks;
}


inline const Poco::Optional<std::string>& WeatherReturn::getResponseText() const
{
	return _responseText;
}


inline const Poco::Optional<std::string>& WeatherReturn::getState() const
{
	return _state;
}


inline bool WeatherReturn::getSuccess() const
{
	return _success;
}


inline const Poco::Optional<std::string>& WeatherReturn::getTemperature() const
{
	return _temperature;
}


inline const Poco::Optional<std::string>& WeatherReturn::getVisibility() const
{
	return _visibility;
}


inline Poco::Int16 WeatherReturn::getWeatherID() const
{
	return _weatherID;
}


inline const Poco::Optional<std::string>& WeatherReturn::getWeatherStationCity() const
{
	return _weatherStationCity;
}


inline const Poco::Optional<std::string>& WeatherReturn::getWind() const
{
	return _wind;
}


inline const Poco::Optional<std::string>& WeatherReturn::getWindChill() const
{
	return _windChill;
}


inline void WeatherReturn::setCity(const Poco::Optional<std::string>& val)
{
	_city = val;
}


inline void WeatherReturn::setDescription(const Poco::Optional<std::string>& val)
{
	_description = val;
}


inline void WeatherReturn::setPressure(const Poco::Optional<std::string>& val)
{
	_pressure = val;
}


inline void WeatherReturn::setRelativeHumidity(const Poco::Optional<std::string>& val)
{
	_relativeHumidity = val;
}


inline void WeatherReturn::setRemarks(const Poco::Optional<std::string>& val)
{
	_remarks = val;
}


inline void WeatherReturn::setResponseText(const Poco::Optional<std::string>& val)
{
	_responseText = val;
}


inline void WeatherReturn::setState(const Poco::Optional<std::string>& val)
{
	_state = val;
}


inline void WeatherReturn::setSuccess(bool val)
{
	_success = val;
}


inline void WeatherReturn::setTemperature(const Poco::Optional<std::string>& val)
{
	_temperature = val;
}


inline void WeatherReturn::setVisibility(const Poco::Optional<std::string>& val)
{
	_visibility = val;
}


inline void WeatherReturn::setWeatherID(Poco::Int16 val)
{
	_weatherID = val;
}


inline void WeatherReturn::setWeatherStationCity(const Poco::Optional<std::string>& val)
{
	_weatherStationCity = val;
}


inline void WeatherReturn::setWind(const Poco::Optional<std::string>& val)
{
	_wind = val;
}


inline void WeatherReturn::setWindChill(const Poco::Optional<std::string>& val)
{
	_windChill = val;
}


} // WeatherWS


#endif // WeatherWS_WeatherReturn_INCLUDED
