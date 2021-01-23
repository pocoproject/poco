//
// JWTTestSuite.cpp
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "JWTTestSuite.h"
#include "SerializerTest.h"
#include "TokenTest.h"
#include "SignerTest.h"


CppUnit::Test* JWTTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JWTTestSuite");

	pSuite->addTest(SerializerTest::suite());
	pSuite->addTest(TokenTest::suite());
	pSuite->addTest(SignerTest::suite());

	return pSuite;
}
