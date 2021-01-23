//
// LRUCacheTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LRUCacheTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/LRUCache.h"
#include "Poco/Bugcheck.h"
#include "Poco/Delegate.h"


using namespace Poco;


LRUCacheTest::LRUCacheTest(const std::string& name): CppUnit::TestCase(name)
{
}


LRUCacheTest::~LRUCacheTest()
{
}


void LRUCacheTest::testClear()
{
	LRUCache<int, int> aCache(3);
	assertTrue (aCache.size() == 0);
	assertTrue (aCache.getAllKeys().size() == 0);
	aCache.add(1, 2);
	aCache.add(3, 4);
	aCache.add(5, 6);
	assertTrue (aCache.size() == 3);
	assertTrue (aCache.getAllKeys().size() == 3);
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


void LRUCacheTest::testCacheSize0()
{
	// cache size 0 is illegal
	try
	{
		LRUCache<int, int> aCache(0);
		failmsg ("cache size of 0 is illegal, test should fail");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void LRUCacheTest::testCacheSize1()
{
	LRUCache<int, int> aCache(1);
	aCache.add(1, 2);
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 2);

	aCache.add(3, 4); // replaces 1
	assertTrue (!aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (*aCache.get(3) == 4);

	aCache.add(5, 6);
	assertTrue (!aCache.has(1));
	assertTrue (!aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (*aCache.get(5) == 6);

	aCache.remove(5);
	assertTrue (!aCache.has(5));

	// removing illegal entries should work too
	aCache.remove(666);
}


void LRUCacheTest::testCacheSize2()
{
	// 3-1 represents the cache sorted by pos, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	LRUCache<int, int> aCache(2);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 2);

	aCache.add(3, 4); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (*aCache.get(1) == 2); // 1-3
	assertTrue (*aCache.get(3) == 4); // 3-1

	aCache.add(5, 6); // 5-3|1
	assertTrue (!aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (*aCache.get(5) == 6);  // 5-3
	assertTrue (*aCache.get(3) == 4);  // 3-5

	// test remove from the end and the beginning of the list
	aCache.remove(5); // 3
	assertTrue (!aCache.has(5));
	assertTrue (*aCache.get(3) == 4);  // 3
	aCache.add(5, 6); // 5-3
	assertTrue (*aCache.get(3) == 4);  // 3-5
	aCache.remove(3); // 5
	assertTrue (!aCache.has(3));
	assertTrue (*aCache.get(5) == 6);  // 5

	// removing illegal entries should work too
	aCache.remove(666);

	aCache.clear();
	assertTrue (!aCache.has(5));
}


void LRUCacheTest::testCacheSizeN()
{
		// 3-1 represents the cache sorted by pos, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	LRUCache<int, int> aCache(3);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 2);

	aCache.add(3, 4); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (*aCache.get(1) == 2); // 1-3
	assertTrue (*aCache.get(3) == 4); // 3-1

	aCache.add(5, 6); // 5-3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (*aCache.get(5) == 6);  // 5-3-1
	assertTrue (*aCache.get(3) == 4);  // 3-5-1

	aCache.add(7, 8); // 7-5-3|1
	assertTrue (!aCache.has(1));
	assertTrue (aCache.has(7));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (*aCache.get(5) == 6);  // 5-7-3
	assertTrue (*aCache.get(3) == 4);  // 3-5-7
	assertTrue (*aCache.get(7) == 8);  // 7-3-5

	// test remove from the end and the beginning of the list
	aCache.remove(5); // 7-3
	assertTrue (!aCache.has(5));
	assertTrue (*aCache.get(3) == 4);  // 3-7
	aCache.add(5, 6); // 5-3-7
	assertTrue (*aCache.get(7) == 8);  // 7-5-3
	aCache.remove(7); // 5-3
	assertTrue (!aCache.has(7));
	assertTrue (aCache.has(3));
	assertTrue (*aCache.get(5) == 6);  // 5-3

	// removing illegal entries should work too
	aCache.remove(666);

	aCache.clear();
	assertTrue (!aCache.has(5));
	assertTrue (!aCache.has(3));
}


void LRUCacheTest::testDuplicateAdd()
{
	LRUCache<int, int> aCache(3);
	aCache.add(1, 2); // 1
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 2);
	aCache.add(1, 3);
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 3);
}


void LRUCacheTest::testUpdate()
{
	addCnt = 0;
	updateCnt = 0;
	removeCnt = 0;
	LRUCache<int, int> aCache(3);
	aCache.Add += delegate(this, &LRUCacheTest::onAdd);
	aCache.Remove += delegate(this, &LRUCacheTest::onRemove);
	aCache.Update += delegate(this, &LRUCacheTest::onUpdate);
	aCache.add(1, 2); // 1 ,one add event
	assertTrue (addCnt == 1);
	assertTrue (updateCnt == 0);
	assertTrue (removeCnt == 0);

	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 2);
	aCache.update(1, 3); // one update event only!
	assertTrue (addCnt == 1);
	assertTrue (updateCnt == 1);
	assertTrue (removeCnt == 0);
	assertTrue (aCache.has(1));
	assertTrue (*aCache.get(1) == 3);
}


void LRUCacheTest::onUpdate(const void* pSender, const Poco::KeyValueArgs<int, int>& args)
{
	++updateCnt;
}


void LRUCacheTest::onAdd(const void* pSender, const Poco::KeyValueArgs<int, int>& args)
{
	++addCnt;
}


void LRUCacheTest::onRemove(const void* pSender, const int& args)
{
	++removeCnt;
}


void LRUCacheTest::setUp()
{
}


void LRUCacheTest::tearDown()
{
}


CppUnit::Test* LRUCacheTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LRUCacheTest");

	CppUnit_addTest(pSuite, LRUCacheTest, testClear);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSize0);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSize1);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSize2);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSizeN);
	CppUnit_addTest(pSuite, LRUCacheTest, testDuplicateAdd);
	CppUnit_addTest(pSuite, LRUCacheTest, testUpdate);

	return pSuite;
}
