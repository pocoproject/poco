//
// HTTPTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPTest_INCLUDED
#define HTTPTest_INCLUDED


#include "CppUnit/TestCase.h"


class HTTPTest: public CppUnit::TestCase
	/// Exercises the HTTP client transport against the in-test TestHTTPHost.
{
public:
	HTTPTest(const std::string& name);
	~HTTPTest();

	void testRoundTrip();
	void testStandardHeaders();
	void testParamHeaders();
	void testUnsafeValuesEncoded();
	void testInvalidAnnotationsDropped();
	void testErrorReplies();
	void testHttpStatus();
	void testEventStream();
	void testCutEventStream();
	void testResponseLimit();
	void testEventAtResponseLimit();
	void testTimeout();
	void testEventStreamDeadline();
	void testStaleKeepAlive();
	void testLostAnswerNotRepeated();
	void testAnswersRefused();
	void testBodyBreaksOff();
	void testHeaderMismatch();
	void testReadsBoundedByTheTimeout();
	void testHeadBoundedByTheTimeout();
	void testDuplicateNotInherited();
	void testEventStreamFraming();
	void testLogShowsNoSecrets();
	void testRequestHook();
	void testSessionFactory();
	void testHostlessSession();
	void testNullSession();
	void testHttpsNeedsInstantiator();
	void testEventStreamReader();
	void testReadAtMostStopsAtTheLimit();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // HTTPTest_INCLUDED
