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
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/AutoPtr.h"


using Poco::NotificationCenter;
using Poco::Observer;
using Poco::NObserver;
using Poco::Notification;
using Poco::AutoPtr;


class TestNotification: public Notification
{
};


NotificationCenterTest::NotificationCenterTest(const std::string& name): CppUnit::TestCase(name)
{
}


NotificationCenterTest::~NotificationCenterTest()
{
}


void NotificationCenterTest::test1()
{
	NotificationCenter nc;
	nc.postNotification(new Notification);
}


void NotificationCenterTest::test2()
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


void NotificationCenterTest::test3()
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


void NotificationCenterTest::test4()
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


void NotificationCenterTest::test5()
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


void NotificationCenterTest::testAuto()
{
	NotificationCenter nc;
	nc.addObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handleAuto") != _set.end());
	nc.removeObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
}


void NotificationCenterTest::testDefaultCenter()
{
	NotificationCenter& nc = NotificationCenter::defaultCenter();
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.postNotification(new Notification);
	assertTrue (_set.size() == 1);
	assertTrue (_set.find("handle1") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
}


void NotificationCenterTest::handle1(Poco::Notification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;	
	_set.insert("handle1");
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
	_set.insert("handleAuto");
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

	CppUnit_addTest(pSuite, NotificationCenterTest, test1);
	CppUnit_addTest(pSuite, NotificationCenterTest, test2);
	CppUnit_addTest(pSuite, NotificationCenterTest, test3);
	CppUnit_addTest(pSuite, NotificationCenterTest, test4);
	CppUnit_addTest(pSuite, NotificationCenterTest, test5);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAuto);
	CppUnit_addTest(pSuite, NotificationCenterTest, testDefaultCenter);

	return pSuite;
}
