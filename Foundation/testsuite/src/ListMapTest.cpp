//
// ListMapTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ListMapTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/ListMap.h"
#include "Poco/Exception.h"
#include <map>


using Poco::ListMap;


ListMapTest::ListMapTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


ListMapTest::~ListMapTest()
{
}


void ListMapTest::testInsert()
{
	const int N = 1000;

	typedef ListMap<int, int> IntMap;
	IntMap hm;
	
	assertTrue (hm.empty());
	
	for (int i = 0; i < N; ++i)
	{
		IntMap::Iterator res = hm.insert(IntMap::ValueType(i, i*2));
		assertTrue (res->first == i);
		assertTrue (res->second == i*2);
		IntMap::Iterator it = hm.find(i);
		assertTrue (it != hm.end());
		assertTrue (it->first == i);
		assertTrue (it->second == i*2);
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
	
	hm.clear();
	for (int i = 0; i < N; ++i)
	{
		IntMap::Iterator res = hm.insert(IntMap::ValueType(i, 0));
		assertTrue (res->first == i);
		assertTrue (res->second == 0);
	}		
}


void ListMapTest::testInsertOrder()
{
	const int N = 1000;

	typedef ListMap<std::string, int> StrToIntMap;
	StrToIntMap lm;

	lm.insert(StrToIntMap::ValueType("foo", 42));
	lm.insert(StrToIntMap::ValueType("bar", 43));
	
	StrToIntMap::Iterator it = lm.begin();
	assertTrue (it != lm.end() && it->first == "foo" && it->second == 42);
	
	++it;
	assertTrue (it != lm.end() && it->first == "bar" && it->second == 43);
	
	++it;
	assertTrue (it == lm.end());

	lm.insert(StrToIntMap::ValueType("foo", 44));

 	it = lm.begin();
	assertTrue (it != lm.end() && it->first == "foo" && it->second == 42);
	
	++it;
	assertTrue (it != lm.end() && it->first == "foo" && it->second == 44);

	++it;
	assertTrue (it != lm.end() && it->first == "bar" && it->second == 43);
	
	++it;
	assertTrue (it == lm.end());	
}


void ListMapTest::testErase()
{
	const int N = 1000;

	typedef ListMap<int, int> IntMap;
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
		assertTrue (it->first == i);
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


void ListMapTest::testIterator()
{
	const int N = 1000;

	typedef ListMap<int, int> IntMap;
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


void ListMapTest::testConstIterator()
{
	const int N = 1000;

	typedef ListMap<int, int> IntMap;
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


void ListMapTest::testIntIndex()
{
	typedef ListMap<int, int> IntMap;
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
		int x = im[4];
		fail("no such key - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void ListMapTest::testStringIndex()
{
	typedef ListMap<const char*, std::string> StringMap;
	StringMap hm;

	hm["index1"] = "value2";
	hm["index2"] = "value4";
	hm["index3"] = "value6";
	
	assertTrue (hm.size() == 3);
	assertTrue (hm["index1"] == "value2");
	assertTrue (hm["Index2"] == "value4");
	assertTrue (hm["inDeX3"] == "value6");
	
	try
	{
		const StringMap& im = hm;
		std::string x = im["index4"];
		fail("no such key - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void ListMapTest::setUp()
{
}


void ListMapTest::tearDown()
{
}


CppUnit::Test* ListMapTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ListMapTest");

	CppUnit_addTest(pSuite, ListMapTest, testInsert);
	CppUnit_addTest(pSuite, ListMapTest, testInsertOrder);
	CppUnit_addTest(pSuite, ListMapTest, testErase);
	CppUnit_addTest(pSuite, ListMapTest, testIterator);
	CppUnit_addTest(pSuite, ListMapTest, testConstIterator);
	CppUnit_addTest(pSuite, ListMapTest, testIntIndex);
	CppUnit_addTest(pSuite, ListMapTest, testStringIndex);

	return pSuite;
}
