//
// ChatServiceTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ChatServiceTest_INCLUDED
#define ChatServiceTest_INCLUDED


#include "CppUnit/TestCase.h"


class ChatServiceTest: public CppUnit::TestCase
	/// Exercises the chat service and its HTTP interface.
{
public:
	ChatServiceTest(const std::string& name);
	~ChatServiceTest();

	void testTurn();
	void testToolCall();
	void testNoProvider();
	void testModels();
	void testFailover();
	void testCancel();
	void testSelect();
	void testConfigure();
	void testStoreRefuses();
	void testSettingsJSON();
	void testDefaultProvider();
	void testEndpointHealth();
	void testEndpointStream();
	void testEndpointCancel();
	void testEndpointManage();
	void testEndpointRefusals();
	void testEndpointHeartbeat();
	void testEndpointClientGone();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ChatServiceTest_INCLUDED
