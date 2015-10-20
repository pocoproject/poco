//
// Connection.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Connection
//
// Definition of the Connection class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Redis_Connection_INCLUDED
#define Redis_Connection_INCLUDED


#include "Poco/Net/SocketAddress.h"
#include "Poco/Optional.h"

#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Array.h"
#include "Poco/Redis/RedisSocket.h"

namespace Poco {
namespace Redis {


class Redis_API Connection
	/// Represents a connection to a Redis server
{
public:
	typedef Poco::SharedPtr<Connection> Ptr;

	Connection();
		/// Default constructor. Use this when you want to
		/// connect later on.

	Connection(const std::string& hostAndPort);
		/// Constructor which connects to the given Redis host/port.
		/// The host and port must be separated with a colon.

	Connection(const std::string& host, int port);
		/// Constructor which connects to the given Redis host/port.

	Connection(const Net::SocketAddress& addrs);
		/// Constructor which connects to the given Redis host/port.

	virtual ~Connection();
		/// Destructor

	Net::SocketAddress address() const;
		/// Returns the address of the Redis connection

	void connect(const std::string& hostAndPort);
		/// Connects to the given Redis server. The host and port must be separated
		/// with a colon.

	void connect(const std::string& host, int port);
		/// Connects to the given Redis server.

	void connect(const Net::SocketAddress& addrs);
		/// Connects to the given Redis server.

	void disconnect();
		/// Disconnects from the Redis server

	void sendCommand(const Array& command);
		/// Sends a request to the Redis server

	template<typename T>
	void sendCommand(const Array& command, T& result)
	{
		Type<T>  resultType;

		sendCommand(command);

		char type = _socket.get();

		switch(type)
		{
			case ':':
			{
				if ( resultType.getType() == AbstractType::REDIS_INTEGER )
				{
					resultType.read(_socket);
				}
				else
				{
					std::string message;
					message.append("Expected ");
					message.append(ElementTraits<T>::typeName());
					message.append(", got an Integer");
					throw InvalidArgumentException(message);
				}
				break;
			}
			case '+': //  Simple String
			{
				if ( resultType.getType() == AbstractType::REDIS_SIMPLE_STRING )
				{
					resultType.read(_socket);
				}
				else
				{
					std::string message;
					message.append("Expected ");
					message.append(ElementTraits<T>::typeName());
					message.append(", got a Simple String");
					throw InvalidArgumentException(message);
				}
				break;
			}
			case '-' : // Error
			{
				std::string error;
				_socket.readLine(error);
				throw RedisException(error);
			}
			case '$' : // Bulk String
			{
				if ( resultType.getType() == AbstractType::REDIS_BULK_STRING )
				{
					resultType.read(_socket);
				}
				else
				{
					std::string message;
					message.append("Expected ");
					message.append(ElementTraits<T>::typeName());
					message.append(", got a Bulk String");
					throw InvalidArgumentException(message);
				}
				break;
			}
			case '*' : //Array
			{
				if ( resultType.getType() == AbstractType::REDIS_ARRAY )
				{
					resultType.read(_socket);
				}
				else
				{
					std::string message;
					message.append("Expected ");
					message.append(ElementTraits<T>::typeName());
					message.append(", got an Array");
					throw InvalidArgumentException(message);
				}
				throw InvalidArgumentException("Expected integer, got an array");
			}
			default:
				throw  IOException("Invalid Redis type returned");
		}

		result = resultType.value();
	}

private:

	Connection(const Connection&);
	Connection& operator = (const Connection&);

	Net::SocketAddress _address;
	RedisSocket _socket;

	void connect();
		/// Connects to the Redis server
};


inline Net::SocketAddress Connection::address() const
{
	return _address;
}


} } // namespace Poco::Redis


#endif //Redis_Connection_INCLUDED
