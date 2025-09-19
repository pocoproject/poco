//
// ModulesTest.h
//
// Definition of the ModulesTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ModulesTest_INCLUDED
#define ModulesTest_INCLUDED

#include <print>
#include <string>

#include "CppUnit/TestCase.h"

class ModulesTest: public CppUnit::TestCase
{
public:
	ModulesTest(const std::string& name);
	~ModulesTest();

	void testModule();

	static CppUnit::Test* suite();
};


#endif // ModulesTest_INCLUDED
