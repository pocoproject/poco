//
// FIFOBufferTestSuite.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/FIFOBufferTestSuite.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FIFOBufferTestSuite.h"
#include "FIFOBufferTest.h"


CppUnit::Test* FIFOBufferTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FIFOBufferTestSuite");

	pSuite->addTest(FIFOBufferTest::suite());

	return pSuite;
}
