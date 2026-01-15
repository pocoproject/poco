//
// NotificationCenterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NotificationCenterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NotificationCenter.h"
#ifdef POCO_TEST_DEPRECATED
#include "Poco/Observer.h"
#endif
#include "Poco/NObserver.h"
#include "Poco/AutoPtr.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <chrono>


using Poco::NotificationCenter;
#ifdef POCO_TEST_DEPRECATED
using Poco::Observer;
#endif
using Poco::NObserver;
using Poco::Notification;
using Poco::AutoPtr;


class TestNotification: public Notification
{
public:
	TestNotification()
	{}

	TestNotification(const std::string& name):
		Notification(name)
	{}
};


namespace
{
	// Thread to continuously post notifications
	class NotifyRunnable : public Poco::Runnable
	{
	public:
		NotifyRunnable(NotificationCenter& nc, std::atomic<bool>& stop, std::atomic<int>& notificationCount):
			_nc(nc), _stop(stop), _notificationCount(notificationCount)
		{}

		void run() override
		{
			while (!_stop)
			{
				_nc.postNotification(new Notification);
				_notificationCount++;
				Poco::Thread::sleep(20);
			}
		}

	private:
		NotificationCenter& _nc;
		std::atomic<bool>& _stop;
		std::atomic<int>& _notificationCount;
	};


	// Thread to add/remove observers (exercises addObserver/removeObserver)
	class ModifyRunnable : public Poco::Runnable
	{
	public:
		ModifyRunnable(NotificationCenter& nc,
		               NotificationCenterTest& test,
		               std::atomic<bool>& handlerStarted,
		               std::atomic<bool>& stop,
		               std::atomic<int>& addCount,
		               std::atomic<int>& removeCount,
		               std::atomic<bool>& modifyDone):
			_nc(nc), _test(test), _handlerStarted(handlerStarted), _stop(stop),
			_addCount(addCount), _removeCount(removeCount), _modifyDone(modifyDone)
		{}

		void run() override
		{
			// Wait for handler to start
			while (!_handlerStarted && !_stop)
				Poco::Thread::sleep(10);

			NObserver<NotificationCenterTest, Notification> tempObs(_test, &NotificationCenterTest::handleAuto);

			// Repeatedly add and remove observers
			for (int i = 0; i < 10 && !_stop; ++i)
			{
				// Test addObserver - calls start() on observer (now outside lock)
				_nc.addObserver(tempObs);
				_addCount++;
				if (!_nc.hasObserver(tempObs))
				{
					_stop = true; // Signal test failure
					_modifyDone = true;
					return;
				}
				Poco::Thread::sleep(30);

				// Test removeObserver - calls disable() on observer (now outside lock)
				_nc.removeObserver(tempObs);
				_removeCount++;
				if (_nc.hasObserver(tempObs))
				{
					_stop = true; // Signal test failure
					_modifyDone = true;
					return;
				}
				Poco::Thread::sleep(30);
			}

			// Signal that we're done with modifications
			_modifyDone = true;
		}

	private:
		NotificationCenter& _nc;
		NotificationCenterTest& _test;
		std::atomic<bool>& _handlerStarted;
		std::atomic<bool>& _stop;
		std::atomic<int>& _addCount;
		std::atomic<int>& _removeCount;
		std::atomic<bool>& _modifyDone;
	};


	// Thread to test clear() method
	class ClearRunnable : public Poco::Runnable
	{
	public:
		ClearRunnable(NotificationCenter& nc,
		              std::atomic<bool>& handlerStarted,
		              std::atomic<bool>& stop,
		              std::atomic<bool>& clearFailed,
		              std::atomic<bool>& modifyDone):
			_nc(nc), _handlerStarted(handlerStarted), _stop(stop),
			_clearFailed(clearFailed), _modifyDone(modifyDone)
		{}

		void run() override
		{
			// Wait for handler to start
			while (!_handlerStarted && !_stop)
				Poco::Thread::sleep(10);

			// Wait for ModifyRunnable to complete its iterations
			// so we can verify clear() without race conditions
			while (!_modifyDone && !_stop)
				Poco::Thread::sleep(50);

			// Test clear() - calls disable() on all observers (now outside lock)
			_nc.clear();

			// Verify all observers were cleared
			// Safe to check now because ModifyRunnable has finished
			if (_nc.hasObservers() || _nc.countObservers() != 0)
			{
				_clearFailed = true;
				_stop = true;
			}
		}

	private:
		NotificationCenter& _nc;
		std::atomic<bool>& _handlerStarted;
		std::atomic<bool>& _stop;
		std::atomic<bool>& _clearFailed;
		std::atomic<bool>& _modifyDone;
	};
}


NotificationCenterTest::NotificationCenterTest(const std::string& name):
	CppUnit::TestCase(name),
	_handle1Done(false),
	_handleAuto1Done(false),
	_handlerStarted(false),
	_handlerCallCount(0),
	_pNC(nullptr)
{
}


NotificationCenterTest::~NotificationCenterTest()
{
}


void NotificationCenterTest::testNotificationCenter1()
{
	NotificationCenter nc;
	nc.postNotification(new Notification);
}


#ifdef POCO_TEST_DEPRECATED
void NotificationCenterTest::testNotificationCenter2()
{
	NotificationCenter nc;
	Observer<NotificationCenterTest, Notification> o(*this, &NotificationCenterTest::handle1);
	nc.addObserver(o);
	assertTrue (nc.hasObserver(o));
	assertTrue (nc.hasObservers());
	assertTrue (nc.countObservers() == 1);
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handle1") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	assertTrue (!nc.hasObserver(o));
	assertTrue (!nc.hasObservers());
	assertTrue (nc.countObservers() == 0);
}
#endif


#ifdef POCO_TEST_DEPRECATED
void NotificationCenterTest::testNotificationCenter3()
{
	NotificationCenter nc;
	Observer<NotificationCenterTest, Notification> o1(*this, &NotificationCenterTest::handle1);
	Observer<NotificationCenterTest, Notification> o2(*this, &NotificationCenterTest::handle2);
	nc.addObserver(o1);
	assertTrue (nc.hasObserver(o1));
	nc.addObserver(o2);
	assertTrue (nc.hasObserver(o2));
	assertTrue (nc.hasObservers());
	assertTrue (nc.countObservers() == 2);
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 2);
	assertTrue (_set.find("handle1") != _set.end());
	assertTrue (_set.find("handle2") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	assertTrue (!nc.hasObserver(o1));
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle2));
	assertTrue (!nc.hasObserver(o2));
	assertTrue (!nc.hasObservers());
	assertTrue (nc.countObservers() == 0);
}
#endif


#ifdef POCO_TEST_DEPRECATED
void NotificationCenterTest::testNotificationCenter4()
{
	NotificationCenter nc;
	Observer<NotificationCenterTest, Notification> o1(*this, &NotificationCenterTest::handle1);
	Observer<NotificationCenterTest, Notification> o2(*this, &NotificationCenterTest::handle2);
	nc.addObserver(o1);
	assertTrue (nc.hasObserver(o1));
	nc.addObserver(o2);
	assertTrue (nc.hasObserver(o2));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 2);
	assertTrue (_set.find("handle1") != _set.end());
	assertTrue (_set.find("handle2") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	assertTrue (!nc.hasObserver(o1));
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle2));
	assertTrue (!nc.hasObserver(o2));
	_set.clear();
	nc.postNotification(new Notification);
	assertTrue (_set.empty());
	Observer<NotificationCenterTest, Notification> o3(*this, &NotificationCenterTest::handle3);
	nc.addObserver(o3);
	assertTrue (nc.hasObserver(o3));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handle3") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle3));
	assertTrue (!nc.hasObserver(o3));
}
#endif


#ifdef POCO_TEST_DEPRECATED
void NotificationCenterTest::testNotificationCenter5()
{
	NotificationCenter nc;
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.addObserver(Observer<NotificationCenterTest, TestNotification>(*this, &NotificationCenterTest::handleTest));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handle1") != _set.end());
	_set.clear();
	nc.postNotification(new TestNotification);
	assertTrue (_set.size() == 2);
	assertTrue (_set.find("handle1") != _set.end());
	assertTrue (_set.find("handleTest") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.removeObserver(Observer<NotificationCenterTest, TestNotification>(*this, &NotificationCenterTest::handleTest));
}
#endif


void NotificationCenterTest::testNotificationCenterAuto()
{
	NotificationCenter nc;
	nc.addObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handleAuto") != _set.end());
	nc.removeObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
}


#ifdef POCO_TEST_DEPRECATED
void NotificationCenterTest::testDefaultNotificationCenter()
{
	NotificationCenter& nc = NotificationCenter::defaultCenter();
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handle1") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
}
#endif


#ifdef POCO_TEST_DEPRECATED
void NotificationCenterTest::handle1(Poco::Notification* pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;
	_set.insert("handle1");
	_handle1Done = true;
}


void NotificationCenterTest::handle2(Poco::Notification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;
	_set.insert("handle2");
}


void NotificationCenterTest::handle3(Poco::Notification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;
	_set.insert("handle3");
}


void NotificationCenterTest::handleTest(TestNotification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<TestNotification> nf = pNf;
	_set.insert("handleTest");
}
#endif


void NotificationCenterTest::handleAuto(const AutoPtr<Notification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAuto");
	_handleAuto1Done = true;
}


void NotificationCenterTest::handleDeadlock(const AutoPtr<Notification>& pNf)
{
	// Signal that handler has started
	_handlerStarted = true;

	// Count how many times this handler has been called
	int callCount = ++_handlerCallCount;

	// Simulate the deadlock scenario: handler tries to access NotificationCenter
	// while holding the Observer's mutex (via NObserver::handle())
	// This would deadlock if NC methods held the lock while calling observer methods

	// Give other threads a chance to call NC methods
	Poco::Thread::sleep(50);

	// Exercise all NotificationCenter methods that acquire locks while handler
	// is holding Observer mutex. This simulates real-world usage where handlers
	// query and modify the NC.
	if (_pNC)
	{
		NObserver<NotificationCenterTest, Notification> obs(*this, &NotificationCenterTest::handleDeadlock);

		// Test read operations - acquire read lock while holding Observer mutex
		_pNC->hasObserver(obs);
		_pNC->hasObservers();
		std::size_t count = _pNC->countObservers();
		(void)count;
		int backlog = _pNC->backlog();
		(void)backlog;

		// Every 3rd call, remove an observer from within the handler
		// This tests the write path: handler holds Observer mutex,
		// removeObserver needs NC mutex then calls disable which needs Observer mutex again
		if (callCount % 3 == 0)
		{
			_pNC->removeObserver(obs);
		}
	}

	Poco::Thread::sleep(50);
}


void NotificationCenterTest::testDeadlock()
{
	// Test 1: Verify correct behavior without concurrency
	{
		NotificationCenter nc;
		NObserver<NotificationCenterTest, Notification> obs(*this, &NotificationCenterTest::handleAuto);

		// Verify initial state
		assertTrue(!nc.hasObservers());
		assertTrue(nc.countObservers() == 0);
		assertTrue(!nc.hasObserver(obs));

		// Add observer
		nc.addObserver(obs);

		// Verify observer was added
		assertTrue(nc.hasObservers());
		assertTrue(nc.countObservers() == 1);
		assertTrue(nc.hasObserver(obs));

		// Remove observer
		nc.removeObserver(obs);

		// Verify observer was removed
		assertTrue(!nc.hasObservers());
		assertTrue(nc.countObservers() == 0);
		assertTrue(!nc.hasObserver(obs));

		// Add multiple observers
		NObserver<NotificationCenterTest, Notification> obs2(*this, &NotificationCenterTest::handleAuto);
		NObserver<NotificationCenterTest, Notification> obs3(*this, &NotificationCenterTest::handleAuto);
		nc.addObserver(obs);
		nc.addObserver(obs2);
		nc.addObserver(obs3);

		assertTrue(nc.countObservers() == 3);

		// Clear all observers
		nc.clear();

		assertTrue(!nc.hasObservers());
		assertTrue(nc.countObservers() == 0);
	}

	// Test 2: Concurrent access to test for deadlocks
	static NotificationCenter nc;
	nc.clear(); // Ensure clean state for each test run
	_pNC = &nc;
	_handlerStarted = false;

	// Create multiple observers to exercise addObserver/removeObserver
	NObserver<NotificationCenterTest, Notification> obs1(*this, &NotificationCenterTest::handleDeadlock);
	NObserver<NotificationCenterTest, Notification> obs2(*this, &NotificationCenterTest::handleDeadlock);
	NObserver<NotificationCenterTest, Notification> obs3(*this, &NotificationCenterTest::handleDeadlock);

	std::atomic<bool> stop(false);
	std::atomic<int> notificationCount(0);
	std::atomic<int> addCount(0);
	std::atomic<int> removeCount(0);
	std::atomic<bool> clearFailed(false);
	std::atomic<bool> modifyDone(false);

	// Add initial observers
	nc.addObserver(obs1);
	nc.addObserver(obs2);
	nc.addObserver(obs3);

	// Verify initial state
	assertTrue(nc.countObservers() == 3);

	NotifyRunnable notifyRunnable(nc, stop, notificationCount);
	ModifyRunnable modifyRunnable(nc, *this, _handlerStarted, stop, addCount, removeCount, modifyDone);
	ClearRunnable clearRunnable(nc, _handlerStarted, stop, clearFailed, modifyDone);

	Poco::Thread notifyThread;
	Poco::Thread modifyThread;
	Poco::Thread clearThread;

	notifyThread.start(notifyRunnable);
	modifyThread.start(modifyRunnable);
	clearThread.start(clearRunnable);

	// Wait for ModifyRunnable and ClearRunnable to complete with timeout
	// NotifyRunnable runs continuously until we set stop flag
	auto startTime = std::chrono::steady_clock::now();
	const auto timeout = std::chrono::seconds(10);

	bool timedOut = false;
	while (modifyThread.isRunning() || clearThread.isRunning())
	{
		Poco::Thread::sleep(100);
		auto elapsed = std::chrono::steady_clock::now() - startTime;
		if (elapsed > timeout)
		{
			// Deadlock detected - test fails
			timedOut = true;
			break;
		}
	}

	// Stop the notification thread
	stop = true;

	// Wait a bit for notifyThread to see the stop flag
	Poco::Thread::sleep(500);

	if (timedOut)
	{
		fail("Deadlock detected: threads did not complete within timeout");
		return;
	}

	notifyThread.join();
	modifyThread.join();
	clearThread.join();

	// Verify operations completed successfully
	assertTrue(notificationCount > 0); // At least some notifications were posted
	assertTrue(addCount == removeCount); // Every add was matched with a remove
	assertTrue(!clearFailed); // Clear operation verified successfully

	// If we got here, no deadlock occurred - test passes
	_pNC = nullptr;
}


void NotificationCenterTest::setUp()
{
	_set.clear();
	_handle1Done = false;
	_handleAuto1Done = false;
	_handlerStarted = false;
	_handlerCallCount = 0;
	_pNC = nullptr;
}


void NotificationCenterTest::tearDown()
{
}


CppUnit::Test* NotificationCenterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NotificationCenterTest");

	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter1);
#ifdef POCO_TEST_DEPRECATED
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter2);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter3);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter4);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter5);
#endif
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenterAuto);
#ifdef POCO_TEST_DEPRECATED
	CppUnit_addTest(pSuite, NotificationCenterTest, testDefaultNotificationCenter);
#endif
	CppUnit_addTest(pSuite, NotificationCenterTest, testDeadlock);

	return pSuite;
}
