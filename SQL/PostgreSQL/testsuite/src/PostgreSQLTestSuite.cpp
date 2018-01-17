//
// PostgreSQLTestSuite.cpp
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PostgreSQLTestSuite.h"
#include "PostgreSQLTest.h"

CppUnit::Test* PostgreSQLTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PostgreSQLTestSuite");

	addTest(pSuite, PostgreSQLTest::suite());
	return pSuite;
}


void PostgreSQLTestSuite::addTest(CppUnit::TestSuite* pSuite, CppUnit::Test* pT)
{
	if (pSuite && pT) pSuite->addTest(pT);
}
