//
// WeatherClientHelper.cpp
//
// Package: Generated
// Module:  WeatherClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WeatherWS/WeatherClientHelper.h"
#include "Poco/SingletonHolder.h"
#include "WeatherWS/WeatherProxyFactory.h"


namespace WeatherWS {


namespace
{
	Poco::SingletonHolder<WeatherClientHelper> shWeatherClientHelper;
}


WeatherClientHelper::WeatherClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("WeatherWS.Weather", new WeatherProxyFactory);
}


WeatherClientHelper::~WeatherClientHelper()
{
}


IWeather::Ptr WeatherClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IWeather>();
}


IWeather::Ptr WeatherClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IWeather::remoting__typeId(), protocol);
	return pInterface.cast<IWeather>();
}


WeatherClientHelper& WeatherClientHelper::instance()
{
	return *shWeatherClientHelper.get();
}


} // namespace WeatherWS

