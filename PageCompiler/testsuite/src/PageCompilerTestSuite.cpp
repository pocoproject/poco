//
// PageCompilerTestSuite.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PageCompilerTestSuite.h"
#include "PageCompilerTest.h"


CppUnit::Test* PageCompilerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PageCompilerTestSuite");

	pSuite->addTest(PageCompilerTest::suite());

	return pSuite;
}
