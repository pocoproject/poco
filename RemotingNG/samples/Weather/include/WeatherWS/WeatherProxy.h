//
// WeatherProxy.h
//
// Package: Generated
// Module:  WeatherProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef WeatherProxy_INCLUDED
#define WeatherProxy_INCLUDED


#include "Poco/RemotingNG/Proxy.h"
#include "WeatherWS/IWeather.h"


namespace WeatherWS {


class WeatherProxy: public WeatherWS::IWeather, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<WeatherProxy>;

	WeatherProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a WeatherProxy.

	virtual ~WeatherProxy();
		/// Destroys the WeatherProxy.

	virtual void getCityForecastByZIP(const Poco::Optional<std::string>& zIP, Poco::Optional<WeatherWS::ForecastReturn>& getCityForecastByZIPResult);

	virtual void getCityWeatherByZIP(const Poco::Optional<std::string>& zIP, WeatherWS::WeatherReturn& getCityWeatherByZIPResult);

	virtual void getWeatherInformation(Poco::Optional<WeatherWS::ArrayOfWeatherDescription>& getWeatherInformationResult);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& WeatherProxy::remoting__typeId() const
{
	return IWeather::remoting__typeId();
}


} // namespace WeatherWS


#endif // WeatherProxy_INCLUDED

