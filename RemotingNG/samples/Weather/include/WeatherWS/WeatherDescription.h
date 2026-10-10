// 
// WeatherDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_WeatherDescription_INCLUDED
#define WeatherWS_WeatherDescription_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>


namespace WeatherWS {


//@ name=WeatherDescription
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class WeatherDescription
{
public:
	WeatherDescription();

	WeatherDescription(
		Poco::Int16 weatherID, 
		const Poco::Optional<std::string>& description, 
		const Poco::Optional<std::string>& pictureURL);

	virtual ~WeatherDescription();

	const Poco::Optional<std::string>& getDescription() const;

	const Poco::Optional<std::string>& getPictureURL() const;

	Poco::Int16 getWeatherID() const;

	void setDescription(const Poco::Optional<std::string>& val);

	void setPictureURL(const Poco::Optional<std::string>& val);

	void setWeatherID(Poco::Int16 val);

private:
	//@ name=WeatherID
	//@ order=0
	Poco::Int16 _weatherID;

	//@ mandatory=false
	//@ name=Description
	//@ order=1
	Poco::Optional<std::string> _description;

	//@ mandatory=false
	//@ name=PictureURL
	//@ order=2
	Poco::Optional<std::string> _pictureURL;

};


inline const Poco::Optional<std::string>& WeatherDescription::getDescription() const
{
	return _description;
}


inline const Poco::Optional<std::string>& WeatherDescription::getPictureURL() const
{
	return _pictureURL;
}


inline Poco::Int16 WeatherDescription::getWeatherID() const
{
	return _weatherID;
}


inline void WeatherDescription::setDescription(const Poco::Optional<std::string>& val)
{
	_description = val;
}


inline void WeatherDescription::setPictureURL(const Poco::Optional<std::string>& val)
{
	_pictureURL = val;
}


inline void WeatherDescription::setWeatherID(Poco::Int16 val)
{
	_weatherID = val;
}


} // WeatherWS


#endif // WeatherWS_WeatherDescription_INCLUDED
