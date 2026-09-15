//
// Listener.h
//
// Library: RemotingNG
// Package: Transport
// Module:  Listener
//
// Definition of the Listener class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Listener_INCLUDED
#define RemotingNG_Listener_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/Authenticator.h"
#include "Poco/RemotingNG/Authorizer.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Listener: public Poco::RefCountedObject
	/// A Listener waits for incoming requests and handles them
	/// by accepting them and passing them to the ORB for processing.
{
public:
	using Ptr = Poco::AutoPtr<Listener>;

	Listener(const std::string& endPoint);
		/// Creates a Listener. 
		///
		/// Takes a local endpoint address as input. For network based listener, this will
		/// typically be a port number or socket address in the form <host>:<port>.

	virtual ~Listener();
		/// Destroys the Listener.

	virtual void start() = 0;
		/// Start accepting requests.

	virtual void stop() = 0;
		/// Stop accepting requests.

	virtual const std::string& protocol() const = 0;
		/// Returns the ID of the Transport for which this Listener
		/// handles requests.

	const std::string& endPoint() const;
		/// Returns the local endpoint of the Listener, as given in the constructor.

	virtual std::string createURI(const Identifiable::TypeId& typeId, const Identifiable::ObjectId& objectId) = 0;
		/// Create an URI for accessing the object with the given TypeId and ObjectId through
		/// the Listener.
		///
		/// Must return a complete URI for the object. URI scheme and authority can be freely
		/// chosen by the Listener; however the URI path must be in format
		/// /<protocol>/<typeId>/<objectId>.

	virtual bool handlesURI(const std::string& uri) = 0;
		/// Check whether the given URI can be handled by this Listener instance.
		///
		/// This is used by the ORB to check whether a given remote service
		/// object identified by the URI is actually located on the same ORB.
		///
		/// In some cases, simply comparing the URI against all locally registered
		/// URIs does not work, as URI may be different even if they reference
		/// the same object. For example, the URIs http://localhost/a/b/c
		/// and http://127.0.0.1/a/b/c are different, but actually reference
		/// the same resource.
		///
		/// An implementation of this function should examine the authority part
		/// (host and port) of the URI and check if it is equivalent to the endpoint
		/// handled by the Listener.
		///
		/// A simple implementation can always return false. In this case, local
		/// objects will only be recognized if the URI exactly matches the URI
		/// created by the Listener when the object was registered.

	virtual void registerObject(RemoteObject::Ptr pRemoteObject, Skeleton::Ptr pSkeleton) = 0;
		/// Registers a RemoteObject and its Skeleton with the Listener. 
		///
		/// Depending on the protocol, the Listener can opt to let the ORB handle all invocations 
		/// (i.e. with a protocol that sends the full URI with each request), which allows 
		/// to implement a request handler that simply calls ORB::invoke(). In this case
		/// the Listener does not need to actually register the RemoteObject and Listener
		/// internally.
		/// 
		/// Note that registerObject() is called from inside the ORB, and thus is not allowed to 
		/// call back into the ORB (danger of dead-lock).

	virtual void unregisterObject(RemoteObject::Ptr pRemoteObject) = 0;
		/// Unregisters a RemoteObject from the Listener.
		
	virtual void setAuthenticator(Authenticator::Ptr pAuthenticator);
		/// Sets the Authenticator instance for this Listener.
		///
		/// An Authenticator should be set immediately after creating the 
		/// Listener, before registering it with the ORB and accepting
		/// requests.
		///
		/// Can be overridden by subclasses, but overriders should
		/// call the base class implementation.
		
	Authenticator::Ptr getAuthenticator() const;
		/// Returns the Authenticator instance for this Listener.

	virtual void setAuthorizer(Authorizer::Ptr pAuthorizer);
		/// Sets the Authorizer instance for this Listener.
		///
		/// An Authorizer should be set immediately after creating the 
		/// Listener, before registering it with the ORB and accepting
		/// requests.
		///
		/// Can be overridden by subclasses, but overriders should
		/// call the base class implementation.
		
	Authorizer::Ptr getAuthorizer() const;
		/// Returns the Authorizer instance for this Listener.

private:
	Listener(const Listener&);
	Listener& operator = (const Listener&);

private:
	std::string _endPoint;
	Authenticator::Ptr _pAuthenticator;
	Authorizer::Ptr _pAuthorizer;
};


//
// inlines
//
inline const std::string& Listener::endPoint() const
{
	return _endPoint;
}


inline Authenticator::Ptr Listener::getAuthenticator() const
{
	return _pAuthenticator;
}


inline Authorizer::Ptr Listener::getAuthorizer() const
{
	return _pAuthorizer;
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Listener_INCLUDED
