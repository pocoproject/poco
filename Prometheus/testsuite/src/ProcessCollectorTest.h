//
// ProcessCollectorTest.h
//
// Definition of the ProcessCollectorTest class.
//
// Copyright (c) 2022-2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ProcessCollectorTest_INCLUDED
#define ProcessCollectorTest_INCLUDED


#include "CppUnit/TestCase.h"


class ProcessCollectorTest: public CppUnit::TestCase
{
public:
	ProcessCollectorTest(const std::string& name);
	~ProcessCollectorTest() = default;

	void testExport();
	void testExportThreadCPU();
	void testCustomName();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ProcessCollectorTest_INCLUDED
