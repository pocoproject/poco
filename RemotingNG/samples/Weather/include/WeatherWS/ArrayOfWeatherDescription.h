// 
// ArrayOfWeatherDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_ArrayOfWeatherDescription_INCLUDED
#define WeatherWS_ArrayOfWeatherDescription_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "WeatherWS/WeatherDescription.h"
#include <vector>


namespace WeatherWS {


//@ name=ArrayOfWeatherDescription
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class ArrayOfWeatherDescription
{
public:
	ArrayOfWeatherDescription();

	ArrayOfWeatherDescription(const std::vector<WeatherDescription>& weatherDescription);

	virtual ~ArrayOfWeatherDescription();

	const std::vector<WeatherDescription>& getWeatherDescription() const;

	std::vector<WeatherDescription>& getWeatherDescription();

	void setWeatherDescription(const std::vector<WeatherDescription>& val);

private:
	//@ mandatory=false
	//@ name=WeatherDescription
	//@ order=0
	std::vector<WeatherDescription> _weatherDescription;

};


inline const std::vector<WeatherDescription>& ArrayOfWeatherDescription::getWeatherDescription() const
{
	return _weatherDescription;
}


inline std::vector<WeatherDescription>& ArrayOfWeatherDescription::getWeatherDescription()
{
	return _weatherDescription;
}


inline void ArrayOfWeatherDescription::setWeatherDescription(const std::vector<WeatherDescription>& val)
{
	_weatherDescription = val;
}


} // WeatherWS


#endif // WeatherWS_ArrayOfWeatherDescription_INCLUDED
