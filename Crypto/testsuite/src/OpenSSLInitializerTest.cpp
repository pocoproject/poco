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
#include "Poco/Crypto/Crypto.h"
#include <openssl/crypto.h>
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/provider.h>
#endif


using Poco::Crypto::CryptoException;
using Poco::Crypto::OpenSSLInitializer;


namespace
{
	class FIPSModeRestorer
		/// Restores the FIPS mode, also when an assertion fails.
	{
	public:
		FIPSModeRestorer(): _wasEnabled(OpenSSLInitializer::isFIPSEnabled())
		{
		}

		~FIPSModeRestorer()
		{
			try
			{
				if (OpenSSLInitializer::isFIPSEnabled() != _wasEnabled)
					OpenSSLInitializer::enableFIPSMode(_wasEnabled);
			}
			catch (...)
			{
			}
		}

		bool wasEnabled() const
		{
			return _wasEnabled;
		}

	private:
		bool _wasEnabled;
	};
}


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
		// init2 destroyed, init1 still alive -- OpenSSL should still work
		// Verify by checking that the library version is available
		assertTrue(OpenSSL_version_num() > 0);
	}
	// Both destroyed -- reinitialize for remaining tests
	OpenSSLInitializer::initialize();
	assertTrue(OpenSSL_version_num() > 0);
	OpenSSLInitializer::uninitialize();
}


void OpenSSLInitializerTest::testFIPSMode()
{
	OpenSSLInitializer init;
	FIPSModeRestorer restorer;
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	const bool haveFIPS = OSSL_PROVIDER_available(nullptr, "fips") == 1;
#else
	const bool haveFIPS = false;
#endif

	if (haveFIPS)
	{
		OpenSSLInitializer::enableFIPSMode(true);
		assertTrue(OpenSSLInitializer::isFIPSEnabled());
		OpenSSLInitializer::enableFIPSMode(false);
		assertTrue(!OpenSSLInitializer::isFIPSEnabled());
	}
	else
	{
		try
		{
			OpenSSLInitializer::enableFIPSMode(true);
			fail("no FIPS provider - must throw");
		}
		catch (const CryptoException&)
		{
		}
		assertTrue(OpenSSLInitializer::isFIPSEnabled() == restorer.wasEnabled());
		if (!restorer.wasEnabled())
			OpenSSLInitializer::enableFIPSMode(false);
	}
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


void OpenSSLInitializerTest::testReinitialize()
{
	// Release the reference held by the test runner (Driver.cpp), so that
	// the count drops to zero; the providers must stay available.
	Poco::Crypto::uninitializeCrypto();
	try
	{
		assertTrue(OSSL_PROVIDER_available(nullptr, "default") == 1);
		OpenSSLInitializer init;
		assertTrue(OSSL_PROVIDER_available(nullptr, "default") == 1);
		assertTrue(OpenSSLInitializer::haveLegacyProvider() == (OSSL_PROVIDER_available(nullptr, "legacy") == 1));
	}
	catch (...)
	{
		Poco::Crypto::initializeCrypto();
		throw;
	}
	Poco::Crypto::initializeCrypto();
}


// Note: if anything else holds an OpenSSLInitializer, the count does not
// reach zero and testReinitialize only checks a nested cycle.

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
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testFIPSMode);
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testDefaultProvider);
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testLegacyProvider);
	CppUnit_addTest(pSuite, OpenSSLInitializerTest, testReinitialize);
#endif

	return pSuite;
}
