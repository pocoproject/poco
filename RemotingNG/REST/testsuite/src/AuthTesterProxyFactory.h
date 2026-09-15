//
// AuthTesterProxyFactory.h
//
// Package: Generated
// Module:  AuthTesterProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef AuthTesterProxyFactory_INCLUDED
#define AuthTesterProxyFactory_INCLUDED


#include "AuthTesterProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


class AuthTesterProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	AuthTesterProxyFactory();
		/// Creates a AuthTesterProxyFactory.

	~AuthTesterProxyFactory();
		/// Destroys the AuthTesterProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};




#endif // AuthTesterProxyFactory_INCLUDED

