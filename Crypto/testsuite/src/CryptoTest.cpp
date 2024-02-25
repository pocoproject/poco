//
// CryptoTest.cpp
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CryptoTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/CryptoStream.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/StreamCopier.h"
#include "Poco/Base64Encoder.h"
#include "Poco/HexBinaryEncoder.h"
#include <sstream>


using namespace Poco::Crypto;


static const std::string APPINF_PEM(
	"-----BEGIN CERTIFICATE-----\n"
	"MIIESzCCAzOgAwIBAgIBATALBgkqhkiG9w0BAQUwgdMxEzARBgNVBAMMCmFwcGlu\n"
	"Zi5jb20xNjA0BgNVBAoMLUFwcGxpZWQgSW5mb3JtYXRpY3MgU29mdHdhcmUgRW5n\n"
	"aW5lZXJpbmcgR21iSDEUMBIGA1UECwwLRGV2ZWxvcG1lbnQxEjAQBgNVBAgMCUNh\n"
	"cmludGhpYTELMAkGA1UEBhMCQVQxHjAcBgNVBAcMFVN0LiBKYWtvYiBpbSBSb3Nl\n"
	"bnRhbDEtMCsGCSqGSIb3DQEJARYeZ3VlbnRlci5vYmlsdHNjaG5pZ0BhcHBpbmYu\n"
	"Y29tMB4XDTA5MDUwNzE0NTY1NloXDTI5MDUwMjE0NTY1NlowgdMxEzARBgNVBAMM\n"
	"CmFwcGluZi5jb20xNjA0BgNVBAoMLUFwcGxpZWQgSW5mb3JtYXRpY3MgU29mdHdh\n"
	"cmUgRW5naW5lZXJpbmcgR21iSDEUMBIGA1UECwwLRGV2ZWxvcG1lbnQxEjAQBgNV\n"
	"BAgMCUNhcmludGhpYTELMAkGA1UEBhMCQVQxHjAcBgNVBAcMFVN0LiBKYWtvYiBp\n"
	"bSBSb3NlbnRhbDEtMCsGCSqGSIb3DQEJARYeZ3VlbnRlci5vYmlsdHNjaG5pZ0Bh\n"
	"cHBpbmYuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA89GolWCR\n"
	"KtLQclJ2M2QtpFqzNC54hUQdR6n8+DAeruH9WFwLSdWW2fEi+jrtd/WEWCdt4PxX\n"
	"F2/eBYeURus7Hg2ZtJGDd3je0+Ygsv7+we4cMN/knaBY7rATqhmnZWk+yBpkf5F2\n"
	"IHp9gBxUaJWmt/bq3XrvTtzrDXpCd4zg4zPXZ8IC8ket5o3K2vnkAOsIsgN+Ffqd\n"
	"4GjF4dsblG6u6E3VarGRLwGtgB8BAZOA/33mV4FHSMkc4OXpAChaK3tM8YhrLw+m\n"
	"XtsfqDiv1825S6OWFCKGj/iX8X2QAkrdB63vXCSpb3de/ByIUfp31PpMlMh6dKo1\n"
	"vf7yj0nb2w0utQIDAQABoyowKDAOBgNVHQ8BAf8EBAMCB4AwFgYDVR0lAQH/BAww\n"
	"CgYIKwYBBQUHAwMwDQYJKoZIhvcNAQEFBQADggEBAM0cpfb4BgiU/rkYe121P581\n"
	"ftg5Ck1PYYda1Fy/FgzbgJh2AwVo/6sn6GF79/QkEcWEgtCMNNO3LMTTddUUApuP\n"
	"jnEimyfmUhIThyud/vryzTMNa/eZMwaAqUQWqLf+AwgqjUsBSMenbSHavzJOpsvR\n"
	"LI0PQ1VvqB+3UGz0JUnBJiKvHs83Fdm4ewPAf3M5fGcIa+Fl2nU5Plzwzskj84f6\n"
	"73ZlEEi3aW9JieNy7RWsMM+1E8Sj2CGRZC4BM9V1Fgnsh4+VHX8Eu7eHucvfeIYx\n"
	"3mmLMoK4sCayL/FGhrUDw5AkWb8tKNpRXY+W60Et281yxQSeWLPIbatVzIWI0/M=\n"
	"-----END CERTIFICATE-----\n"
);

static const std::string UTF8_PEM(
	"-----BEGIN CERTIFICATE-----\n"
	"MIIDEzCCArigAwIBAgIQAKegojl/YLNUPqTyCnQ4LzAKBggqhkjOPQQDAjB0MQsw\n"
	"CQYDVQQGEwJDSDEQMA4GA1UECgwHU2llbWVuczEUMBIGA1UECwwLQlQgRGl2aXNp\n"
	"b24xPTA7BgNVBAMMNEt1cnpSUzFNUDIyc3ByaW50NTlwcmVWVlMgcHJvamVjdCBp\n"
	"bnRlcm1lZGlhdGUgQ0EgRUMwHhcNMTkxMTI3MDAwMDAwWhcNMjQxMTI4MTkzMzQw\n"
	"WjCCAQMxJDAiBgNVBAUTG1BJRDpQWEM1LkUwMDMgU046MTQwMDA0RDhFMjELMAkG\n"
	"A1UEBhMCQ0gxEDAOBgNVBAoMB1NpZW1lbnMxFzAVBgNVBAsMDlNJIEJQIERpdmlz\n"
	"aW9uMQwwCgYDVQQIDANadWcxHjAcBgNVBAcMFVrDpGhsZXJ3ZWcgNyBSb29tIDU0\n"
	"NDEnMCUGCSqGSIb3DQEJARYYcmljaGFyZC5rdXJ6QHNpZW1lbnMuY29tMRgwFgYD\n"
	"VQQLDA9TU0wgQ2VydGlmaWNhdGUxMjAwBgNVBAMMKUt1cnpSUzFNUDIyc3ByaW50\n"
	"NTlwcmVWVlMuS3VyUFhDNUJOUjM3NjUyMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcD\n"
	"QgAEJjy+wx/mN9FbW3/IoOAOXdbfQvF1gF8wNasHUeLdn1UsCABnaAZTytqX7gMD\n"
	"Y5HS32SIvdULYwsy6Dn3CO5tVKOBmjCBlzAOBgNVHQ8BAf8EBAMCA6gwIAYDVR0l\n"
	"AQH/BBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0O\n"
	"BBYEFIbtuXQJoVh7FlYiWZeWT2ooEQRNMB8GA1UdIwQYMBaAFOtUSuT1OYK7bNS4\n"
	"Mqz0UGPoGavuMBUGA1UdEQQOMAyHBAqqt7SHBMCo/AEwCgYIKoZIzj0EAwIDSQAw\n"
	"RgIhANBQnB1HFLHp7t8oZbLYsm8nWI0hshmVQupXV9oFwb4qAiEAg5UqSDnvAax3\n"
	"LWWgnAZJkUS0AEQXu4Rx9ZiP7wBdFtA=\n"
	"-----END CERTIFICATE-----\n"
);

CryptoTest::CryptoTest(const std::string& name): CppUnit::TestCase(name)
{
}


CryptoTest::~CryptoTest()
{
}


void CryptoTest::testEncryptDecrypt()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes256"));

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_NONE);
		std::string result = pCipher->decryptString(out, Cipher::ENC_NONE);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BASE64);
		std::string result = pCipher->decryptString(out, Cipher::ENC_BASE64);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BINHEX);
		std::string result = pCipher->decryptString(out, Cipher::ENC_BINHEX);
		assertTrue (in == result);
	}
}


void CryptoTest::testEncryptDecryptWithSalt()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes256", "simplepwd", "Too much salt"));
	Cipher::Ptr pCipher2 = CipherFactory::defaultFactory().createCipher(CipherKey("aes256", "simplepwd", "Too much salt"));

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_NONE);
		std::string result = pCipher2->decryptString(out, Cipher::ENC_NONE);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BASE64);
		std::string result = pCipher2->decryptString(out, Cipher::ENC_BASE64);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BINHEX);
		std::string result = pCipher2->decryptString(out, Cipher::ENC_BINHEX);
		assertTrue (in == result);
	}
}


void CryptoTest::testEncryptDecryptWithSaltSha1()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(
		CipherKey("aes256", "simplepwd", "Too much salt", 2000, "sha1"));
	Cipher::Ptr pCipher2 = CipherFactory::defaultFactory().createCipher(
		CipherKey("aes256", "simplepwd", "Too much salt", 2000, "sha1"));

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_NONE);
		std::string result = pCipher2->decryptString(out, Cipher::ENC_NONE);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BASE64);
		std::string result = pCipher2->decryptString(out, Cipher::ENC_BASE64);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BINHEX);
		std::string result = pCipher2->decryptString(out, Cipher::ENC_BINHEX);
		assertTrue (in == result);
	}
}


void CryptoTest::testEncryptDecryptDESECB()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("des-ecb", "password"));

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_NONE);
		std::string result = pCipher->decryptString(out, Cipher::ENC_NONE);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BASE64);
		std::string result = pCipher->decryptString(out, Cipher::ENC_BASE64);
		assertTrue (in == result);
	}

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::string in(n, 'x');
		std::string out = pCipher->encryptString(in, Cipher::ENC_BINHEX);
		std::string result = pCipher->decryptString(out, Cipher::ENC_BINHEX);
		assertTrue (in == result);
	}
}


void CryptoTest::testEncryptDecryptGCM()
{
	CipherKey key("aes-256-gcm");

	// 96-bit (12 byte) IV is recommended for usage with GCM.
	// https://crypto.stackexchange.com/questions/41601/aes-gcm-recommended-iv-size-why-12-bytes

	CipherKey::ByteVec iv(12, 213);
	key.setIV(iv);

	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(key);

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::stringstream str;
		CryptoTransform::Ptr pEncryptor = pCipher->createEncryptor();
		CryptoOutputStream encryptorStream(str, pEncryptor);
		std::string in(n, 'x');
		encryptorStream << in;
		encryptorStream.close();
		assertTrue (encryptorStream.good());

		std::string tag = pEncryptor->getTag();

		CryptoTransform::Ptr pDecryptor = pCipher->createDecryptor();
		pDecryptor->setTag(tag);
		CryptoInputStream decryptorStream(str, pDecryptor);
		std::string out;
		decryptorStream >> out;

		assertTrue (in == out);
	}
}


void CryptoTest::testPassword()
{
	CipherKey key("aes256", "password", "salt");

	std::ostringstream keyStream;
	Poco::Base64Encoder base64KeyEnc(keyStream);
	base64KeyEnc.write(reinterpret_cast<const char*>(&key.getKey()[0]), key.keySize());
	base64KeyEnc.close();
	std::string base64Key = keyStream.str();
	assertTrue (base64Key == "hIzxBt58GDd7/6mRp88bewKk42lM4QwaF78ek0FkVoA=");
}


void CryptoTest::testPasswordSha1()
{
	// the test uses 1 iteration, as the openssl executable does not allow to set a custom number
	// of iterations
	CipherKey key("aes256", "password", "saltsalt", 1, "sha1");

	std::ostringstream keyStream;
	Poco::HexBinaryEncoder hexKeyEnc(keyStream);
	hexKeyEnc.write(reinterpret_cast<const char*>(&key.getKey()[0]), key.keySize());
	hexKeyEnc.close();
	std::string hexKey = keyStream.str();

	std::ostringstream ivStream;
	Poco::HexBinaryEncoder hexIvEnc(ivStream);
	hexIvEnc.write(reinterpret_cast<const char*>(&key.getIV()[0]), key.ivSize());
	hexIvEnc.close();
	std::string hexIv = ivStream.str();

	// got Hex value for key and iv using:
	// openssl enc -e -a -md sha1 -aes256 -k password -S 73616c7473616c74 -P
	// (where "salt" == 73616c74 in Hex, doubled for an 8 bytes salt, openssl padds the salt with 0
	// whereas Poco's implementation padds with the existing bytes using a modulo operation)
	assertTrue (hexIv == "c96049b0edc0b67af61ecc43d3de8898");
	assertTrue (hexKey == "cab86dd6261710891e8cb56ee3625691a75df344f0bff4c12cf3596fc00b39c7");
}


void CryptoTest::testEncryptInterop()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes256", "password", "salt"));

	const std::string plainText  = "This is a secret message.";
	const std::string expectedCipherText = "9HITTPaU3A/LaZzldbdnRZ109DKlshouKren/n8BsHc=";
	std::string cipherText = pCipher->encryptString(plainText, Cipher::ENC_BASE64);
	assertTrue (cipherText == expectedCipherText);
}


void CryptoTest::testDecryptInterop()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes256", "password", "salt"));

	const std::string expectedPlainText  = "This is a secret message.";
	const std::string cipherText = "9HITTPaU3A/LaZzldbdnRZ109DKlshouKren/n8BsHc=";
	std::string plainText = pCipher->decryptString(cipherText, Cipher::ENC_BASE64);
	assertTrue (plainText == expectedPlainText);
}


void CryptoTest::testStreams()
{
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes256"));

	static const std::string SECRET_MESSAGE = "This is a secret message. Don't tell anyone.";

	std::stringstream sstr;
	EncryptingOutputStream encryptor(sstr, *pCipher);
	encryptor << SECRET_MESSAGE;
	encryptor.close();

	DecryptingInputStream decryptor(sstr, *pCipher);
	std::string result;
	Poco::StreamCopier::copyToString(decryptor, result);

	assertTrue (result == SECRET_MESSAGE);
	assertTrue (decryptor.eof());
	assertTrue (!decryptor.bad());


	std::istringstream emptyStream;
	DecryptingInputStream badDecryptor(emptyStream, *pCipher);
	Poco::StreamCopier::copyToString(badDecryptor, result);

	assertTrue (badDecryptor.fail());
	assertTrue (badDecryptor.bad());
	assertTrue (!badDecryptor.eof());
}


void CryptoTest::testCertificate()
{
	std::istringstream certStream(APPINF_PEM);
	X509Certificate cert(certStream);

	std::string subjectName(cert.subjectName());
	std::string issuerName(cert.issuerName());
	std::string commonName(cert.commonName());
	std::string country(cert.subjectName(X509Certificate::NID_COUNTRY));
	std::string localityName(cert.subjectName(X509Certificate::NID_LOCALITY_NAME));
	std::string stateOrProvince(cert.subjectName(X509Certificate::NID_STATE_OR_PROVINCE));
	std::string organizationName(cert.subjectName(X509Certificate::NID_ORGANIZATION_NAME));
	std::string organizationUnitName(cert.subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME));

	assertTrue (subjectName == "CN=appinf.com,O=Applied Informatics Software Engineering GmbH,OU=Development,ST=Carinthia,C=AT,L=St. Jakob im Rosental,emailAddress=guenter.obiltschnig@appinf.com");
	assertTrue (issuerName == subjectName);
	assertTrue (commonName == "appinf.com");
	assertTrue (country == "AT");
	assertTrue (localityName == "St. Jakob im Rosental");
	assertTrue (stateOrProvince == "Carinthia");
	assertTrue (organizationName == "Applied Informatics Software Engineering GmbH");
	assertTrue (organizationUnitName == "Development");

	const auto fingerprint = cert.fingerprint();
	assertTrue (Poco::DigestEngine::digestToHex(fingerprint) == "ac84e4eb72c861ccb20f2900f3f17a9ac11f6579");

	// fails with recent OpenSSL versions:
	// assert (cert.issuedBy(cert));

	std::istringstream otherCertStream(APPINF_PEM);
	X509Certificate otherCert(otherCertStream);

	assertTrue (cert.equals(otherCert));
}

void CryptoTest::testCertificateUTF8()
{
	std::istringstream certStream(UTF8_PEM);
	X509Certificate cert(certStream);

	std::string subjectName(cert.subjectName());
	std::string issuerName(cert.issuerName());
	std::string commonName(cert.commonName());
	std::string country(cert.subjectName(X509Certificate::NID_COUNTRY));
	std::string localityName(cert.subjectName(X509Certificate::NID_LOCALITY_NAME));
	std::string stateOrProvince(cert.subjectName(X509Certificate::NID_STATE_OR_PROVINCE));
	std::string organizationName(cert.subjectName(X509Certificate::NID_ORGANIZATION_NAME));
	std::string organizationUnitName(cert.subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME));

	assertTrue (subjectName == "serialNumber=PID:PXC5.E003 SN:140004D8E2,C=CH,O=Siemens,OU=SI BP Division,ST=Zug,L=Zählerweg 7 Room 544,emailAddress=richard.kurz@siemens.com,OU=SSL Certificate,CN=KurzRS1MP22sprint59preVVS.KurPXC5BNR37652");
	assertTrue (commonName == "KurzRS1MP22sprint59preVVS.KurPXC5BNR37652");
	assertTrue (country == "CH");
	assertTrue (localityName == "Zählerweg 7 Room 544");
	assertTrue (stateOrProvince == "Zug");
	assertTrue (organizationName == "Siemens");
	assertTrue (organizationUnitName == "SI BP Division");

	std::istringstream otherCertStream(UTF8_PEM);
	X509Certificate otherCert(otherCertStream);

	assertTrue (cert.equals(otherCert));
}


void CryptoTest::setUp()
{
}


void CryptoTest::tearDown()
{
}


CppUnit::Test* CryptoTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CryptoTest");

	CppUnit_addTest(pSuite, CryptoTest, testEncryptDecrypt);
	CppUnit_addTest(pSuite, CryptoTest, testEncryptDecryptWithSalt);
	CppUnit_addTest(pSuite, CryptoTest, testEncryptDecryptWithSaltSha1);
	CppUnit_addTest(pSuite, CryptoTest, testEncryptDecryptDESECB);
	CppUnit_addTest(pSuite, CryptoTest, testEncryptDecryptGCM);
	CppUnit_addTest(pSuite, CryptoTest, testPassword);
	CppUnit_addTest(pSuite, CryptoTest, testPasswordSha1);
	CppUnit_addTest(pSuite, CryptoTest, testEncryptInterop);
	CppUnit_addTest(pSuite, CryptoTest, testDecryptInterop);
	CppUnit_addTest(pSuite, CryptoTest, testStreams);
	CppUnit_addTest(pSuite, CryptoTest, testCertificate);
	CppUnit_addTest(pSuite, CryptoTest, testCertificateUTF8);

	return pSuite;
}
