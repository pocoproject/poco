//
// OpenSSLInitializerTest.h
//
// Definition of the OpenSSLInitializerTest class.
//
// Copyright (c) 2006-2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OpenSSLInitializerTest_INCLUDED
#define OpenSSLInitializerTest_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "CppUnit/TestCase.h"


class OpenSSLInitializerTest: public CppUnit::TestCase
{
public:
	OpenSSLInitializerTest(const std::string& name);
	~OpenSSLInitializerTest();

	void testMultipleInitialize();
	void testInitializeUninitialize();
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	void testDefaultProvider();
	void testLegacyProvider();
#endif

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // OpenSSLInitializerTest_INCLUDED
