//
// XSDParserTestSuite.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "XSDParserTestSuite.h"
#include "XSDParserTest.h"


CppUnit::Test* XSDParserTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XSDParserTestSuite");

	pSuite->addTest(XSDParserTest::suite());

	return pSuite;
}
