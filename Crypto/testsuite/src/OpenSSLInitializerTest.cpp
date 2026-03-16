//
// OpenSSLInitializerTest.cpp
//
// Copyright (c) 2006-2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OpenSSLInitializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/Crypto/CryptoException.h"
#include <openssl/crypto.h>
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/provider.h>
#endif


using Poco::Crypto::OpenSSLInitializer;


OpenSSLInitializerTest::OpenSSLInitializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


OpenSSLInitializerTest::~OpenSSLInitializerTest()
{
}


void OpenSSLInitializerTest::testMultipleInitialize()
{
	// Multiple initialize calls should not throw.
	// The reference count keeps track internally.
	OpenSSLInitializer::initialize();
	OpenSSLInitializer::initialize();
	OpenSSLInitializer::initialize();

	// Matching uninitialize calls
	OpenSSLInitializer::uninitialize();
	OpenSSLInitializer::uninitialize();
	OpenSSLInitializer::uninitialize();
}


void OpenSSLInitializerTest::testInitializeUninitialize()
{
	// RAII via constructor/destructor
	{
		OpenSSLInitializer init1;
		{
			OpenSSLInitializer init2;
			// Both should coexist without issues
		}
		// init2 destroyed, init1 still alive — OpenSSL should still work
		// Verify by checking that the library version is available
		assertTrue(OpenSSL_version_num() > 0);
	}
	// Both destroyed — reinitialize for remaining tests
	OpenSSLInitializer::initialize();
	assertTrue(OpenSSL_version_num() > 0);
	OpenSSLInitializer::uninitialize();
}


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)

void OpenSSLInitializerTest::testDefaultProvider()
{
	// After initialization, the default provider must be available
	OpenSSLInitializer::initialize();
	assertTrue(OSSL_PROVIDER_available(nullptr, "default"));
	OpenSSLInitializer::uninitialize();
}


void OpenSSLInitializerTest::testLegacyProvider()
{
	// The legacy provider may or may not be available depending
	// on the OpenSSL build configuration. haveLegacyProvider()
	// must reflect whether it was loaded.
	OpenSSLInitializer::initialize();
	bool haveLegacy = OpenSSLInitializer::haveLegacyProvider();
	assertTrue(haveLegacy == OSSL_PROVIDER_available(nullptr, "legacy"));
	OpenSSLInitializer::uninitialize();
}


// Note: provider lifecycle (GH #4451) is not directly tested here.
// Provider cleanup is delegated to OpenSSL's atexit handler
// (OPENSSL_cleanup) because both OSSL_PROVIDER_unload() and nulling
// the static provider pointers cause LeakSanitizer false positives
// on Linux (LSAN runs before atexit handlers).

#endif // POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


void OpenSSLInitializerTest::setUp()
{
}


void OpenSSLInitializerTest::tearDown()
{
}


CppUnit::Test* OpenSSLInitializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OpenSSLInitializerTest");

	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testMultipleInitialize);
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testInitializeUninitialize);
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testDefaultProvider);
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testLegacyProvider);
#endif

	return pSuite;
}
