//
// NotificationQueueBench.cpp
//
// Benchmarks for NotificationQueue, SPSCQueue, and MPSCQueue
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <benchmark/benchmark.h>
#include "Poco/NotificationQueue.h"
#include "Poco/SPSCQueue.h"
#include "Poco/MPSCQueue.h"
#include "Poco/Notification.h"
#include "Poco/Message.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include <atomic>
#include <string>


using Poco::NotificationQueue;
using Poco::SPSCQueue;
using Poco::MPSCQueue;
using Poco::Notification;
using Poco::Message;
using Poco::Thread;
using Poco::Event;


namespace {


// Simple notification for benchmarking
class BenchNotification : public Notification
{
public:
	BenchNotification(int v = 0) : value(v) {}
	int value;
};


//
// Single-threaded enqueue/dequeue benchmarks
// These measure the raw overhead of queue operations without threading
//
// Naming: Queues_<Implementation>_<Test>
//

static void Queues_NotificationQueue_RoundTrip(benchmark::State& state)
{
	NotificationQueue queue;

	for (auto _ : state)
	{
		queue.enqueueNotification(new BenchNotification(42));
		Notification::Ptr pNf = queue.dequeueNotification();
		benchmark::DoNotOptimize(pNf.get());
	}
}
BENCHMARK(Queues_NotificationQueue_RoundTrip);


static void Queues_SPSCQueue_RoundTrip(benchmark::State& state)
{
	SPSCQueue<int> queue(1024);

	for (auto _ : state)
	{
		queue.tryPush(42);
		int value;
		queue.tryPop(value);
		benchmark::DoNotOptimize(value);
	}
}
BENCHMARK(Queues_SPSCQueue_RoundTrip);


static void Queues_MPSCQueue_RoundTrip(benchmark::State& state)
{
	MPSCQueue<int> queue(1024);

	for (auto _ : state)
	{
		queue.tryPush(42);
		int value;
		queue.tryPop(value);
		benchmark::DoNotOptimize(value);
	}
}
BENCHMARK(Queues_MPSCQueue_RoundTrip);


//
// Enqueue-only benchmarks (pre-sized queue)
// Measures pure enqueue overhead
//

static void Queues_NotificationQueue_Enqueue(benchmark::State& state)
{
	NotificationQueue queue;

	for (auto _ : state)
	{
		queue.enqueueNotification(new BenchNotification(42));
	}

	// Drain queue
	while (!queue.empty())
	{
		queue.dequeueNotification();
	}
}
BENCHMARK(Queues_NotificationQueue_Enqueue);


static void Queues_SPSCQueue_Enqueue(benchmark::State& state)
{
	// Use large queue to avoid filling up
	SPSCQueue<int> queue(1 << 20);  // 1M entries

	for (auto _ : state)
	{
		if (!queue.tryPush(42))
		{
			// Queue full - drain some entries
			state.PauseTiming();
			int value;
			for (int i = 0; i < 1000; ++i)
				queue.tryPop(value);
			state.ResumeTiming();
		}
	}
}
BENCHMARK(Queues_SPSCQueue_Enqueue);


static void Queues_MPSCQueue_Enqueue(benchmark::State& state)
{
	// Use large queue to avoid filling up
	MPSCQueue<int> queue(1 << 20);  // 1M entries

	for (auto _ : state)
	{
		if (!queue.tryPush(42))
		{
			// Queue full - drain some entries
			state.PauseTiming();
			int value;
			for (int i = 0; i < 1000; ++i)
				queue.tryPop(value);
			state.ResumeTiming();
		}
	}
}
BENCHMARK(Queues_MPSCQueue_Enqueue);


//
// Multi-threaded producer-consumer benchmarks
// These measure realistic async channel scenarios
//

static void Queues_NotificationQueue_Threaded(benchmark::State& state)
{
	NotificationQueue queue;
	std::atomic<bool> done{false};
	std::atomic<int64_t> consumed{0};

	// Consumer thread
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(NotificationQueue& q, std::atomic<bool>& d, std::atomic<int64_t>& c)
			: queue(q), done(d), consumed(c) {}

		void run() override
		{
			while (!done.load(std::memory_order_acquire) || !queue.empty())
			{
				Notification::Ptr pNf = queue.waitDequeueNotification(1);
				if (pNf)
				{
					consumed.fetch_add(1, std::memory_order_relaxed);
				}
			}
		}

		NotificationQueue& queue;
		std::atomic<bool>& done;
		std::atomic<int64_t>& consumed;
	} consumer(queue, done, consumed);

	Thread consumerThread;
	consumerThread.start(consumer);

	for (auto _ : state)
	{
		queue.enqueueNotification(new BenchNotification(42));
	}

	done.store(true, std::memory_order_release);
	queue.wakeUpAll();
	consumerThread.join();

	state.counters["items_per_second"] = benchmark::Counter(
		static_cast<double>(state.iterations()),
		benchmark::Counter::kIsRate);
}
BENCHMARK(Queues_NotificationQueue_Threaded);


static void Queues_SPSCQueue_Threaded(benchmark::State& state)
{
	SPSCQueue<int> queue(8192);
	std::atomic<bool> done{false};
	std::atomic<int64_t> consumed{0};
	Event hasData;

	// Consumer thread
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(SPSCQueue<int>& q, std::atomic<bool>& d, std::atomic<int64_t>& c, Event& e)
			: queue(q), done(d), consumed(c), hasData(e) {}

		void run() override
		{
			int value;
			while (!done.load(std::memory_order_acquire))
			{
				while (queue.tryPop(value))
				{
					consumed.fetch_add(1, std::memory_order_relaxed);
				}
				hasData.tryWait(1);
			}
			// Drain remaining
			while (queue.tryPop(value))
			{
				consumed.fetch_add(1, std::memory_order_relaxed);
			}
		}

		SPSCQueue<int>& queue;
		std::atomic<bool>& done;
		std::atomic<int64_t>& consumed;
		Event& hasData;
	} consumer(queue, done, consumed, hasData);

	Thread consumerThread;
	consumerThread.start(consumer);

	for (auto _ : state)
	{
		while (!queue.tryPush(42))
		{
			// Queue full, yield to consumer
			Thread::yield();
		}
		hasData.set();
	}

	done.store(true, std::memory_order_release);
	hasData.set();
	consumerThread.join();

	state.counters["items_per_second"] = benchmark::Counter(
		static_cast<double>(state.iterations()),
		benchmark::Counter::kIsRate);
}
BENCHMARK(Queues_SPSCQueue_Threaded);


static void Queues_MPSCQueue_Threaded(benchmark::State& state)
{
	MPSCQueue<int> queue(8192);
	std::atomic<bool> done{false};
	std::atomic<int64_t> consumed{0};
	Event hasData;

	// Consumer thread
	class Consumer : public Poco::Runnable
	{
	public:
		Consumer(MPSCQueue<int>& q, std::atomic<bool>& d, std::atomic<int64_t>& c, Event& e)
			: queue(q), done(d), consumed(c), hasData(e) {}

		void run() override
		{
			int value;
			while (!done.load(std::memory_order_acquire))
			{
				while (queue.tryPop(value))
				{
					consumed.fetch_add(1, std::memory_order_relaxed);
				}
				hasData.tryWait(1);
			}
			// Drain remaining
			while (queue.tryPop(value))
			{
				consumed.fetch_add(1, std::memory_order_relaxed);
			}
		}

		MPSCQueue<int>& queue;
		std::atomic<bool>& done;
		std::atomic<int64_t>& consumed;
		Event& hasData;
	} consumer(queue, done, consumed, hasData);

	Thread consumerThread;
	consumerThread.start(consumer);

	for (auto _ : state)
	{
		while (!queue.tryPush(42))
		{
			// Queue full, yield to consumer
			Thread::yield();
		}
		hasData.set();
	}

	done.store(true, std::memory_order_release);
	hasData.set();
	consumerThread.join();

	state.counters["items_per_second"] = benchmark::Counter(
		static_cast<double>(state.iterations()),
		benchmark::Counter::kIsRate);
}
BENCHMARK(Queues_MPSCQueue_Threaded);


//
// String payload benchmarks
// Tests memory allocation overhead with realistic message types
//

static void Queues_NotificationQueue_String(benchmark::State& state)
{
	class StringNotification : public Notification
	{
	public:
		StringNotification(const std::string& s) : text(s) {}
		std::string text;
	};

	NotificationQueue queue;
	std::string msg = "This is a test log message for benchmarking";

	for (auto _ : state)
	{
		queue.enqueueNotification(new StringNotification(msg));
		Notification::Ptr pNf = queue.dequeueNotification();
		benchmark::DoNotOptimize(pNf.get());
	}
}
BENCHMARK(Queues_NotificationQueue_String);


static void Queues_SPSCQueue_String(benchmark::State& state)
{
	SPSCQueue<std::string> queue(1024);
	std::string msg = "This is a test log message for benchmarking";

	for (auto _ : state)
	{
		queue.tryPush(msg);
		std::string value;
		queue.tryPop(value);
		benchmark::DoNotOptimize(value.data());
	}
}
BENCHMARK(Queues_SPSCQueue_String);


static void Queues_MPSCQueue_String(benchmark::State& state)
{
	MPSCQueue<std::string> queue(1024);
	std::string msg = "This is a test log message for benchmarking";

	for (auto _ : state)
	{
		queue.tryPush(msg);
		std::string value;
		queue.tryPop(value);
		benchmark::DoNotOptimize(value.data());
	}
}
BENCHMARK(Queues_MPSCQueue_String);


//
// Message payload benchmarks
// Tests the actual Message type used by AsyncChannel
//

static void Queues_MPSCQueue_Message(benchmark::State& state)
{
	MPSCQueue<Message> queue(1024);
	Message msg("TestSource", "This is a test log message for benchmarking", Message::PRIO_INFORMATION);

	for (auto _ : state)
	{
		queue.tryPush(msg);
		Message value;
		queue.tryPop(value);
		benchmark::DoNotOptimize(value.getText().data());
	}
}
BENCHMARK(Queues_MPSCQueue_Message);


static void Queues_MPSCQueue_MessageMove(benchmark::State& state)
{
	MPSCQueue<Message> queue(1024);

	for (auto _ : state)
	{
		Message msg("TestSource", "This is a test log message for benchmarking", Message::PRIO_INFORMATION);
		queue.tryPush(std::move(msg));
		Message value;
		queue.tryPop(value);
		benchmark::DoNotOptimize(value.getText().data());
	}
}
BENCHMARK(Queues_MPSCQueue_MessageMove);


} // namespace
