//
// ReplyTest.cpp
//
// Library: Redis
// Package: Redis
// Module:  ReplyTest
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ReplyTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Redis/Array.h"
#include "Poco/Redis/Client.h"
#include "Poco/Redis/Exception.h"
#include "Poco/Redis/RedisStream.h"
#include "Poco/Redis/Type.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/Timespan.h"
#include "Poco/Types.h"
#include <cstddef>
#include <string>
#include <thread>


using Poco::Redis::Array;
using Poco::Redis::BulkString;
using Poco::Redis::Client;
using Poco::Redis::RedisException;
using Poco::Redis::RedisInputStream;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Int64;
using Poco::Timespan;


namespace
{
	template <typename F>
	void readFromPeer(const std::string& raw, F read)
		/// Calls read with a Client connected to a loopback peer that sends
		/// raw and then closes its side of the connection.
	{
		ServerSocket server(SocketAddress("127.0.0.1", 0));
		Client client(server.address());
		client.setReceiveTimeout(Timespan(5, 0));
		StreamSocket peer = server.acceptConnection();
		peer.setSendTimeout(Timespan(5, 0));
		std::thread writer([&peer, &raw]()
		{
			try
			{
				peer.sendBytes(raw.data(), static_cast<int>(raw.size()));
				peer.shutdownSend();
			}
			catch (Poco::Exception&)
			{
			}
		});
		try
		{
			read(client);
		}
		catch (...)
		{
			writer.join();
			throw;
		}
		writer.join();
	}


	template <typename T>
	T readReply(const std::string& raw)
		/// Reads one reply of type T from the peer of readFromPeer().
	{
		T result{};
		readFromPeer(raw, [&result](Client& client) { client.readReply(result); });
		return result;
	}


	std::string nestedArrays(int depth)
		/// Returns depth nested arrays of one element around the integer 1.
	{
		std::string raw;
		for (int i = 0; i < depth; ++i) raw += "*1\r\n";
		raw += ":1\r\n";
		return raw;
	}
}


ReplyTest::ReplyTest(const std::string& name): CppUnit::TestCase(name)
{
}


ReplyTest::~ReplyTest() = default;


void ReplyTest::setUp()
{
}


void ReplyTest::tearDown()
{
}


void ReplyTest::testBulkString()
{
	const BulkString value = readReply<BulkString>("$3\r\nabc\r\n");
	assertFalse (value.isNull());
	assertTrue (value.value() == "abc");
}


void ReplyTest::testEmptyBulkString()
{
	const BulkString value = readReply<BulkString>("$0\r\n\r\n");
	assertFalse (value.isNull());
	assertTrue (value.value().empty());
}


void ReplyTest::testNullBulkString()
{
	const BulkString value = readReply<BulkString>("$-1\r\n");
	assertTrue (value.isNull());
}


void ReplyTest::testLargeBulkString()
{
	// 65536 bytes end exactly on a read chunk; 200000 bytes take several chunks.
	for (const std::size_t size: {std::size_t{65536}, std::size_t{200000}})
	{
		std::string payload(size, 'x');
		for (std::size_t i = 0; i < payload.size(); i += 997) payload[i] = static_cast<char>('a' + (i % 26));
		const BulkString value = readReply<BulkString>("$" + std::to_string(payload.size()) + "\r\n" + payload + "\r\n");
		assertFalse (value.isNull());
		assertTrue (value.value() == payload);
	}
}


void ReplyTest::testTruncatedBulkString()
{
	try
	{
		const BulkString value = readReply<BulkString>("$10\r\nabc");
		fail("a bulk string cut short must throw, got \"" + value.value() + "\"");
	}
	catch (RedisException& exc)
	{
		assertTrue (Poco::startsWith(exc.message(), std::string("Incomplete bulk string reply from Redis server")));
	}
}


void ReplyTest::testTruncatedLargeBulkString()
{
	try
	{
		// The largest length the parser accepts: allocating it up front throws std::length_error
		// or std::bad_alloc instead of RedisException.
		readReply<BulkString>("$9223372036854775807\r\nab");
		fail("a bulk string cut short must throw");
	}
	catch (RedisException& exc)
	{
		assertTrue (Poco::startsWith(exc.message(), std::string("Incomplete bulk string reply from Redis server")));
	}
}


void ReplyTest::testBulkStringWithoutTerminator()
{
	try
	{
		const BulkString value = readReply<BulkString>("$3\r\nabcXY\r\n");
		fail("a bulk string without its terminator must throw, got \"" + value.value() + "\"");
	}
	catch (RedisException& exc)
	{
		assertTrue (Poco::startsWith(exc.message(), std::string("Bulk string reply from Redis server does not end")));
	}
}


void ReplyTest::testTruncatedLine()
{
	try
	{
		const std::string value = readReply<std::string>("+OK");
		fail("a simple string cut short must throw, got \"" + value + "\"");
	}
	catch (RedisException& exc)
	{
		assertTrue (Poco::startsWith(exc.message(), std::string("Incomplete reply from Redis server")));
	}

	try
	{
		const Int64 value = readReply<Int64>(":12");
		fail("an integer cut short must throw, got " + std::to_string(value));
	}
	catch (RedisException& exc)
	{
		assertTrue (Poco::startsWith(exc.message(), std::string("Incomplete reply from Redis server")));
	}
}


void ReplyTest::testNestingAtLimit()
{
	Array value = readReply<Array>(nestedArrays(RedisInputStream::MAX_NESTING_DEPTH));
	for (int i = 1; i < RedisInputStream::MAX_NESTING_DEPTH; ++i)
	{
		assertEqual (1, value.size());
		value = value.get<Array>(0);
	}
	assertEqual (1, value.size());
	assertEqual (1, value.get<Int64>(0));
}


void ReplyTest::testNestingTooDeep()
{
	try
	{
		readReply<Array>(nestedArrays(RedisInputStream::MAX_NESTING_DEPTH + 1));
		fail("an array nested deeper than the limit must throw");
	}
	catch (RedisException& exc)
	{
		assertTrue (Poco::startsWith(exc.message(), std::string("Reply from Redis server has more than")));
	}
}


void ReplyTest::testNestingCountsDepth()
{
	// More sibling arrays than the nesting limit: each one leaves its level when it ends.
	constexpr std::size_t SIBLINGS = 200;
	std::string raw = "*" + std::to_string(SIBLINGS) + "\r\n";
	for (std::size_t i = 0; i < SIBLINGS; ++i) raw += nestedArrays(1);
	const Array value = readReply<Array>(raw);
	assertEqual (SIBLINGS, value.size());
	for (std::size_t i = 0; i < value.size(); ++i)
	{
		assertEqual (1, value.get<Array>(i).get<Int64>(0));
	}
}


void ReplyTest::testConsecutiveReplies()
{
	Array first;
	Array second;
	readFromPeer("*2\r\n$3\r\nabc\r\n$0\r\n\r\n" + nestedArrays(RedisInputStream::MAX_NESTING_DEPTH),
		[&first, &second](Client& client)
		{
			client.readReply(first);
			client.readReply(second);
		});
	assertEqual (2, first.size());
	assertTrue (first.get<BulkString>(0).value() == "abc");
	assertTrue (first.get<BulkString>(1).value().empty());
	assertEqual (1, second.size());
}


CppUnit::Test* ReplyTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ReplyTest");

	CppUnit_addTest(pSuite, ReplyTest, testBulkString);
	CppUnit_addTest(pSuite, ReplyTest, testEmptyBulkString);
	CppUnit_addTest(pSuite, ReplyTest, testNullBulkString);
	CppUnit_addTest(pSuite, ReplyTest, testLargeBulkString);
	CppUnit_addTest(pSuite, ReplyTest, testTruncatedBulkString);
	CppUnit_addTest(pSuite, ReplyTest, testTruncatedLargeBulkString);
	CppUnit_addTest(pSuite, ReplyTest, testBulkStringWithoutTerminator);
	CppUnit_addTest(pSuite, ReplyTest, testTruncatedLine);
	CppUnit_addTest(pSuite, ReplyTest, testNestingAtLimit);
	CppUnit_addTest(pSuite, ReplyTest, testNestingTooDeep);
	CppUnit_addTest(pSuite, ReplyTest, testNestingCountsDepth);
	CppUnit_addTest(pSuite, ReplyTest, testConsecutiveReplies);

	return pSuite;
}
