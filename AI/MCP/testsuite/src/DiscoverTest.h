//
// DiscoverTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DiscoverTest_INCLUDED
#define DiscoverTest_INCLUDED


#include "CppUnit/TestCase.h"


class DiscoverTest: public CppUnit::TestCase
{
public:
	DiscoverTest(const std::string& name);
	~DiscoverTest();

	void testDiscover();
	void testDiscoverCapabilities();
	void testServerInfoOnEveryResult();
	void testCacheHints();
	void testMissingMeta();
	void testUnsupportedVersion();
	void testInitializeRefused();
	void testInvalidIds();
	void testNotificationsGetNoReply();
	void testUnknownMethod();
	void testContextFromMeta();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // DiscoverTest_INCLUDED
