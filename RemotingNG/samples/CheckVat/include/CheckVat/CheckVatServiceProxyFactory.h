//
// CheckVatServiceProxyFactory.h
//
// Package: Generated
// Module:  CheckVatServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef CheckVatServiceProxyFactory_INCLUDED
#define CheckVatServiceProxyFactory_INCLUDED


#include "CheckVat/CheckVatServiceProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace CheckVat {


class CheckVatServiceProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	CheckVatServiceProxyFactory();
		/// Creates a CheckVatServiceProxyFactory.

	~CheckVatServiceProxyFactory();
		/// Destroys the CheckVatServiceProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace CheckVat


#endif // CheckVatServiceProxyFactory_INCLUDED

