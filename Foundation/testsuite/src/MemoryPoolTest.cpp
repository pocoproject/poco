//
// MemoryPoolTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MemoryPoolTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/MemoryPool.h"
#include <vector>


using Poco::MemoryPool;


MemoryPoolTest::MemoryPoolTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


MemoryPoolTest::~MemoryPoolTest()
{
}


void MemoryPoolTest::testMemoryPool()
{
	MemoryPool pool1(100, 0, 10);
	
	assertTrue (pool1.blockSize() == 100);
	assertTrue (pool1.allocated() == 0);
	assertTrue (pool1.available() == 0);
	
	std::vector<void*> ptrs;
	for (int i = 0; i < 10; ++i)
	{
		ptrs.push_back(pool1.get());
		assertTrue (pool1.allocated() == i + 1);
		assertTrue (pool1.available() == 0);
	}
	
	try
	{
		pool1.get();
		fail("pool exhausted - must throw exception");
	}
	catch (Poco::OutOfMemoryException&)
	{
	}
	
	int av = 0;
	for (std::vector<void*>::iterator it = ptrs.begin(); it != ptrs.end(); ++it)
	{
		pool1.release(*it);
		++av;
		assertTrue (pool1.available() == av);
	}
	
	MemoryPool pool2(32, 5, 10);
	assertTrue (pool2.available() == 5);
	assertTrue (pool2.blockSize() == 32);
	assertTrue (pool2.allocated() == 5);
}


void MemoryPoolTest::setUp()
{
}


void MemoryPoolTest::tearDown()
{
}


CppUnit::Test* MemoryPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MemoryPoolTest");

	CppUnit_addTest(pSuite, MemoryPoolTest, testMemoryPool);

	return pSuite;
}
