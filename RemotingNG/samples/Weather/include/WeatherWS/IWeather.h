//
// IWeather.h
//
// Package: Generated
// Module:  IWeather
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef IWeather_INCLUDED
#define IWeather_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "WeatherWS/Weather.h"


namespace WeatherWS {


class IWeather: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IWeather>;

	IWeather();
		/// Creates a IWeather.

	virtual ~IWeather();
		/// Destroys the IWeather.

	virtual void getCityForecastByZIP(const Poco::Optional<std::string>& zIP, Poco::Optional<WeatherWS::ForecastReturn>& getCityForecastByZIPResult) = 0;

	virtual void getCityWeatherByZIP(const Poco::Optional<std::string>& zIP, WeatherWS::WeatherReturn& getCityWeatherByZIPResult) = 0;

	virtual void getWeatherInformation(Poco::Optional<WeatherWS::ArrayOfWeatherDescription>& getWeatherInformationResult) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace WeatherWS


#endif // IWeather_INCLUDED

