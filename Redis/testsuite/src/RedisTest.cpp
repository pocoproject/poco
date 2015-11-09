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
#include "Poco/Redis/Command.h"

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

	Command delCommand = Command::del("mykey");
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

	Command setCommand = Command::set("mykey", "Hello");
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

	Command appendCommand = Command::append("mykey", " World");
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

	Command getCommand = Command::get("mykey");
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

	Command command = Command::set("mykey", "10");
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

	command = Command::incr("mykey");
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

	Command command = Command::set("mykey", "10");
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

	command = Command::incr("mykey", 5);
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
	Command delCommand = Command::del("mylist");
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

	try
	{
		Command rpush = Command::rpush("mylist", "World");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "Hello");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command llen = Command::llen("mylist");
	try
	{
		Poco::Int64 n = _redis.execute<Poco::Int64>(llen);
		assert(n == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException& e)
	{
		fail(e.message());
	}

	Command lrange = Command::lrange("mylist", 0, -1);
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("World") == 0);
		assert(result.get<BulkString>(1).value().compare("Hello") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
	catch(Poco::NullValueException &e)
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
	Command delCommand = Command::del("mylist");
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

	try
	{
		Command lpush = Command::lpush("mylist", "World");
		Poco::Int64 result = _redis.execute<Poco::Int64>(lpush);
		assert(result == 1);

		lpush = Command::lpush("mylist", "Hello");
		result = _redis.execute<Poco::Int64>(lpush);
		assert(result == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lindex = Command::lindex("mylist", 0);
	try
	{
		BulkString result = _redis.execute<BulkString>(lindex);
		assert(result.value().compare("Hello") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testLInsert()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	Command delCommand = Command::del("mylist");
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

	try
	{
		Command rpush = Command::rpush("mylist", "Hello");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "World");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);

		Command linsert = Command::linsert("mylist", true, "World", "There");
		result = _redis.execute<Poco::Int64>(linsert);
		assert(result == 3);

		Command lrange = Command::lrange("mylist", 0, -1);
		Array range = _redis.execute<Array>(lrange);
		assert(range.size() == 3);

		std::cout << range.toString() << std::endl;

		assert(range.get<BulkString>(0).value().compare("Hello") == 0);
		assert(range.get<BulkString>(1).value().compare("There") == 0);
		assert(range.get<BulkString>(2).value().compare("World") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
	catch(Poco::BadCastException &e)
	{
		fail(e.message());
	}
	catch(Poco::NullValueException &e)
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

	void onMessage(const void* pSender, RedisEventArgs& args)
	{
		if ( ! args.message().isNull() )
		{
			Type<Array>* arrayType = dynamic_cast<Type<Array>*>(args.message().get());
			if ( arrayType != NULL )
			{
				Array& array = arrayType->value();
				if ( array.size() == 3 )
				{
					BulkString type = array.get<BulkString>(0);
					if ( type.value().compare("unsubscribe") == 0 )
					{
						Poco::Int64 n = array.get<Poco::Int64>(2);
						// When 0, no subscribers anymore, so stop reading ...
						if ( n == 0 ) args.stop();
					}
				}
				else
				{
					// Wrong array received. Stop the reader
					args.stop();
				}
			}
			else
			{
				// Invalid type of message received. Stop the reader ...
				args.stop();
			}
		}
	}

	void onError(const void* pSender, RedisEventArgs& args)
	{
		std::cout << args.exception()->className() << std::endl;
		// No need to call stop, AsyncReader stops automatically when an
		// exception is received.
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
	reader.redisException += Poco::delegate(&subscriber, &RedisSubscriber::onError);
	reader.start();

	std::cout << "Sleeping ..." << std::endl;
	Poco::Thread::sleep(10000);

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
	CppUnit_addTest(pSuite, RedisTest, testLInsert);
	CppUnit_addTest(pSuite, RedisTest, testMulti);

	CppUnit_addTest(pSuite, RedisTest, testPubSub);

	CppUnit_addTest(pSuite, RedisTest, testPipeliningWithSendCommands);
	CppUnit_addTest(pSuite, RedisTest, testPipeliningWithWriteCommand);

	return pSuite;
}
