//
// WeatherProxyFactory.cpp
//
// Package: Generated
// Module:  WeatherProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WeatherWS/WeatherProxyFactory.h"


namespace WeatherWS {


WeatherProxyFactory::WeatherProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


WeatherProxyFactory::~WeatherProxyFactory()
{
}


Poco::RemotingNG::Proxy* WeatherProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new WeatherWS::WeatherProxy(oid);
}


} // namespace WeatherWS

