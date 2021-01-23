//
// HashMapTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HashMapTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/HashMap.h"
#include "Poco/Exception.h"
#include <map>


using Poco::HashMap;


HashMapTest::HashMapTest(const std::string& name): CppUnit::TestCase(name)
{
}


HashMapTest::~HashMapTest()
{
}


void HashMapTest::testInsert()
{
	const int N = 1000;

	typedef HashMap<int, int> IntMap;
	IntMap hm;
	
	assertTrue (hm.empty());
	
	for (int i = 0; i < N; ++i)
	{
		std::pair<IntMap::Iterator, bool> res = hm.insert(IntMap::ValueType(i, i*2));
		assertTrue (res.first->first == i);
		assertTrue (res.first->second == i*2);
		assertTrue (res.second);
		IntMap::Iterator it = hm.find(i);
		assertTrue (it != hm.end());
		assertTrue (it->first == i);
		assertTrue (it->second == i*2);
		assertTrue (hm.count(i) == 1);
		assertTrue (hm.size() == i + 1);
	}		
	
	assertTrue (!hm.empty());
	
	for (int i = 0; i < N; ++i)
	{
		IntMap::Iterator it = hm.find(i);
		assertTrue (it != hm.end());
		assertTrue (it->first == i);
		assertTrue (it->second == i*2);
	}
	
	for (int i = 0; i < N; ++i)
	{
		std::pair<IntMap::Iterator, bool> res = hm.insert(IntMap::ValueType(i, 0));
		assertTrue (res.first->first == i);
		assertTrue (res.first->second == i*2);
		assertTrue (!res.second);
	}		
}


void HashMapTest::testErase()
{
	const int N = 1000;

	typedef HashMap<int, int> IntMap;
	IntMap hm;

	for (int i = 0; i < N; ++i)
	{
		hm.insert(IntMap::ValueType(i, i*2));
	}
	assertTrue (hm.size() == N);
	
	for (int i = 0; i < N; i += 2)
	{
		hm.erase(i);
		IntMap::Iterator it = hm.find(i);
		assertTrue (it == hm.end());
	}
	assertTrue (hm.size() == N/2);
	
	for (int i = 0; i < N; i += 2)
	{
		IntMap::Iterator it = hm.find(i);
		assertTrue (it == hm.end());
	}
	
	for (int i = 1; i < N; i += 2)
	{
		IntMap::Iterator it = hm.find(i);
		assertTrue (it != hm.end());
		assertTrue (*it == i);
	}

	for (int i = 0; i < N; i += 2)
	{
		hm.insert(IntMap::ValueType(i, i*2));
	}
	
	for (int i = 0; i < N; ++i)
	{
		IntMap::Iterator it = hm.find(i);
		assertTrue (it != hm.end());
		assertTrue (it->first == i);
		assertTrue (it->second == i*2);		
	}
}


void HashMapTest::testIterator()
{
	const int N = 1000;

	typedef HashMap<int, int> IntMap;
	IntMap hm;

	for (int i = 0; i < N; ++i)
	{
		hm.insert(IntMap::ValueType(i, i*2));
	}
	
	std::map<int, int> values;
	IntMap::Iterator it; // do not initialize here to test proper behavior of uninitialized iterators
	it = hm.begin();
	while (it != hm.end())
	{
		assertTrue (values.find(it->first) == values.end());
		values[it->first] = it->second;
		++it;
	}
	
	assertTrue (values.size() == N);
}


void HashMapTest::testConstIterator()
{
	const int N = 1000;

	typedef HashMap<int, int> IntMap;
	IntMap hm;

	for (int i = 0; i < N; ++i)
	{
		hm.insert(IntMap::ValueType(i, i*2));
	}
	
	std::map<int, int> values;
	IntMap::ConstIterator it = hm.begin();
	while (it != hm.end())
	{
		assertTrue (values.find(it->first) == values.end());
		values[it->first] = it->second;
		++it;
	}
	
	assertTrue (values.size() == N);
}


void HashMapTest::testIndex()
{
	typedef HashMap<int, int> IntMap;
	IntMap hm;

	hm[1] = 2;
	hm[2] = 4;
	hm[3] = 6;
	
	assertTrue (hm.size() == 3);
	assertTrue (hm[1] == 2);
	assertTrue (hm[2] == 4);
	assertTrue (hm[3] == 6);
	
	try
	{
		const IntMap& im = hm;
		int POCO_UNUSED x = im[4];
		fail("no such key - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void HashMapTest::setUp()
{
}


void HashMapTest::tearDown()
{
}


CppUnit::Test* HashMapTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HashMapTest");

	CppUnit_addTest(pSuite, HashMapTest, testInsert);
	CppUnit_addTest(pSuite, HashMapTest, testErase);
	CppUnit_addTest(pSuite, HashMapTest, testIterator);
	CppUnit_addTest(pSuite, HashMapTest, testConstIterator);
	CppUnit_addTest(pSuite, HashMapTest, testIndex);

	return pSuite;
}
