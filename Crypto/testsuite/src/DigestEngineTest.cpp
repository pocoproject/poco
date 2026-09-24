//
// DigestEngineTest.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DigestEngineTest.h"
#include "ErrorQueueCleaner.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/Exception.h"
#include <iostream>
#include <openssl/err.h>
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/evp.h>
#include <openssl/provider.h>
#endif


using Poco::Crypto::DigestEngine;
using Poco::Crypto::OpenSSLException;


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
namespace
{
	const std::string SHA256_ABC("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");


	class DefaultPropertiesRestorer
		/// Restores the default property query, also when an assertion fails.
		/// Only "fips=yes" can be detected before OpenSSL 3.5.
	{
	public:
		DefaultPropertiesRestorer():
			_properties(EVP_default_properties_is_fips_enabled(nullptr) == 1 ? "fips=yes" : "")
		{
		}

		~DefaultPropertiesRestorer()
		{
			EVP_set_default_properties(nullptr, _properties.c_str());
		}

	private:
		std::string _properties;
	};
}
#endif


DigestEngineTest::DigestEngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


DigestEngineTest::~DigestEngineTest()
{
}


void DigestEngineTest::testMD5()
{
	DigestEngine engine("MD5");

	// test vectors from RFC 1321

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d41d8cd98f00b204e9800998ecf8427e");

	engine.update("a");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "0cc175b9c0f1b6a831c399e269772661");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "900150983cd24fb0d6963f7d28e17f72");

	engine.update("message digest");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "f96b697d7cb7938d525a2f31aaf161d0");

	engine.update("abcdefghijklmnopqrstuvwxyz");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");

	engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");

	engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "57edf4a22be3c955ac49da2e2107b67a");
}

void DigestEngineTest::testSHA1()
{
	DigestEngine engine("SHA1");

	// test vectors from FIPS 180-1

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "a9993e364706816aba3e25717850c26c9cd0d89d");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
}


void DigestEngineTest::testUnknownAlgorithm()
{
	try
	{
		DigestEngine engine("NoSuchDigest");
		fail("no such digest algorithm - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void DigestEngineTest::testXOFWithoutLength()
{
	ErrorQueueCleaner cleaner;
	ERR_clear_error();
	// OpenSSL 3.5 and newer report no digest size for a SHAKE digest without an output length.
	DigestEngine engine("SHAKE256");
	engine.update("abc");
	try
	{
		const std::size_t length = engine.digestLength();
		assertTrue (length > 0 && length <= 64);
		assertTrue (engine.digest().size() == length);
	}
	catch (OpenSSLException&)
	{
	}
	assertTrue (ERR_peek_error() == 0);
}


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


void DigestEngineTest::testInitFailure()
{
	ErrorQueueCleaner cleaner;
	ERR_clear_error();

	DigestEngine engine("SHA256");

	{
		DefaultPropertiesRestorer restorer;
		assertTrue (EVP_set_default_properties(nullptr, "provider=nonexistent") == 1);

		try
		{
			DigestEngine failing("SHA256");
			fail("no provider for the digest - must throw");
		}
		catch (OpenSSLException& exc)
		{
			assertTrue (exc.displayText().find("SHA256") != std::string::npos);
		}
		assertTrue (ERR_peek_error() == 0);

		try
		{
			engine.reset();
			fail("no provider for the digest - must throw");
		}
		catch (OpenSSLException&)
		{
		}
		assertTrue (ERR_peek_error() == 0);
	}

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == SHA256_ABC);
}


void DigestEngineTest::testFIPSRefusesMD5()
{
	ErrorQueueCleaner cleaner;
	ERR_clear_error();

	if (OSSL_PROVIDER_available(nullptr, "fips") != 1)
	{
		std::cerr << "No FIPS provider, test skipped." << std::endl;
		return;
	}

	DefaultPropertiesRestorer restorer;
	assertTrue (EVP_default_properties_enable_fips(nullptr, 1) == 1);

	try
	{
		DigestEngine md5("MD5");
		fail("MD5 is not available in FIPS mode - must throw");
	}
	catch (OpenSSLException&)
	{
	}

	DigestEngine sha256("SHA256");
	sha256.update("abc");
	assertTrue (DigestEngine::digestToHex(sha256.digest()) == SHA256_ABC);

	assertTrue (EVP_default_properties_enable_fips(nullptr, 0) == 1);

	DigestEngine md5("MD5");
	md5.update("abc");
	assertTrue (DigestEngine::digestToHex(md5.digest()) == "900150983cd24fb0d6963f7d28e17f72");
}


#endif


void DigestEngineTest::setUp()
{
}


void DigestEngineTest::tearDown()
{
}


CppUnit::Test* DigestEngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DigestEngineTest");

	CppUnit_addTest(pSuite, DigestEngineTest, testMD5);
	CppUnit_addTest(pSuite, DigestEngineTest, testSHA1);
	CppUnit_addTest(pSuite, DigestEngineTest, testUnknownAlgorithm);
	CppUnit_addTest(pSuite, DigestEngineTest, testXOFWithoutLength);
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	CppUnit_addTest(pSuite, DigestEngineTest, testInitFailure);
	CppUnit_addTest(pSuite, DigestEngineTest, testFIPSRefusesMD5);
#endif

	return pSuite;
}
