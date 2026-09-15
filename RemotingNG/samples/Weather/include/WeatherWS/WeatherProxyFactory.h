//
// WeatherProxyFactory.h
//
// Package: Generated
// Module:  WeatherProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef WeatherProxyFactory_INCLUDED
#define WeatherProxyFactory_INCLUDED


#include "Poco/RemotingNG/ProxyFactory.h"
#include "WeatherWS/WeatherProxy.h"


namespace WeatherWS {


class WeatherProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	WeatherProxyFactory();
		/// Creates a WeatherProxyFactory.

	~WeatherProxyFactory();
		/// Destroys the WeatherProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace WeatherWS


#endif // WeatherProxyFactory_INCLUDED

