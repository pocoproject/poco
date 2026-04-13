//
// SSHTestSuite.cpp
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "SSHTestSuite.h"
#include "SSHTest.h"


CppUnit::Test* SSHTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SSHTestSuite");

	pSuite->addTest(SSHTest::suite());

	return pSuite;
}
