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
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/ECDSADigestEngine.h"
#include <openssl/pem.h>
#include <iostream>
#include <sstream>
#include <cstring>


using namespace Poco::Crypto;


ECTest::ECTest(const std::string& name): CppUnit::TestCase(name)
{
}


ECTest::~ECTest()
{
}


void ECTest::testEVPPKey()
{
	try
	{
		EVPPKey* pKey = new EVPPKey("secp521r1");
		assert (pKey != 0);
		assert (pKey->type() == EVP_PKEY_EC);

		BIO* bioPriv1 = BIO_new(BIO_s_mem());
		BIO* bioPub1 = BIO_new(BIO_s_mem());
		assert (0 != PEM_write_bio_PrivateKey(bioPriv1, *pKey, NULL, NULL, 0, 0, NULL));
		assert (0 != PEM_write_bio_PUBKEY(bioPub1, *pKey));
		char* pPrivData1;
		long sizePriv1 = BIO_get_mem_data(bioPriv1, &pPrivData1);
		char* pPubData1;
		long sizePub1 = BIO_get_mem_data(bioPub1, &pPubData1);

		// construct EVPPKey from EVP_PKEY*
		EVPPKey evpPKey(pKey->operator EVP_PKEY*());
		assert (evpPKey.type() == EVP_PKEY_EC);
		// EVPPKey makes duplicate, so freeing the original must be ok
		delete pKey;

		BIO* bioPriv2 = BIO_new(BIO_s_mem());
		BIO* bioPub2 = BIO_new(BIO_s_mem());
		assert (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey, NULL, NULL, 0, 0, NULL));
		assert (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey));
		char* pPrivData2;
		long sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
		char* pPubData2;
		long sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

		assert (sizePriv1 && (sizePriv1 == sizePriv2));
		assert (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
		assert (sizePub1 && (sizePub1 == sizePub2));
		assert (0 == memcmp(pPubData1, pPubData2, sizePub1));

		BIO_free(bioPub2);
		BIO_free(bioPriv2);

		// copy
		EVPPKey evpPKey2(evpPKey);
		assert (evpPKey2.type() == EVP_PKEY_EC);
		bioPriv2 = BIO_new(BIO_s_mem());
		bioPub2 = BIO_new(BIO_s_mem());
		assert (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey2, NULL, NULL, 0, 0, NULL));
		assert (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey2));
		sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
		sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

		assert (sizePriv1 && (sizePriv1 == sizePriv2));
		assert (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
		assert (sizePub1 && (sizePub1 == sizePub2));
		assert (0 == memcmp(pPubData1, pPubData2, sizePub1));

#ifdef POCO_ENABLE_CPP11

		BIO_free(bioPub2);
		BIO_free(bioPriv2);

		// move
		EVPPKey evpPKey3(std::move(evpPKey2));
		assert (evpPKey3.type() == EVP_PKEY_EC);
		bioPriv2 = BIO_new(BIO_s_mem());
		bioPub2 = BIO_new(BIO_s_mem());
		assert (0 != PEM_write_bio_PrivateKey(bioPriv2, evpPKey3, NULL, NULL, 0, 0, NULL));
		assert (0 != PEM_write_bio_PUBKEY(bioPub2, evpPKey3));
		sizePriv2 = BIO_get_mem_data(bioPriv2, &pPrivData2);
		sizePub2 = BIO_get_mem_data(bioPub2, &pPubData2);

		assert (sizePriv1 && (sizePriv1 == sizePriv2));
		assert (0 == memcmp(pPrivData1, pPrivData2, sizePriv1));
		assert (sizePub1 && (sizePub1 == sizePub2));
		assert (0 == memcmp(pPubData1, pPubData2, sizePub1));
#endif POCO_ENABLE_CPP11

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


void ECTest::testECNewKeys()
{
	try
	{
		ECKey key("secp521r1");
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
		ECKey key3(0, &iPriv2,  "testpwd");
		std::ostringstream strPub3;
		key3.save(&strPub3);
		std::string pubFromPrivate = strPub3.str();
		assert (pubFromPrivate == pubKey);
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
		ECKey key("secp521r1");
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
		assert (pubFromPrivate == pubKey);
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
		std::string msg("Test this sign message");
		ECKey key("secp521r1");
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
		assert(eng2.verify(sig));
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
		std::string msg("Test this sign message");
		std::string msgManip("Test that sign message");
		ECKey key("secp521r1");
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
		assert (!eng2.verify(sig));
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void ECTest::testECCipher()
{/*
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(ECKey(ECKey::KL_1024, ECKey::EXP_SMALL));
	for (std::size_t n = 1; n <= 1200; n++)
	{
		std::string val(n, 'x');
		std::string enc = pCipher->encryptString(val);
		std::string dec = pCipher->decryptString(enc);
		assert (dec == val);
	}
*/}


void ECTest::testECCipherLarge()
{/*
	std::vector<std::size_t> sizes;
	sizes.push_back (2047);
	sizes.push_back (2048);
	sizes.push_back (2049);
	sizes.push_back (4095);
	sizes.push_back (4096);
	sizes.push_back (4097);
	sizes.push_back (8191);
	sizes.push_back (8192);
	sizes.push_back (8193);
	sizes.push_back (16383);
	sizes.push_back (16384);
	sizes.push_back (16385);
	
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(ECKey(ECKey::KL_1024, ECKey::EXP_SMALL));
	for (std::vector<std::size_t>::const_iterator it = sizes.begin(); it != sizes.end(); ++it)
	{
		std::string val(*it, 'x');
		std::string enc = pCipher->encryptString(val);
		std::string dec = pCipher->decryptString(enc);
		assert (dec == val);
	}
*/}


void ECTest::testECCertificate()
{/*
	std::istringstream str(anyPem);
	X509Certificate cert(str);
	ECKey publicKey(cert);
	std::istringstream str2(anyPem);
	ECKey privateKey(0, &str2, "test");
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(publicKey);
	Cipher::Ptr pCipher2 = CipherFactory::defaultFactory().createCipher(privateKey);
	std::string val("lets do some encryption");
	
	std::string enc = pCipher->encryptString(val);
	std::string dec = pCipher2->decryptString(enc);
	assert (dec == val);
*/}


void ECTest::setUp()
{
}


void ECTest::tearDown()
{
}


CppUnit::Test* ECTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ECTest");

	CppUnit_addTest(pSuite, ECTest, testEVPPKey);
	CppUnit_addTest(pSuite, ECTest, testECNewKeys);
	CppUnit_addTest(pSuite, ECTest, testECNewKeysNoPassphrase);
	CppUnit_addTest(pSuite, ECTest, testECDSASignSha256);
	CppUnit_addTest(pSuite, ECTest, testECDSASignManipulated);
	CppUnit_addTest(pSuite, ECTest, testECCipher);
	CppUnit_addTest(pSuite, ECTest, testECCipherLarge);
	CppUnit_addTest(pSuite, ECTest, testECCertificate);

	return pSuite;
}
