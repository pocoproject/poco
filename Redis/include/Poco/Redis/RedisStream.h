//
// RedisStream.h
//
// Library: Redis
// Package: Redis
// Module:  RedisStream
//
// Definition of the RedisStream class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Redis_RedisStream_INCLUDED
#define Redis_RedisStream_INCLUDED


#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Exception.h"
#include "Poco/BufferedStreamBuf.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberFormatter.h"
#include <istream>
#include <ostream>


namespace Poco::Redis {


class RedisStreamBuf: public BufferedStreamBuf
	/// BufferedStreamBuf for Redis.
{
public:
	RedisStreamBuf(Net::StreamSocket& redis);
		/// Constructor

	~RedisStreamBuf();
		/// Destructor

protected:
	std::streamsize readFromDevice(char* buffer, std::streamsize length);
	std::streamsize writeToDevice(const char* buffer, std::streamsize length);

private:
	static constexpr int STREAM_BUFFER_SIZE = 1024;

	Net::StreamSocket& _redis;
};


class RedisIOS: public virtual std::ios
{
public:
	RedisIOS(Net::StreamSocket& redis);
		/// Creates the RedisIOS with the given socket.

	~RedisIOS();
		/// Destroys the RedisIOS.
		///
		/// Flushes the buffer, but does not close the socket.

	[[nodiscard]] RedisStreamBuf* rdbuf();
		/// Returns a pointer to the internal RedisStreamBuf.

	void close();
		/// Flushes the stream.

protected:
	RedisStreamBuf _buf;
};


class Redis_API RedisOutputStream: public RedisIOS, public std::ostream
	/// An output stream for writing to a Redis server.
{
public:
	RedisOutputStream(Net::StreamSocket& redis);
		/// Creates the RedisOutputStream with the given socket.

	~RedisOutputStream();
		/// Destroys the RedisOutputStream.
		///
		/// Flushes the buffer.
};


class Redis_API RedisInputStream: public RedisIOS, public std::istream
	/// An input stream for reading from a Redis server.
	///
	/// Replies are checked only where a missing check would cause a memory or
	/// stack problem or would return wrong data without an error: a bulk string
	/// is read as its bytes arrive, arrays are nested at most MAX_NESTING_DEPTH
	/// levels deep, and a reply cut short by the end of the stream or a bulk
	/// string without its terminator throws a RedisException. Sizes and the
	/// rest of the protocol structure are not checked.
{
public:
	static constexpr int MAX_NESTING_DEPTH = 128;
		/// The maximum number of nested arrays in a reply.

	class NestingGuard
		/// Counts one level of array nesting of the stream for the lifetime
		/// of the guard.
	{
	public:
		POCO_NODISCARD_CTOR explicit NestingGuard(RedisInputStream& stream);
			/// Enters one level of nesting. Throws a RedisException when the
			/// reply is nested deeper than MAX_NESTING_DEPTH.

		~NestingGuard();
			/// Leaves the level of nesting.

		NestingGuard() = delete;
		NestingGuard(const NestingGuard&) = delete;
		NestingGuard(NestingGuard&&) = delete;
		NestingGuard& operator = (const NestingGuard&) = delete;
		NestingGuard& operator = (NestingGuard&&) = delete;

	private:
		RedisInputStream& _stream;
	};

	RedisInputStream(Net::StreamSocket& redis);
		/// Creates the RedisInputStream with the given socket.

	~RedisInputStream();
		/// Destroys the RedisInputStream.

	[[nodiscard]] std::string getline();
		/// Redis uses \r\n as delimiter. This getline version removes
		/// the \r from the result.
		///
		/// Throws a RedisException when the stream ends before the end
		/// of the line.

private:
	int _depth = 0;
};


//
// inlines
//


inline RedisInputStream::NestingGuard::NestingGuard(RedisInputStream& stream):
	_stream(stream)
{
	if (_stream._depth >= MAX_NESTING_DEPTH)
	{
		throw RedisException("Reply from Redis server has more than "
			+ NumberFormatter::format(MAX_NESTING_DEPTH) + " nested arrays");
	}
	++_stream._depth;
}


inline RedisInputStream::NestingGuard::~NestingGuard()
{
	--_stream._depth;
}


} // namespace Poco::Redis


#endif // Redis_RedisStream_INCLUDED
