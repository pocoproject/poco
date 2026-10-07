//
// EnvelopeTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef EnvelopeTest_INCLUDED
#define EnvelopeTest_INCLUDED


#include "CppUnit/TestCase.h"


class EnvelopeTest: public CppUnit::TestCase
{
public:
	EnvelopeTest(const std::string& name);
	~EnvelopeTest();

	void testRequestBuild();
	void testNotificationBuild();
	void testResultRoundTrip();
	void testErrorRoundTrip();
	void testNullId();
	void testIdTypeFidelity();
	void testSerializeIsSingleLine();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // EnvelopeTest_INCLUDED
