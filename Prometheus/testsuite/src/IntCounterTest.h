//
// IntCounterTest.h
//
// Definition of the IntCounterTest class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef IntCounterTest_INCLUDED
#define IntCounterTest_INCLUDED


#include "CppUnit/TestCase.h"


class IntCounterTest: public CppUnit::TestCase
{
public:
	IntCounterTest(const std::string& name);
	~IntCounterTest() = default;

	void testBasicBehavior();
	void testExport();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // IntCounterTest_INCLUDED
