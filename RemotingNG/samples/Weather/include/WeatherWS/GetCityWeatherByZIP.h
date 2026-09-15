// 
// GetCityWeatherByZIP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_GetCityWeatherByZIP_INCLUDED
#define WeatherWS_GetCityWeatherByZIP_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace WeatherWS {


//@ name="#GetCityWeatherByZIP"
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class GetCityWeatherByZIP
{
public:
	GetCityWeatherByZIP();

	GetCityWeatherByZIP(const Poco::Optional<std::string>& zIP);

	virtual ~GetCityWeatherByZIP();

	const Poco::Optional<std::string>& getZIP() const;

	void setZIP(const Poco::Optional<std::string>& val);

private:
	//@ mandatory=false
	//@ name=ZIP
	//@ order=0
	Poco::Optional<std::string> _zIP;

};


inline const Poco::Optional<std::string>& GetCityWeatherByZIP::getZIP() const
{
	return _zIP;
}


inline void GetCityWeatherByZIP::setZIP(const Poco::Optional<std::string>& val)
{
	_zIP = val;
}


} // WeatherWS


#endif // WeatherWS_GetCityWeatherByZIP_INCLUDED
