//
// ExpireCacheTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ExpireCacheTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/ExpireCache.h"
#include "Poco/AccessExpireCache.h"
#include "Poco/Bugcheck.h"
#include "Poco/Thread.h"


using namespace Poco;


#define DURSLEEP 250
#define DURHALFSLEEP DURSLEEP / 2
#define DURWAIT  300


ExpireCacheTest::ExpireCacheTest(const std::string& name): CppUnit::TestCase(name)
{
}


ExpireCacheTest::~ExpireCacheTest()
{
}


void ExpireCacheTest::testClear()
{
	ExpireCache<int, int> aCache(DURSLEEP);
	aCache.add(1, 2);
	aCache.add(3, 4);
	aCache.add(5, 6);
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (*aCache.get(1) == 2);
	assertTrue (*aCache.get(3) == 4);
	assertTrue (*aCache.get(5) == 6);
	aCache.clear();
	assertTrue (!aCache.has(1));
	assertTrue (!aCache.has(3));
	assertTrue (!aCache.has(5));
}


void ExpireCacheTest::testExpire0()
{
	try
	{
		ExpireCache<int, int> aCache(24);
		failmsg("cache expire lower than 25 is illegal, test should fail");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void ExpireCacheTest::testExpireN()
{
	// 3-1 represents the cache sorted by age, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	ExpireCache<int, int> aCache(DURSLEEP);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	SharedPtr<int> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (*tmp == 2);
	assertTrue (aCache.size() == 1);
	Thread::sleep(DURWAIT);
	assertTrue (aCache.size() == 0);
	assertTrue (!aCache.has(1));

	// tmp must still be valid, access it
	assertTrue (*tmp == 2);
	tmp = aCache.get(1);
	assertTrue (!tmp);

	aCache.add(1, 2); // 1
	Thread::sleep(DURHALFSLEEP);
	aCache.add(3, 4); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	tmp = aCache.get(1);
	SharedPtr<int> tmp2 = aCache.get(3);
	assertTrue (*tmp == 2);
	assertTrue (*tmp2 == 4);

	Thread::sleep(DURHALFSLEEP+25); //3|1
	assertTrue (!aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (*tmp == 2); // 1-3
	assertTrue (*tmp2 == 4); // 3-1
	tmp2 = aCache.get(3);
	assertTrue (*tmp2 == 4);
	Thread::sleep(DURHALFSLEEP+25); //3|1
	assertTrue (!aCache.has(3));
	assertTrue (*tmp2 == 4);
	tmp = aCache.get(1);
	tmp2 = aCache.get(3);
	assertTrue (!tmp);
	assertTrue (!tmp2);

	// removing illegal entries should work too
	aCache.remove(666);

	aCache.clear();
	assertTrue (!aCache.has(5));
	assertTrue (!aCache.has(3));
}


void ExpireCacheTest::testDuplicateAdd()
{
	ExpireCache<int, int> aCache(DURSLEEP);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 2);
	aCache.add(1, 3);
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 3);
}



void ExpireCacheTest::testAccessExpireN()
{
	// 3-1 represents the cache sorted by age, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	AccessExpireCache<int, int> aCache(DURSLEEP);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	SharedPtr<int> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (*tmp == 2);
	assertTrue (aCache.size() == 1);
	Thread::sleep(DURWAIT);
	assertTrue (aCache.size() == 0);
	assertTrue (!aCache.has(1));

	// tmp must still be valid, access it
	assertTrue (*tmp == 2);
	tmp = aCache.get(1);
	assertTrue (!tmp);

	aCache.add(1, 2); // 1
	Thread::sleep(DURHALFSLEEP);
	aCache.add(3, 4); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));

	Thread::sleep(DURHALFSLEEP+50); //3|1
	assertTrue (!aCache.has(1));
	assertTrue (*aCache.get(3) == 4);
	Thread::sleep(DURHALFSLEEP+25); //3|1
	assertTrue (*aCache.get(3) == 4);
	// removing illegal entries should work too
	aCache.remove(666);

	aCache.clear();
	assertTrue (!aCache.has(5));
	assertTrue (!aCache.has(3));
}


void ExpireCacheTest::testExpireWithHas()
{
	// 3-1 represents the cache sorted by age, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	ExpireCache<int, int> aCache(DURSLEEP);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	Thread::sleep(DURWAIT);
	assertTrue (!aCache.has(1));
}


void ExpireCacheTest::testAccessExpireGet()
{
	AccessExpireCache<int, int> aCache(DURSLEEP);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	SharedPtr<int> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (*tmp == 2);
	assertTrue (aCache.size() == 1);
	Thread::sleep(DURWAIT);
	tmp = aCache.get(1);
	assertTrue (tmp.isNull());
}


void ExpireCacheTest::setUp()
{
}


void ExpireCacheTest::tearDown()
{
}


CppUnit::Test* ExpireCacheTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ExpireCacheTest");

	CppUnit_addTest(pSuite, ExpireCacheTest, testClear);
	CppUnit_addTest(pSuite, ExpireCacheTest, testExpire0);
	CppUnit_addTest(pSuite, ExpireCacheTest, testExpireN);
	CppUnit_addTest(pSuite, ExpireCacheTest, testDuplicateAdd);
	CppUnit_addTest(pSuite, ExpireCacheTest, testAccessExpireN);
	CppUnit_addTest(pSuite, ExpireCacheTest, testExpireWithHas);
	CppUnit_addTest(pSuite, ExpireCacheTest, testAccessExpireGet);

	return pSuite;
}
