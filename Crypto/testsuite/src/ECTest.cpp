//
// ECTest.cpp
//
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ECTest.h"
#include "ErrorQueueCleaner.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/ECDSADigestEngine.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/CryptoException.h"
#include <openssl/pem.h>
#include <openssl/err.h>
#include <iostream>
#include <sstream>
#include <cstring>


using namespace Poco::Crypto;


static const std::string RSA_PRIVATE_KEY_PEM(
	"-----BEGIN PRIVATE KEY-----\n"
	"MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAO5R/uEB+ipGEsZg\n"
	"r3aKVbmUixjMJlBV1s9zhb9MsWqMlP5Cg0SueRb4LK7MnE/veYL4B298HmmeHnDf\n"
	"syHklqzasPyb1d14ze61/Fk4HI6vg2kIxme1o22G4+9XMZWILPkAGw0PtGNXSzbc\n"
	"389+UENat9d/96Iv9XP+I+/bJOzdAgMBAAECgYEAl7YrAyI3FuI04YR7LoMxv0oI\n"
	"omiTm2CJttZbtkQkTSfhUVsWJYrMGwsxprUqbNiOCgnK8fWZhrLZvhEp/9F8+KJX\n"
	"IcWdWS1kIMBLf/rj3uZC+G9YGxWaNh0plweCaW31QEVGTW2QcNWJnnr+ipcNBVCW\n"
	"sNRLf59ZrzD81zKQziECQQD7dV8kbkJpuc2aG2Q3DHbJfX/wqQsY20LKWVNNoG31\n"
	"G3+24gkLAEQUj7bDHB2cFA3fvBhamYYHW2ussZlilAE/AkEA8p/hCHf1tbQr4aIF\n"
	"5pSGYpmUJ952sV3qSUgqiVZXCpvE0ZnmsDv2L3lzaEa6icdmJHa00aTfXmU4tCPk\n"
	"ZDGu4wJBAOf5iHMIULZOVXFEsbfjBLau01eF6xncNIqNyz7CsI28954Uejku3p+F\n"
	"URo/lsVE7uOnF2SFWtLwsK2ZDaKWXWsCQBeJtzUxHh9QqWksZNIn2pm6OItdo7o1\n"
	"WENfwvCr9anLzt5exeiXCJ02dSHKqtIvDBPBiG1GR31K6KNhUAkAYO0CQQDcZVnC\n"
	"5JB6TD8nxnYEWQHJc4OxDZHA7XBe5XjLmBgNxo/iPLSwXFxSAPIVGinR1X2jMiU2\n"
	"bpFajGnId9ZwpP5o\n"
	"-----END PRIVATE KEY-----\n"
);


static const std::string RSA_PUBLIC_KEY_PEM(
	"-----BEGIN PUBLIC KEY-----\n"
	"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDuUf7hAfoqRhLGYK92ilW5lIsY\n"
	"zCZQVdbPc4W/TLFqjJT+QoNErnkW+CyuzJxP73mC+AdvfB5pnh5w37Mh5Jas2rD8\n"
	"m9XdeM3utfxZOByOr4NpCMZntaNthuPvVzGViCz5ABsND7RjV0s23N/PflBDWrfX\n"
	"f/eiL/Vz/iPv2yTs3QIDAQAB\n"
	"-----END PUBLIC KEY-----\n"
);


static const std::string RSA_CERT_PEM(
	"-----BEGIN CERTIFICATE-----\n"
	"MIICDjCCAXegAwIBAgIUQ5rju7OGxMaV2x5ZdBHVLXIlg7MwDQYJKoZIhvcNAQEL\n"
	"BQAwGDEWMBQGA1UEAwwNcG9jby1yc2EtdGVzdDAgFw0yNjExMjYwOTU2MDJaGA8y\n"
	"MTI2MTEwMjA5NTYwMlowGDEWMBQGA1UEAwwNcG9jby1yc2EtdGVzdDCBnzANBgkq\n"
	"hkiG9w0BAQEFAAOBjQAwgYkCgYEA7lH+4QH6KkYSxmCvdopVuZSLGMwmUFXWz3OF\n"
	"v0yxaoyU/kKDRK55FvgsrsycT+95gvgHb3weaZ4ecN+zIeSWrNqw/JvV3XjN7rX8\n"
	"WTgcjq+DaQjGZ7WjbYbj71cxlYgs+QAbDQ+0Y1dLNtzfz35QQ1q313/3oi/1c/4j\n"
	"79sk7N0CAwEAAaNTMFEwHQYDVR0OBBYEFMoQ4d9+J8k9Od5+99mVHi5aEKC8MB8G\n"
	"A1UdIwQYMBaAFMoQ4d9+J8k9Od5+99mVHi5aEKC8MA8GA1UdEwEB/wQFMAMBAf8w\n"
	"DQYJKoZIhvcNAQELBQADgYEAL1o5hrPXIjZliabWKX/1GqEVdKBcWirV6gMZ34yO\n"
	"ACZ7Smj7TQ9ScNuTbDbPu6EIwHw8P0PrWVDTUx3ZXL0ngh/4LD7qLSdOz2u4qDE8\n"
	"ZtPXELZNC/ZJrTGfsX6o6v/B2qiq2GF4E/OtphaWq3AYzeEj3vOTmB7GlC6sxyCI\n"
	"j0Y=\n"
	"-----END CERTIFICATE-----\n"
);


#ifndef OPENSSL_NO_SM2

static const std::string SM2_PRIVATE_KEY_PEM(
	"-----BEGIN PRIVATE KEY-----\n"
	"MIGHAgEAMBMGByqGSM49AgEGCCqBHM9VAYItBG0wawIBAQQgAZOSG6QDMJxJnide\n"
	"Pd7GjDeIoRClWY06WQrVcKeW57ehRANCAARdq6AfskBBqsCo49mjc8eGnwIvwVLs\n"
	"wlPTGhz1TjnWhkAdKeXTpG34A7YWZDPWF3eYzQt4d79m2LFRnIJbuxFC\n"
	"-----END PRIVATE KEY-----\n"
);


static const std::string SM2_PUBLIC_KEY_PEM(
	"-----BEGIN PUBLIC KEY-----\n"
	"MFkwEwYHKoZIzj0CAQYIKoEcz1UBgi0DQgAEXaugH7JAQarAqOPZo3PHhp8CL8FS\n"
	"7MJT0xoc9U451oZAHSnl06Rt+AO2FmQz1hd3mM0LeHe/ZtixUZyCW7sRQg==\n"
	"-----END PUBLIC KEY-----\n"
);

#endif


ECTest::ECTest(const std::string& name): CppUnit::TestCase(name)
{
}


ECTest::~ECTest()
{
}

#if defined(POCO_TEST_DEPRECATED)

void ECTest::testECNewKeys()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			ECKey key(curveName);
			std::ostringstream strPub;
			std::ostringstream strPriv;
			key.save(&strPub, &strPriv, "testpwd");
			std::string pubKey = strPub.str();
			std::string privKey = strPriv.str();

			// now do the round trip
			std::istringstream iPub(pubKey);
			std::istringstream iPriv(privKey);
			ECKey key2(&iPub, &iPriv, "testpwd");

			std::istringstream iPriv2(privKey);
			ECKey key3(0, &iPriv2, "testpwd");
			std::ostringstream strPub3;
			key3.save(&strPub3);
			std::string pubFromPrivate = strPub3.str();
			assertTrue (pubFromPrivate == pubKey);
		}
		else
			std::cerr << "No elliptic curves found!" << std::endl;
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void ECTest::testECNewKeysNoPassphrase()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			ECKey key(curveName);
			std::ostringstream strPub;
			std::ostringstream strPriv;
			key.save(&strPub, &strPriv);
			std::string pubKey = strPub.str();
			std::string privKey = strPriv.str();

			// now do the round trip
			std::istringstream iPub(pubKey);
			std::istringstream iPriv(privKey);
			ECKey key2(&iPub, &iPriv);

			std::istringstream iPriv2(privKey);
			ECKey key3(0, &iPriv2);
			std::ostringstream strPub3;
			key3.save(&strPub3);
			std::string pubFromPrivate = strPub3.str();
			assertTrue (pubFromPrivate == pubKey);
		}
		else
			std::cerr << "No elliptic curves found!" << std::endl;
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void ECTest::testECDSASignSha256()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			std::string msg("Test this sign message");
			ECKey key(curveName);
			ECDSADigestEngine eng(key, "SHA256");
			eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
			const Poco::DigestEngine::Digest& sig = eng.signature();

			// verify
			std::ostringstream strPub;
			key.save(&strPub);
			std::string pubKey = strPub.str();
			std::istringstream iPub(pubKey);
			ECKey keyPub(&iPub);
			ECDSADigestEngine eng2(keyPub, "SHA256");
			eng2.update(msg.c_str(), static_cast<unsigned>(msg.length()));
			assertTrue(eng2.verify(sig));
		}
		else
			std::cerr << "No elliptic curves found!" << std::endl;
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void ECTest::testECDSASignManipulated()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			std::string msg("Test this sign message");
			std::string msgManip("Test that sign message");
			ECKey key(curveName);
			ECDSADigestEngine eng(key, "SHA256");
			eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
			const Poco::DigestEngine::Digest& sig = eng.signature();
			std::string hexDig = Poco::DigestEngine::digestToHex(sig);

			// verify
			std::ostringstream strPub;
			key.save(&strPub);
			std::string pubKey = strPub.str();
			std::istringstream iPub(pubKey);
			ECKey keyPub(&iPub);
			ECDSADigestEngine eng2(keyPub, "SHA256");
			eng2.update(msgManip.c_str(), static_cast<unsigned>(msgManip.length()));
			assertTrue (!eng2.verify(sig));
		}
		else
			std::cerr << "No elliptic curves found!" << std::endl;
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}

#endif


void ECTest::testECDSASignatureEmptyRS()
{
	// The constructor must not leak the allocated ECDSA_SIG when it rejects
	// empty r and s. The leak is only detected in a sanitizer build.
	try
	{
		ECDSASignature sig((ECDSASignature::ByteVec()), (ECDSASignature::ByteVec()));
		fail("Empty r and s must be rejected");
	}
	catch (Poco::AssertionViolationException&)
	{
	}
}


void ECTest::testCurveNameUnknownNID()
{
	ErrorQueueCleaner cleaner;

	ERR_clear_error();
	assertTrue (ECKey::getCurveName(999999).empty());
	assertTrue (ERR_peek_error() == 0);
}


void ECTest::testCurveNIDDefaultName()
{
	ErrorQueueCleaner cleaner;

	std::string name;
	ERR_clear_error();
	int nid = ECKey::getCurveNID(name);
	assertTrue (ERR_peek_error() == 0);
	assertTrue (nid != -1);
	assertTrue (!name.empty());
	assertTrue (name == ECKey::getCurveName(nid));
	assertTrue (name == ECKey::getCurveName());
}


void ECTest::testUnknownCurveName()
{
	ErrorQueueCleaner cleaner;

	ERR_clear_error();
	try
	{
		ECKey key("nosuchcurve");
		fail("An unknown curve name must throw");
	}
	catch (const OpenSSLException&) {}
	assertTrue (ERR_peek_error() == 0);
}


void ECTest::testNotECKey()
{
	ErrorQueueCleaner cleaner;

	std::istringstream publicKeyStream(RSA_PUBLIC_KEY_PEM);
	try
	{
		ECKey key(&publicKeyStream);
		fail("ECKey from an RSA public key must throw");
	}
	catch (const OpenSSLException&) {}

	std::istringstream privateKeyStream(RSA_PRIVATE_KEY_PEM);
	try
	{
		ECKey key(nullptr, &privateKeyStream);
		fail("ECKey from an RSA private key must throw");
	}
	catch (const OpenSSLException&) {}

	std::istringstream certStream(RSA_CERT_PEM);
	X509Certificate cert(certStream);
	try
	{
		ECKey key(cert);
		fail("ECKey from an RSA certificate must throw");
	}
	catch (const OpenSSLException&) {}
}


#ifndef OPENSSL_NO_SM2

void ECTest::testSM2Key()
{
	ErrorQueueCleaner cleaner;

	std::istringstream privateKeyStream(SM2_PRIVATE_KEY_PEM);
	ECKey privateKey(nullptr, &privateKeyStream);
	std::istringstream publicKeyStream(SM2_PUBLIC_KEY_PEM);
	ECKey publicKey(&publicKeyStream);

	const std::string msg("Test this sign message");
	ECDSADigestEngine signer(privateKey, "SHA256");
	signer.update(msg);
	const Poco::DigestEngine::Digest& sig = signer.signature();
	ECDSADigestEngine verifier(publicKey, "SHA256");
	verifier.update(msg);
	assertTrue (verifier.verify(sig));
}

#endif


void ECTest::setUp()
{
}


void ECTest::tearDown()
{
}


CppUnit::Test* ECTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ECTest");

#if defined(POCO_TEST_DEPRECATED)

	CppUnit_addTest(pSuite, ECTest, testECNewKeys);
	CppUnit_addTest(pSuite, ECTest, testECNewKeysNoPassphrase);
	CppUnit_addTest(pSuite, ECTest, testECDSASignSha256);
	CppUnit_addTest(pSuite, ECTest, testECDSASignManipulated);

#endif

	CppUnit_addTest(pSuite, ECTest, testECDSASignatureEmptyRS);
	CppUnit_addTest(pSuite, ECTest, testCurveNameUnknownNID);
	CppUnit_addTest(pSuite, ECTest, testCurveNIDDefaultName);
	CppUnit_addTest(pSuite, ECTest, testUnknownCurveName);
	CppUnit_addTest(pSuite, ECTest, testNotECKey);
#ifndef OPENSSL_NO_SM2
	CppUnit_addTest(pSuite, ECTest, testSM2Key);
#endif

	return pSuite;
}
