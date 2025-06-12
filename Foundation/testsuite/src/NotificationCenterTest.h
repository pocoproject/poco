//
// NotificationCenterTest.h
//
// Definition of the NotificationCenterTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NotificationCenterTest_INCLUDED
#define NotificationCenterTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include <set>


class TestNotification;


class NotificationCenterTest: public CppUnit::TestCase
{
public:
	NotificationCenterTest(const std::string& name);
	~NotificationCenterTest() override;

	void testNotificationCenter1();
	void testNotificationCenter2();
	void testNotificationCenter3();
	void testNotificationCenter4();
	void testNotificationCenter5();
	void testNotificationCenterAuto();
	void testAsyncObserver();
	void testAsyncNotificationCenter();
	void testAsyncNotificationCenter2();
	void testAsyncNotificationCenterSyncNotify();
	void testAsyncNotificationCenterAsyncNotify();
	void testAsyncNotificationCenterAsyncBoth();
	void testAsyncNotificationCenterAsyncNotifyStress();
	void testAsyncNotificationCenterAsyncRemoveObserver();
	void testDefaultNotificationCenter();
	void testMixedObservers();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

protected:
	void handle1(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handle2(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handle3(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handleTest(const Poco::AutoPtr<TestNotification>& pNf);
	void handleAuto(const Poco::AutoPtr<Poco::Notification>& pNf);
	void handleAsync1(const Poco::AutoPtr<TestNotification>& pNf);
	void handleAsync2(const Poco::AutoPtr<TestNotification>& pNf);
	Poco::NotificationResult handleSync(const Poco::AutoPtr<TestNotification>& pNf);
	bool matchAsync(const std::string& name) const;

private:
	std::set<std::string> _set;
	std::atomic<bool> _handle1Done {false};
	std::atomic<bool> _handleAuto1Done {false};
	std::atomic<bool> _handleAsync1Done {false};
	std::atomic<bool> _handleAsync2Done {false};

	std::atomic<std::size_t> _handleAsync1Counter {0};
	std::atomic<std::size_t> _handleAsync2Counter {0};

	Poco::Mutex _mutex;
};


#endif // NotificationCenterTest_INCLUDED
