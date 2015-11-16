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

	delKey("mykey");

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

void RedisTest::testBLpop()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the lists are not there yet ...
	std::vector<std::string> lists;
	lists.push_back("list1");
	lists.push_back("list2");
	Command delCommand = Command::del(lists);
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

	std::vector<std::string> values;
	values.push_back("a");
	values.push_back("b");
	values.push_back("c");

	try
	{
		Command rpush = Command::rpush("list1", values);
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command blpop = Command::blpop(lists);
	try
	{
		Array result = _redis.execute<Array>(blpop);
		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("list1") == 0);
		assert(result.get<BulkString>(1).value().compare("a") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testBRpop()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the lists are not there yet ...
	std::vector<std::string> lists;
	lists.push_back("list1");
	lists.push_back("list2");
	Command delCommand = Command::del(lists);
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

	std::vector<std::string> values;
	values.push_back("a");
	values.push_back("b");
	values.push_back("c");

	try
	{
		Command rpush = Command::rpush("list1", values);
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command brpop = Command::brpop(lists);
	try
	{
		Array result = _redis.execute<Array>(brpop);
		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("list1") == 0);
		assert(result.get<BulkString>(1).value().compare("c") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testDecr()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Command set = Command::set("mykey", 10);
	try
	{
		std::string result = _redis.execute<std::string>(set);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}

	Command decr = Command::decr("mykey");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(decr);
		assert(result == 9);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}

	set = Command::set("mykey", "234293482390480948029348230948");
	try
	{
		std::string result = _redis.execute<std::string>(set);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}

	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(decr);
		fail("This must fail");
	}
	catch(RedisException& e)
	{
		// ERR value is not an integer or out of range
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

void RedisTest::testError()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("Wrong Command");

	try
	{
		BulkString result = _redis.execute<BulkString>(command);
		fail("Invalid command must throw RedisException");
	}
	catch(RedisException &e)
	{
		// Must fail
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

void RedisTest::testLPop()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	delKey("mylist");

	try
	{
		Command rpush = Command::rpush("mylist", "one");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "two");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);

		rpush = Command::rpush("mylist", "three");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lpop = Command::lpop("mylist");
	try
	{
		BulkString result = _redis.execute<BulkString>(lpop);
		assert(result.value().compare("one") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lrange = Command::lrange("mylist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("two") == 0);
		assert(result.get<BulkString>(1).value().compare("three") == 0);
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

void RedisTest::testLSet()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	delKey("mylist");

	try
	{
		Command rpush = Command::rpush("mylist", "one");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "two");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);

		rpush = Command::rpush("mylist", "three");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}

	Command lset = Command::lset("mylist", 0, "four");
	try
	{
		std::string result = _redis.execute<std::string>(lset);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}

	lset = Command::lset("mylist", -2, "five");
	try
	{
		std::string result = _redis.execute<std::string>(lset);
	}
	catch(RedisException& e)
	{
		fail(e.message());
	}

	Command lrange = Command::lrange("mylist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 3);
		assert(result.get<BulkString>(0).value().compare("four") == 0);
		assert(result.get<BulkString>(1).value().compare("five") == 0);
		assert(result.get<BulkString>(2).value().compare("three") == 0);
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
	delKey("mylist");

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
	delKey("mylist");

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

void RedisTest::testLRem()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	delKey("mylist");

	try
	{
		std::vector<std::string> list;
		list.push_back("hello");
		list.push_back("hello");
		list.push_back("foo");
		list.push_back("hello");
		Command rpush = Command::rpush("mylist", list);
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 4);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lrem = Command::lrem("mylist", -2, "hello");
	try
	{
		Poco::Int64 n = _redis.execute<Poco::Int64>(lrem);
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

	Command lrange = Command::lrange("mylist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("hello") == 0);
		assert(result.get<BulkString>(1).value().compare("foo") == 0);
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

void RedisTest::testLTrim()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	delKey("mylist");

	try
	{
		Command rpush = Command::rpush("mylist", "one");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "two");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);

		rpush = Command::rpush("mylist", "three");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command ltrim = Command::ltrim("mylist", 1);
	try
	{
		std::string result = _redis.execute<std::string>(ltrim);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lrange = Command::lrange("mylist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("two") == 0);
		assert(result.get<BulkString>(1).value().compare("three") == 0);
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

void RedisTest::testMSet()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Command command("MSET");
	command << "key1" << "Hello" << "key2" << "World";

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

void RedisTest::testMSetWithMap()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	std::map<std::string, std::string> keyValuePairs;
	keyValuePairs.insert(std::make_pair<std::string, std::string>("key1", "Hello"));
	keyValuePairs.insert(std::make_pair<std::string, std::string>("key2", "World"));

	Command mset = Command::mset(keyValuePairs);

	// A MSET responds with a simple OK string
	try
	{
		std::string result = _redis.execute<std::string>(mset);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> keys;
	keys.push_back("key1");
	keys.push_back("key2");
	keys.push_back("nonexisting");

	Command mget = Command::mget(keys);
	try
	{
		Array result = _redis.execute<Array>(mget);

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

void RedisTest::testMulti()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure keys are gone from a previous testrun ...
	delKey("foo");
	delKey("bar");

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
	subscribe.add("SUBSCRIBE").add("test");

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

void RedisTest::testSAdd()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("myset");

	Command sadd = Command::sadd("myset", "Hello");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sadd = Command::sadd("myset", "World");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sadd = Command::sadd("myset", "World");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSCard()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("myset");

	Command sadd = Command::sadd("myset", "Hello");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sadd = Command::sadd("myset", "World");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command scard = Command::scard("myset");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(scard);
		assert(result == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSDiff()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("key1");
	delKey("key2");

	std::vector<std::string> values1;
	values1.push_back("a");
	values1.push_back("b");
	values1.push_back("c");
	Command sadd = Command::sadd("key1", values1);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> values2;
	values2.push_back("c");
	values2.push_back("d");
	values2.push_back("e");
	sadd = Command::sadd("key2", values2);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sdiff = Command::sdiff("key1", "key2");
	try
	{
		Array result = _redis.execute<Array>(sdiff);
		assert(result.size() == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSDiffStore()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("key");
	delKey("key1");
	delKey("key2");

	std::vector<std::string> values1;
	values1.push_back("a");
	values1.push_back("b");
	values1.push_back("c");
	Command sadd = Command::sadd("key1", values1);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> values2;
	values2.push_back("c");
	values2.push_back("d");
	values2.push_back("e");
	sadd = Command::sadd("key2", values2);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sdiffstore = Command::sdiffstore("key", "key1", "key2");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sdiffstore);
		assert(result == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smembers = Command::smembers("key");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 2);
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
	command.add("SET").add("mykey").add("Hello");

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


void RedisTest::testSInter()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("key1");
	delKey("key2");

	std::vector<std::string> values1;
	values1.push_back("a");
	values1.push_back("b");
	values1.push_back("c");
	Command sadd = Command::sadd("key1", values1);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> values2;
	values2.push_back("c");
	values2.push_back("d");
	values2.push_back("e");
	sadd = Command::sadd("key2", values2);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sinter = Command::sinter("key1", "key2");
	try
	{
		Array result = _redis.execute<Array>(sinter);
		assert(result.size() == 1);
		assert(result.get<BulkString>(0).value().compare("c") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSInterStore()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("key");
	delKey("key1");
	delKey("key2");

	std::vector<std::string> values1;
	values1.push_back("a");
	values1.push_back("b");
	values1.push_back("c");
	Command sadd = Command::sadd("key1", values1);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> values2;
	values2.push_back("c");
	values2.push_back("d");
	values2.push_back("e");
	sadd = Command::sadd("key2", values2);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sinterstore = Command::sinterstore("key", "key1", "key2");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sinterstore);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smembers = Command::smembers("key");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 1);
		assert(result.get<BulkString>(0).value().compare("c") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSIsmember()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("myset");

	Command sadd = Command::sadd("myset", "one");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sismember = Command::sismember("myset", "one");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sismember);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sismember = Command::sismember("myset", "two");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sismember);
		assert(result == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSMembers()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("myset");

	Command sadd = Command::sadd("myset", "Hello");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sadd = Command::sadd("myset", "World");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smembers = Command::smembers("myset");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSMove()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("myset");

	Command sadd = Command::sadd("myset", "one");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sadd = Command::sadd("myset", "two");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	sadd = Command::sadd("myotherset", "three");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smove = Command::smove("myset", "myotherset", "two");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(smove);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smembers = Command::smembers("myset");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 1);
		assert(result.get<BulkString>(0).value().compare("one") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	smembers = Command::smembers("myotherset");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 2);
	}
	catch(RedisException &e)
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
	command.add("SET").add("mykey").add("Hello World");

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

void RedisTest::testSRem()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("myset");

	Command sadd = Command::sadd("myset", "one");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
	sadd = Command::sadd("myset", "two");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
	sadd = Command::sadd("myset", "three");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command srem = Command::srem("myset", "one");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(srem);
		assert(result == 1);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	srem = Command::srem("myset", "four");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(srem);
		assert(result == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smembers = Command::smembers("myset");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 2);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSUnion()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("key1");
	delKey("key2");

	std::vector<std::string> values1;
	values1.push_back("a");
	values1.push_back("b");
	values1.push_back("c");
	Command sadd = Command::sadd("key1", values1);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> values2;
	values2.push_back("c");
	values2.push_back("d");
	values2.push_back("e");
	sadd = Command::sadd("key2", values2);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sunion = Command::sunion("key1", "key2");
	try
	{
		Array result = _redis.execute<Array>(sunion);
		assert(result.size() == 5);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testSUnionStore()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	delKey("key");
	delKey("key1");
	delKey("key2");

	std::vector<std::string> values1;
	values1.push_back("a");
	values1.push_back("b");
	values1.push_back("c");
	Command sadd = Command::sadd("key1", values1);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	std::vector<std::string> values2;
	values2.push_back("c");
	values2.push_back("d");
	values2.push_back("e");
	sadd = Command::sadd("key2", values2);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sadd);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command sunionstore = Command::sunionstore("key", "key1", "key2");
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(sunionstore);
		assert(result == 5);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command smembers = Command::smembers("key");
	try
	{
		Array result = _redis.execute<Array>(smembers);
		assert(result.size() == 5);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testRename()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Command set = Command::set("mykey", "Hello");
	try
	{
		std::string result = _redis.execute<std::string>(set);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command rename = Command::rename("mykey", "myotherkey");
	try
	{
		std::string result = _redis.execute<std::string>(rename);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command get = Command::get("myotherkey");
	try
	{
		BulkString result = _redis.execute<BulkString>(get);
		assert(result.value().compare("Hello") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testRenameNx()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Command set = Command::set("mykey", "Hello");
	try
	{
		std::string result = _redis.execute<std::string>(set);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	set = Command::set("myotherkey", "World");
	try
	{
		std::string result = _redis.execute<std::string>(set);
		assert(result.compare("OK") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command rename = Command::rename("mykey", "myotherkey", false);
	try
	{
		Poco::Int64 result = _redis.execute<Poco::Int64>(rename);
		assert(result == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command get = Command::get("myotherkey");
	try
	{
		BulkString result = _redis.execute<BulkString>(get);
		assert(result.value().compare("World") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testRPop()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	delKey("mylist");

	try
	{
		Command rpush = Command::rpush("mylist", "one");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "two");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);

		rpush = Command::rpush("mylist", "three");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command rpop = Command::rpop("mylist");
	try
	{
		BulkString result = _redis.execute<BulkString>(rpop);
		assert(result.value().compare("three") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lrange = Command::lrange("mylist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("one") == 0);
		assert(result.get<BulkString>(1).value().compare("two") == 0);
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

void RedisTest::testRPoplPush()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the lists are not there yet ...
	std::vector<std::string> lists;
	lists.push_back("mylist");
	lists.push_back("myotherlist");
	Command delCommand = Command::del(lists);
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
		Command rpush = Command::rpush("mylist", "one");
		Poco::Int64 result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 1);

		rpush = Command::rpush("mylist", "two");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 2);

		rpush = Command::rpush("mylist", "three");
		result = _redis.execute<Poco::Int64>(rpush);
		assert(result == 3);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command rpoplpush = Command::rpoplpush("mylist", "myotherlist");
	try
	{
		BulkString result = _redis.execute<BulkString>(rpoplpush);
		assert(result.value().compare("three") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}

	Command lrange = Command::lrange("mylist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 2);
		assert(result.get<BulkString>(0).value().compare("one") == 0);
		assert(result.get<BulkString>(1).value().compare("two") == 0);
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
	catch(Poco::NullValueException &e)
	{
		fail(e.message());
	}

	lrange = Command::lrange("myotherlist");
	try
	{
		Array result = _redis.execute<Array>(lrange);

		assert(result.size() == 1);
		assert(result.get<BulkString>(0).value().compare("three") == 0);
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

void RedisTest::testRPush()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	// Make sure the list is not there yet ...
	delKey("mylist");

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


void RedisTest::delKey(const std::string& key)
{
	Command delCommand = Command::del(key);
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
}


CppUnit::Test* RedisTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RedisTest");

	CppUnit_addTest(pSuite, RedisTest, testAppend);
	CppUnit_addTest(pSuite, RedisTest, testBLpop);
	CppUnit_addTest(pSuite, RedisTest, testBRpop);
	CppUnit_addTest(pSuite, RedisTest, testDecr);
	CppUnit_addTest(pSuite, RedisTest, testDecr);
	CppUnit_addTest(pSuite, RedisTest, testIncr);
	CppUnit_addTest(pSuite, RedisTest, testIncrBy);
	CppUnit_addTest(pSuite, RedisTest, testEcho);
	CppUnit_addTest(pSuite, RedisTest, testError);
	CppUnit_addTest(pSuite, RedisTest, testLIndex);
	CppUnit_addTest(pSuite, RedisTest, testLInsert);
	CppUnit_addTest(pSuite, RedisTest, testLPop);
	CppUnit_addTest(pSuite, RedisTest, testLRem);
	CppUnit_addTest(pSuite, RedisTest, testLSet);
	CppUnit_addTest(pSuite, RedisTest, testLTrim);
	CppUnit_addTest(pSuite, RedisTest, testMSet);
	CppUnit_addTest(pSuite, RedisTest, testMSetWithMap);
	CppUnit_addTest(pSuite, RedisTest, testMulti);
	CppUnit_addTest(pSuite, RedisTest, testPing);
	CppUnit_addTest(pSuite, RedisTest, testPipeliningWithSendCommands);
	CppUnit_addTest(pSuite, RedisTest, testPipeliningWithWriteCommand);
	CppUnit_addTest(pSuite, RedisTest, testPubSub);
	CppUnit_addTest(pSuite, RedisTest, testSAdd);
	CppUnit_addTest(pSuite, RedisTest, testSCard);
	CppUnit_addTest(pSuite, RedisTest, testSDiff);
	CppUnit_addTest(pSuite, RedisTest, testSDiffStore);
	CppUnit_addTest(pSuite, RedisTest, testSet);
	CppUnit_addTest(pSuite, RedisTest, testSInter);
	CppUnit_addTest(pSuite, RedisTest, testSInterStore);
	CppUnit_addTest(pSuite, RedisTest, testSIsmember);
	CppUnit_addTest(pSuite, RedisTest, testSMembers);
	CppUnit_addTest(pSuite, RedisTest, testSMove);
	CppUnit_addTest(pSuite, RedisTest, testSRem);
	CppUnit_addTest(pSuite, RedisTest, testStrlen);
	CppUnit_addTest(pSuite, RedisTest, testSUnion);
	CppUnit_addTest(pSuite, RedisTest, testSUnionStore);
	CppUnit_addTest(pSuite, RedisTest, testRename);
	CppUnit_addTest(pSuite, RedisTest, testRenameNx);
	CppUnit_addTest(pSuite, RedisTest, testRPop);
	CppUnit_addTest(pSuite, RedisTest, testRPoplPush);
	CppUnit_addTest(pSuite, RedisTest, testRPush);

	return pSuite;
}
