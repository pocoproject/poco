// 
// WeatherDescription.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/WeatherDescription.h"


namespace WeatherWS {


WeatherDescription::WeatherDescription():
	_weatherID(),
	_description(),
	_pictureURL()
{
}


WeatherDescription::WeatherDescription(Poco::Int16 weatherID, const Poco::Optional<std::string>& description, const Poco::Optional<std::string>& pictureURL):
	_weatherID(weatherID),
	_description(description),
	_pictureURL(pictureURL)
{
}


WeatherDescription::~WeatherDescription()
{
}


} // WeatherWS


