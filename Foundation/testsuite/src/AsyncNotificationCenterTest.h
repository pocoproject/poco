//
// AsyncNotificationCenterTest.h
//
// Definition of the AsyncNotificationCenterTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering LLC,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AsyncNotificationCenterTest_INCLUDED
#define AsyncNotificationCenterTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include "Poco/AsyncNotificationCenter.h"
#include <set>


class TestNotification;


class AsyncNotificationCenterTest: public CppUnit::TestCase
{
public:
	AsyncNotificationCenterTest(const std::string& name);
	~AsyncNotificationCenterTest();

	void testAsyncObserver();
		/// Tests AsyncObserver with NotificationCenter (async dispatch in observer's own thread)

	void testAsyncNotificationCenter();
		/// Tests AsyncObserver with AsyncNotificationCenter (async dispatch in NC's worker pool)

	void testAsyncNotificationCenterModes();
		/// Tests AsyncNotificationCenter modes: ENQUEUE, NOTIFY, and BOTH

	void testAsyncNotificationCenterWorkerCount();
		/// Tests AsyncNotificationCenter with custom worker thread count

	void testAsyncNotificationCenterDefaultWorkers();
		/// Tests that default worker count scales with CPU cores (2-6 workers)

	void testAsyncNotificationCenterBacklog();
		/// Tests backlog() method returns queued notification count

	void testAsyncNotificationCenterParallelDispatch();
		/// Tests that multiple workers dispatch notifications in parallel

	void testMixedNObserversAndAsyncObservers();
		/// Tests mixing NObserver and AsyncObserver in same NotificationCenter

#ifdef POCO_TEST_DEPRECATED
	void testMixedObservers();
		/// Tests mixing Observer, NObserver, and AsyncObserver in same NotificationCenter (deprecated)
#endif

	void testSynchronousDispatch();
		/// Tests synchronousDispatch() method that returns results from sync observers

	void testErrorHandling();
		/// Tests that exceptions thrown by observers are handled gracefully

	void testThreadSafety();
		/// Tests concurrent postNotification() calls from multiple threads

	void testObserverLifecycle();
		/// Tests removing observers while notifications are being dispatched

	void testCleanupDestruction();
		/// Tests stopping AsyncNotificationCenter while notifications are queued/processing

	void testEdgeCases();
		/// Tests edge cases: empty observer list, stress testing, dynamic observer registration

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void handleNObs(const Poco::AutoPtr<Poco::Notification>& pNf);
#ifdef POCO_TEST_DEPRECATED
	void handle1(Poco::Notification* pNf);
#endif
	void handleAsync1(const Poco::AutoPtr<TestNotification>& pNf);
	void handleAsync2(const Poco::AutoPtr<TestNotification>& pNf);
	void handleAuto(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handleCount(const Poco::AutoPtr<Poco::Notification>& pNf);
	Poco::NotificationResult handleSync(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handleThrow(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handleThreadSafe(const Poco::AutoPtr<Poco::Notification>& pNf);
	bool matchAsync(const std::string& name) const;

	void resetState();
		/// Resets test state between sub-tests. Not to be confused with setUp() which is called by the framework.

#if (POCO_HAVE_JTHREAD)
	void workerCount(Poco::AsyncNotificationCenter::AsyncMode mode);
	void backlog(Poco::AsyncNotificationCenter::AsyncMode mode);
	void parallelDispatch(Poco::AsyncNotificationCenter::AsyncMode mode);
	void mixedNObserversAndAsyncObservers(Poco::AsyncNotificationCenter::AsyncMode mode);
#ifdef POCO_TEST_DEPRECATED
	void mixedObservers(Poco::AsyncNotificationCenter::AsyncMode mode);
#endif
	void synchronousDispatch(Poco::AsyncNotificationCenter::AsyncMode mode);
#endif

private:
	std::set<std::string> _set;
	std::atomic<bool> _handleNObsDone;
#ifdef POCO_TEST_DEPRECATED
	std::atomic<bool> _handle1Done;
#endif
	std::atomic<bool> _handleAuto1Done;
	std::atomic<bool> _handleAsync1Done;
	std::atomic<bool> _handleAsync2Done;
	std::atomic<int> _notificationCount;
	std::atomic<int> _syncCallCount;
	std::atomic<int> _threadSafeCount;
	std::atomic<int> _exceptionCount;
	Poco::Mutex _mutex;
};


#endif // AsyncNotificationCenterTest_INCLUDED
