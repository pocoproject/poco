//
// ECTest.cpp
//
// $Id: //poco/1.4/Crypto/testsuite/src/ECTest.cpp#1 $
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ECTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include <openssl/pem.h>
#include <sstream>
#include <cstring>


using namespace Poco::Crypto;


static const std::string anyPem(
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
	"-----BEGIN EC PRIVATE KEY-----\r\n"
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
	"-----END EC PRIVATE KEY-----\r\n"
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


ECTest::ECTest(const std::string& name): CppUnit::TestCase(name)
{
}


ECTest::~ECTest()
{
}


void ECTest::testEVPPKey()
{
	int eccGroup = OBJ_txt2nid("secp521r1");
	EC_KEY* pEC = EC_KEY_new_by_curve_name(eccGroup);
	assert (pEC != 0);
	assert (0 != EC_KEY_generate_key(pEC));
	EVP_PKEY* pKey = EVP_PKEY_new();
	assert (pKey != 0);
	assert (0 != EVP_PKEY_set1_EC_KEY(pKey, pEC));
	EC_KEY_free(pEC);

	BIO* bioPriv1 = BIO_new(BIO_s_mem());
	BIO* bioPub1 = BIO_new(BIO_s_mem());
	assert (0 != PEM_write_bio_PrivateKey(bioPriv1, pKey, NULL, NULL, 0, 0, NULL));
	assert (0 != PEM_write_bio_PUBKEY(bioPub1, pKey));
	char* pPrivData1;
	long sizePriv1 = BIO_get_mem_data(bioPriv1, &pPrivData1);
	char* pPubData1;
	long sizePub1 = BIO_get_mem_data(bioPub1, &pPubData1);

	// construct EVPPKey from EVP_PKEY*
	EVPPKey evpPKey(pKey);
	// EVPPKey makes duplicate, so freeing the original must be ok
	EVP_PKEY_free(pKey);

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

	BIO_free(bioPub2);
	BIO_free(bioPriv2);

	// move
	EVPPKey evpPKey3(std::move(evpPKey2));
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

	BIO_free(bioPub2);
	BIO_free(bioPriv2);
	BIO_free(bioPub1);
	BIO_free(bioPriv1);
}


void ECTest::testECNewKeys()
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


void ECTest::testECNewKeysNoPassphrase()
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


void ECTest::testECSign()
{/*
	std::string msg("Test this sign message");
	ECKey key(ECKey::KL_2048, ECKey::EXP_LARGE);
	ECDigestEngine eng(key);
	eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	// verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	ECKey keyPub(&iPub);
	ECDigestEngine eng2(keyPub);
	eng2.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	assert (eng2.verify(sig));
*/}


void ECTest::testECSignSha256()
{/*
	std::string msg("Test this sign message");
	ECKey key(ECKey::KL_2048, ECKey::EXP_LARGE);
	ECDigestEngine eng(key, "SHA256");
	eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	// verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	ECKey keyPub(&iPub);
	ECDigestEngine eng2(keyPub, "SHA256");
	eng2.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	assert (eng2.verify(sig));
*/}


void ECTest::testECSignManipulated()
{/*
	std::string msg("Test this sign message");
	std::string msgManip("Test that sign message");
	ECKey key(ECKey::KL_2048, ECKey::EXP_LARGE);
	ECDigestEngine eng(key);
	eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	// verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	ECKey keyPub(&iPub);
	ECDigestEngine eng2(keyPub);
	eng2.update(msgManip.c_str(), static_cast<unsigned>(msgManip.length()));
	assert (!eng2.verify(sig));
*/}


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
	CppUnit_addTest(pSuite, ECTest, testECSign);
	CppUnit_addTest(pSuite, ECTest, testECSignSha256);
	CppUnit_addTest(pSuite, ECTest, testECSignManipulated);
	CppUnit_addTest(pSuite, ECTest, testECCipher);
	CppUnit_addTest(pSuite, ECTest, testECCipherLarge);
	CppUnit_addTest(pSuite, ECTest, testECCertificate);

	return pSuite;
}
