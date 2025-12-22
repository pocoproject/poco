//
// GaugeTest.h
//
// Definition of the GaugeTest class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef GaugeTest_INCLUDED
#define GaugeTest_INCLUDED


#include "CppUnit/TestCase.h"


class GaugeTest: public CppUnit::TestCase
{
public:
	GaugeTest(const std::string& name);
	~GaugeTest() = default;

	void testBasicBehavior();
	void testInvalidName();
	void testLabels();
	void testExport();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // GaugeTest_INCLUDED
