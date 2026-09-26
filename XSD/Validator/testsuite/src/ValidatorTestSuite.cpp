//
// ValidatorTestSuite.cpp
//
// Copyright (c) 2021-2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ValidatorTestSuite.h"
#include "ValidatorTest.h"


CppUnit::Test* ValidatorTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ValidatorTestSuite");

	pSuite->addTest(ValidatorTest::suite());

	return pSuite;
}
