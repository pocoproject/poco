//
// NotificationQueueTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NotificationQueueTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Notification.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Random.h"
#include <atomic>
#include <functional>
#include <vector>


using Poco::NotificationQueue;
using Poco::Notification;
using Poco::Thread;
using Poco::RunnableAdapter;
using CppUnit::waitForCondition;


namespace
{
	class QTestNotification: public Notification
	{
	public:
		QTestNotification(const std::string& data): _data(data)
		{
		}
		~QTestNotification()
		{
		}
		const std::string& data() const
		{
			return _data;
		}

	private:
		std::string _data;
	};
}


NotificationQueueTest::NotificationQueueTest(const std::string& name): CppUnit::TestCase(name)
{
}


NotificationQueueTest::~NotificationQueueTest()
{
}


void NotificationQueueTest::testQueueDequeue()
{
	NotificationQueue queue;
	assertTrue (queue.empty());
	assertTrue (queue.size() == 0);
	Notification* pNf = queue.dequeueNotification();
	assertNullPtr(pNf);
	queue.enqueueNotification(new Notification);
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 1);
	pNf = queue.dequeueNotification();
	assertNotNullPtr(pNf);
	assertTrue (queue.empty());
	assertTrue (queue.size() == 0);
	pNf->release();

	queue.enqueueNotification(new QTestNotification("first"));
	queue.enqueueNotification(new QTestNotification("second"));
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 2);
	QTestNotification* pTNf = dynamic_cast<QTestNotification*>(queue.dequeueNotification());
	assertNotNullPtr(pTNf);
	assertTrue (pTNf->data() == "first");
	pTNf->release();
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 1);
	pTNf = dynamic_cast<QTestNotification*>(queue.dequeueNotification());
	assertNotNullPtr(pTNf);
	assertTrue (pTNf->data() == "second");
	pTNf->release();
	assertTrue (queue.empty());
	assertTrue (queue.size() == 0);

	pNf = queue.dequeueNotification();
	assertNullPtr(pNf);
}


void NotificationQueueTest::testQueueDequeueUrgent()
{
	NotificationQueue queue;
	queue.enqueueNotification(new QTestNotification("first"));
	queue.enqueueNotification(new QTestNotification("second"));
	queue.enqueueUrgentNotification(new QTestNotification("third"));
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 3);
	QTestNotification* pTNf = dynamic_cast<QTestNotification*>(queue.dequeueNotification());
	assertNotNullPtr(pTNf);
	assertTrue (pTNf->data() == "third");
	pTNf->release();
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 2);
	pTNf = dynamic_cast<QTestNotification*>(queue.dequeueNotification());
	assertTrue (pTNf->data() == "first");
	pTNf->release();
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 1);
	pTNf = dynamic_cast<QTestNotification*>(queue.dequeueNotification());
	assertNotNullPtr(pTNf);
	assertTrue (pTNf->data() == "second");
	pTNf->release();
	assertTrue (queue.empty());
	assertTrue (queue.size() == 0);

	Notification* pNf = queue.dequeueNotification();
	assertNullPtr(pNf);
}


void NotificationQueueTest::testWaitDequeue()
{
	NotificationQueue queue;
	queue.enqueueNotification(new QTestNotification("third"));
	queue.enqueueNotification(new QTestNotification("fourth"));
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 2);
	QTestNotification* pTNf = dynamic_cast<QTestNotification*>(queue.waitDequeueNotification(10));
	assertNotNullPtr(pTNf);
	assertTrue (pTNf->data() == "third");
	pTNf->release();
	assertTrue (!queue.empty());
	assertTrue (queue.size() == 1);
	pTNf = dynamic_cast<QTestNotification*>(queue.waitDequeueNotification(10));
	assertNotNullPtr(pTNf);
	assertTrue (pTNf->data() == "fourth");
	pTNf->release();
	assertTrue (queue.empty());
	assertTrue (queue.size() == 0);

	Notification* pNf = queue.waitDequeueNotification(10);
	assertNullPtr(pNf);
}


void NotificationQueueTest::testThreads()
{
	const int NOTIFICATION_COUNT = 2000;

	Thread t1("thread1");
	Thread t2("thread2");
	Thread t3("thread3");

	RunnableAdapter<NotificationQueueTest> ra(*this, &NotificationQueueTest::work);
	t1.start(ra);
	t2.start(ra);
	t3.start(ra);
	for (int i = 0; i < NOTIFICATION_COUNT; ++i)
	{
		_queue.enqueueNotification(new Notification);
	}
	// Wait for queue to drain, but always cleanup threads before asserting
	// to avoid detached threads accessing destroyed Thread objects
	bool queueEmptied = waitForCondition([&]{ return _queue.empty(); }, 20000);
	Thread::sleep(20);
	_queue.wakeUpAll();
	t1.join();
	t2.join();
	t3.join();
	assertTrue (queueEmptied);
	assertTrue (_handled.size() == NOTIFICATION_COUNT);
	assertTrue (_handled.count("thread1") > 0);
	assertTrue (_handled.count("thread2") > 0);
	assertTrue (_handled.count("thread3") > 0);
}


void NotificationQueueTest::testDefaultQueue()
{
	NotificationQueue& queue = NotificationQueue::defaultQueue();
	assertTrue (queue.empty());
	assertTrue (queue.size() == 0);
}


void NotificationQueueTest::setUp()
{
	_handled.clear();
}


void NotificationQueueTest::tearDown()
{
}


void NotificationQueueTest::work()
{
	Poco::Random rnd;
	Thread::sleep(50);
	Notification* pNf = _queue.waitDequeueNotification();
	while (pNf)
	{
		pNf->release();
		_mutex.lock();
		_handled.insert(Thread::current()->name());
		_mutex.unlock();
		Thread::sleep(rnd.next(5));
		pNf = _queue.waitDequeueNotification();
	}
}


void NotificationQueueTest::testWakeUpAllBeforeWait()
{
	// Reproduce lost-wakeup race: wakeUpAll() is called when no thread is
	// in waitDequeueNotification() yet.  Without the fix, the thread that
	// subsequently enters the wait blocks forever.
	const int NUM_THREADS = 3;
	NotificationQueue queue;
	std::atomic<int> exited(0);

	// Call wakeUpAll() BEFORE any thread enters the wait.
	queue.wakeUpAll();

	// Now start threads that call waitDequeueNotification().
	// They must all return null immediately rather than blocking.
	auto worker = [&queue, &exited]()
	{
		Notification* pNf = queue.waitDequeueNotification();
		poco_assert(pNf == nullptr);
		++exited;
	};

	std::vector<Thread*> threads;

	// Use raw threads to run the lambda via a simple Runnable wrapper
	struct LambdaRunnable : public Poco::Runnable
	{
		std::function<void()> fn;
		explicit LambdaRunnable(std::function<void()> f) : fn(std::move(f)) {}
		void run() override { fn(); }
	};

	std::vector<LambdaRunnable*> runnables;
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		auto* r = new LambdaRunnable(worker);
		runnables.push_back(r);
		auto* t = new Thread;
		threads.push_back(t);
		t->start(*r);
	}

	// Wait for threads — with the fix they return immediately.
	// Without the fix this would hang, so use a generous timeout.
	for (auto* t : threads)
	{
		assertTrue(t->tryJoin(5000));
		delete t;
	}
	for (auto* r : runnables) delete r;

	assertTrue(exited == NUM_THREADS);
}


CppUnit::Test* NotificationQueueTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NotificationQueueTest");

	CppUnit_addTest(pSuite, NotificationQueueTest, testQueueDequeue);
	CppUnit_addTest(pSuite, NotificationQueueTest, testQueueDequeueUrgent);
	CppUnit_addTest(pSuite, NotificationQueueTest, testWaitDequeue);
	CppUnit_addTest(pSuite, NotificationQueueTest, testThreads);
	CppUnit_addTest(pSuite, NotificationQueueTest, testDefaultQueue);
	CppUnit_addTest(pSuite, NotificationQueueTest, testWakeUpAllBeforeWait);

	return pSuite;
}
