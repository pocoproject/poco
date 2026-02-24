//
// NotificationTest.h
//
// Definition of the NotificationTest class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NotificationTest_INCLUDED
#define NotificationTest_INCLUDED


#include "Poco/Redis/Redis.h"
#include "Poco/Redis/RedisNotifications.h"
#include "Poco/NObserver.h"
#include "Poco/AutoPtr.h"
#include "Poco/Event.h"
#include "CppUnit/TestCase.h"
#include <string>
#include <atomic>


class NotificationTest: public CppUnit::TestCase
{
public:
	NotificationTest(const std::string& name);
	~NotificationTest();

	void testRedisNotifications();
	void testAsyncNotificationCenter();
	void testClientNotifications();

	// Notification handlers for testing
	void onConnect(const Poco::AutoPtr<Poco::Redis::RedisConnectNotification>& pNotf);
	void onDisconnect(const Poco::AutoPtr<Poco::Redis::RedisDisconnectNotification>& pNotf);
	void onError(const Poco::AutoPtr<Poco::Redis::RedisErrorNotification>& pNotf);

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	// Test state tracking
	std::atomic<int> _connectCount;
	std::atomic<int> _disconnectCount;
	std::atomic<int> _errorCount;
	std::string _lastErrorMessage;
	Poco::Event _connectEvent;
	Poco::Event _disconnectEvent;
	Poco::Event _errorEvent;
};


#endif // NotificationTest_INCLUDED
