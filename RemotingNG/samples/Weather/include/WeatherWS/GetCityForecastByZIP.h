// 
// GetCityForecastByZIP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_GetCityForecastByZIP_INCLUDED
#define WeatherWS_GetCityForecastByZIP_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace WeatherWS {


//@ name="#GetCityForecastByZIP"
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class GetCityForecastByZIP
{
public:
	GetCityForecastByZIP();

	GetCityForecastByZIP(const Poco::Optional<std::string>& zIP);

	virtual ~GetCityForecastByZIP();

	const Poco::Optional<std::string>& getZIP() const;

	void setZIP(const Poco::Optional<std::string>& val);

private:
	//@ mandatory=false
	//@ name=ZIP
	//@ order=0
	Poco::Optional<std::string> _zIP;

};


inline const Poco::Optional<std::string>& GetCityForecastByZIP::getZIP() const
{
	return _zIP;
}


inline void GetCityForecastByZIP::setZIP(const Poco::Optional<std::string>& val)
{
	_zIP = val;
}


} // WeatherWS


#endif // WeatherWS_GetCityForecastByZIP_INCLUDED
