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
Poco::Redis::Connection RedisTest::_redis;


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

	BulkString result;
	_redis.sendCommand(command, result);
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

	std::string result;
	_redis.sendCommand(command, result);
}

CppUnit::Test* RedisTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RedisTest");

	CppUnit_addTest(pSuite, RedisTest, testEcho);
	CppUnit_addTest(pSuite, RedisTest, testPing);

	return pSuite;
}
