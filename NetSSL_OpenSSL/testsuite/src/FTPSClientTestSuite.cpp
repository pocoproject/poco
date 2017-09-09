//
// FTPClientTestSuite.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FTPSClientTestSuite.h"
#include "FTPSClientSessionTest.h"


CppUnit::Test* FTPSClientTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FTPSClientTestSuite");

	pSuite->addTest(FTPSClientSessionTest::suite());

	return pSuite;
}
