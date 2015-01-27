//
// ASNTestSuite.cpp
//
// $Id: //poco/1.4/ASN/testsuite/src/ASNTestSuite.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ASNTestSuite.h"
#include "ASN1Test.h"


CppUnit::Test* ASNTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ASNTestSuite");

	pSuite->addTest(ASN1Test::suite());

	return pSuite;
}
