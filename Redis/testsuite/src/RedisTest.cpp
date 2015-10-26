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

#include "Poco/DateTime.h"
#include "Poco/ObjectPool.h"

#include "Poco/Net/NetException.h"

#include "RedisTest.h"
#include "Poco/Redis/Client.h"

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
			_redis.connect(_host, _port);
			_connected = true;
			std::cout << "Connected to [" << _host << ':' << _port << ']' << std::endl;
		}
		catch (Poco::Net::ConnectionRefusedException& e)
		{
			std::cout << "Couldn't connect to " << e.message() << ". " << std::endl;
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


void RedisTest::testEcho()
{
	if (!_connected)
	{
		std::cout << "Not connected, test skipped." << std::endl;
		return;
	}

	Array command;
	command.add("ECHO");
	command.add("Hello World");

	try
	{
		BulkString result;
		_redis.sendCommand(command, result);

		assert(!result.isNull());
		assert(result.value().compare("Hello World") == 0);
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
		std::string result;
		_redis.sendCommand(command, result);

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
		BulkString result;
		_redis.sendCommand(command, result);

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
	command.add("SET");
	command.add("mykey");
	command.add("Hello");

	// A set responds with a simple OK string
	try
	{
		std::string result;
		_redis.sendCommand(command, result);

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
		BulkString result;
		_redis.sendCommand(command, result);

		assert(result.isNull());
	}
	catch(RedisException &e)
	{
		fail(e.message());
	}
}

void RedisTest::testPipelining()
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

	std::vector<RedisType::Ptr> result;
	_redis.sendCommands(commands, result);

	// We expect 2 results
	assert(result.size() == 2);

	// The 2 results must be simple PONG strings
	for(std::vector<RedisType::Ptr>::iterator it = result.begin(); it != result.end(); ++it)
	{
		assert((*it)->isSimpleString());

		std::string pong = ((Type<std::string>*) it->get())->value();
		assert(pong.compare("PONG") == 0);
	}
}


CppUnit::Test* RedisTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RedisTest");

	CppUnit_addTest(pSuite, RedisTest, testEcho);
	CppUnit_addTest(pSuite, RedisTest, testPing);
	CppUnit_addTest(pSuite, RedisTest, testSet);

	CppUnit_addTest(pSuite, RedisTest, testPipelining);

	return pSuite;
}
