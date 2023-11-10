//
// LinearHashTableTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LinearHashTableTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/LinearHashTable.h"
#include "Poco/HashTable.h"
#include "Poco/Stopwatch.h"
#include "Poco/NumberFormatter.h"
#include <set>
#include <iostream>


#ifdef POCO_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4834) // divide by zero
#endif // POCO_COMPILER_MSVC


using Poco::LinearHashTable;
using Poco::Hash;
using Poco::HashTable;
using Poco::Stopwatch;
using Poco::NumberFormatter;


LinearHashTableTest::LinearHashTableTest(const std::string& name): CppUnit::TestCase(name)
{
}


LinearHashTableTest::~LinearHashTableTest()
{
}


void LinearHashTableTest::testInsert()
{
	const int N = 1000;

	LinearHashTable<int, Hash<int> > ht;

	assertTrue (ht.empty());

	for (int i = 0; i < N; ++i)
	{
		std::pair<LinearHashTable<int, Hash<int> >::Iterator, bool> res = ht.insert(i);
		assertTrue (*res.first == i);
		assertTrue (res.second);
		LinearHashTable<int, Hash<int> >::Iterator it = ht.find(i);
		assertTrue (it != ht.end());
		assertTrue (*it == i);
		assertTrue (ht.size() == i + 1);
	}
	assertTrue (ht.buckets() == N + 1);

	assertTrue (!ht.empty());

	for (int i = 0; i < N; ++i)
	{
		LinearHashTable<int, Hash<int> >::Iterator it = ht.find(i);
		assertTrue (it != ht.end());
		assertTrue (*it == i);
	}

	for (int i = 0; i < N; ++i)
	{
		std::pair<LinearHashTable<int, Hash<int> >::Iterator, bool> res = ht.insert(i);
		assertTrue (*res.first == i);
		assertTrue (!res.second);
		assertTrue (ht.size() == N);
		assertTrue (ht.buckets() == N + 1);
	}
}


void LinearHashTableTest::testErase()
{
	const int N = 1000;

	LinearHashTable<int, Hash<int> > ht;

	for (int i = 0; i < N; ++i)
	{
		ht.insert(i);
	}
	assertTrue (ht.size() == N);

	for (int i = 0; i < N; i += 2)
	{
		ht.erase(i);
		LinearHashTable<int, Hash<int> >::Iterator it = ht.find(i);
		assertTrue (it == ht.end());
	}
	assertTrue (ht.size() == N/2);

	for (int i = 0; i < N; i += 2)
	{
		LinearHashTable<int, Hash<int> >::Iterator it = ht.find(i);
		assertTrue (it == ht.end());
	}

	for (int i = 1; i < N; i += 2)
	{
		LinearHashTable<int, Hash<int> >::Iterator it = ht.find(i);
		assertTrue (it != ht.end());
		assertTrue (*it == i);
	}

	for (int i = 0; i < N; i += 2)
	{
		ht.insert(i);
	}

	for (int i = 0; i < N; ++i)
	{
		LinearHashTable<int, Hash<int> >::Iterator it = ht.find(i);
		assertTrue (it != ht.end());
		assertTrue (*it == i);
	}
}


void LinearHashTableTest::testIterator()
{
	const int N = 1000;

	LinearHashTable<int, Hash<int> > ht;

	for (int i = 0; i < N; ++i)
	{
		ht.insert(i);
	}

	std::set<int> values;
	LinearHashTable<int, Hash<int> >::Iterator it = ht.begin();
	while (it != ht.end())
	{
		assertTrue (values.find(*it) == values.end());
		values.insert(*it);
		++it;
	}

	assertTrue (values.size() == N);
}


void LinearHashTableTest::testConstIterator()
{
	const int N = 1000;

	LinearHashTable<int, Hash<int> > ht;

	for (int i = 0; i < N; ++i)
	{
		ht.insert(i);
	}

	std::set<int> values;
	LinearHashTable<int, Hash<int> >::ConstIterator it = ht.begin();
	while (it != ht.end())
	{
		assertTrue (values.find(*it) == values.end());
		values.insert(*it);
		++it;
	}

	assertTrue (values.size() == N);

	values.clear();
	const LinearHashTable<int, Hash<int> > cht(ht);

	LinearHashTable<int, Hash<int> >::ConstIterator cit = cht.begin();
	while (cit != cht.end())
	{
		assertTrue (values.find(*cit) == values.end());
		values.insert(*cit);
		++cit;
	}

	assertTrue (values.size() == N);
}


void LinearHashTableTest::testPerformanceInt()
{
	const int N = 5000000;
	Stopwatch sw;

	{
		LinearHashTable<int, Hash<int> > lht(N);
		sw.start();
		for (int i = 0; i < N; ++i)
		{
			lht.insert(i);
		}
		sw.stop();
		std::cout << "Insert LHT: " << sw.elapsedSeconds() << std::endl;
		sw.reset();

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			lht.find(i);
		}
		sw.stop();
		std::cout << "Find LHT: " << sw.elapsedSeconds() << std::endl;
		sw.reset();
	}

	{
		HashTable<int, int> ht;

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			ht.insert(i, i);
		}
		sw.stop();
		std::cout << "Insert HT: " << sw.elapsedSeconds() << std::endl;
		sw.reset();

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			ht.exists(i);
		}
		sw.stop();
		std::cout << "Find HT: " << sw.elapsedSeconds() << std::endl;
	}

	{
		std::set<int> s;
		sw.start();
		for (int i = 0; i < N; ++i)
		{
			s.insert(i);
		}
		sw.stop();
		std::cout << "Insert set: " << sw.elapsedSeconds() << std::endl;
		sw.reset();

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			auto it = s.find(i);
		}
		sw.stop();
		std::cout << "Find set: " << sw.elapsedSeconds() << std::endl;
		sw.reset();
	}

}


void LinearHashTableTest::testPerformanceStr()
{
	const int N = 5000000;
	Stopwatch sw;

	std::vector<std::string> values;
	for (int i = 0; i < N; ++i)
	{
		values.push_back(NumberFormatter::format0(i, 8));
	}

	{
		LinearHashTable<std::string, Hash<std::string> > lht(N);
		sw.start();
		for (int i = 0; i < N; ++i)
		{
			lht.insert(values[i]);
		}
		sw.stop();
		std::cout << "Insert LHT: " << sw.elapsedSeconds() << std::endl;
		sw.reset();

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			lht.find(values[i]);
		}
		sw.stop();
		std::cout << "Find LHT: " << sw.elapsedSeconds() << std::endl;
		sw.reset();
	}

	{
		HashTable<std::string, int> ht;

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			ht.insert(values[i], i);
		}
		sw.stop();
		std::cout << "Insert HT: " << sw.elapsedSeconds() << std::endl;
		sw.reset();

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			ht.exists(values[i]);
		}
		sw.stop();
		std::cout << "Find HT: " << sw.elapsedSeconds() << std::endl;
	}

	{
		std::set<std::string> s;
		sw.start();
		for (int i = 0; i < N; ++i)
		{
			s.insert(values[i]);
		}
		sw.stop();
		std::cout << "Insert set: " << sw.elapsedSeconds() << std::endl;
		sw.reset();

		sw.start();
		for (int i = 0; i < N; ++i)
		{
			auto it = s.find(values[i]);
		}
		sw.stop();
		std::cout << "Find set: " << sw.elapsedSeconds() << std::endl;
		sw.reset();
	}
}


void LinearHashTableTest::setUp()
{
}


void LinearHashTableTest::tearDown()
{
}


CppUnit::Test* LinearHashTableTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LinearHashTableTest");

	CppUnit_addTest(pSuite, LinearHashTableTest, testInsert);
	CppUnit_addTest(pSuite, LinearHashTableTest, testErase);
	CppUnit_addTest(pSuite, LinearHashTableTest, testIterator);
	CppUnit_addTest(pSuite, LinearHashTableTest, testConstIterator);
	//CppUnit_addTest(pSuite, LinearHashTableTest, testPerformanceInt);
	//CppUnit_addTest(pSuite, LinearHashTableTest, testPerformanceStr);

	return pSuite;
}

#ifdef POCO_COMPILER_MSVC
#pragma warning(pop)
#endif // POCO_COMPILER_MSVC
