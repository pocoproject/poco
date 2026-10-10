//
// XSDTypesTestSuite.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "XSDTypesTestSuite.h"
#include "XSDTypesTest.h"


CppUnit::Test* XSDTypesTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XSDTypesTestSuite");

	pSuite->addTest(XSDTypesTest::suite());

	return pSuite;
}
