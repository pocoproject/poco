//
// RedisTest.cpp
//
// $Id$
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
#include <iostream>

#include "Poco/Exception.h"
#include "Poco/Delegate.h"
#include "Poco/Thread.h"

#include "RedisTest.h"
#include "Poco/Redis/AsyncReader.h"

#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

using namespace Poco::Redis;


bool RedisTest::_connected = false;
Poco::Redis::Client RedisTest::_redis;


RedisTest::RedisTest(const std::string& name):
	CppUnit::TestCase("Redis"),
	_host("localhost"),
	_port(6379)
{
	if (!_connected)
	{
		try
		{
			Poco::Timespan t(10, 0); // Connect within 10 seconds
			_redis.connect(_host, _port, t);
			_redis.setReceiveTimeout(t); // Receive answers within 10 seconds
			_connected = true;
			std::cout << "Connected to [" << _host << ':' << _port << ']' << std::endl;
		}
		catch (Poco::Exception& e)
		{
			std::cout << "Couldn't connect to [" << _host << ':' << _port << ']' << e.message() << ". " << std::endl;
		}
	}
}


RedisTest::~RedisTest()
{
	if (_connected)
	{
		_redis.disconnect();
		_connected = false;
		std::cout << "Disconnected from [" << _host << ':' << _port << ']' << std::endl;
	}
}


void RedisTest::setUp()
{

}


void RedisTest::tearDown()
{
}


void RedisTest::testAppend()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array delCommand;
	delCommand.add("DEL")
		.add("mykey");
	try
	{
		_redis.execute<Poco::Int64>(delCommand);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	Array setCommand;
	setCommand.add("SET")
		.add("mykey")
		.add("Hello");
	try
	{
		std::string result = _redis.execute<std::string>(setCommand);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	Array appendCommand;
	appendCommand.add("APPEND")
		.add("mykey")
		.add(" World");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(appendCommand);
		assert(result == 11);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	Array getCommand;
	getCommand.add("GET")
		.add("mykey");
	try
	{
		BulkString result = _redis.execute<BulkString>(getCommand);
		assert(result.value().compare("Hello World") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}
}

void RedisTest::testEcho()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("ECHO")
		.add("Hello World");

	try
	{
		BulkString result = _redis.execute<BulkString>(command);
		assert(!result.isNull());
		assert(result.value().compare("Hello World") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testIncr()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("SET")
		.add("mykey")
		.add("10");

	// A set responds with a simple OK string
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("INCR")
		.add("mykey");

	try
	{
		Poco::Int64 value = _redis.execute<Poco::Int64>(command);
		assert(value == 11);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testIncrBy()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("SET")
		.add("mykey")
		.add("10");

	// A set responds with a simple OK string
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("INCRBY")
		.add("mykey")
		.add("5");

	try
	{
		Poco::Int64 value = _redis.execute<Poco::Int64>(command);
		assert(value == 15);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testPing()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("PING");

	// A PING without a custom strings, responds with a simple "PONG" string
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("PONG") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	// A PING with a custom string responds with a bulk string
	command.add("Hello");
	try
	{
		BulkString result = _redis.execute<BulkString>(command);
		assert(!result.isNull());
		assert(result.value().compare("Hello") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

}

void RedisTest::testSet()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("SET")
		.add("mykey")
		.add("Hello");

	// A set responds with a simple OK string
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	command.add("NX");
	// A set NX responds with a Null bulk string
	// when the key is already set
	try
	{
		BulkString result = _redis.execute<BulkString>(command);
		assert(result.isNull());
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testMSet()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("MSET")
		.add("key1")
		.add("Hello")
		.add("key2")
		.add("World");

	// A MSET responds with a simple OK string
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("MGET")
		.add("key1")
		.add("key2")
		.add("nonexisting");
	try
	{
		Array result = _redis.execute<Array>(command);

		assert(result.size() == 3);
		BulkString value = result.get<BulkString>(0);
		assert(value.value().compare("Hello") == 0);

		value = result.get<BulkString>(1);
		assert(value.value().compare("World") == 0);

		value = result.get<BulkString>(2);
		assert(value.isNull());
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}
}

void RedisTest::testStrlen()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("SET")
		.add("mykey")
		.add("Hello World");

	// A set responds with a simple OK string
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("STRLEN")
		.add("mykey");

	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(command);

		assert(result == 11);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testRPush()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	Array delCommand;
	delCommand.add("DEL")
		.add("mylist");
	try
	{
		_redis.execute<Poco::Int64>(delCommand);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	for(int i = 0; i < 2; ++i)
	{
		Array command;
		command.add("RPUSH")
			.add("mylist");

		if ( i == 0 ) command.add("Hello");
		else command.add("World");

		// A RPUSH responds with an integer
		try
		{
			Poco::Int64 result = _redis.execute<Poco::Int64>(command);
			assert(result == (i + 1));
		}
		catch(RedisException &e)
		{
			fail(e.message());
		}
	}

	Array command;
	command.add("LRANGE")
		.add("mylist")
		.add("0")
		.add("-1");

	try
	{
		Array result = _redis.execute<Array>(command);

		assert(result.size() == 2);
		BulkString value = result.get<BulkString>(0);
		assert(value.value().compare("Hello") == 0);

		value = result.get<BulkString>(1);
		assert(value.value().compare("World") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testLIndex()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	Array delCommand;
	delCommand.add("DEL")
		.add("mylist");
	try
	{
		_redis.execute<Poco::Int64>(delCommand);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	for(int i = 0; i < 2; ++i)
	{
		Array command;
		command.add("LPUSH")
			.add("mylist");

		if ( i == 0 ) command.add("World");
		else command.add("Hello");

		// A RPUSH responds with an integer
		try
		{
			Poco::Int64 result = _redis.execute<Poco::Int64>(command);
			assert(result == (i + 1));
		}
		catch(RedisException &e)
		{
			fail(e.message());
		}
	}

	Array command;
	command.add("LINDEX")
		.add("mylist")
		.add("0");

	try
	{
		BulkString result = _redis.execute<BulkString>(command);
		assert(result.value().compare("Hello") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testMulti()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure keys are gone from a previous testrun ...
	Array delCommand;
	delCommand.add("DEL")
		.add("foo")
		.add("bar");
	try
	{
		_redis.execute<Poco::Int64>(delCommand);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	Array command;
	command.add("MULTI");
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("INCR")
		.add("foo");
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("QUEUED") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("INCR")
		.add("bar");
	try
	{
		std::string result = _redis.execute<std::string>(command);
		assert(result.compare("QUEUED") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	command.clear();
	command.add("EXEC");
	try
	{
		Array result = _redis.execute<Array>(command);
		assert(result.size() == 2);

		Poco::Int64 v = result.get<Poco::Int64>(0);
		assert(v == 1);
		v = result.get<Poco::Int64>(1);
		assert(v == 1);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}
}

void RedisTest::testPipeliningWithSendCommands()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	std::vector<Array> commands;

	Array ping;
	ping.add("PING");
	commands.push_back(ping);
	commands.push_back(ping);

	Array result = _redis.sendCommands(commands);

	// We expect 2 results
	assert(result.size() == 2);

	// The 2 results must be simple PONG strings
	for(size_t i = 0; i < 2; ++i)
	{
		try
		{
			std::string pong = result.get<std::string>(i);
			assert(pong.compare("PONG") == 0);
		}
		catch(...)
		{
			fail("An exception occurred");
		}
	}
}

void RedisTest::testPipeliningWithWriteCommand()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array ping;
	ping.add("PING");

	_redis.execute<void>(ping);
	_redis.execute<void>(ping);
	_redis.flush();

	// We expect 2 results with simple "PONG" strings
	for(int i = 0; i < 2; ++i)
	{
		std::string pong;
		try
		{
			_redis.readReply<std::string>(pong);
			assert(pong.compare("PONG") == 0);
		}
		catch(RedisException& e)
		{
			fail(e.message());
		}
	}
}

class RedisSubscriber
{
public:

	void onMessage(const void* pSender, RedisType::Ptr& message)
	{
		std::cout << message->toString() << std::endl;
	}

};

void RedisTest::testPubSub()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	RedisSubscriber subscriber;

	Array subscribe;
	subscribe.add("SUBSCRIBE")
		.add("test");

	_redis.execute<void>(subscribe);
	_redis.flush();

	AsyncReader reader(_redis);
	reader.redisResponse += Poco::delegate(&subscriber, &RedisSubscriber::onMessage);
	reader.start();

	Poco::Thread::sleep(30000);

	Array unsubscribe;
	unsubscribe.add("UNSUBSCRIBE");

	_redis.execute<void>(unsubscribe);
	_redis.flush();
}

CppUnit::Test* RedisTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RedisTest");

	CppUnit_addTest(pSuite, RedisTest, testAppend);
	CppUnit_addTest(pSuite, RedisTest, testIncr);
	CppUnit_addTest(pSuite, RedisTest, testIncrBy);
	CppUnit_addTest(pSuite, RedisTest, testEcho);
	CppUnit_addTest(pSuite, RedisTest, testPing);
	CppUnit_addTest(pSuite, RedisTest, testSet);
	CppUnit_addTest(pSuite, RedisTest, testMSet);
	CppUnit_addTest(pSuite, RedisTest, testStrlen);
	CppUnit_addTest(pSuite, RedisTest, testRPush);
	CppUnit_addTest(pSuite, RedisTest, testLIndex);
	CppUnit_addTest(pSuite, RedisTest, testMulti);

	CppUnit_addTest(pSuite, RedisTest, testPubSub);

	CppUnit_addTest(pSuite, RedisTest, testPipeliningWithSendCommands);
	CppUnit_addTest(pSuite, RedisTest, testPipeliningWithWriteCommand);

	return pSuite;
}
