//
// IWeather.cpp
//
// Package: Generated
// Module:  IWeather
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WeatherWS/IWeather.h"


namespace WeatherWS {


IWeather::IWeather():
	Poco::RefCountedObject()

{
}


IWeather::~IWeather()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IWeather::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("WeatherWS.Weather");
	return REMOTING__TYPE_ID;
}


} // namespace WeatherWS

