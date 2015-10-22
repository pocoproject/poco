//
// Client.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Client
//
// Definition of the Client class.
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
#include "Poco/Redis/Error.h"
#include "Poco/Redis/RedisSocket.h"

namespace Poco {
namespace Redis {


class Redis_API Client
	/// Represents a connection to a Redis server
{
public:
	typedef Poco::SharedPtr<Client> Ptr;

	Client();
		/// Default constructor. Use this when you want to
		/// connect later on.

	Client(const std::string& hostAndPort);
		/// Constructor which connects to the given Redis host/port.
		/// The host and port must be separated with a colon.

	Client(const std::string& host, int port);
		/// Constructor which connects to the given Redis host/port.

	Client(const Net::SocketAddress& addrs);
		/// Constructor which connects to the given Redis host/port.

	virtual ~Client();
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

	RedisType::Ptr sendCommand(const Array& command);

	template<typename T>
	void  sendCommand(const Array& command, T& result)
	{
		sendCommand(command);
		readReply(result);
	}

	RedisType::Ptr readReply();

	template<typename T>
	void readReply(T& result)
	{
		RedisType::Ptr redisResult = readReply();
		if ( redisResult->type() == ElementTraits<T>::TypeId )
			result = ((Type<T>*) redisResult.get())->value();
		else  throw BadCastException();
	}

	void sendCommands(const std::vector<Array>& commands, std::vector<RedisType::Ptr>& results);

	void writeCommand(const Array& command);
		/// Sends a request to the Redis server

private:

	Client(const Client&);
	Client& operator = (const Client&);

	Net::SocketAddress _address;
	RedisSocket _socket;

	void connect();
		/// Connects to the Redis server

	static RedisType::Ptr createRedisType(char marker);
};


inline Net::SocketAddress Client::address() const
{
	return _address;
}


} } // namespace Poco::Redis


#endif //Redis_Connection_INCLUDED
