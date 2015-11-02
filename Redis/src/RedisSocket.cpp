//
// RedistSocket.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  RedistSocket
//
// Implementation of the RedistSocket class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
#include "Poco/Redis/RedisSocket.h"

namespace Poco {
namespace Redis {

RedisSocket::RedisSocket() : _socket(),  _buffer(0), _current(0), _end(0)
{
}

RedisSocket::~RedisSocket()
{
	if ( _buffer ) delete[] _buffer;

	try
	{
		_socket.close();
	}
	catch (...)
	{
	}
}

void RedisSocket::close()
{
	_socket.close();
}

void RedisSocket::connect(const Net::SocketAddress& addrs)
{
	_socket.connect(addrs);
}

void RedisSocket::connect(const Net::SocketAddress& addrs, const Timespan& timeout)
{
	_socket.connect(addrs, timeout);
}


int RedisSocket::get()
{
	if ( _current == _end ) refill();

	if ( _current < _end ) return *_current++;

	return std::char_traits<char>::eof();
}

int RedisSocket::peek()
{
	if ( _current == _end ) refill();

	if ( _current < _end ) return *_current;

	return std::char_traits<char>::eof();
}

void RedisSocket::read(UInt64 length, std::string& data)
{
	UInt64 count = 0;
	data.clear();

	while(count < length)
	{
		int c = get();
		if ( c == -1 ) throw IOException("Invalid EOF");

		data += c;
		++count;
	}
}

int RedisSocket::write(const char* buffer, std::streamsize length)
{
	return _socket.sendBytes(buffer, (int) length);
}

void RedisSocket::refill()
{
	if ( ! _buffer ) _buffer = new char[1024];

	_current = _end = _buffer;
	int n = _socket.receiveBytes(_buffer, 1024);
	_end += n;
}


void RedisSocket::readLine(std::string& line)
{
	line.clear();

	int c = get();

	while(1)
	{
		if ( c == -1 ) throw IOException("Invalid EOF");

		if ( c == '\r' && peek() == '\n' )
		{
			get();
			break;
		}

		line += c;
		c = get();
	}
}

} }
