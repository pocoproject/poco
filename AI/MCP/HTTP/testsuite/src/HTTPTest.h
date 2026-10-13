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
	/// Exercises the HTTP client transport against the in-test TestHTTPHost,
	/// the server-side peer the library itself does not ship.
{
public:
	HTTPTest(const std::string& name);
	~HTTPTest();

	void testRoundTrip();
	void testSessionRequired();
	void testProtocolVersion();
	void testEventStream();
	void testSessionFactory();
	void testHttpsNeedsInstantiator();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // HTTPTest_INCLUDED
