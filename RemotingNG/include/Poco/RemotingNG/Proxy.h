//
// Proxy.h
//
// Library: RemotingNG
// Package: ORB
// Module:  Proxy
//
// Definition of the Proxy class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Proxy_INCLUDED
#define RemotingNG_Proxy_INCLUDED


#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Proxy: public Identifiable
	/// A Proxy maintains a connection to a remote endpoint
	/// (using a Transport) and sends method invocations
	/// to a remote object, using the connection.
	///
	/// The RemotingNG code generator will generate subclasses
	/// of Proxy creating appropriate serialization and deserialization
	/// code for all service object methods.
{
public:
	using Ptr = Poco::AutoPtr<Proxy>;

	Proxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a Proxy.

	virtual ~Proxy();
		/// Destroys the Proxy.

	void remoting__connect(const std::string& protocol, const std::string& uri);
		/// Connects the Proxy to the endpoint specified by the
		/// given URI, using the Transport for the given protocol.

	void remoting__disconnect();
		/// Disconnects the Proxy from the endpoint.

	bool remoting__isConnected() const;
		/// Returns true if the proxy is connected to an endpoint, false otherwise.

	Transport& remoting__transport() const;
		/// Returns the Transport used for the connection to the endpoint.
		///
		/// Throws an exception if the Proxy is not connected to an endpoint.

	void remoting__setEventURI(const Poco::URI& eventURI);
		/// Sets the URI to be used for sending event subscription and
		/// unsubscription requests.
		
	const Poco::URI& remoting__getEventURI() const;
		/// Returns the URI to be used for sending event subscription and
		/// unsubscription requests.

protected:
	Poco::FastMutex& remoting__mutex() const;
		/// Returns the Proxy's internal mutex.

private:
	mutable Transport::Ptr _pTransport;
	Poco::URI _eventURI;
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//
inline bool Proxy::remoting__isConnected() const
{
	return _pTransport && _pTransport->connected();
}


inline Transport& Proxy::remoting__transport() const
{
	return *_pTransport;
}


inline Poco::FastMutex& Proxy::remoting__mutex() const
{
	return _mutex;
}


inline const Poco::URI& Proxy::remoting__getEventURI() const
{
	return _eventURI;
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Proxy_INCLUDED
