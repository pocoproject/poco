//
// CancellationTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CancellationTest_INCLUDED
#define CancellationTest_INCLUDED


#include "CppUnit/TestCase.h"


class CancellationTest: public CppUnit::TestCase
{
public:
	CancellationTest(const std::string& name);
	~CancellationTest();

	void testCancelCallsEveryAbort();
	void testAbortAfterCancel();
	void testUnregistered();
	void testThrowingAbort();
	void testThrowingAbortInConstructor();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // CancellationTest_INCLUDED
