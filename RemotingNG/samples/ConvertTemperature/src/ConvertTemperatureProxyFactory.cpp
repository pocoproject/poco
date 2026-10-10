//
// ConvertTemperatureProxyFactory.cpp
//
// Package: Generated
// Module:  ConvertTemperatureProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WebServiceX/ConvertTemperatureProxyFactory.h"


namespace WebServiceX {


ConvertTemperatureProxyFactory::ConvertTemperatureProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


ConvertTemperatureProxyFactory::~ConvertTemperatureProxyFactory()
{
}


Poco::RemotingNG::Proxy* ConvertTemperatureProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new WebServiceX::ConvertTemperatureProxy(oid);
}


} // namespace WebServiceX

