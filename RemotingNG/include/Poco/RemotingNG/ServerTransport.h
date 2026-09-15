//
// ServerTransport.h
//
// Library: RemotingNG
// Package: Transport
// Module:  ServerTransport
//
// Definition of the ServerTransport class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_ServerTransport_INCLUDED
#define RemotingNG_ServerTransport_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/SerializerBase.h"
#include "Poco/RemotingNG/AttributedObject.h"


namespace Poco {
namespace RemotingNG {


class Serializer;
class Deserializer;


class RemotingNG_API ServerTransport: public AttributedObject
	/// ServerTransport objects are used as mediators between
	/// a Listener, receiving a request, and the ORB, Skeleton and
	/// MethodHandlers, processing the request.
	///
	/// The ServerTransport allows the Skeleton to deserialize the
	/// request message in order to determine the method name,
	/// select an appropriate MethodHandler, based on the method
	/// name and pass control to the MethodHandler. The
	/// MethodHandler then uses the ServerTransport to send
	/// back a response message.
{
public:
	ServerTransport();
		/// Creates a ServerTransport.

	virtual ~ServerTransport();
		/// Destroys the ServerTransport.

	virtual bool authenticate(const std::string& method);
		/// Verify that the request is properly authenticated.
		///
		/// This method is called if the method or its class has
		/// the @authenticated attribute set to true, or if it
		/// has a @permission attribute set to a non-empty permission.
		///
		/// An implementation should check whether the current request
		/// contains valid authentication information.
		/// Typically, an implementation will obtain the Credentials object
		/// for the request, obtain the Authenticator from the Listener,
		/// and call its authenticate() method to check the credentials.
		///
		/// Returns true if the request is properly authenticated, otherwise
		/// false.
		///
		/// The default implementation simply returns true.

	virtual bool authorize(const std::string& method, const std::string& permission);
		/// Verify that an authenticated user has the given permission, which is required
		/// to invoke the given method.
		///
		/// The given permission will be the the one specified with the @permission
		/// attribute for the respective remote method. If no @permission attribute
		/// has been specified for a remote method, or the permission is an empty
		/// string, this method will not be called.
		///
		/// An implementation should obtain the Authorizer object from the Listener,
		/// and call the Authorizer's authorize() method to authorize the
		/// method call.
		///
		/// Returns true if authorization is successful, otherwise false.
		///
		/// The default implementation simply returns true.

	virtual Deserializer& beginRequest() = 0;
		/// Prepare a Deserializer for reading the request message.
		///
		/// Returns a Deserializer that can be used to read the
		/// request message.

	virtual Serializer& sendReply(SerializerBase::MessageType messageType) = 0;
		/// Prepares the ServerTransport to send a response message back to
		/// the caller.
		///
		/// The given MessageType must be either MESSAGE_REPLY, MESSAGE_EVENT_REPLY
		/// or MESSAGE_FAULT.
		///
		/// Returns a Serializer for writing the reply (or fault reply) message.

	virtual void endRequest() = 0;
		/// Signals the ServerTransport that request processing is
		/// finished for this request.

	virtual void reportException(const std::string& method, const Poco::Exception& exc);
		/// Reports an exception thrown by the service method
		/// invoked by the generated Skeleton and MethodHandler.
		///
		/// The default implementation does nothing.
		/// Subclasses can override this method to log the
		/// exception for diagnostic purposes.

private:
	ServerTransport(const ServerTransport&);
	ServerTransport& operator = (const ServerTransport&);
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Transport_INCLUDED
