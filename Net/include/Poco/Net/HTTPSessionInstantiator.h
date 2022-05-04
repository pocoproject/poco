//
// HTTPSessionInstantiator.h
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPSessionInstantiator
//
// Definition of the HTTPSessionInstantiator class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPSessionInstantiator_INCLUDED
#define Net_HTTPSessionInstantiator_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/URI.h"


namespace Poco {
namespace Net {


class Net_API HTTPSessionInstantiator
	/// A factory for HTTPClientSession objects.
	///
	/// Creates a HTTP session for a given URI.
	/// A HTTPSessionInstantiator is not used directly.
	/// Instances are registered with a HTTPSessionFactory,
	/// and used through it.
{
public:
	HTTPSessionInstantiator();
		/// Creates the HTTPSessionInstantiator.

	virtual ~HTTPSessionInstantiator();
		/// Destroys the HTTPSessionInstantiator.

	virtual HTTPClientSession* createClientSession(const Poco::URI& uri);
		/// Creates a HTTPClientSession for the given URI.

	static void registerInstantiator();
		/// Registers the instantiator with the global HTTPSessionFactory.

	static void unregisterInstantiator();
		/// Unregisters the factory with the global HTTPSessionFactory.

protected:
	void setProxyConfig(const HTTPClientSession::ProxyConfig& proxyConfig);
		/// Sets the proxy configuration.

	const HTTPClientSession::ProxyConfig& getProxyConfig() const;
		/// Returns the proxy configuration.

private:
	HTTPClientSession::ProxyConfig _proxyConfig;

	friend class HTTPSessionFactory;
};


//
// inlines
//
inline const HTTPClientSession::ProxyConfig& HTTPSessionInstantiator::getProxyConfig() const
{
	return _proxyConfig;
}


} } // namespace Poco::Net


#endif // Net_HTTPSessionInstantiator_INCLUDED
