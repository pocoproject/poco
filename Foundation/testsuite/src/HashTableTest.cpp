//
// HashTableTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#if defined(POCO_TEST_DEPRECATED)


#include "HashTableTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/HashTable.h"
#include "Poco/NumberFormatter.h"


using namespace Poco;


HashTableTest::HashTableTest(const std::string& name): CppUnit::TestCase(name)
{
}


HashTableTest::~HashTableTest()
{
}


void HashTableTest::testInsert()
{
	std::string s1("str1");
	std::string s2("str2");
	HashTable<std::string, int> hashTable;
	assertTrue (!hashTable.exists(s1));
	hashTable.insert(s1, 13);
	assertTrue (hashTable.exists(s1));
	assertTrue (hashTable.get(s1) == 13);
	int retVal = 0;

	assertTrue (hashTable.get(s1, retVal));
	assertTrue (retVal == 13);
	try
	{
		hashTable.insert(s1, 22);
		failmsg ("duplicate insert must fail");
	}
	catch (Exception&){}
	try
	{
		hashTable.get(s2);
		failmsg ("getting a non inserted item must fail");
	}
	catch (Exception&){}

	assertTrue (!hashTable.exists(s2));
	hashTable.insert(s2, 13);
	assertTrue (hashTable.exists(s2));
}


void HashTableTest::testUpdate()
{
	// add code for second test here
	std::string s1("str1");
	std::string s2("str2");
	HashTable<std::string, int> hashTable;
	hashTable.insert(s1, 13);
	hashTable.update(s1, 14);
	assertTrue (hashTable.exists(s1));
	assertTrue (hashTable.get(s1) == 14);
	int retVal = 0;

	assertTrue (hashTable.get(s1, retVal));
	assertTrue (retVal == 14);

	// updating a non existing item must work too
	hashTable.update(s2, 15);
	assertTrue (hashTable.get(s2) == 15);
}


void HashTableTest::testOverflow()
{
	HashTable<std::string, int> hashTable(13);
	for (int i = 0; i < 1024; ++i)
	{
		hashTable.insert(Poco::NumberFormatter::format(i), i*i);
	}

	for (int i = 0; i < 1024; ++i)
	{
		std::string tmp = Poco::NumberFormatter::format(i);
		assertTrue (hashTable.exists(tmp));
		assertTrue (hashTable.get(tmp) == i*i);
	}
}


void HashTableTest::testSize()
{
	HashTable<std::string, int> hashTable(13);
	assertTrue (hashTable.size() == 0);
	Poco::UInt32 POCO_UNUSED h1 = hashTable.insert("1", 1);
	assertTrue (hashTable.size() == 1);
	Poco::UInt32 POCO_UNUSED h2 = hashTable.update("2", 2);
	assertTrue (hashTable.size() == 2);
	hashTable.remove("1");
	assertTrue (hashTable.size() == 1);
	hashTable.remove("3");
	assertTrue (hashTable.size() == 1);
	hashTable.removeRaw("2", h2);
	assertTrue (hashTable.size() == 0);
	hashTable.insert("1", 1);
	hashTable.insert("2", 2);
	assertTrue (hashTable.size() == 2);
	hashTable.clear();
	assertTrue (hashTable.size() == 0);
}


void HashTableTest::testResize()
{
	HashTable<std::string, int> hashTable(13);
	assertTrue (hashTable.size() == 0);
	hashTable.resize(19);
	for (int i = 0; i < 1024; ++i)
	{
		hashTable.insert(Poco::NumberFormatter::format(i), i*i);
	}
	hashTable.resize(1009);

	for (int i = 0; i < 1024; ++i)
	{
		std::string tmp = Poco::NumberFormatter::format(i);
		assertTrue (hashTable.exists(tmp));
		assertTrue (hashTable.get(tmp) == i*i);
	}
}


void HashTableTest::testStatistic()
{
	double relax = 0.001;
	HashTable<std::string, int> hashTable(13);
	assertTrue (hashTable.size() == 0);
	HashStatistic stat1(hashTable.currentState());
	assertTrue (stat1.avgEntriesPerHash() < relax && stat1.avgEntriesPerHash() > -relax);
	assertTrue (stat1.maxPositionsOfTable() == 13);
	assertTrue (stat1.maxEntriesPerHash() == 0);

	hashTable.resize(19);
	stat1 = hashTable.currentState(true);
	assertTrue (stat1.avgEntriesPerHash() < relax && stat1.avgEntriesPerHash() > -relax);
	assertTrue (stat1.maxPositionsOfTable() == 19);
	assertTrue (stat1.maxEntriesPerHash() == 0);
	assertTrue (stat1.detailedEntriesPerHash().size() == 19);

	for (int i = 0; i < 1024; ++i)
	{
		hashTable.insert(Poco::NumberFormatter::format(i), i*i);
	}
	stat1 = hashTable.currentState(true);
	double expAvg = 1024.0/ 19;
	assertTrue (stat1.avgEntriesPerHash() < (expAvg + relax) && stat1.avgEntriesPerHash() > (expAvg - relax));
	assertTrue (stat1.maxPositionsOfTable() == 19);
	assertTrue (stat1.maxEntriesPerHash() > expAvg);
	hashTable.resize(1009);
	stat1 = hashTable.currentState(true);

	expAvg = 1024.0/ 1009;

	assertTrue (stat1.avgEntriesPerHash() < (expAvg + relax) && stat1.avgEntriesPerHash() > (expAvg - relax));
	assertTrue (stat1.maxPositionsOfTable() == 1009);
	assertTrue (stat1.maxEntriesPerHash() > expAvg);
}


void HashTableTest::setUp()
{
}


void HashTableTest::tearDown()
{
}


CppUnit::Test* HashTableTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HashTableTest");

	CppUnit_addTest(pSuite, HashTableTest, testInsert);
	CppUnit_addTest(pSuite, HashTableTest, testUpdate);
	CppUnit_addTest(pSuite, HashTableTest, testOverflow);
	CppUnit_addTest(pSuite, HashTableTest, testSize);
	CppUnit_addTest(pSuite, HashTableTest, testResize);
	CppUnit_addTest(pSuite, HashTableTest, testStatistic);

	return pSuite;
}

#endif
