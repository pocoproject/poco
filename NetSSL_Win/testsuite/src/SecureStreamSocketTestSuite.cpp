//
// SecureStreamSocketTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SecureStreamSocketTestSuite.h"
#include "SecureStreamSocketTest.h"


CppUnit::Test* SecureStreamSocketTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SecureStreamSocketTestSuite");

	pSuite->addTest(SecureStreamSocketTest::suite());

	return pSuite;
}
