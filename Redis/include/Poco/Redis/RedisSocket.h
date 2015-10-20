//
// RedistSocket.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  RedistSocket
//
// Definition of the RedistSocket class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Redis_RedisSocket_INCLUDED
#define Redis_RedisSocket_INCLUDED

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"

#include "Poco/Redis/Redis.h"

namespace Poco {
namespace Redis {

class Redis_API RedisSocket
{
public:

	RedisSocket();
	virtual ~RedisSocket();

	void close();

	void connect(const Net::SocketAddress& addrs);

	int get();

	int peek();

	void read(UInt64 length, std::string& data);

	int write(const char* buffer, std::streamsize length);

	int buffered();

	void refill();

	void readLine(std::string& line);

private:

	RedisSocket(RedisSocket& copy);
	RedisSocket& operator = (const RedisSocket&);

	Net::StreamSocket _socket;

	char* _buffer;
	char* _current;
	char* _end;
};

} }

#endif // Redis_RedisSocket_INCLUDED
