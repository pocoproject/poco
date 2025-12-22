//
// ODBCTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCTestSuite.h"
#include "ODBCDB2Test.h"
#include "ODBCMySQLTest.h"
#include "ODBCOracleTest.h"
#include "ODBCPostgreSQLTest.h"
#include "ODBCSQLiteTest.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "ODBCAccessTest.h"
#endif
#include "ODBCSQLServerTest.h"


CppUnit::Test* ODBCTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCTestSuite");

	addTest(pSuite, ODBCOracleTest::suite());
	addTest(pSuite, ODBCSQLServerTest::suite());
	addTest(pSuite, ODBCMySQLTest::suite());
	addTest(pSuite, ODBCPostgreSQLTest::suite());
	addTest(pSuite, ODBCSQLiteTest::suite());
	addTest(pSuite, ODBCDB2Test::suite());
// MS Access driver does not support connection status detection
// disabled for the time being
#if 0 //defined(POCO_OS_FAMILY_WINDOWS)
	addTest(pSuite, ODBCAccessTest::suite());
#endif

	return pSuite;
}


void ODBCTestSuite::addTest(CppUnit::TestSuite* pSuite, CppUnit::Test* pT)
{
	if (pSuite && pT) pSuite->addTest(pT);
}
