//
// RSATest.cpp
//
// $Id: //poco/1.4/Crypto/testsuite/src/RSATest.cpp#1 $
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RSATest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/X509Certificate.h"
#include <sstream>


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


RSATest::RSATest(const std::string& name): CppUnit::TestCase(name)
{
}


RSATest::~RSATest()
{
}


void RSATest::testNewKeys()
{
	RSAKey key(RSAKey::KL_1024);
	std::ostringstream strPub;
	std::ostringstream strPriv;
	key.save(&strPub, &strPriv, "testpwd");
	std::string pubKey = strPub.str();
	std::string privKey = strPriv.str();

	// now do the round trip
	std::istringstream iPub(pubKey);
	std::istringstream iPriv(privKey);
	RSAKey key2(&iPub, &iPriv, "testpwd");
	std::ostringstream oPub;
	std::ostringstream oPriv;
	key2.save(&oPub, &oPriv, "testpwd");
	assert(oPub.str() == pubKey);
	assert(oPriv.str() == privKey);

	std::istringstream iPriv2(privKey);
	RSAKey key3(0, &iPriv2,  "testpwd");
	std::ostringstream strPub3;
	key3.save(&strPub3);
	std::string pubFromPrivate = strPub3.str();
	assert(!pubFromPrivate.empty() && pubFromPrivate == pubKey);
}


void RSATest::testNewKeysNoPassphrase()
{
	RSAKey key(RSAKey::KL_1024);
	std::ostringstream strPub;
	std::ostringstream strPriv;
	key.save(&strPub, &strPriv);
	std::string pubKey = strPub.str();
	std::string privKey = strPriv.str();

	// now do the round trip
	std::istringstream iPub(pubKey);
	std::istringstream iPriv(privKey);
	RSAKey key2(&iPub, &iPriv);
	std::ostringstream oPub;
	std::ostringstream oPriv;
	key2.save(&oPub, &oPriv);
	assert(oPub.str() == pubKey);
	assert(oPriv.str() == privKey);

	std::istringstream iPriv2(privKey);
	RSAKey key3(0, &iPriv2);
	std::ostringstream strPub3;
	key3.save(&strPub3);
	std::string pubFromPrivate = strPub3.str();
	assert(!pubFromPrivate.empty() && pubFromPrivate == pubKey);
}


void RSATest::testSign()
{
	std::string msg("Test this sign message");
	RSAKey key(RSAKey::KL_2048);
	RSADigestEngine eng(key);
	eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	// verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	RSAKey keyPub(&iPub);
	RSADigestEngine eng2(key);
	eng2.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	assert (eng2.verify(sig));
}


void RSATest::testSignManipulated()
{
	std::string msg("Test this sign message");
	std::string msgManip("Test that sign message");
	RSAKey key(RSAKey::KL_2048, RSAKey::EXP_LARGE);
	RSADigestEngine eng(key);
	eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
	const Poco::DigestEngine::Digest& sig = eng.signature();
	std::string hexDig = Poco::DigestEngine::digestToHex(sig);

	// verify
	std::ostringstream strPub;
	key.save(&strPub);
	std::string pubKey = strPub.str();
	std::istringstream iPub(pubKey);
	RSAKey keyPub(&iPub);
	RSADigestEngine eng2(key);
	eng2.update(msgManip.c_str(), static_cast<unsigned>(msgManip.length()));
	assert (!eng2.verify(sig));
}


void RSATest::testRSACipher()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(RSAKey(RSAKey::KL_1024));
	for (std::size_t n = 1; n <= 1200; n++)
	{
		std::string val(n, 'x');
		std::string enc = pCipher->encryptString(val);
		std::string dec = pCipher->decryptString(enc);
		assert (dec == val);
	}
}


void RSATest::testRSACipherLarge()
{
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
	
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(RSAKey(RSAKey::KL_1024));
	for (std::vector<std::size_t>::const_iterator it = sizes.begin(); it != sizes.end(); ++it)
	{
		std::string val(*it, 'x');
		std::string enc = pCipher->encryptString(val);
		std::string dec = pCipher->decryptString(enc);
		assert (dec == val);
	}
}


void RSATest::testCertificate()
{
	/*TODO
	std::istringstream str(anyPem);
	Poco::Crypto::X509Certificate cert(str);
	RSAKey publicKey(cert);
	std::istringstream str2(anyPem);
	RSAKey privateKey(0, &str2, "test");
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(publicKey);
	Cipher::Ptr pCipher2 = CipherFactory::defaultFactory().createCipher(privateKey);
	std::string val("lets do some encryption");
	
	std::string enc = pCipher->encryptString(val);
	std::string dec = pCipher2->decryptString(enc);
	assert (dec == val);
	*/
}


void RSATest::setUp()
{
}


void RSATest::tearDown()
{
}


CppUnit::Test* RSATest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RSATest");

	CppUnit_addTest(pSuite, RSATest, testNewKeys);
	CppUnit_addTest(pSuite, RSATest, testNewKeysNoPassphrase);
	CppUnit_addTest(pSuite, RSATest, testSign);
	CppUnit_addTest(pSuite, RSATest, testSignManipulated);
	CppUnit_addTest(pSuite, RSATest, testRSACipher);
	CppUnit_addTest(pSuite, RSATest, testRSACipherLarge);
	CppUnit_addTest(pSuite, RSATest, testCertificate);
	//CppUnit_addTest(pSuite, RSATest, testCAPIEncryptDecrypt);
	//CppUnit_addTest(pSuite, RSATest, testCAPIKeys);

	return pSuite;
}


void RSATest::testCAPIKeys()
{
	HCRYPTPROV hProv = 0;
	BOOL rc = FALSE;
	LPBYTE pPubKeyBLOB = 0;
	HCRYPTKEY hPrivateKey = 0;
	DWORD flags = 0;
	DWORD size = 0;
	PCERT_PUBLIC_KEY_INFO pKeyInfo = 0;
	HCRYPTKEY hPublicKey = 0;
	LPBYTE pPrivKeyBLOB = 0;
	LPBYTE pDER = 0;
	LPBYTE pEncDER = 0;
	DWORD derSize = 0;
	HCRYPTKEY  hSecretKey = 0;
	HCRYPTHASH hHash = 0;
	const char* pwd = "pwd";
	DWORD dwMode = 0;
	DWORD dwBlockLen = 0;
	DWORD dwDataLen = 0;
	LPBYTE pIV = 0;
	DWORD encSize = 0;
	DWORD pemSize = 0;
	LPSTR pPEM = 0;

	/* get provider */
	rc = CryptAcquireContext(&hProv,
		NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES,
		CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
	if (!rc) goto bad;

	/* generate private key */
	flags = 1024 /*key length*/ << 16;
	flags |= CRYPT_EXPORTABLE;
	rc = CryptGenKey(hProv, AT_SIGNATURE, flags, &hPrivateKey);
	if (!rc) goto bad;

	/* extport public key */
	rc = CryptExportKey(hPrivateKey, NULL, PUBLICKEYBLOB, 0, NULL, &size);
	if (!rc) goto bad;

	pPubKeyBLOB = (LPBYTE)LocalAlloc(0, size);
	rc = CryptExportKey(hPrivateKey, NULL, PUBLICKEYBLOB, 0, pPubKeyBLOB, &size);
	if (!rc) goto bad;

	rc = CryptImportKey(hProv, pPubKeyBLOB, size, 0, CRYPT_EXPORTABLE, &hPublicKey);
	if (!rc) goto bad;

	/* export private key */
	rc = CryptExportKey(hPrivateKey, 0, PRIVATEKEYBLOB, 0, 0, &size);
	if (!rc) goto bad;

	pPrivKeyBLOB = (LPBYTE)LocalAlloc(0, size);
	rc = CryptExportKey(hPrivateKey, 0, PRIVATEKEYBLOB, 0,
		pPrivKeyBLOB, &size);
	if (!rc) goto bad;

	/* save private key as PEM */

	/* DER */
	rc = CryptEncodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		PKCS_RSA_PRIVATE_KEY, pPrivKeyBLOB, 0, NULL, NULL, &size);
	if (!rc) goto bad;

	pDER = (LPBYTE)LocalAlloc(0, size);
	rc = CryptEncodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		PKCS_RSA_PRIVATE_KEY, pPrivKeyBLOB, 0, NULL, pDER, &size);
	if (!rc) goto bad;

	/* encrypt */
	rc = CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash);
	if (!rc) goto bad;

	rc = CryptHashData(hHash, (LPBYTE)pwd, 3, 0);
	if (!rc) goto bad;

	rc = CryptDeriveKey(hProv, CALG_3DES, hHash, 0, &hSecretKey);
	if (!rc) goto bad;

	dwMode = CRYPT_MODE_CBC;
	rc = CryptSetKeyParam(hSecretKey, KP_MODE, reinterpret_cast<LPBYTE>(&dwMode), 0);
	if (!rc) goto bad;

	dwDataLen = sizeof(dwBlockLen);
	rc = CryptGetKeyParam(hSecretKey, KP_BLOCKLEN, (BYTE *)&dwBlockLen, &dwDataLen, 0);
	if (!rc) goto bad;

	dwBlockLen /= 8;
	pIV = (LPBYTE)LocalAlloc(LMEM_FIXED, dwBlockLen);
	rc = CryptGetKeyParam(hSecretKey, KP_IV, pIV, &dwBlockLen, 0);
	if (!rc) goto bad;

	rc = CryptGenRandom(hProv, dwBlockLen, pIV);
	if (!rc) goto bad;

	rc = CryptSetKeyParam(hSecretKey, KP_IV, pIV, 0);
	if (!rc) goto bad;

	encSize = size;
	rc = CryptEncrypt(hSecretKey, NULL, TRUE, 0, NULL, &encSize, 0);
	if (!rc) goto bad;

	pEncDER = (LPBYTE)LocalAlloc(0, encSize);
	memset(pEncDER, 0, encSize);
	memcpy(pEncDER, pDER, size);
	LocalFree(pDER);
	rc = CryptEncrypt(hSecretKey, NULL, TRUE, 0, pEncDER, &size, encSize);
	if (!rc) goto bad;
	pDER = (LPBYTE)LocalAlloc(0, size);
	memcpy(pDER, pEncDER, size);
	/* PEM */
	rc = CryptBinaryToStringA(pDER, encSize,
		CRYPT_STRING_BASE64HEADER, NULL, &pemSize);
	if (!rc) goto bad;

	pPEM = (LPSTR)LocalAlloc(0, pemSize);
	rc = CryptBinaryToStringA(pDER, encSize,
		CRYPT_STRING_BASE64HEADER, pPEM, &pemSize);
	if (!rc) goto bad;

	/*************************************************************/
	/* start fresh and import both keys from the private key PEM */
	/*************************************************************/

	CryptDestroyKey(hPublicKey);
	CryptDestroyKey(hPrivateKey);
	CryptDestroyKey(hSecretKey);
	CryptReleaseContext(hProv, 0);

	rc = CryptAcquireContext(&hProv,
		NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES,
		CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
	if (!rc) goto bad;

	/* back to DER */
	rc = CryptStringToBinaryA(pPEM, pemSize,
		CRYPT_STRING_BASE64HEADER, NULL, &size, NULL, NULL);
	if (!rc) goto bad;

	LocalFree(pDER);
	pDER = (LPBYTE)LocalAlloc(0, size);
	rc = CryptStringToBinaryA(pPEM, pemSize,
		CRYPT_STRING_BASE64HEADER, pDER, &size, NULL, NULL);
	if (!rc) goto bad;

	/* decrypt */
	rc = CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash);
	if (!rc) goto bad;

	rc = CryptHashData(hHash, (LPBYTE)pwd, 3, 0);
	if (!rc) goto bad;

	rc = CryptDeriveKey(hProv, CALG_3DES, hHash, 0, &hSecretKey);
	if (!rc) goto bad;

	dwMode = CRYPT_MODE_CBC;
	rc = CryptSetKeyParam(hSecretKey, KP_MODE, reinterpret_cast<LPBYTE>(&dwMode), 0);
	if (!rc) goto bad;

	// reuse IV
	rc = CryptSetKeyParam(hSecretKey, KP_IV, pIV, 0);
	if (!rc) goto bad;

	rc = CryptDecrypt(hSecretKey, NULL, TRUE, 0, pDER, &size);
	if (!rc) goto bad;

	/* get private key blob */
	rc = CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		PKCS_RSA_PRIVATE_KEY, pDER, size, 0, NULL, NULL, &derSize);
	if (!rc) goto bad;

	LocalFree(pPrivKeyBLOB);
	pPrivKeyBLOB = (LPBYTE)LocalAlloc(0, derSize);
	rc = CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		PKCS_RSA_PRIVATE_KEY, pDER, size, 0, NULL, pPrivKeyBLOB, &derSize);
	if (!rc) goto bad;

	/* import private key */
	rc = CryptImportKey(hProv, pPrivKeyBLOB, size, 0, CRYPT_EXPORTABLE, &hPrivateKey);
	if (!rc) goto bad;

	/* export public key */
	rc = CryptExportKey(hPrivateKey, 0, PUBLICKEYBLOB, 0, 0, &size);
	if (!rc) goto bad;

	pPubKeyBLOB = (LPBYTE)LocalAlloc(0, size);
	rc = CryptExportKey(hPrivateKey, 0, PUBLICKEYBLOB, 0, pPubKeyBLOB, &size);
	if (!rc) goto bad;

	/* import public key */
	rc = CryptImportKey(hProv, pPubKeyBLOB, size, 0, CRYPT_EXPORTABLE, &hPublicKey);
	if (!rc) goto bad;

	/* import public key */
	rc = CryptExportPublicKeyInfo(hProv,
		AT_KEYEXCHANGE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		NULL, &size);
	if (!rc) goto bad;

	LocalFree(pKeyInfo);
	pKeyInfo = (PCERT_PUBLIC_KEY_INFO)LocalAlloc(0, size);
	rc = CryptExportPublicKeyInfo(hProv,
		AT_KEYEXCHANGE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		pKeyInfo, &size);
	if (!rc) goto bad;

	hPublicKey = 0;
	rc = CryptImportPublicKeyInfo(hProv,
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		pKeyInfo, &hPublicKey);
	if (!rc) goto bad;

	/* clean up */
	if (hPublicKey) CryptDestroyKey(hPublicKey);
	if (hPrivateKey) CryptDestroyKey(hPrivateKey);
	if (hProv) CryptReleaseContext(hProv, 0);

	LocalFree(pPEM);
	LocalFree(pDER);
	LocalFree(pEncDER);
	LocalFree(pPrivKeyBLOB);
	LocalFree(pPubKeyBLOB);
	LocalFree(pKeyInfo);
	LocalFree(pIV);

	return;

bad:
	DWORD errorCode = GetLastError();
	DWORD dwFlg = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS;
	LPTSTR lpMsgBuf = 0;
	FormatMessageA(dwFlg, 0, errorCode, 0, (LPTSTR)& lpMsgBuf, 0, NULL);
	printf("%s", lpMsgBuf);
	LocalFree(lpMsgBuf);

	if (hPublicKey) CryptDestroyKey(hPublicKey);
	if (hPrivateKey) CryptDestroyKey(hPrivateKey);
	if (hProv) CryptReleaseContext(hProv, 0);

	LocalFree(pPEM);
	LocalFree(pDER);
	LocalFree(pEncDER);
	LocalFree(pPrivKeyBLOB);
	LocalFree(pPubKeyBLOB);
	LocalFree(pKeyInfo);
	LocalFree(pIV);
}


void RSATest::testCAPIEncryptDecrypt()
{
	HCRYPTPROV hProv = 0;
	BOOL rc = FALSE;
	LPBYTE pPrivKeyBLOB = 0;
	LPBYTE pPubKeyBLOB = 0;
	HCRYPTKEY hSessionKey = 0;
	HCRYPTKEY hPrivateKey = 0;
	DWORD flags = 0;
	DWORD size = 0;
	PCERT_PUBLIC_KEY_INFO pKeyInfo = 0;
	HCRYPTKEY hPublicKey = 0;

	Poco::Buffer<BYTE> data(1);
	data[0] = 'x';

	/* get provider */
	rc = CryptAcquireContext(&hProv,
		NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES,
		CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
	if (!rc) goto bad;

	/* generate private key */
	flags = 1024 /*key length*/ << 16;
	flags |= CRYPT_EXPORTABLE;
	rc = CryptGenKey(hProv, AT_KEYEXCHANGE/*AT_SIGNATURE*/, flags, &hSessionKey);
	if (!rc) goto bad;

	/* extract public key */
	rc = CryptExportKey(hSessionKey, NULL, PUBLICKEYBLOB, 0, NULL, &size);
	if (!rc) goto bad;

	pPubKeyBLOB = (LPBYTE)LocalAlloc(0, size);
	rc = CryptExportKey(hSessionKey, NULL, PUBLICKEYBLOB, 0, pPubKeyBLOB, &size);
	if (!rc) goto bad;

	rc = CryptImportKey(hProv, pPubKeyBLOB, size, 0, CRYPT_EXPORTABLE, &hPublicKey);
	if (!rc) goto bad;

	/* extract private key */
	rc = CryptExportKey(hSessionKey, NULL, PRIVATEKEYBLOB, 0, NULL, &size);
	if (!rc) goto bad;

	pPrivKeyBLOB = (LPBYTE)LocalAlloc(0, size);
	rc = CryptExportKey(hSessionKey, NULL, PRIVATEKEYBLOB, 0, pPrivKeyBLOB, &size);
	if (!rc) goto bad;

	rc = CryptImportKey(hProv, pPrivKeyBLOB, size, 0, CRYPT_EXPORTABLE, &hPrivateKey);
	if (!rc) goto bad;

	DWORD dataSize = static_cast<DWORD>(data.size());
	DWORD n = dataSize;
	rc = CryptEncrypt(hPublicKey, NULL, TRUE, 0, NULL, &n, 0);
	if (!rc) goto bad;

	data.resize(n, true);
	n = dataSize;
	rc = CryptEncrypt(hPublicKey, NULL, TRUE, 0, data.begin(), &n, static_cast<DWORD>(data.size()));
	if (!rc) goto bad;

	n = static_cast<DWORD>(data.size());
	rc = CryptDecrypt(hPrivateKey, NULL, TRUE, 0, data.begin(), &n);
	if (!rc) goto bad;

	CryptDestroyKey(hPublicKey);
	CryptDestroyKey(hPrivateKey);
	CryptReleaseContext(hProv, 0);

	LocalFree(pPubKeyBLOB);
	LocalFree(pPrivKeyBLOB);

	return;

bad:
	DWORD errorCode = GetLastError();
	DWORD dwFlg = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS;
	LPTSTR lpMsgBuf = 0;
	FormatMessageA(dwFlg, 0, errorCode, 0, (LPTSTR)& lpMsgBuf, 0, NULL);
	printf("%s", lpMsgBuf);
	LocalFree(lpMsgBuf);

	if (hPublicKey) CryptDestroyKey(hPublicKey);
	if (hPrivateKey) CryptDestroyKey(hPrivateKey);
	if (hProv) CryptReleaseContext(hProv, 0);

	LocalFree(pPubKeyBLOB);
	LocalFree(pPrivKeyBLOB);

	return;
}