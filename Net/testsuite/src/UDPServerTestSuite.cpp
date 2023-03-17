//
// UDPServerTestSuite.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UDPServerTestSuite.h"
#include "UDPServerTest.h"


CppUnit::Test* UDPServerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UDPServerTestSuite");

	pSuite->addTest(UDPServerTest::suite());

	return pSuite;
}
