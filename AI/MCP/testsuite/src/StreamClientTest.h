//
// StreamClientTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef StreamClientTest_INCLUDED
#define StreamClientTest_INCLUDED


#include "CppUnit/TestCase.h"


class StreamClientTest: public CppUnit::TestCase
{
public:
	StreamClientTest(const std::string& name);
	~StreamClientTest();

	void testRoundTrip();
	void testRequestCarriesMeta();
	void testPassesOverOtherMessages();
	void testErrorReply();
	void testMalformedReplies();
	void testResultTypeRefused();
	void testEndOfStream();
	void testPagination();
	void testPageLimit();
	void testDiscoverChecksVersion();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // StreamClientTest_INCLUDED
