//
// EventDispatcherTest.h
//
// Definition of the EventDispatcherTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef EventDispatcherTest_INCLUDED
#define EventDispatcherTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"


class EventDispatcherTest: public CppUnit::TestCase
{
public:
	EventDispatcherTest(const std::string& name);
	~EventDispatcherTest();

	void testTrySubscribe();
	void testTryUnsubscribe();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // EventDispatcherTest_INCLUDED
