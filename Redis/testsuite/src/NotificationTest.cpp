//
// NotificationTest.cpp
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "NotificationTest.h"
#include "Poco/Redis/Client.h"
#include "Poco/Redis/RedisNotifications.h"
#include "Poco/NObserver.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <iostream>


using namespace Poco::Redis;
using namespace Poco;


NotificationTest::NotificationTest(const std::string& name): CppUnit::TestCase(name)
{
}


NotificationTest::~NotificationTest()
{
}


void NotificationTest::testRedisNotifications()
{
	// Test basic notification creation
	{
		RedisConnectNotification connectNotif;
		// Just verify it's a valid notification
		assertTrue (connectNotif.name().length() > 0);
	}

	{
		RedisDisconnectNotification disconnectNotif;
		assertTrue (disconnectNotif.name().length() > 0);
	}

	{
		Exception ex("Test error");
		RedisErrorNotification errorNotif(ex);
		assertTrue (errorNotif.name().length() > 0);
		assertTrue (errorNotif.error().message() == "Test error");
		assertTrue (errorNotif.message().find("Test error") != std::string::npos);
	}

	{
		RedisErrorNotification errorNotif("Simple error message");
		assertTrue (errorNotif.name().length() > 0);
		assertTrue (errorNotif.message() == "Simple error message");
	}
}


void NotificationTest::testAsyncNotificationCenter()
{
	Client client;
	auto pNC = client.notificationCenter();

	// Test observer registration
	NObserver<NotificationTest, RedisConnectNotification> connectObserver(*this, &NotificationTest::onConnect);
	NObserver<NotificationTest, RedisDisconnectNotification> disconnectObserver(*this, &NotificationTest::onDisconnect);
	NObserver<NotificationTest, RedisErrorNotification> errorObserver(*this, &NotificationTest::onError);

	pNC->addObserver(connectObserver);
	pNC->addObserver(disconnectObserver);
	pNC->addObserver(errorObserver);

	// Reset counters
	_connectCount = 0;
	_disconnectCount = 0;
	_errorCount = 0;
	_lastErrorMessage.clear();
	_connectEvent.reset();
	_disconnectEvent.reset();
	_errorEvent.reset();

	// Test manual notification posting
	pNC->postNotification(new RedisConnectNotification);
	assertTrue (_connectEvent.tryWait(5000)); // Wait up to 5 seconds for async processing
	assertTrue (_connectCount == 1);

	pNC->postNotification(new RedisDisconnectNotification);
	assertTrue (_disconnectEvent.tryWait(5000));
	assertTrue (_disconnectCount == 1);

	Exception testEx("Test async error");
	pNC->postNotification(new RedisErrorNotification(testEx));
	assertTrue (_errorEvent.tryWait(5000));
	assertTrue (_errorCount == 1);
	assertTrue (_lastErrorMessage.find("Test async error") != std::string::npos);

	// Clean up observers
	pNC->removeObserver(connectObserver);
	pNC->removeObserver(disconnectObserver);
	pNC->removeObserver(errorObserver);
}


void NotificationTest::testClientNotifications()
{
	Client client;
	auto pNC = client.notificationCenter();

	NObserver<NotificationTest, RedisConnectNotification> connectObserver(*this, &NotificationTest::onConnect);
	NObserver<NotificationTest, RedisErrorNotification> errorObserver(*this, &NotificationTest::onError);

	pNC->addObserver(connectObserver);
	pNC->addObserver(errorObserver);

	// Reset counters
	_connectCount = 0;
	_errorCount = 0;
	_lastErrorMessage.clear();
	_errorEvent.reset();

	// Test manual notification posting through client's notification center
	Exception testEx("Simulated client error");
	pNC->postNotification(new RedisErrorNotification(testEx));
	assertTrue (_errorEvent.tryWait(5000));

	// Should have received error notification
	assertTrue (_errorCount == 1);
	assertTrue (_lastErrorMessage.find("Simulated client error") != std::string::npos);

	// Clean up observers
	pNC->removeObserver(connectObserver);
	pNC->removeObserver(errorObserver);
}


void NotificationTest::onConnect(const AutoPtr<RedisConnectNotification>& pNotf)
{
	++_connectCount;
	std::cout << "Connect notification received" << std::endl;
	_connectEvent.set();
}


void NotificationTest::onDisconnect(const AutoPtr<RedisDisconnectNotification>& pNotf)
{
	++_disconnectCount;
	std::cout << "Disconnect notification received" << std::endl;
	_disconnectEvent.set();
}


void NotificationTest::onError(const AutoPtr<RedisErrorNotification>& pNotf)
{
	++_errorCount;
	_lastErrorMessage = pNotf->message();
	std::cout << "Error notification received: " << _lastErrorMessage << std::endl;
	_errorEvent.set();
}


void NotificationTest::setUp()
{
	_connectCount = 0;
	_disconnectCount = 0;
	_errorCount = 0;
	_lastErrorMessage.clear();
}


void NotificationTest::tearDown()
{
}


CppUnit::Test* NotificationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NotificationTest");

	CppUnit_addTest(pSuite, NotificationTest, testRedisNotifications);
	CppUnit_addTest(pSuite, NotificationTest, testAsyncNotificationCenter);
	CppUnit_addTest(pSuite, NotificationTest, testClientNotifications);

	return pSuite;
}
