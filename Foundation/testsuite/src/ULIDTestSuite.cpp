//
// ULIDTestSuite.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ULIDTestSuite.h"
#include "ULIDTest.h"
#include "ULIDGeneratorTest.h"


CppUnit::Test* ULIDTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ULIDTestSuite");

	pSuite->addTest(ULIDTest::suite());
	pSuite->addTest(ULIDGeneratorTest::suite());

	return pSuite;
}
