//
// TesterProxyFactory.h
//
// Package: Generated
// Module:  TesterProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterProxyFactory_INCLUDED
#define TesterProxyFactory_INCLUDED


#include "Poco/RemotingNG/ProxyFactory.h"
#include "TesterProxy.h"


class TesterProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	TesterProxyFactory();
		/// Creates a TesterProxyFactory.

	~TesterProxyFactory();
		/// Destroys the TesterProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};




#endif // TesterProxyFactory_INCLUDED

