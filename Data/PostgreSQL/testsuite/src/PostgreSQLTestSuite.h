//
// PostgreSQLTestSuite.h
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PostgreSQLTestSuite_INCLUDED
#define PostgreSQLTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"

class PostgreSQLTestSuite
{
public:
	static CppUnit::Test* suite();

private:
	static void addTest(CppUnit::TestSuite* pSuite, CppUnit::Test* pT);
};


#endif // PostgreSQLTestSuite_INCLUDED
