//
// IntGaugeTest.h
//
// Definition of the IntGaugeTest class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef IntGaugeTest_INCLUDED
#define IntGaugeTest_INCLUDED


#include "CppUnit/TestCase.h"


class IntGaugeTest: public CppUnit::TestCase
{
public:
	IntGaugeTest(const std::string& name);
	~IntGaugeTest() = default;

	void testBasicBehavior();
	void testExport();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // IntGaugeTest_INCLUDED
