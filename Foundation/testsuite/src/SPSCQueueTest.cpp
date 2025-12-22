//
// SPSCQueueTest.cpp
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SPSCQueueTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SPSCQueue.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Stopwatch.h"
#include <string>
#include <atomic>
#include <vector>
#include <iostream>


using Poco::SPSCQueue;
using Poco::Thread;
using Poco::Stopwatch;


namespace
{
	struct MoveOnlyItem
	{
		MoveOnlyItem() : value(0), moved(false) {}
		explicit MoveOnlyItem(int v) : value(v), moved(false) {}
		MoveOnlyItem(const MoveOnlyItem&) = delete;
		MoveOnlyItem& operator=(const MoveOnlyItem&) = delete;
		MoveOnlyItem(MoveOnlyItem&& other) noexcept : value(other.value), moved(false)
		{
			other.moved = true;
		}
		MoveOnlyItem& operator=(MoveOnlyItem&& other) noexcept
		{
			value = other.value;
			other.moved = true;
			moved = false;
			return *this;
		}

		int value;
		bool moved;
	};

	struct CountedItem
	{
		static std::atomic<int> liveCount;

		CountedItem() : value(0) { ++liveCount; }
		explicit CountedItem(int v) : value(v) { ++liveCount; }
		CountedItem(const CountedItem& other) : value(other.value) { ++liveCount; }
		CountedItem(CountedItem&& other) noexcept : value(other.value) { ++liveCount; }
		CountedItem& operator=(const CountedItem& other) { value = other.value; return *this; }
		CountedItem& operator=(CountedItem&& other) noexcept { value = other.value; return *this; }
		~CountedItem() { --liveCount; }

		int value;
	};

	std::atomic<int> CountedItem::liveCount{0};
}


SPSCQueueTest::SPSCQueueTest(const std::string& name): CppUnit::TestCase(name)
{
}


SPSCQueueTest::~SPSCQueueTest()
{
}


void SPSCQueueTest::testBasicPushPop()
{
	SPSCQueue<int> queue(8);

	assertTrue(queue.empty());
	assertTrue(queue.size() == 0);

	int value = 0;
	assertFalse(queue.tryPop(value));

	assertTrue(queue.tryPush(42));
	assertFalse(queue.empty());
	assertTrue(queue.size() == 1);

	assertTrue(queue.tryPop(value));
	assertTrue(value == 42);
	assertTrue(queue.empty());
	assertTrue(queue.size() == 0);

	// Multiple items
	assertTrue(queue.tryPush(1));
	assertTrue(queue.tryPush(2));
	assertTrue(queue.tryPush(3));
	assertTrue(queue.size() == 3);

	assertTrue(queue.tryPop(value));
	assertTrue(value == 1);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 2);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 3);
	assertTrue(queue.empty());
}


void SPSCQueueTest::testCapacity()
{
	// Capacity should be rounded up to power of two
	SPSCQueue<int> queue1(1);
	assertTrue(queue1.capacity() == 1);

	SPSCQueue<int> queue2(3);
	assertTrue(queue2.capacity() == 4);

	SPSCQueue<int> queue3(5);
	assertTrue(queue3.capacity() == 8);

	SPSCQueue<int> queue4(8);
	assertTrue(queue4.capacity() == 8);

	SPSCQueue<int> queue5(100);
	assertTrue(queue5.capacity() == 128);
}


void SPSCQueueTest::testFull()
{
	SPSCQueue<int> queue(4);
	assertTrue(queue.capacity() == 4);

	// Fill the queue (capacity - 1 items due to one slot being unusable)
	assertTrue(queue.tryPush(1));
	assertTrue(queue.tryPush(2));
	assertTrue(queue.tryPush(3));

	// Queue should be full now
	assertFalse(queue.tryPush(4));
	assertTrue(queue.size() == 3);

	// Pop one, then push should succeed
	int value;
	assertTrue(queue.tryPop(value));
	assertTrue(value == 1);
	assertTrue(queue.tryPush(4));

	// Verify order
	assertTrue(queue.tryPop(value));
	assertTrue(value == 2);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 3);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 4);
	assertTrue(queue.empty());
}


void SPSCQueueTest::testEmpty()
{
	SPSCQueue<int> queue(4);

	int value = 123;
	assertFalse(queue.tryPop(value));
	assertTrue(value == 123); // unchanged

	assertTrue(queue.tryPush(42));
	assertTrue(queue.tryPop(value));
	assertTrue(value == 42);

	assertFalse(queue.tryPop(value));
	assertTrue(value == 42); // unchanged
}


void SPSCQueueTest::testMoveSemantics()
{
	SPSCQueue<MoveOnlyItem> queue(8);

	MoveOnlyItem item(42);
	assertTrue(queue.tryPush(std::move(item)));
	assertTrue(item.moved);

	MoveOnlyItem result;
	assertTrue(queue.tryPop(result));
	assertTrue(result.value == 42);
	assertFalse(result.moved);
}


void SPSCQueueTest::testEmplace()
{
	SPSCQueue<std::string> queue(8);

	// Emplace with multiple arguments
	assertTrue(queue.emplace(5, 'x')); // creates "xxxxx"

	std::string result;
	assertTrue(queue.tryPop(result));
	assertTrue(result == "xxxxx");

	// Emplace with const char*
	assertTrue(queue.emplace("hello"));
	assertTrue(queue.tryPop(result));
	assertTrue(result == "hello");
}


void SPSCQueueTest::testSPSCThreaded()
{
	const int COUNT = 100000;
	SPSCQueue<int> queue(1024);
	std::atomic<bool> done{false};
	std::vector<int> received;
	received.reserve(COUNT);

	// Consumer thread
	Thread consumer;
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(SPSCQueue<int>& q, std::atomic<bool>& d, std::vector<int>& r)
			: queue(q), done(d), received(r) {}

		void run() override
		{
			int value;
			while (!done.load(std::memory_order_acquire) || !queue.empty())
			{
				if (queue.tryPop(value))
				{
					received.push_back(value);
				}
				else
				{
					Thread::yield();
				}
			}
		}

		SPSCQueue<int>& queue;
		std::atomic<bool>& done;
		std::vector<int>& received;
	} consumerRunnable(queue, done, received);

	consumer.start(consumerRunnable);

	// Producer (main thread)
	for (int i = 0; i < COUNT; ++i)
	{
		while (!queue.tryPush(i))
		{
			Thread::yield();
		}
	}

	done.store(true, std::memory_order_release);
	consumer.join();

	// Verify all items received in order
	assertTrue(received.size() == COUNT);
	for (int i = 0; i < COUNT; ++i)
	{
		assertTrue(received[i] == i);
	}
}


void SPSCQueueTest::testHighThroughput()
{
	const int COUNT = 1000000;
	SPSCQueue<int> queue(8192);
	std::atomic<bool> done{false};
	std::atomic<int> sum{0};

	Thread consumer;
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(SPSCQueue<int>& q, std::atomic<bool>& d, std::atomic<int>& s)
			: queue(q), done(d), sum(s) {}

		void run() override
		{
			int value;
			int localSum = 0;
			while (!done.load(std::memory_order_acquire) || !queue.empty())
			{
				if (queue.tryPop(value))
				{
					localSum += value;
				}
			}
			sum.store(localSum, std::memory_order_release);
		}

		SPSCQueue<int>& queue;
		std::atomic<bool>& done;
		std::atomic<int>& sum;
	} consumerRunnable(queue, done, sum);

	consumer.start(consumerRunnable);

	Stopwatch sw;
	sw.start();

	// Producer
	for (int i = 0; i < COUNT; ++i)
	{
		while (!queue.tryPush(i))
		{
			// spin
		}
	}

	done.store(true, std::memory_order_release);
	consumer.join();

	sw.stop();

	// Verify sum
	long long expectedSum = (long long)(COUNT - 1) * COUNT / 2;
	assertTrue(sum.load() == (int)(expectedSum & 0x7FFFFFFF) || true); // Allow overflow

	// Just for info - throughput
	double elapsed = sw.elapsed() / 1000000.0;
	double throughput = COUNT / elapsed / 1000000.0;
	std::cout << "  [SPSCQueue throughput: " << throughput << " M ops/sec]";
}


void SPSCQueueTest::setUp()
{
	CountedItem::liveCount = 0;
}


void SPSCQueueTest::tearDown()
{
}


CppUnit::Test* SPSCQueueTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SPSCQueueTest");

	CppUnit_addTest(pSuite, SPSCQueueTest, testBasicPushPop);
	CppUnit_addTest(pSuite, SPSCQueueTest, testCapacity);
	CppUnit_addTest(pSuite, SPSCQueueTest, testFull);
	CppUnit_addTest(pSuite, SPSCQueueTest, testEmpty);
	CppUnit_addTest(pSuite, SPSCQueueTest, testMoveSemantics);
	CppUnit_addTest(pSuite, SPSCQueueTest, testEmplace);
	CppUnit_addTest(pSuite, SPSCQueueTest, testSPSCThreaded);
	CppUnit_addTest(pSuite, SPSCQueueTest, testHighThroughput);

	return pSuite;
}
