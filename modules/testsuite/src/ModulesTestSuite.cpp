//
// ModulesTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ModulesTestSuite.h"
#include "ModulesTest.h"


CppUnit::Test* ModulesTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ModulesTestSuite");

	pSuite->addTest(ModulesTest::suite());

	return pSuite;
}
