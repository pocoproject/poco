//
// HTTPTestSuite.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPTestSuite.h"
#include "HTTPTest.h"


CppUnit::Test* HTTPTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPTestSuite");

	pSuite->addTest(HTTPTest::suite());

	return pSuite;
}
