//
// HistogramTest.h
//
// Definition of the HistogramTest class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HistogramTest_INCLUDED
#define HistogramTest_INCLUDED


#include "CppUnit/TestCase.h"


class HistogramTest: public CppUnit::TestCase
{
public:
	HistogramTest(const std::string& name);
	~HistogramTest() = default;

	void testNoBuckets();
	void testBuckets();
	void testLabels();
	void testExport();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // HistogramTest_INCLUDED
