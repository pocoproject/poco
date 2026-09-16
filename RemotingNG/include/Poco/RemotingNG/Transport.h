//
// Transport.h
//
// Library: RemotingNG
// Package: Transport
// Module:  Transport
//
// Definition of the Transport class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Transport_INCLUDED
#define RemotingNG_Transport_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/AttributedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/SerializerBase.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include <map>


namespace Poco {
namespace RemotingNG {


class Serializer;
class Deserializer;


class RemotingNG_API Transport: public AttributedObject, public Poco::RefCountedObject
	/// Transport objects are responsible for setting up and maintaining a
	/// network (or other kind of) connection between Proxy objects on the
	/// client side and Listener objects on the server side, and for exchanging
	/// messages over that connection.
	///
	/// A Transport object also maintains Serializer and Deserializer objects.
	///
	/// Transport objects must support two message exchange patterns (MEP):
	///   - One-Way: a message is sent from the client to the server and the client
	///     does not expect to receive a reply from the server. With this pattern,
	///     the server has no way to provide status information back to the client,
	///     and the client has no way to know whether the call was successful.
	///   - Request-Reply: a message is sent from the client to the server and
	///     the client expects to receive a reply from the server. The reply may
	///     be a regular reply containing a return value and/or output parameters,
	///     or it may be a fault message, reporting an exception to the client.
	///
	/// Provided a Transport object is already connected, sending a one-way
	/// message is a three-step process:
	///   1. A new one-way message is started with a call to beginMessage(). This will
	///      prepare the Transport to deliver a one-way message to the server, as well
	///      as setting up a Serializer for creating the one-way message.
	///   2. The Serializer returned by beginMessage() is used to compose
	///      the actual message.
	///   3. The one-way message is delivered to the server with a
	///      call to sendMessage(). This completes the message exchange.
	///
	/// Doing a request-reply message exchange is a five-step process.
	///   1. A new request message is started with a call to beginRequest(). This will
	///      prepare the Transport to deliver a request message, as well as
	///      setting up a Serializer for creating the request message.
	///   2. The Serializer returned by beginRequest() is used to compose
	///      the actual message.
	///   3. The request is delivered to the server with a call to
	///      sendRequest(). After delivering the request, sendRequest() will 
	///      wait for a reply from the server, and set up a Deserializer
	///      for reading the reply.
	///   4. The reply message is read using the Deserializer returned
	///      by sendRequest().
	///   5. A call to endRequest() ends the message exchange.
	///
	/// Transport objects must be able to deal with incomplete message
	/// exchange sequences. For example, if an exception occurs between
	/// a call to beginRequest() and the corresponding call to sendRequest(),
	/// sendRequest() and endRequest() will not be called for this exchange.
	/// When beginRequest() or beginMessage() is called the next time, it
	/// must do the necessary cleanup to reset the Transport object to a
	/// well-defined state.
{
public:
	using Ptr = Poco::AutoPtr<Transport>;
	using NameValueMap = std::map<std::string,std::string>;

	Transport();
		/// Creates a Transport.

	virtual ~Transport();
		/// Destroys the Transport.

	virtual const std::string& endPoint() const = 0;
		/// Returns the endpoint to which this Transport is connected.
		/// If not connected, an empty string will be returned.

	virtual void connect(const std::string& endPoint) = 0;
		/// Connects the transport to the given endpoint.
		///
		/// The format of the endpoint is specific to the Transport.
		/// Some Transport implementations may accept a complete URI as endpoint.

	virtual void disconnect() = 0;
		/// Disconnects the transport.

	virtual bool connected() const = 0;
		/// Returns true iff the Transport is connected to a Listener, false otherwise.

	virtual Serializer& beginMessage(const Identifiable::ObjectId& oid, const Identifiable::TypeId& tid, const std::string& messageName, SerializerBase::MessageType messageType) = 0;
		/// Prepare a one-way message and set up a Serializer for writing the request.
		///
		/// The messageType should be MESSAGE_REQUEST or MESSAGE_EVENT. Whether this is checked
		/// is up to the actual implementation.
		///
		/// Returns a Serializer for composing the message.
		///
		/// No response message will be sent back for a one-way request.

	virtual void sendMessage(const Identifiable::ObjectId& oid, const Identifiable::TypeId& tid, const std::string& messageName, SerializerBase::MessageType messageType) = 0;
		/// Deliver the one-way message to the server and complete the message exchange.
		///
		/// All parameters must have the same value as the parameters passed to beginMessage().
		/// If not, the behavior is undefined.
		
	virtual Serializer& beginRequest(const Identifiable::ObjectId& oid, const Identifiable::TypeId& tid, const std::string& messageName, SerializerBase::MessageType messageType) = 0;
		/// Prepare a two-way (request - reply) message exchange and set up a Serializer for writing the request. 
		///
		/// The messageType should be MESSAGE_REQUEST or MESSAGE_EVENT. Whether this is checked
		/// is up to the actual implementation.
		///
		/// Returns a Serializer for composing the request message.

	virtual Deserializer& sendRequest(const Identifiable::ObjectId& oid, const Identifiable::TypeId& tid, const std::string& messageName, SerializerBase::MessageType messageType) = 0;
		/// Deliver the request message to the server, wait for a response and prepare a Deserializer for reading the reply.
		///
		/// All parameters must have the same value as the parameters passed to beginRequest().
		/// If not, the behavior is undefined.
		///
		/// Returns a Deserializer for reading the reply message.
	
	virtual void endRequest() = 0;
		/// Ends a request - reply message exchange.
	
	void lock();
		/// Locks the Transport's mutex.
		
	void unlock();
		/// Unlocks the Transport's mutex.
	
private:
	Poco::FastMutex _mutex;
};


//
// inlines
//
inline void Transport::lock()
{
	_mutex.lock();
}


inline void Transport::unlock()
{
	_mutex.unlock();
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Transport_INCLUDED
