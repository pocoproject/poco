//
// TesterProxyFactory.cpp
//
// Package: Generated
// Module:  TesterProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "TesterProxyFactory.h"


TesterProxyFactory::TesterProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


TesterProxyFactory::~TesterProxyFactory()
{
}


Poco::RemotingNG::Proxy* TesterProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new TesterProxy(oid);
}


