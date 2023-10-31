//
// DataTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DataTestSuite.h"
#include "DataTest.h"
#include "SessionPoolTest.h"
#ifndef POCO_DATA_NO_SQL_PARSER
#include "SQLParserTest.h"
#endif //  POCO_DATA_NO_SQL_PARSER


CppUnit::Test* DataTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DataTestSuite");

	pSuite->addTest(DataTest::suite());
	pSuite->addTest(SessionPoolTest::suite());
#ifndef POCO_DATA_NO_SQL_PARSER
	pSuite->addTest(SQLParserTest::suite());
#endif //  POCO_DATA_NO_SQL_PARSER

	return pSuite;
}
