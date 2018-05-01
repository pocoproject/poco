//
// SQLTestSuite.h
//
// Definition of the SQLTestSuite class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLTestSuite_INCLUDED
#define SQLTestSuite_INCLUDED


#include "Poco/CppUnit/TestSuite.h"


class SQLTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // SQLTestSuite_INCLUDED
