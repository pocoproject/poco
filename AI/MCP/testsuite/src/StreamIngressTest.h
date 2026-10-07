//
// StreamIngressTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef StreamIngressTest_INCLUDED
#define StreamIngressTest_INCLUDED


#include "CppUnit/TestCase.h"


class StreamIngressTest: public CppUnit::TestCase
{
public:
	StreamIngressTest(const std::string& name);
	~StreamIngressTest();

	void testFraming();
	void testParseError();
	void testBatchRejected();
	void testClientRoundTrip();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // StreamIngressTest_INCLUDED
