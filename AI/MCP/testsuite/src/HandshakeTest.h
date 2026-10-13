//
// HandshakeTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HandshakeTest_INCLUDED
#define HandshakeTest_INCLUDED


#include "CppUnit/TestCase.h"


class HandshakeTest: public CppUnit::TestCase
{
public:
	HandshakeTest(const std::string& name);
	~HandshakeTest();

	void testInitialize();
	void testInitializedNotification();
	void testPingBeforeInitialize();
	void testToolsRequireInitialize();
	void testUnknownMethod();
	void testMethodMustBeString();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // HandshakeTest_INCLUDED
