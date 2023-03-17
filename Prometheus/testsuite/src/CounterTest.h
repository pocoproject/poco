//
// CounterTest.h
//
// Definition of the CounterTest class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CounterTest_INCLUDED
#define CounterTest_INCLUDED


#include "CppUnit/TestCase.h"


class CounterTest: public CppUnit::TestCase
{
public:
	CounterTest(const std::string& name);
	~CounterTest() = default;

	void testBasicBehavior();
	void testInvalidName();
	void testLabels();
	void testConcurrency();
	void testExport();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // CounterTest_INCLUDED
