//
// ConvertTemperatureProxyFactory.h
//
// Package: Generated
// Module:  ConvertTemperatureProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ConvertTemperatureProxyFactory_INCLUDED
#define ConvertTemperatureProxyFactory_INCLUDED


#include "Poco/RemotingNG/ProxyFactory.h"
#include "WebServiceX/ConvertTemperatureProxy.h"


namespace WebServiceX {


class ConvertTemperatureProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	ConvertTemperatureProxyFactory();
		/// Creates a ConvertTemperatureProxyFactory.

	~ConvertTemperatureProxyFactory();
		/// Destroys the ConvertTemperatureProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace WebServiceX


#endif // ConvertTemperatureProxyFactory_INCLUDED

