//
// CheckVatServiceProxyFactory.cpp
//
// Package: Generated
// Module:  CheckVatServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "CheckVat/CheckVatServiceProxyFactory.h"


namespace CheckVat {


CheckVatServiceProxyFactory::CheckVatServiceProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


CheckVatServiceProxyFactory::~CheckVatServiceProxyFactory()
{
}


Poco::RemotingNG::Proxy* CheckVatServiceProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new CheckVat::CheckVatServiceProxy(oid);
}


} // namespace CheckVat

