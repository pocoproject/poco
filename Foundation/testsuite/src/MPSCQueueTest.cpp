//
// MPSCQueueTest.cpp
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MPSCQueueTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MPSCQueue.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Stopwatch.h"
#include <string>
#include <atomic>
#include <vector>
#include <iostream>
#include <set>


using Poco::MPSCQueue;
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
}


MPSCQueueTest::MPSCQueueTest(const std::string& name): CppUnit::TestCase(name)
{
}


MPSCQueueTest::~MPSCQueueTest()
{
}


void MPSCQueueTest::testBasicPushPop()
{
	MPSCQueue<int> queue(8);

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


void MPSCQueueTest::testCapacity()
{
	// Capacity should be rounded up to power of two
	MPSCQueue<int> queue1(1);
	assertTrue(queue1.capacity() == 1);

	MPSCQueue<int> queue2(3);
	assertTrue(queue2.capacity() == 4);

	MPSCQueue<int> queue3(5);
	assertTrue(queue3.capacity() == 8);

	MPSCQueue<int> queue4(8);
	assertTrue(queue4.capacity() == 8);

	MPSCQueue<int> queue5(100);
	assertTrue(queue5.capacity() == 128);
}


void MPSCQueueTest::testFull()
{
	MPSCQueue<int> queue(4);
	assertTrue(queue.capacity() == 4);

	// Fill the queue
	assertTrue(queue.tryPush(1));
	assertTrue(queue.tryPush(2));
	assertTrue(queue.tryPush(3));
	assertTrue(queue.tryPush(4));

	// Queue should be full now
	assertFalse(queue.tryPush(5));
	assertTrue(queue.size() == 4);

	// Pop one, then push should succeed
	int value;
	assertTrue(queue.tryPop(value));
	assertTrue(value == 1);
	assertTrue(queue.tryPush(5));

	// Verify order
	assertTrue(queue.tryPop(value));
	assertTrue(value == 2);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 3);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 4);
	assertTrue(queue.tryPop(value));
	assertTrue(value == 5);
	assertTrue(queue.empty());
}


void MPSCQueueTest::testEmpty()
{
	MPSCQueue<int> queue(4);

	int value = 123;
	assertFalse(queue.tryPop(value));
	assertTrue(value == 123); // unchanged

	assertTrue(queue.tryPush(42));
	assertTrue(queue.tryPop(value));
	assertTrue(value == 42);

	assertFalse(queue.tryPop(value));
	assertTrue(value == 42); // unchanged
}


void MPSCQueueTest::testMoveSemantics()
{
	MPSCQueue<MoveOnlyItem> queue(8);

	MoveOnlyItem item(42);
	assertTrue(queue.tryPush(std::move(item)));
	assertTrue(item.moved);

	MoveOnlyItem result;
	assertTrue(queue.tryPop(result));
	assertTrue(result.value == 42);
	assertFalse(result.moved);
}


void MPSCQueueTest::testEmplace()
{
	MPSCQueue<std::string> queue(8);

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


void MPSCQueueTest::testSingleProducerSingleConsumer()
{
	const int COUNT = 100000;
	MPSCQueue<int> queue(1024);
	std::atomic<bool> done{false};
	std::vector<int> received;
	received.reserve(COUNT);

	// Consumer thread
	Thread consumer;
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(MPSCQueue<int>& q, std::atomic<bool>& d, std::vector<int>& r)
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

		MPSCQueue<int>& queue;
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


void MPSCQueueTest::testMultipleProducers()
{
	const int PRODUCERS = 4;
	const int COUNT_PER_PRODUCER = 25000;
	const int TOTAL = PRODUCERS * COUNT_PER_PRODUCER;

	MPSCQueue<int> queue(1024);
	std::atomic<bool> done{false};
	std::atomic<int> producersDone{0};
	std::vector<int> received;
	received.reserve(TOTAL);

	// Consumer thread
	Thread consumer;
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(MPSCQueue<int>& q, std::atomic<bool>& d, std::vector<int>& r)
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

		MPSCQueue<int>& queue;
		std::atomic<bool>& done;
		std::vector<int>& received;
	} consumerRunnable(queue, done, received);

	consumer.start(consumerRunnable);

	// Producer threads
	class Producer : public Poco::Runnable
	{
	public:
		Producer(MPSCQueue<int>& q, int id, int count, std::atomic<int>& pd)
			: queue(q), producerId(id), itemCount(count), producersDone(pd) {}

		void run() override
		{
			for (int i = 0; i < itemCount; ++i)
			{
				// Encode producer ID and sequence in the value
				int value = producerId * 1000000 + i;
				while (!queue.tryPush(value))
				{
					Thread::yield();
				}
			}
			producersDone.fetch_add(1, std::memory_order_release);
		}

		MPSCQueue<int>& queue;
		int producerId;
		int itemCount;
		std::atomic<int>& producersDone;
	};

	std::vector<Producer> producerRunnables;
	std::vector<Thread> producerThreads(PRODUCERS);

	for (int i = 0; i < PRODUCERS; ++i)
	{
		producerRunnables.emplace_back(queue, i, COUNT_PER_PRODUCER, producersDone);
	}

	for (int i = 0; i < PRODUCERS; ++i)
	{
		producerThreads[i].start(producerRunnables[i]);
	}

	// Wait for all producers
	for (int i = 0; i < PRODUCERS; ++i)
	{
		producerThreads[i].join();
	}

	done.store(true, std::memory_order_release);
	consumer.join();

	// Verify all items received
	assertTrue(received.size() == TOTAL);

	// Verify each producer's items are in order (but interleaved with others)
	std::vector<int> lastSeen(PRODUCERS, -1);
	for (int value : received)
	{
		int producerId = value / 1000000;
		int seq = value % 1000000;
		assertTrue(producerId >= 0 && producerId < PRODUCERS);
		assertTrue(seq > lastSeen[producerId]);
		lastSeen[producerId] = seq;
	}

	// Verify each producer sent all its items
	for (int i = 0; i < PRODUCERS; ++i)
	{
		assertTrue(lastSeen[i] == COUNT_PER_PRODUCER - 1);
	}
}


void MPSCQueueTest::testHighContention()
{
	const int PRODUCERS = 8;
	const int COUNT_PER_PRODUCER = 10000;
	const int TOTAL = PRODUCERS * COUNT_PER_PRODUCER;

	MPSCQueue<int> queue(256);  // Small queue to force contention
	std::atomic<bool> done{false};
	std::atomic<int> totalReceived{0};

	// Consumer thread
	Thread consumer;
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(MPSCQueue<int>& q, std::atomic<bool>& d, std::atomic<int>& r)
			: queue(q), done(d), received(r) {}

		void run() override
		{
			int value;
			while (!done.load(std::memory_order_acquire) || !queue.empty())
			{
				if (queue.tryPop(value))
				{
					received.fetch_add(1, std::memory_order_relaxed);
				}
			}
		}

		MPSCQueue<int>& queue;
		std::atomic<bool>& done;
		std::atomic<int>& received;
	} consumerRunnable(queue, done, totalReceived);

	consumer.start(consumerRunnable);

	// Producer threads
	class Producer : public Poco::Runnable
	{
	public:
		Producer(MPSCQueue<int>& q, int count) : queue(q), itemCount(count) {}

		void run() override
		{
			for (int i = 0; i < itemCount; ++i)
			{
				while (!queue.tryPush(i))
				{
					// Spin on full queue
				}
			}
		}

		MPSCQueue<int>& queue;
		int itemCount;
	};

	std::vector<Producer> producerRunnables;
	std::vector<Thread> producerThreads(PRODUCERS);

	for (int i = 0; i < PRODUCERS; ++i)
	{
		producerRunnables.emplace_back(queue, COUNT_PER_PRODUCER);
	}

	Stopwatch sw;
	sw.start();

	for (int i = 0; i < PRODUCERS; ++i)
	{
		producerThreads[i].start(producerRunnables[i]);
	}

	for (int i = 0; i < PRODUCERS; ++i)
	{
		producerThreads[i].join();
	}

	done.store(true, std::memory_order_release);
	consumer.join();

	sw.stop();

	assertTrue(totalReceived.load() == TOTAL);

	double elapsed = sw.elapsed() / 1000000.0;
	double throughput = TOTAL / elapsed / 1000000.0;
	std::cout << "  [MPSCQueue " << PRODUCERS << " producers: " << throughput << " M ops/sec]";
}


void MPSCQueueTest::setUp()
{
}


void MPSCQueueTest::tearDown()
{
}


CppUnit::Test* MPSCQueueTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MPSCQueueTest");

	CppUnit_addTest(pSuite, MPSCQueueTest, testBasicPushPop);
	CppUnit_addTest(pSuite, MPSCQueueTest, testCapacity);
	CppUnit_addTest(pSuite, MPSCQueueTest, testFull);
	CppUnit_addTest(pSuite, MPSCQueueTest, testEmpty);
	CppUnit_addTest(pSuite, MPSCQueueTest, testMoveSemantics);
	CppUnit_addTest(pSuite, MPSCQueueTest, testEmplace);
	CppUnit_addTest(pSuite, MPSCQueueTest, testSingleProducerSingleConsumer);
	CppUnit_addTest(pSuite, MPSCQueueTest, testMultipleProducers);
	CppUnit_addTest(pSuite, MPSCQueueTest, testHighContention);

	return pSuite;
}
