//
// Connection.h
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  Connection
//
// Definition of the Connection class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Connection_INCLUDED
#define MongoDB_Connection_INCLUDED


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Mutex.h"
#include "Poco/MongoDB/RequestMessage.h"
#include "Poco/MongoDB/ResponseMessage.h"


namespace Poco {
namespace MongoDB {


class MongoDB_API Connection
	/// Represents a connection to a MongoDB server
	/// using the MongoDB wire protocol.
	/// 
	/// See https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/
	/// for more information on the wire protocol.
{
public:
	typedef Poco::SharedPtr<Connection> Ptr;

	Connection();
		/// Creates an unconnected Connection.
		///
		/// Use this when you want to connect later on.

	Connection(const std::string& hostAndPort);
		/// Creates a Connection connected to the given MongoDB instance at host:port.
		///
		/// The host and port must be separated with a colon.

	Connection(const std::string& host, int port);
		/// Creates a Connection connected to the given MongoDB instance at host and port.

	Connection(const Poco::Net::SocketAddress& addrs);
		/// Creates a Connection connected to the given MongoDB instance at the given address.

	Connection(const Poco::Net::StreamSocket& socket);
		/// Creates a Connection connected to the given MongoDB instance using the given socket,
		/// which must already be connected.

	virtual ~Connection();
		/// Destroys the Connection.

	Poco::Net::SocketAddress address() const;
		/// Returns the address of the MongoDB server.

	void connect(const std::string& hostAndPort);
		/// Connects to the given MongoDB server. 
		///
		/// The host and port must be separated with a colon.

	void connect(const std::string& host, int port);
		/// Connects to the given MongoDB server.

	void connect(const Poco::Net::SocketAddress& addrs);
		/// Connects to the given MongoDB server.
		
	void connect(const Poco::Net::StreamSocket& socket);
		/// Connects using an already connected socket.

	void disconnect();
		/// Disconnects from the MongoDB server.

	void sendRequest(RequestMessage& request);
		/// Sends a request to the MongoDB server.
		/// 
		/// Used for one-way requests without a response.

	void sendRequest(RequestMessage& request, ResponseMessage& response);
		/// Sends a request to the MongoDB server and receives the response.
		/// 
		/// Use this when a response is expected: only a "query" or "getmore"
		/// request will return a response.

protected:
	void connect();

private:
	Poco::Net::SocketAddress _address;
	Poco::Net::StreamSocket _socket;
};


//
// inlines
//
inline Net::SocketAddress Connection::address() const
{
	return _address;
}


} } // namespace Poco::MongoDB


#endif // MongoDB_Connection_INCLUDED
