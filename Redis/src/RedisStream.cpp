//
// RedisStream.cpp
//
// Library: Redis
// Package: Redis
// Module:  RedisStream
//
// Implementation of the RedisStream class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Redis/RedisStream.h"
#include <iostream>


namespace Poco {
namespace Redis {


//
// RedisStreamBuf
//


RedisStreamBuf::RedisStreamBuf(Net::StreamSocket& redis):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in | std::ios::out),
	_redis(redis)
{
}


RedisStreamBuf::~RedisStreamBuf()
{
}


std::streamsize RedisStreamBuf::readFromDevice(char* buffer, std::streamsize len)
{
	try
	{
		return _redis.receiveBytes(buffer, static_cast<int>(len));
	}
	catch (...)
	{
		return -1;
	}
}


std::streamsize RedisStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	try
	{
		return _redis.sendBytes(buffer, static_cast<int>(length));
	}
	catch (...)
	{
		return -1;
	}
}


//
// RedisIOS
//


RedisIOS::RedisIOS(Net::StreamSocket& redis):
	_buf(redis)
{
	poco_ios_init(&_buf);
}


RedisIOS::~RedisIOS()
{
	try
	{
		_buf.sync();
	}
	catch (...)
	{
	}
}


RedisStreamBuf* RedisIOS::rdbuf()
{
	return &_buf;
}


void RedisIOS::close()
{
	_buf.sync();
}


//
// RedisOutputStream
//


RedisOutputStream::RedisOutputStream(Net::StreamSocket& redis):
	RedisIOS(redis),
	std::ostream(&_buf)
{
}


RedisOutputStream::~RedisOutputStream()
{
}


RedisInputStream::RedisInputStream(Net::StreamSocket& redis):
	RedisIOS(redis),
	std::istream(&_buf)
{
}


//
// RedisInputStream
//


RedisInputStream::~RedisInputStream()
{
}


std::string RedisInputStream::getline()
{
	std::string line;
	std::getline(*this, line);
	if ( line.size() > 0 ) line.erase(line.end() - 1);
	return line;
}


} } // namespace Poco::Redis
