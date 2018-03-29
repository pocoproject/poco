//
// ObjectPoolTest.cpp
//
// Copyright (c) 2010-2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ObjectPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ObjectPool.h"
#include "Poco/Exception.h"


using Poco::ObjectPool;


ObjectPoolTest::ObjectPoolTest(const std::string& name): CppUnit::TestCase(name)
{
}


ObjectPoolTest::~ObjectPoolTest()
{
}


void ObjectPoolTest::testObjectPool()
{
	ObjectPool<std::string, Poco::SharedPtr<std::string> > pool(3, 4);
	
	assertTrue (pool.capacity() == 3);
	assertTrue (pool.peakCapacity() == 4);
	assertTrue (pool.size() == 0);
	assertTrue (pool.available() == 4);
	
	Poco::SharedPtr<std::string> pStr1 = pool.borrowObject();
	pStr1->assign("first");
	assertTrue (pool.size() == 1);
	assertTrue (pool.available() == 3);
	
	Poco::SharedPtr<std::string> pStr2 = pool.borrowObject();
	pStr2->assign("second");
	assertTrue (pool.size() == 2);
	assertTrue (pool.available() == 2);

	Poco::SharedPtr<std::string> pStr3 = pool.borrowObject();
	pStr3->assign("third");
	assertTrue (pool.size() == 3);
	assertTrue (pool.available() == 1);
	
	Poco::SharedPtr<std::string> pStr4 = pool.borrowObject();
	pStr4->assign("fourth");
	assertTrue (pool.size() == 4);
	assertTrue (pool.available() == 0);
	
	Poco::SharedPtr<std::string> pStr5 = pool.borrowObject();
	assertTrue (pStr5.isNull());
	
	pool.returnObject(pStr4);
	assertTrue (pool.size() == 4);
	assertTrue (pool.available() == 1);
	
	pool.returnObject(pStr3);
	assertTrue (pool.size() == 4);
	assertTrue (pool.available() == 2);

	pStr3 = pool.borrowObject();
	assertTrue (*pStr3 == "third");
	assertTrue (pool.available() == 1);

	pool.returnObject(pStr3);
	pool.returnObject(pStr2);
	pool.returnObject(pStr1);
	
	assertTrue (pool.size() == 3);
	assertTrue (pool.available() == 4);
	
	pStr1 = pool.borrowObject();
	assertTrue (*pStr1 == "second");
	assertTrue (pool.available() == 3);

	pool.returnObject(pStr1);
	assertTrue (pool.available() == 4);
}


void ObjectPoolTest::setUp()
{
}


void ObjectPoolTest::tearDown()
{
}


CppUnit::Test* ObjectPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ObjectPoolTest");

	CppUnit_addTest(pSuite, ObjectPoolTest, testObjectPool);

	return pSuite;
}
