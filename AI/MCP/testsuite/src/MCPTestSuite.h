//
// MCPTestSuite.h
//
// Definition of the MCPTestSuite class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MCPTestSuite_INCLUDED
#define MCPTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class MCPTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // MCPTestSuite_INCLUDED
