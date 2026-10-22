//
// MemoryPoolTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MemoryPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MemoryPool.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Stopwatch.h"
#include <vector>
#include <cstring>
#include <iostream>
#include <atomic>
#include <thread>


using Poco::MemoryPool;
using Poco::NumberFormatter;


MemoryPoolTest::MemoryPoolTest(const std::string& name): CppUnit::TestCase(name)
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
		(void) pool1.get();
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


namespace
{

template<typename T>
bool fastMemPoolTest(int n)
{
	bool ret = true;
	std::vector<T*> vec(n, (T*)nullptr);
	Poco::FastMemoryPool<T> pool(sizeof(T*), vec.size()/2);
	for (int i = 0; i < vec.size(); ++i)
	{
		vec[i] = new (pool.get()) T(i);
	}

	for (int i = 0; i < vec.size(); ++i)
	{
		ret = ret && (vec[i] && *vec[i] == i);
	}

	for (int i = 0; i < vec.size(); ++i)
	{
		pool.release(vec[i]);
	}

	return ret;
}

template <typename T>
bool fastMemPoolTestCustom(int n, const T& val)
{
	bool ret = true;
	std::vector<T*> vec(n, (T*)nullptr);
	Poco::FastMemoryPool<T> pool(sizeof(T*), vec.size()/2);
	for (int i = 0; i < vec.size(); ++i)
	{
		vec[i] = new (pool.get()) T(val);
	}

	for (int i = 0; i < vec.size(); ++i)
	{
		ret = ret && (vec[i] && *vec[i] == val);
	}

	for (int i = 0; i < vec.size(); ++i)
	{
		pool.release(vec[i]);
	}

	return ret;
}

struct Custom
{
	Custom(): i(42), s("abc") {}
	virtual ~Custom() {}
	bool operator ==(const Custom& other) const
	{
		return i == other.i && s == other.s;
	}
	int i;
	std::string s;
};

struct CustomChild1: virtual public Custom
{
	CustomChild1(): j(0) {}
	bool operator ==(const CustomChild1& other) const
	{
		return i == other.i && s == other.s && j == other.j;
	}
	int j;
};

struct CustomChild2: virtual public Custom
{
	CustomChild2(): k(0) {}
	bool operator ==(const CustomChild2& other) const
	{
		return i == other.i && s == other.s && k == other.k;
	}
	int k;
};

struct CustomGrandChild: public CustomChild1, CustomChild2
{
	CustomGrandChild(): l(0) {}
	bool operator ==(const CustomGrandChild& other) const
	{
		return i == other.i && s == other.s &&
				j == other.j && k == other.k && l == other.l;
	}
	int l;
};

constexpr int THREAD_COUNT = 4;
constexpr int ITERATIONS = 1000;
constexpr int BLOCKS_PER_ITERATION = 8;

template <typename Work, typename Observe>
bool observeWhileRunning(Work work, Observe observe)
	/// Runs work in THREAD_COUNT threads and calls observe until all of them
	/// have finished. Returns false if any call of observe returned false.
{
	std::atomic<int> running{THREAD_COUNT};
	std::vector<std::thread> threads;
	threads.reserve(THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.emplace_back([&work, &running]()
		{
			work();
			--running;
		});
	}
	bool consistent = true;
	while (running > 0)
	{
		consistent = observe() && consistent;
	}
	for (auto& t: threads) t.join();
	return consistent;
}

}


void MemoryPoolTest::testFastMemoryPool()
{
	int blocks = 10;

	Poco::FastMemoryPool<int> fastIntPool(blocks);
	Poco::FastMemoryPool<std::string> fastStringPool(blocks);

	std::vector<int*> intVec(blocks, (int*)nullptr);
	std::vector<std::string*> strVec(blocks);

	for (int i = 0; i < blocks; ++i)
	{
		intVec[i] = new (fastIntPool.get()) int(i);
		strVec[i] = new (fastStringPool.get()) std::string(NumberFormatter::format(i));
	}

	for (int i = 0; i < blocks; ++i)
	{
		assertTrue (intVec[i] && *intVec[i] == i);
		assertTrue (strVec[i] && *strVec[i] == NumberFormatter::format(i));
	}

	for (int i = 0; i < blocks; ++i)
	{
		fastIntPool.release(intVec[i]);
		fastStringPool.release(strVec[i]);
	}

	int sz = 5;
	assertTrue (fastMemPoolTest<char>(sz));
	assertTrue (fastMemPoolTest<signed char>(sz));
	assertTrue (fastMemPoolTest<unsigned char>(sz));
	assertTrue (fastMemPoolTest<short>(sz));
	assertTrue (fastMemPoolTest<unsigned short>(sz));
	assertTrue (fastMemPoolTest<int>(sz));
	assertTrue (fastMemPoolTest<unsigned int>(sz));
	assertTrue (fastMemPoolTest<long>(sz));
	assertTrue (fastMemPoolTest<unsigned long>(sz));

	std::string str = "123abc";
	assertTrue (fastMemPoolTestCustom(sz, str));

	Custom c;
	assertTrue (fastMemPoolTestCustom(sz, c));

	CustomChild1 c1;
	assertTrue (fastMemPoolTestCustom(sz, c1));

	CustomChild2 c2;
	assertTrue (fastMemPoolTestCustom(sz, c2));

	CustomGrandChild gc;
	assertTrue (fastMemPoolTestCustom(sz, gc));

	const int elements = 16;
	Poco::FastMemoryPool<char[elements]> fastArrayPool(blocks);
	char* pC = reinterpret_cast<char*>(fastArrayPool.get());
	const char* pStr = "1234567890abcde";
	std::memcpy(pC, pStr, elements);
	assert (strlen(pC) == elements - 1);
	assert (std::strcmp(pC, pStr) == 0);
	fastArrayPool.release(pC);
}


void MemoryPoolTest::testMemoryPoolObserversDuringGetAndRelease()
{
	constexpr std::size_t BLOCK_SIZE = 32;
	MemoryPool pool(BLOCK_SIZE);
	const bool consistent = observeWhileRunning(
		[&pool]()
		{
			std::vector<void*> blocks(BLOCKS_PER_ITERATION);
			for (int i = 0; i < ITERATIONS; ++i)
			{
				for (auto& p: blocks) p = pool.get();
				for (auto* p: blocks) pool.release(p);
			}
		},
		[&pool]()
		{
			// available() first: a later allocated() can only be larger
			const int available = pool.available();
			const int allocated = pool.allocated();
			return available <= allocated && allocated <= THREAD_COUNT * BLOCKS_PER_ITERATION;
		});
	assertTrue (consistent);
	assertEqual (pool.allocated(), pool.available());
}


void MemoryPoolTest::testFastMemoryPoolObserversDuringGetAndRelease()
{
	// the smallest bucket that FastMemoryPool accepts, so that get() calls resize() as often as possible
	// while the other threads release blocks
	constexpr std::size_t BLOCKS_PER_BUCKET = 2;
	Poco::FastMemoryPool<int> pool(BLOCKS_PER_BUCKET);
	std::size_t lastAllocated = 0;
	const bool consistent = observeWhileRunning(
		[&pool]()
		{
			std::vector<int*> blocks(BLOCKS_PER_ITERATION);
			for (int i = 0; i < ITERATIONS; ++i)
			{
				for (auto& p: blocks) p = new (pool.get()) int(i);
				for (auto* p: blocks) pool.release(p);
			}
		},
		[&pool, &lastAllocated]()
		{
			// buckets are never freed, so allocated() never decreases
			const std::size_t allocated = pool.allocated();
			const bool grown = allocated >= lastAllocated;
			lastAllocated = allocated;
			return grown;
		});
	assertTrue (consistent);
	assertEqual (pool.allocated(), pool.available());
}


void MemoryPoolTest::memoryPoolBenchmark()
{
	Poco::Stopwatch sw;

	int reps = 1000000;
	std::vector<int*> iVec(reps, (int*)nullptr);
	Poco::FastMemoryPool<int> fastPool(reps);
	sw.restart();
	for (int i = 0; i < reps; ++i)
	{
		iVec[i] = new (fastPool.get()) int(i);
	}
	sw.stop();
	std::cout << '\n' << reps << "x\n";
	std::cout << "get()\t\tFastMemoryPool=" << sw.elapsed() / 1000 << std::endl;

	for (int i = 0; i < reps; ++i)
	{
		assertTrue(*iVec[i] == i);
	}

	sw.restart();
	for (int i = 0; i < reps; ++i)
	{
		fastPool.release(iVec[i]);
	}
	sw.stop();
	std::cout << "release()\tFastMemoryPool=" << sw.elapsed() / 1000 << std::endl;

	MemoryPool pool(sizeof(int*), reps);
	sw.start();
	for (int i = 0; i < reps; ++i)
	{
		iVec[i] = new (pool.get()) int(i);
	}
	sw.stop();
	std::cout << "get()\t\tMemoryPool=" << sw.elapsed() / 1000 << std::endl;

	for (int i = 0; i < reps; ++i)
	{
		assertTrue(iVec[i] && *iVec[i] == i);
	}

	sw.restart();
	for (int i = 0; i < reps; ++i)
	{
		pool.release(iVec[i]);
	}
	sw.stop();
	std::cout << "release()\tMemoryPool=" << sw.elapsed() / 1000 << std::endl;
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
	CppUnit_addTest(pSuite, MemoryPoolTest, testFastMemoryPool);
	CppUnit_addTest(pSuite, MemoryPoolTest, testMemoryPoolObserversDuringGetAndRelease);
	CppUnit_addTest(pSuite, MemoryPoolTest, testFastMemoryPoolObserversDuringGetAndRelease);
	//CppUnit_addTest(pSuite, MemoryPoolTest, memoryPoolBenchmark);

	return pSuite;
}
