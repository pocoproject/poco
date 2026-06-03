//
// SQLiteTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SQLiteTestSuite.h"
#include "SQLiteTest.h"
#include "UtilityTest.h"
#include "SQLiteThreadSafetyTest.h"
#ifndef POCO_DATA_NO_SQL_PARSER
#include "MemoryDBTest.h"
#endif


CppUnit::Test* SQLiteTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SQLiteTestSuite");

	pSuite->addTest(SQLiteTest::suite());
	pSuite->addTest(UtilityTest::suite());
	pSuite->addTest(SQLiteThreadSafetyTest::suite());
#ifndef POCO_DATA_NO_SQL_PARSER
	pSuite->addTest(MemoryDBTest::suite());
#endif

	return pSuite;
}
