//
// NetTestSuite.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/NetTestSuite.cpp#2 $
//
// Copyright (c) 2005-2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NetExTestSuite.h"
#include "ICMPClientTestSuite.h"
#include "NTPClientTestSuite.h"
#include "SNMPClientTestSuite.h"


CppUnit::Test* NetExTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NetExTestSuite");

	pSuite->addTest(ICMPClientTestSuite::suite());
	pSuite->addTest(NTPClientTestSuite::suite());
	pSuite->addTest(SNMPClientTestSuite::suite());

	return pSuite;
}
