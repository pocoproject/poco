//
// RedisTest.h
//
// $Id$
//
// Definition of the RedisTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RedisTest_INCLUDED
#define RedisTest_INCLUDED


#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"

#include "CppUnit/TestCase.h"


class RedisTest: public CppUnit::TestCase
{
public:
	RedisTest(const std::string& name);


	virtual ~RedisTest();

	void testAppend();
	void testEcho();
	void testPing();
	void testSet();
	void testPipeliningWithSendCommands();
	void testPipeliningWithWriteCommand();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

	std::string _host;
	unsigned    _port;
	static bool _connected;
	static Poco::Redis::Client _redis;

};


#endif // RedisTest_INCLUDED
