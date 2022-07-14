//
// UniqueExpireCacheTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UniqueExpireCacheTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/UniqueExpireCache.h"
#include "Poco/UniqueAccessExpireCache.h"
#include "Poco/ExpirationDecorator.h"
#include "Poco/AccessExpirationDecorator.h"
#include "Poco/Bugcheck.h"
#include "Poco/Thread.h"


using namespace Poco;


struct IntVal
{
	int value;
	Poco::Timestamp validUntil;
	IntVal(int val, Poco::Timestamp::TimeDiff v):value(val), validUntil()
	{
		validUntil += (v*1000);
	}

	const Poco::Timestamp& getExpiration() const
	{
		return validUntil;
	}
};

typedef AccessExpirationDecorator<int> DIntVal;

#define DURSLEEP 250
#define DURHALFSLEEP DURSLEEP / 2
#define DURWAIT  300


UniqueExpireCacheTest::UniqueExpireCacheTest(const std::string& name): CppUnit::TestCase(name)
{
}


UniqueExpireCacheTest::~UniqueExpireCacheTest()
{
}


void UniqueExpireCacheTest::testClear()
{
	UniqueExpireCache<int, IntVal> aCache;
	aCache.add(1, IntVal(2, DURSLEEP));
	aCache.add(3, IntVal(4, DURSLEEP));
	aCache.add(5, IntVal(6, DURSLEEP));
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (aCache.get(1)->value == 2);
	assertTrue (aCache.get(3)->value == 4);
	assertTrue (aCache.get(5)->value == 6);
	aCache.clear();
	assertTrue (!aCache.has(1));
	assertTrue (!aCache.has(3));
	assertTrue (!aCache.has(5));
}


void UniqueExpireCacheTest::testAccessClear()
{
	UniqueAccessExpireCache<int, DIntVal> aCache;
	aCache.add(1, DIntVal(2, DURSLEEP));
	aCache.add(3, DIntVal(4, DURSLEEP));
	aCache.add(5, DIntVal(6, DURSLEEP));
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (aCache.get(1)->value() == 2);
	assertTrue (aCache.get(3)->value() == 4);
	assertTrue (aCache.get(5)->value() == 6);
	aCache.clear();
	assertTrue (!aCache.has(1));
	assertTrue (!aCache.has(3));
	assertTrue (!aCache.has(5));
}


void UniqueExpireCacheTest::testAccessUpdate()
{
	UniqueAccessExpireCache<int, DIntVal> aCache;
	aCache.add(1, DIntVal(2, DURSLEEP));
	aCache.add(3, DIntVal(4, DURSLEEP));
	aCache.add(5, DIntVal(6, DURSLEEP));
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (aCache.has(5));
	assertTrue (aCache.get(1)->value() == 2);
	Thread::sleep(DURSLEEP/2);
	assertTrue (aCache.get(1)->value() == 2);
	Thread::sleep(DURSLEEP/2);
	assertTrue (aCache.get(1)->value() == 2);
	Thread::sleep(DURSLEEP/2);
	assertTrue (aCache.get(1)->value() == 2);
	assertTrue (!aCache.has(3));
	assertTrue (!aCache.has(5));
	Thread::sleep(DURSLEEP*2);

	assertTrue (!aCache.has(1));
	assertTrue (!aCache.has(3));
	assertTrue (!aCache.has(5));
	aCache.remove(666); //must work too
}


void UniqueExpireCacheTest::testExpire0()
{
	UniqueExpireCache<int, IntVal> aCache;
	aCache.add(1, IntVal(2, 0));
	assertTrue (!aCache.has(1));
}



void UniqueExpireCacheTest::testAccessExpire0()
{
	UniqueAccessExpireCache<int, DIntVal> aCache;
	aCache.add(1, DIntVal(2, Timespan(0, 0)));
	assertTrue (!aCache.has(1));
}


void UniqueExpireCacheTest::testExpireN()
{
	// 3-1 represents the cache sorted by age, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	UniqueExpireCache<int, IntVal> aCache;
	aCache.add(1, IntVal(2, DURSLEEP)); // 1
	assertTrue (aCache.has(1));
	SharedPtr<IntVal> tmp = aCache.get(1);
	assertTrue (!tmp.isNull());
	assertTrue (tmp->value == 2);
	Thread::sleep(DURWAIT);
	assertTrue (!aCache.has(1));

	// tmp must still be valid, access it
	assertTrue (tmp->value == 2);
	tmp = aCache.get(1);
	assertTrue (tmp.isNull());

	aCache.add(1, IntVal(2, DURSLEEP)); // 1
	Thread::sleep(DURHALFSLEEP);
	aCache.add(3, IntVal(4, DURSLEEP)); // 3-1
	assertTrue (aCache.has(1));
	assertTrue (aCache.has(3));
	tmp = aCache.get(1);
	SharedPtr<IntVal> tmp2 = aCache.get(3);
	assertTrue (tmp->value == 2);
	assertTrue (tmp2->value == 4);

	Thread::sleep(DURHALFSLEEP+25); //3|1
	assertTrue (!aCache.has(1));
	assertTrue (aCache.has(3));
	assertTrue (tmp->value == 2); // 1-3
	assertTrue (tmp2->value == 4); // 3-1
	tmp2 = aCache.get(3);
	assertTrue (tmp2->value == 4);
	Thread::sleep(DURHALFSLEEP+25); //3|1
	assertTrue (!aCache.has(3));
	assertTrue (tmp2->value == 4);
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


void UniqueExpireCacheTest::testDuplicateAdd()
{
	UniqueExpireCache<int, IntVal> aCache;
	aCache.add(1, IntVal(2, DURSLEEP)); // 1
	assertTrue (aCache.has(1));
	assertTrue (aCache.get(1)->value == 2);
	aCache.add(1, IntVal(3, DURSLEEP));
	assertTrue (aCache.has(1));
	assertTrue (aCache.get(1)->value == 3);
}


void UniqueExpireCacheTest::testAccessDuplicateAdd()
{
	UniqueAccessExpireCache<int, DIntVal> aCache;
	aCache.add(1, DIntVal(2, DURSLEEP)); // 1
	assertTrue (aCache.has(1));
	assertTrue (aCache.get(1)->value() == 2);
	aCache.add(1, DIntVal(3, DURSLEEP));
	assertTrue (aCache.has(1));
	assertTrue (aCache.get(1)->value() == 3);
}


void UniqueExpireCacheTest::testExpirationDecorator()
{
	typedef ExpirationDecorator<int> ExpireInt;
	UniqueExpireCache<int, ExpireInt> aCache;
	aCache.add(1, ExpireInt(2, DURSLEEP)); // 1
	assertTrue (aCache.has(1));
	assertTrue (aCache.get(1)->value() == 2);
	aCache.add(1, ExpireInt(3, DURSLEEP));
	assertTrue (aCache.has(1));
	assertTrue (aCache.get(1)->value() == 3);
}


void UniqueExpireCacheTest::setUp()
{
}


void UniqueExpireCacheTest::tearDown()
{
}


CppUnit::Test* UniqueExpireCacheTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UniqueExpireCacheTest");

	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testClear);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testAccessClear);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testAccessUpdate);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testExpire0);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testAccessExpire0);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testExpireN);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testDuplicateAdd);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testAccessDuplicateAdd);
	CppUnit_addTest(pSuite, UniqueExpireCacheTest, testExpirationDecorator);

	return pSuite;
}
