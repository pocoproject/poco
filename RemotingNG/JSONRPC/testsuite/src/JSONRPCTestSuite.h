//
// JSONRPCTestSuite.h
//
// Definition of the JSONRPCTestSuite class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSONRPCTestSuite_INCLUDED
#define JSONRPCTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class JSONRPCTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // JSONRPCTestSuite_INCLUDED
