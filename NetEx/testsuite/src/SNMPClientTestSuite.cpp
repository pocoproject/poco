//
// SNMPClientTestSuite.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/SNMPClientTestSuite.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SNMPClientTestSuite.h"
#include "SNMPClientTest.h"


CppUnit::Test* SNMPClientTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SNMPClientTestSuite");

	pSuite->addTest(SNMPClientTest::suite());

	return pSuite;
}
