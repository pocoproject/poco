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


void OpenSSLInitializerTest::testProviderCleanup()
{
	// Verify that after a full initialize/uninitialize cycle down to
	// ref count 0, providers are properly cleaned up (GH #4451).
	// The test driver holds one global init, so we must drain that too.
	// We verify via haveDefaultProvider()/haveLegacyProvider() which
	// check the managed pointers.

	OpenSSLInitializer::initialize();
	assertTrue(OpenSSLInitializer::haveDefaultProvider());

	// Drain ref count to 0: undo our init + the driver's global init
	OpenSSLInitializer::uninitialize();
	OpenSSLInitializer::uninitialize();

	// Both provider pointers should be null now (ref count was 0)
	assertFalse(OpenSSLInitializer::haveDefaultProvider());
	assertFalse(OpenSSLInitializer::haveLegacyProvider());

	// Re-initialize to restore state for the driver's uninitialize at exit.
	// Note: we do not verify provider reload consistency here because
	// OpenSSL internally leaks OSSL_LIB_CTX state on provider reload,
	// which triggers LeakSanitizer false positives.
	OpenSSLInitializer::initialize();
}

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
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testProviderCleanup);
#endif

	return pSuite;
}
