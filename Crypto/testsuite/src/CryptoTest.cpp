//
// CryptoTest.cpp
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CryptoTest.h"
#include "ErrorQueueCleaner.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/CryptoStream.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/StreamCopier.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Exception.h"
#include "Poco/Base64Encoder.h"
#include "Poco/HexBinaryEncoder.h"
#include "Poco/HexBinaryDecoder.h"
#include <memory>
#include <sstream>
#include <openssl/x509.h>
#include <openssl/err.h>


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

static const std::string SAN_PEM(
	"-----BEGIN CERTIFICATE-----\n"
	"MIIBpTCCAUugAwIBAgIUfiHfNKdSHwtV0Me8XmHXpWS1AncwCgYIKoZIzj0EAwIw\n"
	"GTEXMBUGA1UEAwwOY24uZXhhbXBsZS5jb20wIBcNMjYwOTIxMTEzODUwWhgPMjEy\n"
	"NjA4MjgxMTM4NTBaMBkxFzAVBgNVBAMMDmNuLmV4YW1wbGUuY29tMFkwEwYHKoZI\n"
	"zj0CAQYIKoZIzj0DAQcDQgAEY+C7Dwh2tgrH9WsPN5Y2wKEUrhnGmpBipXkDfR1z\n"
	"PJWyN8rHiyKftlJwxVkYf13co18hd+4MClD1qsBNTKuoPqNvMG0wHQYDVR0OBBYE\n"
	"FGAc1IlnVG/NA2uPVHgkUwYPNtWWMB8GA1UdIwQYMBaAFGAc1IlnVG/NA2uPVHgk\n"
	"UwYPNtWWMA8GA1UdEwEB/wQFMAMBAf8wGgYDVR0RBBMwEYIPc2FuLmV4YW1wbGUu\n"
	"Y29tMAoGCCqGSM49BAMCA0gAMEUCIG0wXnlgBNCK422Ifbtk9NS96WRfbIwuJmxH\n"
	"0tcGh5UtAiEAoRlUMD2If9iSGyBNmuO2sptJPs9kHnq2WQAZpAJN2Kw=\n"
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


namespace
{
	std::string fromHex(const std::string& hex)
	{
		std::istringstream istr(hex);
		Poco::HexBinaryDecoder decoder(istr);
		std::string bytes;
		Poco::StreamCopier::copyToString(decoder, bytes);
		return bytes;
	}


	struct GCMVector
	{
		const char* cipher;
		std::string key;
		std::string iv;
		std::string plaintext;
		std::string ciphertext;
		std::string tag;
	};
}


void CryptoTest::testEncryptDecryptGCMIVLength()
{
	// Known answers for IVs shorter and longer than the 12-byte default (OpenSSL evpciph test data).
	const GCMVector vectors[] =
	{
		{
			"aes-256-gcm",
			"9473c28f6e978eb15e1967b888282aa6b078d320034fe5f40f8bb68674f1ecda",
			"0a",
			"2d2e2798c10bcfcce742e92d3c390fef",
			"c4e5ab2c6a4316e57c6c37d2c2acb42c",
			"03337df7e1e68d77706abef9edaf5e07"
		},
		{
			"aes-128-gcm",
			std::string(32, '0'),
			"ffffffff" + std::string(120, '0'),
			std::string(384, '0'),
			"56b3373ca9ef6e4a2b64fe1e9a17b61425f10d47a75a5fce13efc6bc784af24f"
			"4141bdd48cf7c770887afd573cca5418a9aeffcd7c5ceddfc6a78397b9a85b49"
			"9da558257267caab2ad0b23ca476a53cb17fb41c4b8b475cb4f3f7165094c229"
			"c9e8c4dc0a2a5ff1903e501511221376a1cdb8364c5061a20cae74bc4acd76ce"
			"b0abc9fd3217ef9f8c90be402ddf6d8697f4f880dff15bfb7a6b28241ec8fe18"
			"3c2d59e3f9dfff653c7126f0acb9e64211f42bae12af462b1070bef1ab5e3606",
			"566f8ef683078bfdeeffa869d751a017"
		}
	};

	for (const auto& v: vectors)
	{
		const std::string keyBytes = fromHex(v.key);
		const std::string ivBytes = fromHex(v.iv);
		CipherKey key(v.cipher,
			CipherKey::ByteVec(keyBytes.begin(), keyBytes.end()),
			CipherKey::ByteVec(ivBytes.begin(), ivBytes.end()));
		Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(key);

		std::stringstream str;
		CryptoTransform::Ptr pEncryptor = pCipher->createEncryptor();
		CryptoOutputStream encryptorStream(str, pEncryptor);
		encryptorStream << fromHex(v.plaintext);
		encryptorStream.close();
		assertTrue (encryptorStream.good());
		assertTrue (str.str() == fromHex(v.ciphertext));

		const std::string tag = pEncryptor->getTag();
		assertTrue (tag == fromHex(v.tag));

		CryptoTransform::Ptr pDecryptor = pCipher->createDecryptor();
		pDecryptor->setTag(tag);
		CryptoInputStream decryptorStream(str, pDecryptor);
		std::string out;
		Poco::StreamCopier::copyToString(decryptorStream, out);
		assertTrue (out == fromHex(v.plaintext));
	}

	CipherKey emptyIVKey("aes-256-gcm", CipherKey::ByteVec(32, 1), CipherKey::ByteVec());
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(emptyIVKey);
	try
	{
		CryptoTransform::Ptr pEncryptor = pCipher->createEncryptor();
		fail("empty GCM IV - must throw");
	}
	catch (Poco::IOException&)
	{
	}
}


void CryptoTest::testLegacyProviderErrorQueue()
{
	ErrorQueueCleaner cleaner;

	// A legacy provider that cannot be loaded must not leave errors behind.
	ERR_clear_error();
	CipherFactory factory;
	assertTrue (ERR_peek_error() == 0);

	// The test driver holds the only other reference: at count zero
	// initialize() retries a legacy provider that could not be loaded.
	OpenSSLInitializer::uninitialize();
	OpenSSLInitializer::initialize();
	assertTrue (ERR_peek_error() == 0);

	CipherKey key("aes-256-cbc");
	assertTrue (ERR_peek_error() == 0);
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


void CryptoTest::testCertificateSubjectAltName()
{
	ErrorQueueCleaner cleaner;
	ERR_clear_error();

	std::string commonName;
	std::set<std::string> domainNames;

	std::istringstream sanStream(SAN_PEM);
	X509Certificate sanCert(sanStream);
	sanCert.extractNames(commonName, domainNames);
	assertTrue (commonName == "cn.example.com");
	assertTrue (domainNames == std::set<std::string>{"san.example.com"});

	std::istringstream noSANStream(APPINF_PEM);
	X509Certificate noSANCert(noSANStream);
	noSANCert.extractNames(commonName, domainNames);
	assertTrue (domainNames == std::set<std::string>{"appinf.com"});

	// The length of the dNSName changes from 0x0F to 0x7F, beyond the end of the extension.
	std::string undecodablePEM(SAN_PEM);
	const std::string::size_type pos = undecodablePEM.find("EYIPc2Fu");
	assertTrue (pos != std::string::npos);
	undecodablePEM.replace(pos, 8, "EYJ/c2Fu");
	std::istringstream undecodableStream(undecodablePEM);
	X509Certificate undecodableCert(undecodableStream);
	try
	{
		undecodableCert.extractNames(commonName, domainNames);
		fail("undecodable subjectAltName - must throw");
	}
	catch (OpenSSLException&)
	{
	}
	assertTrue (ERR_peek_error() == 0);
}


void CryptoTest::testCertificateDuplicate()
{
	ErrorQueueCleaner cleaner;

	std::istringstream certStream(APPINF_PEM);
	X509Certificate cert(certStream);

	X509Certificate duplicate(cert.dup());
	assertTrue (cert.equals(duplicate));
	X509Certificate copy(cert);
	assertTrue (cert.equals(copy));

	// A moved-from certificate has nothing to duplicate and copying it stays legal.
	X509Certificate moved(std::move(cert));
	assertTrue (cert.dup() == nullptr); // NOLINT(bugprone-use-after-move)
	X509Certificate copyOfMoved(cert); // NOLINT(bugprone-use-after-move)
	assertTrue (copyOfMoved.certificate() == nullptr);
	assertTrue (moved.equals(copy));
}


void CryptoTest::testCertificateSaveCannotCreateFile()
{
	ErrorQueueCleaner cleaner;
	ERR_clear_error();

	std::istringstream certStream(APPINF_PEM);
	X509Certificate cert(certStream);
	try
	{
		cert.save(Poco::TemporaryFile::tempName() + "/cert.pem");
		fail("directory does not exist - must throw");
	}
	catch (Poco::CreateFileException&)
	{
	}
	assertTrue (ERR_peek_error() == 0);
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

	// verify validFrom/expiresOn parse ASN1 timestamps with Z suffix (GH #5263)
	Poco::DateTime validFrom = cert.validFrom();
	assertTrue (validFrom.year() == 2009);
	assertTrue (validFrom.month() == 5);
	assertTrue (validFrom.day() == 7);

	Poco::DateTime expiresOn = cert.expiresOn();
	assertTrue (expiresOn.year() == 2029);
	assertTrue (expiresOn.month() == 5);
	assertTrue (expiresOn.day() == 2);

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


// Raw ASN.1 types: the MBSTRING_* types re-encode the value and limit its length.
static bool addNameEntry(X509_NAME* pName, int nid, int type, const void* pBytes, std::size_t length)
{
	return X509_NAME_add_entry_by_NID(pName, nid, type,
		reinterpret_cast<const unsigned char*>(pBytes), static_cast<int>(length), -1, 0) == 1;
}


void CryptoTest::testCertificateNameEntries()
{
	const std::string commonName("good.com\0.evil.com", 18);
	const std::string organizationName(300, 'a');
	// UCS-2 big-endian "Po", U+010D, "o".
	static const unsigned char LOCALITY_NAME[] = { 0x00, 0x50, 0x00, 0x6F, 0x01, 0x0D, 0x00, 0x6F };
	// Not valid UTF-8: OpenSSL cannot convert it.
	static const unsigned char STATE_OR_PROVINCE[] = { 0x41, 0xFF, 0x42 };

	ERR_clear_error();

	std::unique_ptr<X509, decltype(&X509_free)> pX509(X509_new(), X509_free);
	assertNotNullPtr(pX509.get());
	// X509_set_subject_name() would reject the invalid UTF-8 value, so the
	// entries go into the certificate's own name (const since OpenSSL 4.0).
	X509_NAME* pName = const_cast<X509_NAME*>(X509_get_subject_name(pX509.get()));
	assertNotNullPtr(pName);
	assertTrue (addNameEntry(pName, NID_commonName, V_ASN1_UTF8STRING, commonName.data(), commonName.size()));
	assertTrue (addNameEntry(pName, NID_organizationName, V_ASN1_UTF8STRING, organizationName.data(), organizationName.size()));
	assertTrue (addNameEntry(pName, NID_localityName, V_ASN1_BMPSTRING, LOCALITY_NAME, sizeof(LOCALITY_NAME)));
	assertTrue (addNameEntry(pName, NID_stateOrProvinceName, V_ASN1_UTF8STRING, STATE_OR_PROVINCE, sizeof(STATE_OR_PROVINCE)));

	X509Certificate cert(pX509.release());

	assertTrue (cert.commonName().size() == 18);
	assertTrue (cert.commonName() != "good.com");
	assertTrue (cert.commonName() == commonName);
	assertTrue (cert.subjectName(X509Certificate::NID_ORGANIZATION_NAME).size() == 300);
	assertTrue (cert.subjectName(X509Certificate::NID_LOCALITY_NAME) == "Po\xC4\x8D" "o");
	assertTrue (cert.subjectName(X509Certificate::NID_STATE_OR_PROVINCE) == std::string("A\xFF" "B", 3));
	assertTrue (ERR_peek_error() == 0);
	assertTrue (cert.subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());

	// A queued error must survive a failed conversion.
	const unsigned char junk[] = { 0xFF };
	const unsigned char* pJunk = junk;
	assertNullPtr(d2i_X509(nullptr, &pJunk, 1));
	const unsigned long pending = ERR_peek_last_error();
	assertTrue (pending != 0);
	assertTrue (cert.subjectName(X509Certificate::NID_STATE_OR_PROVINCE) == std::string("A\xFF" "B", 3));
	assertTrue (ERR_peek_last_error() == pending);
	ERR_clear_error();
}


void CryptoTest::testOpenSSLException()
{
	ErrorQueueCleaner cleaner;

	ERR_clear_error();
	const unsigned char junk[] = { 0xFF };
	const unsigned char* pJunk = junk;
	assertTrue (d2i_X509(nullptr, &pJunk, 1) == nullptr);
	assertTrue (ERR_peek_error() != 0);
	assertTrue (ERR_peek_error() != ERR_peek_last_error());

	OpenSSLException exc("test");
	assertTrue (ERR_peek_error() == 0);
	const std::string message = exc.message();
	assertTrue (message.compare(0, 4, "test") == 0);
	const std::string::size_type firstError = message.find("error:");
	assertTrue (firstError != std::string::npos);
	assertTrue (message.find("error:", firstError + 1) != std::string::npos);

	OpenSSLException plain("plain");
	assertTrue (plain.message().substr(0, plain.message().find('\n')) == "plain");
	assertTrue (plain.displayText().find("error:00000000") == std::string::npos);

	pJunk = junk;
	assertTrue (d2i_X509(nullptr, &pJunk, 1) == nullptr);
	OpenSSLException copy(exc);
	assertTrue (copy.message() == message);
	assertTrue (ERR_peek_error() != 0);
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
	CppUnit_addTest(pSuite, CryptoTest, testEncryptDecryptGCMIVLength);
	CppUnit_addTest(pSuite, CryptoTest, testLegacyProviderErrorQueue);
	CppUnit_addTest(pSuite, CryptoTest, testPassword);
	CppUnit_addTest(pSuite, CryptoTest, testPasswordSha1);
	CppUnit_addTest(pSuite, CryptoTest, testEncryptInterop);
	CppUnit_addTest(pSuite, CryptoTest, testDecryptInterop);
	CppUnit_addTest(pSuite, CryptoTest, testStreams);
	CppUnit_addTest(pSuite, CryptoTest, testCertificateSubjectAltName);
	CppUnit_addTest(pSuite, CryptoTest, testCertificateDuplicate);
	CppUnit_addTest(pSuite, CryptoTest, testCertificateSaveCannotCreateFile);
	CppUnit_addTest(pSuite, CryptoTest, testCertificate);
	CppUnit_addTest(pSuite, CryptoTest, testCertificateUTF8);
	CppUnit_addTest(pSuite, CryptoTest, testCertificateNameEntries);
	CppUnit_addTest(pSuite, CryptoTest, testOpenSSLException);

	return pSuite;
}
