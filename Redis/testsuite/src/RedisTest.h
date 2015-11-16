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
	void testBLpop();
	void testBRpop();
	void testDecr();
	void testEcho();
	void testError();
	void testIncr();
	void testIncrBy();
	void testLIndex();
	void testLInsert();
	void testLPop();
	void testLRem();
	void testLSet();
	void testLTrim();
	void testMulti();
	void testMSet();
	void testMSetWithMap();
	void testPing();
	void testPipeliningWithSendCommands();
	void testPipeliningWithWriteCommand();
	void testPubSub();
	void testSAdd();
	void testSCard();
	void testSDiff();
	void testSDiffStore();
	void testSet();
	void testSInter();
	void testSInterStore();
	void testSIsmember();
	void testSMembers();
	void testSMove();
	void testSPop();
	void testSRandMember();
	void testSRem();
	void testSUnion();
	void testSUnionStore();
	void testStrlen();
	void testRename();
	void testRenameNx();
	void testRPop();
	void testRPoplPush();
	void testRPush();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

	void delKey(const std::string& key);

	std::string _host;
	unsigned    _port;
	static bool _connected;
	static Poco::Redis::Client _redis;

};


#endif // RedisTest_INCLUDED
