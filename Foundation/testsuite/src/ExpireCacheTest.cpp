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
using CppUnit::waitForCondition;


// Cache item expiration time in milliseconds
#define EXPIRE_TIME 500

// Maximum time to wait for a condition (must be > EXPIRE_TIME)
#define MAX_WAIT_TIME 5000


ExpireCacheTest::ExpireCacheTest(const std::string& name): CppUnit::TestCase(name)
{
}


ExpireCacheTest::~ExpireCacheTest()
{
}


void ExpireCacheTest::testClear()
{
	ExpireCache<int, int> aCache(EXPIRE_TIME);
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
	ExpireCache<int, int> aCache(EXPIRE_TIME);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	SharedPtr<int> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (*tmp == 2);
	assertTrue (aCache.size() == 1);

	// Wait for item 1 to expire
	assertTrue (waitForCondition([&]{ return aCache.size() == 0; }, MAX_WAIT_TIME));
	assertTrue (!aCache.has(1));

	// tmp must still be valid, access it
	assertTrue (*tmp == 2);
	tmp = aCache.get(1);
	assertTrue (!tmp);

	// Add item 1, wait a bit, then add item 3
	// Item 1 should expire before item 3
	aCache.add(1, 2); // 1
	Thread::sleep(EXPIRE_TIME / 4); // Small delay so items have different expiration times
	aCache.add(3, 4); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	tmp = aCache.get(1);
	SharedPtr<int> tmp2 = aCache.get(3);
	assertTrue (*tmp == 2);
	assertTrue (*tmp2 == 4);

	// Wait for item 1 to expire (item 3 should still be valid)
	assertTrue (waitForCondition([&]{ return !aCache.has(1); }, MAX_WAIT_TIME));
	assertTrue (aCache.has(3));
	assertTrue (*tmp == 2); // SharedPtr still valid
	assertTrue (*tmp2 == 4);
	tmp2 = aCache.get(3);
	assertTrue (*tmp2 == 4);

	// Wait for item 3 to expire
	assertTrue (waitForCondition([&]{ return !aCache.has(3); }, MAX_WAIT_TIME));
	assertTrue (*tmp2 == 4); // SharedPtr still valid
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
	ExpireCache<int, int> aCache(EXPIRE_TIME);
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
	AccessExpireCache<int, int> aCache(EXPIRE_TIME);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	SharedPtr<int> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (*tmp == 2);
	assertTrue (aCache.size() == 1);

	// Wait for item 1 to expire
	assertTrue (waitForCondition([&]{ return aCache.size() == 0; }, MAX_WAIT_TIME));
	assertTrue (!aCache.has(1));

	// tmp must still be valid, access it
	assertTrue (*tmp == 2);
	tmp = aCache.get(1);
	assertTrue (!tmp);

	// Add item 1, wait a bit, then add item 3
	aCache.add(1, 2); // 1
	Thread::sleep(EXPIRE_TIME / 4);
	aCache.add(3, 4); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));

	// Wait for item 1 to expire
	assertTrue (waitForCondition([&]{ return !aCache.has(1); }, MAX_WAIT_TIME));
	assertTrue (*aCache.get(3) == 4);

	// Access item 3 to refresh its expiration, then verify it's still accessible
	Thread::sleep(EXPIRE_TIME / 4);
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
	ExpireCache<int, int> aCache(EXPIRE_TIME);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));

	// Wait for item 1 to expire
	assertTrue (waitForCondition([&]{ return !aCache.has(1); }, MAX_WAIT_TIME));
}


void ExpireCacheTest::testAccessExpireGet()
{
	AccessExpireCache<int, int> aCache(EXPIRE_TIME);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	SharedPtr<int> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (*tmp == 2);
	assertTrue (aCache.size() == 1);

	// Wait for item to expire (use has() not get() to avoid refreshing expiration)
	assertTrue (waitForCondition([&]{ return !aCache.has(1); }, MAX_WAIT_TIME));
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
