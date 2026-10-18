//
// SerialTestSuite.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SerialTestSuite.h"
#include "SerialPortTest.h"


CppUnit::Test* SerialTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerialTestSuite");

	pSuite->addTest(SerialPortTest::suite());

	return pSuite;
}
