//
// ActiveRecordTestSuite.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ActiveRecordTestSuite.h"
#include "ActiveRecordTest.h"


CppUnit::Test* ActiveRecordTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ActiveRecordTestSuite");

	pSuite->addTest(ActiveRecordTest::suite());

	return pSuite;
}
