//
// HTTPTestSuite.h
//
// Definition of the HTTPTestSuite class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HTTPTestSuite_INCLUDED
#define HTTPTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class HTTPTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // HTTPTestSuite_INCLUDED
