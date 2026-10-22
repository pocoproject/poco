//
// ServerTransportTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ServerTransportTest_INCLUDED
#define ServerTransportTest_INCLUDED


#include "CppUnit/TestCase.h"


class ServerTransportTest: public CppUnit::TestCase
	/// Exercises the HTTP server transport with requests sent as they are,
	/// and with the HTTP client of the library.
{
public:
	ServerTransportTest(const std::string& name);
	~ServerTransportTest();

	void testMethods();
	void testOrigin();
	void testContentType();
	void testAccept();
	void testBodyLimit();
	void testMalformedBodies();
	void testNotifications();
	void testStandardHeaders();
	void testNameHeader();
	void testRepeatedHeaders();
	void testParamHeaders();
	void testEncodedWords();
	void testDecodedHeadersRefused();
	void testOtherRevisions();
	void testReplyStatus();
	void testContext();
	void testBodyReadByHost();
	void testUnavailable();
	void testEndpointPath();
	void testClientRoundTrip();
	void testConcurrentRequests();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ServerTransportTest_INCLUDED
