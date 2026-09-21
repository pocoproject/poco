//
// DigestEngineTest.h
//
// Definition of the DigestEngineTest class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DigestEngineTest_INCLUDED
#define DigestEngineTest_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "CppUnit/TestCase.h"


class DigestEngineTest: public CppUnit::TestCase
{
public:
	DigestEngineTest(const std::string& name);
	~DigestEngineTest();

	void testMD5();
	void testSHA1();
	void testUnknownAlgorithm();
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	void testInitFailure();
	void testFIPSRefusesMD5();
#endif

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // DigestEngineTest_INCLUDED
