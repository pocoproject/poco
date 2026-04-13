//
// SSHTestSuite.h
//
// Definition of the SSHTestSuite class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSHTestSuite_INCLUDED
#define SSHTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class SSHTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // SSHTestSuite_INCLUDED
