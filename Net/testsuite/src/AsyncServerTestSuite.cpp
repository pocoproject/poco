//
// AsyncServerTestSuite.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AsyncServerTestSuite.h"
#include "UDPServerTest.h"


CppUnit::Test* AsyncServerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AsyncServerTestSuite");

	pSuite->addTest(UDPServerTest::suite());

	return pSuite;
}
