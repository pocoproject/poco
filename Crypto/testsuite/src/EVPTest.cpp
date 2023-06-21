//
// EVPTest.cpp
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EVPTest.h"
#include "PKCS12ContainerTest.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/MemoryStream.h"
#include "Poco/StreamCopier.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>


using namespace Poco::Crypto;
using Poco::TemporaryFile;
using Poco::StreamCopier;


static const std::string anyPemRSA(
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICaDCCAdECCQCzfxSsk7yaLjANBgkqhkiG9w0BAQUFADBzMQswCQYDVQQGEwJB\r\n"
	"VDESMBAGA1UECBMJQ2FyaW50aGlhMRIwEAYDVQQHEwlTdC4gSmFrb2IxDzANBgNV\r\n"
	"BAoTBkFwcEluZjEPMA0GA1UEAxMGQXBwSW5mMRowGAYJKoZIhvcNAQkBFgthcHBA\r\n"
	"aW5mLmNvbTAeFw0wNjAzMDExMzA3MzFaFw0wNjAzMzExMzA3MzFaMH4xCzAJBgNV\r\n"
	"BAYTAkFUMRIwEAYDVQQIEwlDYXJpbnRoaWExETAPBgNVBAcTCFN0IEpha29iMRww\r\n"
	"GgYDVQQKExNBcHBsaWVkIEluZm9ybWF0aWNzMQowCAYDVQQDFAEqMR4wHAYJKoZI\r\n"
	"hvcNAQkBFg9pbmZvQGFwcGluZi5jb20wgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJ\r\n"
	"AoGBAJHGyXDHyCYoWz+65ltNwwZbhwOGnxr9P1WMATuFJh0bPBZxKbZRdbTm9KhZ\r\n"
	"OlvsEIsfgiYdsxURYIqXfEgISYLZcZY0pQwGEOmB+0NeC/+ENSfOlNSthx6zSVlc\r\n"
	"zhJ7+dJOGwepHAiLr1fRuc5jogYLraE+lKTnqAAFfzwvti77AgMBAAEwDQYJKoZI\r\n"
	"hvcNAQEFBQADgYEAY/ZoeY1ukkEJX7259NeoVM0oahlulWV0rlCqyaeosOiDORPT\r\n"
	"m6X1w/5MTCf9VyaD1zukoSZ4QqNVjHFXcXidbB7Tgt3yRuZ5PC5LIFCDPv9mgPne\r\n"
	"mUA70yfctNfza2z3ZiQ6NDkW3mZX+1tmxYIrJQIrkVeYeqf1Gh2nyZrUMcE=\r\n"
	"-----END CERTIFICATE-----\r\n"
	"-----BEGIN RSA PRIVATE KEY-----\r\n"
	"Proc-Type: 4,ENCRYPTED\r\n"
	"DEK-Info: DES-EDE3-CBC,E7AE93C9E49184EA\r\n"
	"\r\n"
	"A2IqzNcWs+I5vzV+i+woDk56+yr58eU0Onw8eEvXkLjnSc58JU4327IF7yUbKWdW\r\n"
	"Q7BYGGOkVFiZ7ANOwviDg5SUhxRDWCcW8dS6/p1vfdQ1C3qj2OwJjkpg0aDBIzJn\r\n"
	"FzgguT3MF3ama77vxv0S3kOfmCj62MLqPGpj5pQ0/1hefRFbL8oAX8bXUN7/rmGM\r\n"
	"Zc0QyzFZv2iQ04dY/6TNclwKPB4H0On4K+8BMs3PRkWA0clCaQaFO2+iwnk3XZfe\r\n"
	"+MsKUEbLCpAQeYspYv1cw38dCdWq1KTP5aJk+oXgwjfX5cAaPTz74NTqTIsCcaTD\r\n"
	"3vy7ukJYFlDR9Kyo7z8rMazYrKJslhnuRH0BhK9st9McwL957j5tZmrKyraCcmCx\r\n"
	"dMAGcsis1va3ayYZpIpFqA4EhYrTM+6N8ZRfUap20+b5IQwHfTQDejUhL6rBwy7j\r\n"
	"Ti5yD83/itoOMyXq2sV/XWfVD5zk/P5iv22O1EAQMhhnPB9K/I/JhuSGQJfn3cNh\r\n"
	"ykOUYT0+vDeSeEVa+FVEP1W35G0alTbKbNs5Tb8KxJ3iDJUxokM//SvPXZy9hOVX\r\n"
	"Y05imB04J15DaGbAHlNzunhuJi7121WV/JRXZRW9diE6hwpD8rwqi3FMuRUmy7U9\r\n"
	"aFA5poKRAYlo9YtZ3YpFyjGKB6MfCQcB2opuSnQ/gbugV41m67uQ4CDwWLaNRkTb\r\n"
	"GlsMBNcHnidg15Bsat5HaB7l250ukrI13Uw1MYdDUzaS3gPfw9aC4F2w0p3U+DPH\r\n"
	"80/zePxtroR7T4/+rI136Rl+aMXDMOEGCX1TVP8rjuZzuRyUSUKC8Q==\r\n"
	"-----END RSA PRIVATE KEY-----\r\n"
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICXTCCAcYCCQC1Vk/N8qR4AjANBgkqhkiG9w0BAQUFADBzMQswCQYDVQQGEwJB\r\n"
	"VDESMBAGA1UECBMJQ2FyaW50aGlhMRIwEAYDVQQHEwlTdC4gSmFrb2IxDzANBgNV\r\n"
	"BAoTBkFwcEluZjEPMA0GA1UEAxMGQXBwSW5mMRowGAYJKoZIhvcNAQkBFgthcHBA\r\n"
	"aW5mLmNvbTAeFw0wNjAyMjcxMzI3MThaFw0wNjAzMjkxMzI3MThaMHMxCzAJBgNV\r\n"
	"BAYTAkFUMRIwEAYDVQQIEwlDYXJpbnRoaWExEjAQBgNVBAcTCVN0LiBKYWtvYjEP\r\n"
	"MA0GA1UEChMGQXBwSW5mMQ8wDQYDVQQDEwZBcHBJbmYxGjAYBgkqhkiG9w0BCQEW\r\n"
	"C2FwcEBpbmYuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCsFXiPuicN\r\n"
	"Im4oJwF8NuaFN+lgYwcZ6dAO3ILIR3kLA2PxF8HSQLfF8J8a4odZhLhctIMAKTxm\r\n"
	"k0w8TW5qhL8QLdGzY9vzvkgdKOkan2t3sMeXJAfrM1AphTsmgntAQazGZjOj5p4W\r\n"
	"jDnxQ+VXAylqwjHh49eSBxM3wgoscF4iLQIDAQABMA0GCSqGSIb3DQEBBQUAA4GB\r\n"
	"AIpfLdXiKchPvFMhQS8xTtXvrw5dVL3yImUMYs4GQi8RrjGmfGB3yMAR7B/b8v4a\r\n"
	"+ztfusgWAWiUKuSGTk4S8YB0fsFlmOv0WDr+PyZ4Lui/a8opbyzGE7rqpnF/s0GO\r\n"
	"M7uLCNNwIN7WhmxcWV0KZU1wTppoSWPJda1yTbBzF9XP\r\n"
	"-----END CERTIFICATE-----\r\n"
);


EVPTest::EVPTest(const std::string& name): CppUnit::TestCase(name)
{
}


EVPTest::~EVPTest()
{
}


void EVPTest::testRSAEVPPKey()
{
	try
	{
		std::unique_ptr<RSAKey> key(new RSAKey(RSAKey::KL_1024, RSAKey::EXP_SMALL));
		assertTrue(key->type() == Poco::Crypto::KeyPair::KT_RSA);
		// construct EVPPKey from RSAKey*
		EVPPKey* pKey = new EVPPKey(key.get());
		// EVPPKey increments reference count, so freeing the original must be ok
		key.reset();

		assertTrue (!pKey->isSupported(0));
		assertTrue (!pKey->isSupported(-1));
		assertTrue (pKey->isSupported(pKey->type()));
		assertTrue (pKey->type() == EVP_PKEY_RSA);

		// construct RSAKey from const EVPPKey&
		key.reset(new RSAKey(*pKey));
		delete pKey;
		assertTrue(key->type() == Poco::Crypto::KeyPair::KT_RSA);
		// construct EVPPKey from RSAKey*
		pKey = new EVPPKey(key.get());
		assertTrue (pKey->type() == EVP_PKEY_RSA);

		BIO* bioPriv1 = BIO_new(BIO_s_mem());
		BIO* bioPub1 = BIO_new(BIO_s_mem());
		assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv1, *pKey, NULL, NULL, 0, 0, NULL));
		assertTrue (0 != PEM_write_bio_PUBKEY(bioPub1, *pKey));
		char* pPrivData1;
		long sizePriv1 = BIO_get_mem_data(bioPriv1, &pPrivData1);
		char* pPubData1;
		long sizePub1 = BIO_get_mem_data(bioPub1, &pPubData1);

		// construct EVPPKey from EVP_PKEY*
		EVPPKey evpPKey(pKey->operator EVP_PKEY*());
		// EVPPKey makes duplicate, so freeing the original must be ok
		delete pKey;
		assertTrue (evpPKey.type() == EVP_PKEY_RSA);

		BIO* bioPriv2 = BIO_new(BIO_s_mem());
		BIO* bioPub2 = BIO_new(BIO_s_mem());
		assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey, NULL, NULL, 0, 0, NULL));
		assertTrue (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey));
		char* pPrivData2;
		long sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
		char* pPubData2;
		long sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

		assertTrue (sizePriv1 && (sizePriv1 == sizePriv2));
		assertTrue (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
		assertTrue (sizePub1 && (sizePub1 == sizePub2));
		assertTrue (0 == memcmp(pPubData1, pPubData2, sizePub1));

		BIO_free(bioPub2);
		BIO_free(bioPriv2);

		// copy
		EVPPKey evpPKey2(evpPKey);
		assertTrue (evpPKey2.type() == EVP_PKEY_RSA);
		bioPriv2 = BIO_new(BIO_s_mem());
		bioPub2 = BIO_new(BIO_s_mem());
		assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey2, NULL, NULL, 0, 0, NULL));
		assertTrue (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey2));
		sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
		sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

		assertTrue (sizePriv1 && (sizePriv1 == sizePriv2));
		assertTrue (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
		assertTrue (sizePub1 && (sizePub1 == sizePub2));
		assertTrue (0 == memcmp(pPubData1, pPubData2, sizePub1));

		BIO_free(bioPub2);
		BIO_free(bioPriv2);

		// move
		EVPPKey evpPKey3(std::move(evpPKey2));
		assertTrue (evpPKey3.type() == EVP_PKEY_RSA);
		bioPriv2 = BIO_new(BIO_s_mem());
		bioPub2 = BIO_new(BIO_s_mem());
		assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey3, NULL, NULL, 0, 0, NULL));
		assertTrue (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey3));
		sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
		sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

		assertTrue (sizePriv1 && (sizePriv1 == sizePriv2));
		assertTrue (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
		assertTrue (sizePub1 && (sizePub1 == sizePub2));
		assertTrue (0 == memcmp(pPubData1, pPubData2, sizePub1));

		BIO_free(bioPub2);
		BIO_free(bioPriv2);
		BIO_free(bioPub1);
		BIO_free(bioPriv1);
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void EVPTest::testRSAEVPSaveLoadStream()
{
	RSAKey rsaKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey key(&rsaKey);
	std::ostringstream strPub;
	std::ostringstream strPriv;
	key.save(&strPub, &strPriv, "testpwd");
	std::string pubKey = strPub.str();
	std::string privKey = strPriv.str();

	// now do the round trip
	std::istringstream iPub(pubKey);
	std::istringstream iPriv(privKey);
	EVPPKey key2(&iPub, &iPriv, "testpwd");

	assertTrue (key == key2);
	assertTrue (!(key != key2));
	RSAKey rsaKeyNE(RSAKey::KL_1024, RSAKey::EXP_LARGE);
	EVPPKey keyNE(&rsaKeyNE);
	assertTrue (key != keyNE);
	assertTrue (!(key == keyNE));
	assertTrue (key2 != keyNE);;
	assertTrue (!(key2 == keyNE));

	std::ostringstream strPub2;
	std::ostringstream strPriv2;
	key2.save(&strPub2, &strPriv2, "testpwd");
	assertTrue (strPub2.str() == pubKey);

	std::istringstream iPriv2(strPriv2.str());
	EVPPKey key3(0, &iPriv2,  "testpwd");
	std::ostringstream strPub3;
	key3.save(&strPub3);
	assertTrue (strPub3.str() == pubKey);
}


void EVPTest::testRSAEVPSaveLoadStreamNoPass()
{
	RSAKey rsaKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey key(&rsaKey);
	std::ostringstream strPub;
	std::ostringstream strPriv;
	key.save(&strPub, &strPriv);
	std::string pubKey = strPub.str();
	std::string privKey = strPriv.str();

	// now do the round trip
	std::istringstream iPub(pubKey);
	std::istringstream iPriv(privKey);
	EVPPKey key2(&iPub, &iPriv);

	assertTrue (key == key2);
	assertTrue (!(key != key2));
	RSAKey rsaKeyNE(RSAKey::KL_1024, RSAKey::EXP_LARGE);
	EVPPKey keyNE(&rsaKeyNE);
	assertTrue (key != keyNE);
	assertTrue (!(key == keyNE));
	assertTrue (key2 != keyNE);;
	assertTrue (!(key2 == keyNE));

	std::istringstream iPriv2(privKey);
	EVPPKey key3(0, &iPriv2);
	std::ostringstream strPub3;
	key3.save(&strPub3);
	std::string pubFromPrivate = strPub3.str();
	assertTrue (pubFromPrivate == pubKey);
}


void EVPTest::testECEVPPKey()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			EVPPKey* pKey = new EVPPKey(curveName);
			assertTrue (pKey != 0);
			assertTrue (!pKey->isSupported(0));
			assertTrue (!pKey->isSupported(-1));
			assertTrue (pKey->isSupported(pKey->type()));
			assertTrue (pKey->type() == EVP_PKEY_EC);

			BIO* bioPriv1 = BIO_new(BIO_s_mem());
			BIO* bioPub1 = BIO_new(BIO_s_mem());
			assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv1, *pKey, NULL, NULL, 0, 0, NULL));
			assertTrue (0 != PEM_write_bio_PUBKEY(bioPub1, *pKey));
			char* pPrivData1;
			long sizePriv1 = BIO_get_mem_data(bioPriv1, &pPrivData1);
			char* pPubData1;
			long sizePub1 = BIO_get_mem_data(bioPub1, &pPubData1);

			// construct EVPPKey from EVP_PKEY*
			EVPPKey evpPKey(pKey->operator EVP_PKEY*());
			assertTrue (evpPKey.type() == EVP_PKEY_EC);
			// EVPPKey makes duplicate, so freeing the original must be ok
			delete pKey;

			BIO* bioPriv2 = BIO_new(BIO_s_mem());
			BIO* bioPub2 = BIO_new(BIO_s_mem());
			assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey, NULL, NULL, 0, 0, NULL));
			assertTrue (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey));
			char* pPrivData2;
			long sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
			char* pPubData2;
			long sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

			assertTrue (sizePriv1 && (sizePriv1 == sizePriv2));
			assertTrue (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
			assertTrue (sizePub1 && (sizePub1 == sizePub2));
			assertTrue (0 == memcmp(pPubData1, pPubData2, sizePub1));

			BIO_free(bioPub2);
			BIO_free(bioPriv2);

			// copy
			EVPPKey evpPKey2(evpPKey);
			assertTrue (evpPKey2.type() == EVP_PKEY_EC);
			bioPriv2 = BIO_new(BIO_s_mem());
			bioPub2 = BIO_new(BIO_s_mem());
			assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey2, NULL, NULL, 0, 0, NULL));
			assertTrue (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey2));
			sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
			sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

			assertTrue (sizePriv1 && (sizePriv1 == sizePriv2));
			assertTrue (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
			assertTrue (sizePub1 && (sizePub1 == sizePub2));
			assertTrue (0 == memcmp(pPubData1, pPubData2, sizePub1));

			BIO_free(bioPub2);
			BIO_free(bioPriv2);

			// move
			EVPPKey evpPKey3(std::move(evpPKey2));
			assertTrue (evpPKey3.type() == EVP_PKEY_EC);
			bioPriv2 = BIO_new(BIO_s_mem());
			bioPub2 = BIO_new(BIO_s_mem());
			assertTrue (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey3, NULL, NULL, 0, 0, NULL));
			assertTrue (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey3));
			sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
			sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

			assertTrue (sizePriv1 && (sizePriv1 == sizePriv2));
			assertTrue (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
			assertTrue (sizePub1 && (sizePub1 == sizePub2));
			assertTrue (0 == memcmp(pPubData1, pPubData2, sizePub1));

			BIO_free(bioPub2);
			BIO_free(bioPriv2);
			BIO_free(bioPub1);
			BIO_free(bioPriv1);
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


void EVPTest::testECEVPSaveLoadStream()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			EVPPKey key(curveName);
			std::ostringstream strPub;
			std::ostringstream strPriv;
			key.save(&strPub, &strPriv, "testpwd");
			std::string pubKey = strPub.str();
			std::string privKey = strPriv.str();

			// now do the round trip
			std::istringstream iPub(pubKey);
			std::istringstream iPriv(privKey);
			EVPPKey key2(&iPub, &iPriv, "testpwd");

			std::ostringstream strPubE;
			std::ostringstream strPrivE;
			key2.save(&strPubE, &strPrivE, "testpwd");
			assertTrue (strPubE.str() == pubKey);
			assertTrue (key == key2);
			assertTrue (!(key != key2));
			ECKey ecKeyNE(curveName);
			EVPPKey keyNE(&ecKeyNE);
			assertTrue (key != keyNE);
			assertTrue (!(key == keyNE));
			assertTrue (key2 != keyNE);
			assertTrue (!(key2 == keyNE));

			std::ostringstream strPub2;
			std::ostringstream strPriv2;
			key2.save(&strPub2, &strPriv2, "testpwd");
			assertTrue (strPub2.str() == pubKey);

			std::istringstream iPriv2(strPriv2.str());
			EVPPKey key3(0, &iPriv2,  "testpwd");
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


void EVPTest::testECEVPSaveLoadStreamNoPass()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			EVPPKey key(curveName);
			std::ostringstream strPub;
			std::ostringstream strPriv;
			key.save(&strPub, &strPriv);
			std::string pubKey = strPub.str();
			std::string privKey = strPriv.str();

			// now do the round trip
			std::istringstream iPub(pubKey);
			std::istringstream iPriv(privKey);
			EVPPKey key2(&iPub, &iPriv);

			std::ostringstream strPubE;
			std::ostringstream strPrivE;
			key2.save(&strPubE, &strPrivE);
			assertTrue (strPubE.str() == pubKey);
			assertTrue (key == key2);
			assertTrue (!(key != key2));
			ECKey ecKeyNE(curveName);
			EVPPKey keyNE(&ecKeyNE);
			assertTrue (key != keyNE);
			assertTrue (!(key == keyNE));
			assertTrue (key2 != keyNE);
			assertTrue (!(key2 == keyNE));

			std::ostringstream strPub2;
			std::ostringstream strPriv2;
			key2.save(&strPub2, &strPriv2);
			assertTrue (strPub2.str() == pubKey);
			assertTrue (strPriv2.str() == privKey);

			std::istringstream iPriv2(privKey);
			EVPPKey key3(0, &iPriv2);
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


void EVPTest::testECEVPSaveLoadFile()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			EVPPKey key(curveName);
			TemporaryFile filePub;
			TemporaryFile filePriv;
			key.save(filePub.path(), filePriv.path(), "testpwd");
			std::ifstream ifPub(filePub.path().c_str());
			std::ifstream ifPriv(filePriv.path().c_str());
			std::string pubKey;
			std::string privKey;
			StreamCopier::copyToString(ifPub, pubKey);
			StreamCopier::copyToString(ifPriv, privKey);

			EVPPKey key2(filePub.path(), filePriv.path(), "testpwd");

			std::ostringstream strPubE;
			std::ostringstream strPrivE;
			key2.save(&strPubE, &strPrivE, "testpwd");
			assertTrue (strPubE.str() == pubKey);
			assertTrue (key == key2);
			assertTrue (!(key != key2));
			ECKey ecKeyNE(curveName);
			EVPPKey keyNE(&ecKeyNE);
			assertTrue (key != keyNE);
			assertTrue (!(key == keyNE));
			assertTrue (key2 != keyNE);
			assertTrue (!(key2 == keyNE));

			std::ostringstream strPub2;
			std::ostringstream strPriv2;
			key2.save(&strPub2, &strPriv2, "testpwd");
			assertTrue (strPub2.str() == pubKey);

			EVPPKey key3("", filePriv.path(),  "testpwd");
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


void EVPTest::testECEVPSaveLoadFileNoPass()
{
	try
	{
		std::string curveName = ECKey::getCurveName();
		if (!curveName.empty())
		{
			EVPPKey key(curveName);
			TemporaryFile filePub;
			TemporaryFile filePriv;
			key.save(filePub.path(), filePriv.path());
			std::ifstream ifPub(filePub.path().c_str());
			std::ifstream ifPriv(filePriv.path().c_str());
			std::string pubKey;
			std::string privKey;
			StreamCopier::copyToString(ifPub, pubKey);
			StreamCopier::copyToString(ifPriv, privKey);

			EVPPKey key2(filePub.path(), filePriv.path());
			std::ostringstream strPub2;
			std::ostringstream strPriv2;
			key2.save(&strPub2, &strPriv2);
			assertTrue (strPub2.str() == pubKey);

			EVPPKey key3("", filePriv.path());
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


void EVPTest::testRSAEVPKeyFromX509()
{
	std::istringstream str(anyPemRSA);
	X509Certificate cert(str);
	EVPPKey publicKey(cert);
	std::istringstream str2(anyPemRSA);
	EVPPKey privateKey(0, &str2, "test");
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(publicKey);
	Cipher::Ptr pCipher2 = CipherFactory::defaultFactory().createCipher(privateKey);
	std::string val("lets do some encryption");
	std::string enc = pCipher->encryptString(val);
	std::string dec = pCipher2->decryptString(enc);
	assertTrue (dec == val);
}


void EVPTest::testRSAEVPKeyFromPKCS12()
{
	std::string file = PKCS12ContainerTest::getTestFilesPath("full");
	PKCS12Container pkcs12(file.c_str(), "crypto");
	assertTrue (pkcs12.hasKey());
	EVPPKey pKey(pkcs12);
	assertTrue (EVP_PKEY_RSA == pKey.type());
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(pKey);
	Cipher::Ptr pCipher2 = CipherFactory::defaultFactory().createCipher(pKey);
	std::string val("lets do some encryption");
	std::string enc = pCipher->encryptString(val);
	std::string dec = pCipher2->decryptString(enc);
	assertTrue (dec == val);
}


#if OPENSSL_VERSION_NUMBER >= 0x10000000L

void EVPTest::testRSAEVPKeyByLength()
{
	try
	{
		EVPPKey key(EVP_PKEY_RSA, 1024);
		std::ostringstream strPub;
		std::ostringstream strPriv;
		key.save(&strPub, &strPriv);
		std::string pubKey = strPub.str();
		std::string privKey = strPriv.str();

		// now do the round trip
		std::istringstream iPub(pubKey);
		std::istringstream iPriv(privKey);
		EVPPKey key2(&iPub, &iPriv);

		assertTrue (key == key2);
		assertTrue (!(key != key2));

		EVPPKey keyNE(EVP_PKEY_RSA, 2048);
		assertTrue (key != keyNE);
		assertTrue (!(key == keyNE));
		assertTrue (key2 != keyNE);;
		assertTrue (!(key2 == keyNE));

		std::istringstream iPriv2(privKey);
		EVPPKey key3(0, &iPriv2);
		std::ostringstream strPub3;
		key3.save(&strPub3);
		std::string pubFromPrivate = strPub3.str();
		assertTrue (pubFromPrivate == pubKey);
	}
	catch(Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}

#if OPENSSL_VERSION_NUMBER >= 0x30000000L

void EVPTest::testECEVPKeyByLength()
{
	try
	{
		EVPPKey key(EVP_PKEY_EC, NID_X9_62_prime256v1);
		std::ostringstream strPub;
		std::ostringstream strPriv;
		key.save(&strPub, &strPriv);
		std::string pubKey = strPub.str();
		std::string privKey = strPriv.str();

		// now do the round trip
		std::istringstream iPub(pubKey);
		std::istringstream iPriv(privKey);
		EVPPKey key2(&iPub, &iPriv);

		assertTrue (key == key2);
		assertTrue (!(key != key2));

		EVPPKey keyNE(EVP_PKEY_EC, NID_X9_62_prime256v1);
		assertTrue (key != keyNE);
		assertTrue (!(key == keyNE));
		assertTrue (key2 != keyNE);;
		assertTrue (!(key2 == keyNE));

		std::istringstream iPriv2(privKey);
		EVPPKey key3(0, &iPriv2);
		std::ostringstream strPub3;
		key3.save(&strPub3);
		std::string pubFromPrivate = strPub3.str();
		assertTrue (pubFromPrivate == pubKey);
	}
	catch(Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}

void EVPTest::testEVPKeyByModulus()
{
	std::string e = "AQAB";
	std::string n = "ylJgMkU_bDwCLzMlo47igdZ-AC8oUbtGJUOUHnuJdjflpim7FOxw0zXYf9m0tzND0Bt1y7MPyVtf-3rwInvdgi65CZEJ3kt5PE0g6trPbvyW6hJcVeOsQvSErj33mY6RsjndLhNE-RY36G8o603au64lTOYSb9HjzzRFo4F_faEgQ02jpEYkLIWwf7PboExDbd6NGMV0uume8YA6eB3z5BwfMMHyRZA0FcIzj6F0V-hDqBaJkWegpJsukgpfO7JDKaU5rlor7j6CdbfLaWorYTCUH3F-bXZ1ojBe0wHRGEgEZBNa46A3clgNohQuuNzf4K12NFGnEl_TIFRcLm6M0Q";
        try
	{
		unsigned long exponent = 0;
		if ((e == "AQAB") || (e == "AAEAAQ")) 
		{
			exponent = RSA_F4; //Poco::Crypto::RSAKey::Exponent::EXP_LARGE
		}
		else
		{
			std::cerr << "invalid exponent" << std::endl;
			throw;
		}
		std::string nBinary;
		std::ostringstream ofs;
		Poco::MemoryInputStream cIn(n.data(), n.length());
		Poco::Base64Decoder decoder(cIn, Poco::Base64EncodingOptions::BASE64_URL_ENCODING | Poco::Base64EncodingOptions::BASE64_NO_PADDING);
		Poco::StreamCopier::copyStream(decoder, ofs);
		nBinary=ofs.str();
		std::vector<unsigned char> public_key(nBinary.begin(), nBinary.end());
		Poco::Crypto::EVPPKey cKey(&public_key, nullptr, exponent, EVP_PKEY_RSA);
		Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(cKey);
	}
	catch(Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}	
}

#endif // OPENSSL_VERSION_NUMBER >= 0x30000000
#endif // OPENSSL_VERSION_NUMBER >= 0x10000000L


void EVPTest::setUp()
{
}


void EVPTest::tearDown()
{
}


CppUnit::Test* EVPTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EVPTest");

	CppUnit_addTest(pSuite, EVPTest, testRSAEVPPKey);
	CppUnit_addTest(pSuite, EVPTest, testRSAEVPSaveLoadStream);
	CppUnit_addTest(pSuite, EVPTest, testRSAEVPSaveLoadStreamNoPass);
	CppUnit_addTest(pSuite, EVPTest, testECEVPPKey);
	CppUnit_addTest(pSuite, EVPTest, testECEVPSaveLoadStream);
	CppUnit_addTest(pSuite, EVPTest, testECEVPSaveLoadStreamNoPass);
	CppUnit_addTest(pSuite, EVPTest, testECEVPSaveLoadFile);
	CppUnit_addTest(pSuite, EVPTest, testECEVPSaveLoadFileNoPass);
	CppUnit_addTest(pSuite, EVPTest, testRSAEVPKeyFromX509);
	CppUnit_addTest(pSuite, EVPTest, testRSAEVPKeyFromPKCS12);
#if OPENSSL_VERSION_NUMBER >= 0x10000000L
	CppUnit_addTest(pSuite, EVPTest, testRSAEVPKeyByLength);
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	CppUnit_addTest(pSuite, EVPTest, testECEVPKeyByLength);
	CppUnit_addTest(pSuite, EVPTest, testEVPKeyByModulus);
#endif // OPENSSL_VERSION_NUMBER >= 0x30000000
#endif // OPENSSL_VERSION_NUMBER >= 0x10000000L

	return pSuite;
}
