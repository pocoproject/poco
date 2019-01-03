//
// SQLTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SQLTestSuite.h"
#include "SQLTest.h"
#include "SessionPoolTest.h"


CppUnit::Test* SQLTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SQLTestSuite");

	pSuite->addTest(SQLTest::suite());
	pSuite->addTest(SessionPoolTest::suite());

	return pSuite;
}
