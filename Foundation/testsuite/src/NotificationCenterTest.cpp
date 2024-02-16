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
	TestNotification()
	{}

	TestNotification(const std::string& name):
		Notification(name)
	{}
};


NotificationCenterTest::NotificationCenterTest(const std::string& name):
	CppUnit::TestCase(name),
	_handle1Done(false),
	_handleAuto1Done(false),
	_handleAsync1Done(false),
	_handleAsync2Done(false)
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
	using ObserverT = AsyncObserver<NotificationCenterTest, TestNotification>::Type;

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
}


void NotificationCenterTest::handleAsync2(const AutoPtr<TestNotification>& pNf)
{
	Poco::Mutex::ScopedLock l(_mutex);
	_set.insert("handleAsync2");
	_handleAsync2Done = true;
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
	CppUnit_addTest(pSuite, NotificationCenterTest, testDefaultNotificationCenter);
	CppUnit_addTest(pSuite, NotificationCenterTest, testMixedObservers);

	return pSuite;
}
