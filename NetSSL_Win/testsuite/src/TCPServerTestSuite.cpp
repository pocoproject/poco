//
// TCPServerTestSuite.cpp
//
// $Id: //poco/1.4/NetSSL_Win/testsuite/src/TCPServerTestSuite.cpp#1 $
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TCPServerTestSuite.h"
#include "TCPServerTest.h"


CppUnit::Test* TCPServerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TCPServerTestSuite");

	pSuite->addTest(TCPServerTest::suite());

	return pSuite;
}
