//
// HTTPClientTestSuite.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPClientTestSuite.h"
#include "HTTPClientSessionTest.h"
#include "HTTPClientSessionPoolTest.h"
#include "HTTPStreamFactoryTest.h"


CppUnit::Test* HTTPClientTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPClientTestSuite");

	pSuite->addTest(HTTPClientSessionTest::suite());
	pSuite->addTest(HTTPClientSessionPoolTest::suite());
	pSuite->addTest(HTTPStreamFactoryTest::suite());

	return pSuite;
}
