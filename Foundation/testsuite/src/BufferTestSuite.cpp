//
// BufferTestSuite.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/BufferTestSuite.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BufferTestSuite.h"
#include "BasicFIFOBufferTest.h"


CppUnit::Test* BufferTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BufferTestSuite");

	pSuite->addTest(BasicFIFOBufferTest::suite());

	return pSuite;
}
