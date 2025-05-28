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
#include "CppUnit/TestCase.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Notification.h"
#include "Poco/NotificationCenter.h"
#include "Poco/AsyncNotificationCenter.h"
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/AsyncObserver.h"
#include "Poco/AutoPtr.h"


using Poco::NotificationCenter;
using Poco::AsyncNotificationCenter;
using Poco::Observer;
using Poco::NObserver;
using Poco::AsyncObserver;
using Poco::Notification;
using Poco::AutoPtr;

class TestNotification: public Notification
{
public:
	TestNotification() = default;

	TestNotification(const std::string& name, int n = 0):
		Notification(name), num(n)
	{}

	int num {0};
};


NotificationCenterTest::NotificationCenterTest(const std::string& name):
	CppUnit::TestCase(name),
	_handle1Done(false),
	_handleAuto1Done(false),
	_handleAsync1Done(false),
	_handleAsync2Done(false)
{
}


NotificationCenterTest::~NotificationCenterTest() = default;


void NotificationCenterTest::testNotificationCenter1()
{
	NotificationCenter nc;
	nc.postNotification(new Notification);
}


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


void NotificationCenterTest::testNotificationCenterAuto()
{
	NotificationCenter nc;
	nc.addObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handleAuto") != _set.end());
	nc.removeObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
}


void NotificationCenterTest::testAsyncObserver()
{
	using ObserverT = AsyncObserver<NotificationCenterTest, TestNotification>::Type;

	NotificationCenter nc;

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
}


void NotificationCenterTest::testAsyncNotificationCenter()
{
	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	AsyncNotificationCenter nc;

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
}


void NotificationCenterTest::testAsyncNotificationCenter2()
{
	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	_set.clear();
	_handleAsync1Done = false;
	_handleAsync2Done = false;
	_handleAsync1Counter  = 0;
	_handleAsync2Counter  = 0;

	AsyncNotificationCenter nc;

	const auto matchAsync = [](const std::string& s) -> bool
	{
		return s.find("asyncNotification") == 0;
	};

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, matchAsync));

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
}

void NotificationCenterTest::testAsyncNotificationCenterSyncronousNotify()
{
	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	AsyncNotificationCenter nc;

	const auto matchAsync = [](const std::string& s) -> bool
	{
		return s.find("asyncNotification") == 0;
	};

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, matchAsync, &NotificationCenterTest::handleSync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, matchAsync));

	const auto res = nc.synchronousDispatch(new TestNotification("asyncNotification"));
	assertFalse(res.empty());
	assertEquals(res.size(), 1);
	assertEquals(res[0].first, "handleAsync1");

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 1);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_handleAsync1Counter == 1);
}


void NotificationCenterTest::testAsyncNotificationCenterAsyncNotify()
{
#if (POCO_HAVE_JTHREAD)

	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	_set.clear();
	_handleAsync1Done = false;
	_handleAsync2Done = false;
	_handleAsync1Counter  = 0;
	_handleAsync2Counter  = 0;

	AsyncNotificationCenter nc(AsyncNotificationCenter::AsyncMode::NOTIFY);

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
#endif
}


void NotificationCenterTest::testAsyncNotificationCenterAsyncBoth()
{
#if (POCO_HAVE_JTHREAD)

	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	_set.clear();
	_handleAsync1Done = false;
	_handleAsync2Done = false;
	_handleAsync1Counter  = 0;
	_handleAsync2Counter  = 0;

	AsyncNotificationCenter nc(AsyncNotificationCenter::AsyncMode::BOTH);

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	nc.postNotification(new TestNotification("asyncNotification"));
	nc.postNotification(new TestNotification("anotherNotification"));
	nc.postNotification(new Notification);

	while (!_handleAsync1Done || !_handleAsync2Done)
		Poco::Thread::sleep(100);

	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());
#endif
}


void NotificationCenterTest::testAsyncNotificationCenterAsyncNotifyStress()
{
#if (POCO_HAVE_JTHREAD)

	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	_set.clear();
	_handleAsync1Done = false;
	_handleAsync2Done = false;
	_handleAsync1Counter  = 0;
	_handleAsync2Counter  = 0;

	AsyncNotificationCenter nc(AsyncNotificationCenter::AsyncMode::NOTIFY);

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	for (int i {0}; i < 1000; ++i)
	{
		nc.postNotification(new TestNotification("asyncNotification", i));
		nc.postNotification(new TestNotification("anotherNotification", i));
		nc.postNotification(new Notification);
	}

	// Give enough time for the notifications to be delivered
	Poco::Thread::sleep(2000);

	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());

	assertTrue(_handleAsync1Counter == 1000ul);
	assertTrue(_handleAsync2Counter == 1000ul);

#endif
}


void NotificationCenterTest::testAsyncNotificationCenterAsyncRemoveObserver()
{
#if (POCO_HAVE_JTHREAD)

	using ObserverT = NObserver<NotificationCenterTest, TestNotification>::Type;

	_set.clear();
	_handleAsync1Done = false;
	_handleAsync2Done = false;
	_handleAsync1Counter  = 0;
	_handleAsync2Counter  = 0;

	AsyncNotificationCenter nc(AsyncNotificationCenter::AsyncMode::NOTIFY);

	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.addObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));

	for (int i {0}; i < 1000; ++i)
	{
		nc.postNotification(new TestNotification("asyncNotification", i));
		nc.postNotification(new TestNotification("anotherNotification", i));

		if (i == 100)
		{
			while (!_handleAsync2Done)
				Poco::Thread::sleep(50);
			// Remove one observer while notifications are still being posted
			nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync2, &NotificationCenterTest::matchAsync));
		}
	}

	// Give enough time for the notifications to be delivered
	Poco::Thread::sleep(2000);

	nc.removeObserver(ObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));

	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue(_set.size() == 2);
	assertTrue(_set.find("handleAsync1") != _set.end());
	assertTrue(_set.find("handleAsync2") != _set.end());

	assertTrue(_handleAsync1Counter == 1000ul);
	assertTrue(_handleAsync2Counter < 1000ul);

#endif
}


void NotificationCenterTest::testDefaultNotificationCenter()
{
	NotificationCenter& nc = NotificationCenter::defaultCenter();
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handle1") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
}


void NotificationCenterTest::testMixedObservers()
{
	using AObserverT = AsyncObserver<NotificationCenterTest, TestNotification>::Type;

	AsyncNotificationCenter nc;
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.addObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
	nc.addObserver(AObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.postNotification(new Notification);
	nc.postNotification(new TestNotification("asyncNotification"));

	while (!_handle1Done || !_handleAuto1Done || !_handleAsync1Done)
		Poco::Thread::sleep(100);

	nc.removeObserver(AObserverT(*this, &NotificationCenterTest::handleAsync1, &NotificationCenterTest::matchAsync));
	nc.removeObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	Poco::Mutex::ScopedLock l(_mutex);
	assertTrue (_set.size() == 3);
	assertTrue (_set.find("handle1") != _set.end());
	assertTrue (_set.find("handleAuto") != _set.end());
	assertTrue (_set.find("handleAsync1") != _set.end());
}


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


void NotificationCenterTest::handleAuto(const AutoPtr<Notification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAuto");
	_handleAuto1Done = true;
}


void NotificationCenterTest::handleAsync1(const AutoPtr<TestNotification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAsync1");
	_handleAsync1Done = true;
	_handleAsync1Counter++;
}

Poco::NotificationResult NotificationCenterTest::handleSync(const AutoPtr<TestNotification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);

	_set.insert("handleAsync1");
	return std::make_pair("handleAsync1", Poco::Any(++_handleAsync1Counter));
}


void NotificationCenterTest::handleAsync2(const AutoPtr<TestNotification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAsync2");
	_handleAsync2Done = true;
	_handleAsync2Counter++;
}


bool NotificationCenterTest::matchAsync(const std::string& name) const
{
	return name.find("asyncNotification") == 0;
}


void NotificationCenterTest::setUp()
{
	_set.clear();
}


void NotificationCenterTest::tearDown()
{
}


CppUnit::Test* NotificationCenterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NotificationCenterTest");

	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter1);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter2);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter3);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter4);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenter5);
	CppUnit_addTest(pSuite, NotificationCenterTest, testNotificationCenterAuto);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncObserver);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenter);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenter2);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenterSyncronousNotify);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenterAsyncNotify);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenterAsyncBoth);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenterAsyncNotifyStress);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAsyncNotificationCenterAsyncRemoveObserver);
	CppUnit_addTest(pSuite, NotificationCenterTest, testDefaultNotificationCenter);
	CppUnit_addTest(pSuite, NotificationCenterTest, testMixedObservers);

	return pSuite;
}
