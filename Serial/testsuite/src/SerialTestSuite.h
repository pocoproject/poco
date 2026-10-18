//
// SerialTestSuite.h
//
// Definition of the SerialTestSuite class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SerialTestSuite_INCLUDED
#define SerialTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class SerialTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // SerialTestSuite_INCLUDED
