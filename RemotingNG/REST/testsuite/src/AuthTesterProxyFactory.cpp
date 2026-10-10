//
// AuthTesterProxyFactory.cpp
//
// Package: Generated
// Module:  AuthTesterProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "AuthTesterProxyFactory.h"


AuthTesterProxyFactory::AuthTesterProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


AuthTesterProxyFactory::~AuthTesterProxyFactory()
{
}


Poco::RemotingNG::Proxy* AuthTesterProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new AuthTesterProxy(oid);
}


