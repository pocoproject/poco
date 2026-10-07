//
// StdIOTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef StdIOTest_INCLUDED
#define StdIOTest_INCLUDED


#include "CppUnit/TestCase.h"


class StdIOTest: public CppUnit::TestCase
{
public:
	StdIOTest(const std::string& name);
	~StdIOTest();

	void testStdoutIsJsonOnly();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // StdIOTest_INCLUDED
