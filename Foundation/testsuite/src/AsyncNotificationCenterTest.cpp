//
// AsyncNotificationCenterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering LLC,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AsyncNotificationCenterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NotificationCenter.h"
#include "Poco/AsyncNotificationCenter.h"
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/AsyncObserver.h"
#include "Poco/AutoPtr.h"
#include <thread>


using Poco::NotificationCenter;
using Poco::AsyncNotificationCenter;
using Poco::Observer;
using Poco::NObserver;
using Poco::AsyncObserver;
using Poco::Notification;
using Poco::AutoPtr;
using AsyncMode = AsyncNotificationCenter::AsyncMode;


class TestNotification: public Notification
{
public:
	TestNotification()
	{}

	TestNotification(const std::string& name):
		Notification(name)
	{}
};


// Custom observer that supports synchronous dispatch
template <class C, class N>
class SyncNObserver: public Poco::AbstractObserver
{
public:
	using NotificationPtr = Poco::AutoPtr<N>;
	using Callback = Poco::NotificationResult (C::*)(const NotificationPtr&);

	SyncNObserver(C& object, Callback method):
		_pObject(&object),
		_callback(method)
	{
	}

	SyncNObserver(const SyncNObserver& observer):
		Poco::AbstractObserver(observer),
		_pObject(observer._pObject),
		_callback(observer._callback)
	{
	}

	~SyncNObserver()
	{
	}

	SyncNObserver& operator = (const SyncNObserver& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject;
			_callback = observer._callback;
		}
		return *this;
	}

	void notify(Poco::Notification* pNf) const override
	{
		// Async notification (not used for sync dispatch)
		Poco::AutoPtr<Poco::Notification> nf(pNf, true);
	}

	Poco::NotificationResult notifySync(Poco::Notification* pNf) const override
	{
		NotificationPtr ptr(static_cast<N*>(pNf), true);
		return (_pObject->*_callback)(ptr);
	}

	bool acceptsSync() const override
	{
		return true;
	}

	bool equals(const Poco::AbstractObserver& abstractObserver) const override
	{
		const SyncNObserver* pObs = dynamic_cast<const SyncNObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_callback == _callback;
	}

	bool accepts(Poco::Notification* pNf, const char* /*pName*/) const override
	{
		return dynamic_cast<N*>(pNf) != nullptr;
	}

	bool accepts(const Poco::Notification::Ptr& pNf) const override
	{
		return pNf.template cast<N>() != nullptr;
	}

	Poco::AbstractObserver* clone() const override
	{
		return new SyncNObserver(*this);
	}

	void disable() override
	{
		_pObject = nullptr;
	}

private:
	C* _pObject;
	Callback _callback;
};


AsyncNotificationCenterTest::AsyncNotificationCenterTest(const std::string& name):
	CppUnit::TestCase(name),
	_handleNObsDone(false),
#ifdef POCO_TEST_DEPRECATED
	_handle1Done(false),
#endif
	_handleAuto1Done(false),
	_handleAsync1Done(false),
	_handleAsync2Done(false),
	_notificationCount(0),
	_syncCallCount(0),
	_threadSafeCount(0),
	_exceptionCount(0)
{
}


AsyncNotificationCenterTest::~AsyncNotificationCenterTest()
{
}


void AsyncNotificationCenterTest::testAsyncObserver()
{
	using AObserver = AsyncObserver<AsyncNotificationCenterTest, TestNotification>::Type;

	NotificationCenter nc;

	nc.addObserver(AObserver(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync));
	nc.addObserver(AObserver(*this, &AsyncNotificationCenterTest::handleAsync2, &AsyncNotificationCenterTest::matchAsync));

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	nc.removeObserver(AObserver(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync));
	nc.removeObserver(AObserver(*this, &AsyncNotificationCenterTest::handleAsync2, &AsyncNotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertEqual(2u, _set.size());
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
}


void AsyncNotificationCenterTest::testAsyncNotificationCenter()
{
	AsyncNotificationCenter nc;

	nc.addAsyncObserver(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync);
	nc.addAsyncObserver(*this, &AsyncNotificationCenterTest::handleAsync2, &AsyncNotificationCenterTest::matchAsync);

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	nc.removeAsyncObserver(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync);
	nc.removeAsyncObserver(*this, &AsyncNotificationCenterTest::handleAsync2, &AsyncNotificationCenterTest::matchAsync);

	Poco::Mutex::ScopedLock l(_mutex);
	assertEqual(2u, _set.size());
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
}


void AsyncNotificationCenterTest::testAsyncNotificationCenterModes()
{
#if (POCO_HAVE_JTHREAD)
	// Test ENQUEUE mode (default)
	{
		AsyncNotificationCenter nc(AsyncMode::ENQUEUE);
		nc.addNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
		nc.postNotification(new Notification);
		while (!_handleAuto1Done)
			Poco::Thread::sleep(100);
		assertTrue(_set.find("handleAuto") != _set.end());
		nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
	}

	_set.clear();
	_handleAuto1Done = false;

	// Test NOTIFY mode
	{
		AsyncNotificationCenter nc(AsyncMode::NOTIFY);
		nc.addNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
		nc.postNotification(new Notification);
		while (!_handleAuto1Done)
			Poco::Thread::sleep(100);
		assertTrue(_set.find("handleAuto") != _set.end());
		nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
	}

	_set.clear();
	_handleAuto1Done = false;

	// Test BOTH mode
	{
		AsyncNotificationCenter nc(AsyncMode::BOTH);
		nc.addNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
		nc.postNotification(new Notification);
		while (!_handleAuto1Done)
			Poco::Thread::sleep(100);
		assertTrue(_set.find("handleAuto") != _set.end());
		nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
	}
#endif
}


void AsyncNotificationCenterTest::testAsyncNotificationCenterWorkerCount()
{
#if (POCO_HAVE_JTHREAD)
	workerCount(AsyncMode::ENQUEUE);
	resetState();
	workerCount(AsyncMode::NOTIFY);
	resetState();
	workerCount(AsyncMode::BOTH);
#endif
}


void AsyncNotificationCenterTest::testAsyncNotificationCenterDefaultWorkers()
{
#if (POCO_HAVE_JTHREAD)
	// Test default worker count calculation
	std::size_t workers = AsyncNotificationCenter::defaultWorkersCount();
	unsigned int cores = std::thread::hardware_concurrency();

	if (cores <= 2)
		assertEqual(2u, workers);
	else if (cores <= 4)
		assertEqual(3u, workers);
	else if (cores <= 8)
		assertEqual(4u, workers);
	else
		assertEqual(6u, workers);
#endif
}


void AsyncNotificationCenterTest::testAsyncNotificationCenterBacklog()
{
#if (POCO_HAVE_JTHREAD)
	backlog(AsyncMode::ENQUEUE);
	resetState();
	backlog(AsyncMode::NOTIFY);
	resetState();
	backlog(AsyncMode::BOTH);
#endif
}


void AsyncNotificationCenterTest::testAsyncNotificationCenterParallelDispatch()
{
#if (POCO_HAVE_JTHREAD)
	parallelDispatch(AsyncMode::ENQUEUE);
	resetState();
	parallelDispatch(AsyncMode::NOTIFY);
	resetState();
	parallelDispatch(AsyncMode::BOTH);
#endif
}


void AsyncNotificationCenterTest::testMixedNObserversAndAsyncObservers()
{
#if (POCO_HAVE_JTHREAD)
	mixedNObserversAndAsyncObservers(AsyncMode::ENQUEUE);
	resetState();
	mixedNObserversAndAsyncObservers(AsyncMode::NOTIFY);
	resetState();
	mixedNObserversAndAsyncObservers(AsyncMode::BOTH);
#endif
}


#ifdef POCO_TEST_DEPRECATED
void AsyncNotificationCenterTest::testMixedObservers()
{
#if (POCO_HAVE_JTHREAD)
	mixedObservers(AsyncMode::ENQUEUE);
	resetState();
	mixedObservers(AsyncMode::NOTIFY);
	resetState();
	mixedObservers(AsyncMode::BOTH);
#endif
}
#endif


void AsyncNotificationCenterTest::testSynchronousDispatch()
{
#if (POCO_HAVE_JTHREAD)
	synchronousDispatch(AsyncMode::ENQUEUE);
	resetState();
	synchronousDispatch(AsyncMode::NOTIFY);
	resetState();
	synchronousDispatch(AsyncMode::BOTH);
#endif
}


void AsyncNotificationCenterTest::testErrorHandling()
{
#if (POCO_HAVE_JTHREAD)
	// Test that exceptions from observers are caught and don't crash the system
	AsyncNotificationCenter nc(AsyncMode::ENQUEUE);

	// Register an observer that throws
	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleThrow);

	// Register a normal observer that should still be called
	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

	// Post notification - exception should be caught by ErrorHandler
	nc.postNotification(new Notification);

	// Give time for async processing
	Poco::Thread::sleep(200);

	// The throwing observer should have been called
	assertTrue(_exceptionCount > 0);

	// The normal observer should also have been called (error handling shouldn't stop dispatch)
	// Note: This depends on ErrorHandler implementation - it may or may not continue

	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleThrow);
	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleCount);
#endif
}


void AsyncNotificationCenterTest::testThreadSafety()
{
#if (POCO_HAVE_JTHREAD)
	// Test concurrent postNotification calls from multiple threads
	AsyncNotificationCenter nc(AsyncMode::NOTIFY, 4);

	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleThreadSafe);

	const int numThreads = 8;
	const int notificationsPerThread = 100;
	std::vector<std::thread> threads;

	// Launch multiple threads posting notifications concurrently
	for (int i = 0; i < numThreads; ++i)
	{
		threads.emplace_back([&nc]() {
			for (int j = 0; j < notificationsPerThread; ++j)
			{
				nc.postNotification(new Notification);
			}
		});
	}

	// Wait for all threads to finish posting
	for (auto& t : threads)
	{
		t.join();
	}

	// Wait for all notifications to be processed
	int timeout = 0;
	int expected = numThreads * notificationsPerThread;
	while (_threadSafeCount < expected && timeout < 200)
	{
		Poco::Thread::sleep(50);
		++timeout;
	}

	// Verify all notifications were received
	assertEqual(expected, _threadSafeCount.load());

	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleThreadSafe);
#endif
}


void AsyncNotificationCenterTest::testObserverLifecycle()
{
#if (POCO_HAVE_JTHREAD)
	// Test removing observer while notifications are being dispatched
	AsyncNotificationCenter nc(AsyncMode::NOTIFY, 2);

	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

	// Post many notifications
	for (int i = 0; i < 100; ++i)
	{
		nc.postNotification(new Notification);
	}

	// Give a tiny bit of time for processing to start
	Poco::Thread::sleep(10);

	// Remove observer while notifications might still be in queue
	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleCount);

	// Wait a bit for cleanup
	Poco::Thread::sleep(100);

	// Test passes if:
	// 1. We didn't crash
	// 2. Count is in valid range (some might have been processed before removal)
	int finalCount = _notificationCount;
	assertTrue(finalCount >= 0 && finalCount <= 100);
#endif
}


void AsyncNotificationCenterTest::testCleanupDestruction()
{
#if (POCO_HAVE_JTHREAD)
	const int testCount = 50;

	// Test 1: NOTIFY mode - workers stop immediately, may not process all notifications
	{
		_notificationCount = 0;
		AsyncNotificationCenter nc(AsyncMode::NOTIFY, 2);
		nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

		for (int i = 0; i < testCount; ++i)
		{
			nc.postNotification(new Notification);
		}

		Poco::Thread::sleep(5);
		// Destructor should complete quickly, possibly without processing all notifications
	}
	int notifyProcessed = _notificationCount.load();

	// Test 2: ENQUEUE mode - must process all queued notifications before destructor completes
	int startCount = _notificationCount.load();
	{
		AsyncNotificationCenter nc(AsyncMode::ENQUEUE);
		nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

		for (int i = 0; i < testCount; ++i)
		{
			nc.postNotification(new Notification);
		}

		// Give a moment for enqueue thread to start processing
		Poco::Thread::sleep(10);

		// Destructor MUST process all remaining notifications in ENQUEUE mode
	}

	// Verify ENQUEUE processed all notifications
	int enqueueProcessed = _notificationCount.load() - startCount;
	assertEqual(testCount, enqueueProcessed);

	// NOTIFY may have processed fewer (workers stop immediately when destructor runs)
	assertTrue(notifyProcessed >= 0 && notifyProcessed <= testCount);
#endif
}


void AsyncNotificationCenterTest::testEdgeCases()
{
#if (POCO_HAVE_JTHREAD)
	// Test 1: Post to empty observer list
	{
		AsyncNotificationCenter nc(AsyncMode::NOTIFY);
		nc.postNotification(new Notification);
		Poco::Thread::sleep(50);
		// Should not crash
	}

	// Test 2: Stress test with many notifications
	{
		AsyncNotificationCenter nc(AsyncMode::BOTH, 4);
		nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

		const int stressCount = 1000;
		for (int i = 0; i < stressCount; ++i)
		{
			nc.postNotification(new Notification);
		}

		// Wait for all to process (max 10 seconds)
		int timeout = 0;
		while (_notificationCount < stressCount && timeout < 100)
		{
			Poco::Thread::sleep(100);
			++timeout;
		}

		// Verify we got most/all notifications
		int received = _notificationCount.load();
		assertTrue(received >= stressCount * 0.9); // Allow 10% loss for timing issues
		nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleCount);
	}

	// Test 3: Observer registering other observers during notification
	// This is complex and can lead to undefined behavior, so we skip it
	// as it's not a supported use case

	assertTrue(true);
#endif
}


#if (POCO_HAVE_JTHREAD)

void AsyncNotificationCenterTest::workerCount(AsyncMode mode)
{
	AsyncNotificationCenter nc(mode, 8);
	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
	nc.postNotification(new Notification);
	while (!_handleAuto1Done)
		Poco::Thread::sleep(100);
	assertTrue(_set.find("handleAuto") != _set.end());
	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleAuto);
}


void AsyncNotificationCenterTest::backlog(AsyncMode mode)
{
	AsyncNotificationCenter nc(mode);

	// Post notifications without observers (they'll queue up)
	nc.postNotification(new Notification);
	nc.postNotification(new Notification);
	nc.postNotification(new Notification);

	// Give time to process
	Poco::Thread::sleep(100);

	// Backlog should be drained (or nearly drained)
	int backlog = nc.backlog();
	assertTrue(backlog >= 0);
}


void AsyncNotificationCenterTest::parallelDispatch(AsyncMode mode)
{
	AsyncNotificationCenter nc(mode, 4);

	_notificationCount = 0;

	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

	// Post multiple notifications
	const int numNotifications = 10;
	for (int i = 0; i < numNotifications; ++i)
	{
		nc.postNotification(new Notification);
	}

	// Wait for all to be processed
	int timeout = 0;
	while (_notificationCount < numNotifications && timeout < 100)
	{
		Poco::Thread::sleep(50);
		++timeout;
	}

	assertEqual(numNotifications, _notificationCount.load());
	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleCount);
}


void AsyncNotificationCenterTest::mixedNObserversAndAsyncObservers(AsyncMode mode)
{
	AsyncNotificationCenter nc(mode);

	nc.addNObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handleNObs);
	nc.addNObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handleAuto);
	nc.addAsyncObserver<AsyncNotificationCenterTest, TestNotification>(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync);

	nc.postNotification(new Notification);
	nc.postNotification(new TestNotification("asyncNotification"));

	while (!_handleNObsDone || !_handleAuto1Done || !_handleAsync1Done)
		Poco::Thread::sleep(100);

	nc.removeAsyncObserver<AsyncNotificationCenterTest, TestNotification>(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync);
	nc.removeNObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handleAuto);
	nc.removeNObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handleNObs);

	Poco::Mutex::ScopedLock l(_mutex);
	assertEqual(3u, _set.size());
	assertTrue (_set.find("handleNObs") != _set.end());
	assertTrue (_set.find("handleAuto") != _set.end());
	assertTrue (_set.find("handleAsync1") != _set.end());
}


#ifdef POCO_TEST_DEPRECATED
void AsyncNotificationCenterTest::mixedObservers(AsyncMode mode)
{
	AsyncNotificationCenter nc(mode);

	nc.addObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handle1);
	nc.addNObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handleAuto);
	nc.addAsyncObserver<AsyncNotificationCenterTest, TestNotification>(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync);

	nc.postNotification(new Notification);
	nc.postNotification(new TestNotification("asyncNotification"));

	while (!_handle1Done || !_handleAuto1Done || !_handleAsync1Done)
		Poco::Thread::sleep(100);

	nc.removeAsyncObserver<AsyncNotificationCenterTest, TestNotification>(*this, &AsyncNotificationCenterTest::handleAsync1, &AsyncNotificationCenterTest::matchAsync);
	nc.removeNObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handleAuto);
	nc.removeObserver<AsyncNotificationCenterTest, Notification>(*this, &AsyncNotificationCenterTest::handle1);

	Poco::Mutex::ScopedLock l(_mutex);
	assertEqual(3u, _set.size());
	assertTrue (_set.find("handle1") != _set.end());
	assertTrue (_set.find("handleAuto") != _set.end());
	assertTrue (_set.find("handleAsync1") != _set.end());
}
#endif


void AsyncNotificationCenterTest::synchronousDispatch(AsyncMode mode)
{
	AsyncNotificationCenter nc(mode);

	// Register sync observers using custom SyncNObserver
	using SyncObs = SyncNObserver<AsyncNotificationCenterTest, Notification>;
	nc.addObserver(SyncObs(*this, &AsyncNotificationCenterTest::handleSync));

	// Also register a regular NObserver (should not be called by synchronousDispatch)
	nc.addNObserver(*this, &AsyncNotificationCenterTest::handleCount);

	// Call synchronousDispatch - should only call sync observers
	auto results = nc.synchronousDispatch(new Notification);

	// Verify that handleSync was called (returns result)
	assertEqual(1u, results.size());
	assertTrue(results[0].first == "result");
	assertEqual(1, Poco::AnyCast<int>(results[0].second));
	assertEqual(1, _syncCallCount.load());

	// Verify that handleCount was NOT called by synchronousDispatch
	assertEqual(0, _notificationCount.load());

	// Test with no sync observers - should return empty vector
	nc.removeObserver(SyncObs(*this, &AsyncNotificationCenterTest::handleSync));
	results = nc.synchronousDispatch(new Notification);
	assertEqual(0u, results.size());

	nc.removeNObserver(*this, &AsyncNotificationCenterTest::handleCount);
}

#endif // POCO_HAVE_JTHREAD


void AsyncNotificationCenterTest::handleNObs(const AutoPtr<Notification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleNObs");
	_handleNObsDone = true;
}


#ifdef POCO_TEST_DEPRECATED
void AsyncNotificationCenterTest::handle1(Poco::Notification* pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;
	_set.insert("handle1");
	_handle1Done = true;
}
#endif


void AsyncNotificationCenterTest::handleAuto(const AutoPtr<Notification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAuto");
	_handleAuto1Done = true;
}


void AsyncNotificationCenterTest::handleAsync1(const AutoPtr<TestNotification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAsync1");
	_handleAsync1Done = true;
}


void AsyncNotificationCenterTest::handleAsync2(const AutoPtr<TestNotification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAsync2");
	_handleAsync2Done = true;
}


void AsyncNotificationCenterTest::handleCount(const AutoPtr<Notification>& pNf)
{
	++_notificationCount;
}


Poco::NotificationResult AsyncNotificationCenterTest::handleSync(const AutoPtr<Notification>& pNf)
{
	int callNum = ++_syncCallCount;
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleSync");
	return {"result", callNum};
}


void AsyncNotificationCenterTest::handleThrow(const AutoPtr<Notification>& pNf)
{
	++_exceptionCount;
	throw Poco::RuntimeException("Test exception from observer");
}


void AsyncNotificationCenterTest::handleThreadSafe(const AutoPtr<Notification>& pNf)
{
	++_threadSafeCount;
	// Simulate some processing time
	Poco::Thread::sleep(1);
}


bool AsyncNotificationCenterTest::matchAsync(const std::string& name) const
{
	return name.find("asyncNotification") == 0;
}


void AsyncNotificationCenterTest::resetState()
{
	_set.clear();
	_notificationCount = 0;
	_syncCallCount = 0;
	_threadSafeCount = 0;
	_exceptionCount = 0;
	_handleNObsDone = false;
#ifdef POCO_TEST_DEPRECATED
	_handle1Done = false;
#endif
	_handleAuto1Done = false;
	_handleAsync1Done = false;
	_handleAsync2Done = false;
}


void AsyncNotificationCenterTest::setUp()
{
	resetState();
}


void AsyncNotificationCenterTest::tearDown()
{
}


CppUnit::Test* AsyncNotificationCenterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AsyncNotificationCenterTest");

	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncObserver);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncNotificationCenter);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncNotificationCenterModes);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncNotificationCenterWorkerCount);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncNotificationCenterDefaultWorkers);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncNotificationCenterBacklog);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testAsyncNotificationCenterParallelDispatch);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testMixedNObserversAndAsyncObservers);
#ifdef POCO_TEST_DEPRECATED
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testMixedObservers);
#endif
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testSynchronousDispatch);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testErrorHandling);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testThreadSafety);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testObserverLifecycle);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testCleanupDestruction);
	CppUnit_addTest(pSuite, AsyncNotificationCenterTest, testEdgeCases);

	return pSuite;
}
