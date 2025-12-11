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


NotificationCenterTest::NotificationCenterTest(const std::string& name):
	CppUnit::TestCase(name),
	_handle1Done(false),
	_handleAuto1Done(false)
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


void NotificationCenterTest::setUp()
{
	_set.clear();
	_handle1Done = false;
	_handleAuto1Done = false;
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

	return pSuite;
}
