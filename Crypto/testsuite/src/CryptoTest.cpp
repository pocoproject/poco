//
// CryptoTest.cpp
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CryptoTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/X509Request.h"
#include "Poco/Crypto/X509Extension.h"
#include "Poco/Crypto/X509Store.h"
#include "Poco/Crypto/X509RevocationList.h"
#include "Poco/Crypto/X509Revoked.h"
#include "Poco/Crypto/CryptoStream.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/StreamCopier.h"
#include "Poco/Base64Encoder.h"
#include "Poco/HexBinaryEncoder.h"
#include <openssl/x509v3.h>
#include <sstream>
#include <vector>

#include <iostream>

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
	
	CipherKey::ByteVec iv(20, 213);
	key.setIV(iv);

	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(key);

	for (std::size_t n = 1; n < MAX_DATA_SIZE; n++)
	{
		std::stringstream str;
		CryptoTransform* pEncryptor = pCipher->createEncryptor();
		CryptoOutputStream encryptorStream(str, pEncryptor);
		std::string in(n, 'x');
		encryptorStream << in;
		encryptorStream.close();
		assertTrue (encryptorStream.good());

		std::string tag = pEncryptor->getTag();

		CryptoTransform* pDecryptor = pCipher->createDecryptor();
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
	std::string emailAddress(cert.subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS));
	std::string serialNumber(cert.subjectName(X509Certificate::NID_SERIAL_NUMBER));

	assertTrue (subjectName == "/CN=appinf.com/O=Applied Informatics Software Engineering GmbH/OU=Development/ST=Carinthia/C=AT/L=St. Jakob im Rosental/emailAddress=guenter.obiltschnig@appinf.com");
	assertTrue (issuerName == subjectName);
	assertTrue (commonName == "appinf.com");
	assertTrue (country == "AT");
	assertTrue (localityName == "St. Jakob im Rosental");
	assertTrue (stateOrProvince == "Carinthia");
	assertTrue (organizationName == "Applied Informatics Software Engineering GmbH");
	assertTrue (organizationUnitName == "Development");
	assertTrue (emailAddress == "guenter.obiltschnig@appinf.com");
	assertTrue (serialNumber == "");

	// fails with recent OpenSSL versions:
	// assertTrue (cert.issuedBy(cert));

	std::istringstream otherCertStream(APPINF_PEM);
	X509Certificate otherCert(otherCertStream);

	assertTrue (cert.equals(otherCert));
}


void CryptoTest::testSignCertificate()
{
	X509Certificate certificate;

	certificate.setCommonName("Common name test");
	certificate.addSubject(X509Certificate::NID_COUNTRY, "DE");
	certificate.addSubject(X509Certificate::NID_ORGANIZATION_NAME, "Test organization");

	certificate.addIssuer(X509Certificate::NID_COMMON_NAME, "Issue common name");
	certificate.addIssuer(X509Certificate::NID_COUNTRY, "DE");
	certificate.addIssuer(X509Certificate::NID_ORGANIZATION_NAME, "The organization");

	certificate.addExtension(X509Extension::createWithBasicConstraints(X509Extension::CRITICAL_CA_TRUE));

	certificate.setSerialNumber(42);

	Poco::DateTime expireDate(2323, 12, 31);
	certificate.setExpiresOn(expireDate);
	Poco::DateTime validFrom(2017, 8, 7);
	certificate.setValidFrom(validFrom);

	RSAKey rsaKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey key = rsaKey.evppkey();
	certificate.setPublicKey(key);

	certificate.sign(key);

	std::string certificateSubjectName = certificate.subjectName();
	assert (certificateSubjectName == "/CN=Common name test/C=DE/O=Test organization");
	std::string certificateIssuerName = certificate.issuerName();
	assert (certificateIssuerName == "/CN=Issue common name/C=DE/O=The organization");

	std::ostringstream stream;
	certificate.save(stream);

	std::string content = stream.str();
	std::istringstream certStream(content);

	X509Certificate cert(certStream);

	std::string subjectName(cert.subjectName());
	std::string issuerName(cert.issuerName());
	std::string commonName(cert.commonName());
	std::string country(cert.subjectName(X509Certificate::NID_COUNTRY));
	std::string organizationName(cert.subjectName(X509Certificate::NID_ORGANIZATION_NAME));

	X509Extension::List extensions = cert.findExtensionByNID(NID_basic_constraints);

	assert (extensions.size() == 1);
	X509Extension extension = extensions.at(0);

	assert (extension.isCritical() == true);
	assert (extension.data()[0] == 0x30);
	assert (extension.data()[1] == 0x03);
	assert (extension.data()[2] == 0x01);
	assert (extension.data()[3] == 0x01);
	assert (extension.data()[4] == 0xff);

	assert (subjectName == "/CN=Common name test/C=DE/O=Test organization");
	assert (issuerName == "/CN=Issue common name/C=DE/O=The organization");
	assert (commonName == "Common name test");
	assert (country == "DE");
	assert (organizationName == "Test organization");

	assert (expireDate == cert.expiresOn());
	assert (validFrom == cert.validFrom());
}


void CryptoTest::testSignRequestCertificate()
{
	X509Request certificate;

	certificate.setCommonName("Common name test");
	certificate.addSubject(X509Request::NID_COUNTRY, "DE");
	certificate.addSubject(X509Request::NID_ORGANIZATION_NAME, "Test organization");

	X509Extension::List extList;
	extList.push_back(X509Extension::create(NID_ext_key_usage, "critical,1.3.6.1.5.5.7.3.2"));

	certificate.setExtensions(extList);

	certificate.addExtension(X509Extension::create(NID_basic_constraints, "critical,CA:FALSE"));
	certificate.addExtension(X509Extension::create(NID_key_usage, "critical,digitalSignature"));
	certificate.addExtension(X509Extension::createWithBasicConstraints(X509Extension::CRITICAL_CA_TRUE));

	X509Extension::List extList2;
	extList.push_back(X509Extension::createWithBasicConstraints(X509Extension::CRITICAL_CA_TRUE));
	extList.push_back(X509Extension::create(NID_key_usage, "digitalSignature"));

	certificate.setExtensions(extList2);

	RSAKey key(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey publicKey = key.evppkey();
	certificate.setPublicKey(publicKey);

	std::string certificateSubjectName = certificate.subjectName();
	assert (certificateSubjectName == "/CN=Common name test/C=DE/O=Test organization");

	certificate.sign(publicKey);

	std::ostringstream stream;
	certificate.save(stream);

	std::string content = stream.str();
	std::istringstream certStream(content);

	X509Request cert(certStream);

	std::string subjectName(cert.subjectName());
	std::string commonName(cert.commonName());
	std::string country(cert.subjectName(X509Request::NID_COUNTRY));
	std::string organizationName(cert.subjectName(X509Request::NID_ORGANIZATION_NAME));

	X509Extension::List extensions = certificate.extensions();

	assert (extensions.size() == 4);
	X509Extension extension = extensions.at(0);

	assert (extension.isCritical() == true);
	assert (extension.data()[0] == 0x30);
	assert (extension.data()[1] == 0x03);
	assert (extension.data()[2] == 0x01);
	assert (extension.data()[3] == 0x01);
	assert (extension.data()[4] == 0xff);

	assert (subjectName == "/CN=Common name test/C=DE/O=Test organization");
	assert (commonName == "Common name test");
	assert (country == "DE");
	assert (organizationName == "Test organization");
}


void CryptoTest::testVerifyCertificate()
{
	Poco::DateTime validFrom(2017, 8, 7);
	Poco::DateTime expireDate(2323, 12, 31);

	X509Certificate rootCaCert;
	rootCaCert.setCommonName("Root CA");
	rootCaCert.setValidFrom(validFrom);
	rootCaCert.setExpiresOn(expireDate);
	rootCaCert.addExtension(X509Extension::createWithBasicConstraints(X509Extension::CRITICAL_CA_TRUE));
	RSAKey caRsaKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey caKey = caRsaKey.evppkey();
	rootCaCert.setPublicKey(caKey);
	rootCaCert.addIssuer(X509Certificate::NID_COMMON_NAME, rootCaCert.commonName());
	rootCaCert.sign(caKey);

	X509Certificate intermediateCaCert;
	intermediateCaCert.setCommonName("Intermediate CA");
	intermediateCaCert.setValidFrom(validFrom);
	intermediateCaCert.setExpiresOn(expireDate);
	intermediateCaCert.addExtension(X509Extension::createWithBasicConstraints(X509Extension::CRITICAL_CA_TRUE));
	intermediateCaCert.addIssuer(X509Certificate::NID_COMMON_NAME, rootCaCert.commonName());
	RSAKey intermediateCaRsaKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey intermediateCaKey = intermediateCaRsaKey.evppkey();
	intermediateCaCert.setPublicKey(intermediateCaKey);
	intermediateCaCert.sign(caKey);

	X509Certificate cert;
	cert.setCommonName("Server Cert");
	cert.setValidFrom(validFrom);
	cert.setExpiresOn(expireDate);
	cert.addIssuer(X509Certificate::NID_COMMON_NAME, intermediateCaCert.commonName());
	RSAKey certRsaKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey certKey = certRsaKey.evppkey();
	cert.setPublicKey(certKey);
	cert.sign(intermediateCaKey);

	std::istringstream wrongCertStream(APPINF_PEM);
	X509Certificate wrongCert(wrongCertStream);

	X509Store store;
	store.addCa(rootCaCert);
	store.addCa(intermediateCaCert);

	X509Store::VerifyResult result = store.verifyCertificateChain(cert);
	assert (result.isOk() == true);
	result = store.verifyCertificateChain(wrongCert);
	assert (result.isOk() == false);
	assert (wrongCert.equals(result.certificate()) == true);
}


void CryptoTest::testRevokeCertificate()
{
	RSAKey rsaKey(RSAKey::KL_2048, RSAKey::EXP_LARGE);
	EVPPKey key = rsaKey.evppkey();

	X509Certificate rootCa;
	rootCa.setCommonName("Root CA");
	rootCa.addIssuer(X509Certificate::NID_COMMON_NAME, rootCa.subjectName(X509Certificate::NID_COMMON_NAME));
	rootCa.addExtension(X509Extension::createWithBasicConstraints(X509Extension::CRITICAL_CA_TRUE));
	rootCa.setSerialNumber(42);
	Poco::DateTime expireDate(2323, 12, 31);
	rootCa.setExpiresOn(expireDate);
	Poco::DateTime validFrom(2017, 8, 7);
	rootCa.setValidFrom(validFrom);
	rootCa.setPublicKey(key);
	rootCa.sign(key);


	RSAKey rsaCertKey(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	EVPPKey certKey = rsaCertKey.evppkey();
	X509Certificate cert;
	cert.setCommonName("Test certificate");
	cert.setPublicKey(certKey);
	cert.addIssuer(X509Certificate::NID_COMMON_NAME, rootCa.subjectName(X509Certificate::NID_COMMON_NAME));
	cert.setSerialNumber(232323);
	cert.setExpiresOn(expireDate);
	cert.setValidFrom(validFrom);
	cert.sign(key);


	X509Revoked revoked;
	Poco::DateTime revokeDate(2017, 10, 8);
	revoked.setRevocationDate(revokeDate);
	revoked.setSerialNumber(cert.serialNumber());
	revoked.setReasonCode(X509Revoked::KEY_COMPROMISE);


	X509RevocationList crl;
	Poco::DateTime lastUpdate(2017,10,9);
	crl.setLastUpdate(lastUpdate);
	crl.addIssuer(X509RevocationList::NID_COMMON_NAME, rootCa.subjectName(X509Certificate::NID_COMMON_NAME));
	crl.addRevoked(revoked);
	crl.sign(key);


	X509Store store;
	store.setX509VerifyFlags(X509_V_FLAG_CRL_CHECK);
	store.addCa(rootCa);
	store.addCrl(crl);


	X509Store::VerifyResult result = store.verifyCertificateChain(cert);
	assert (result.isOk() == false);
	assert (result.code() == X509_V_ERR_CERT_REVOKED);
	assert (result.certificate().commonName() == "Test certificate");


	assert (revoked.getRevocationDate() == revokeDate);
	assert (revoked.getReasonCode() == X509Revoked::KEY_COMPROMISE);
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
	CppUnit_addTest(pSuite, CryptoTest, testSignCertificate);
	CppUnit_addTest(pSuite, CryptoTest, testSignRequestCertificate);
	CppUnit_addTest(pSuite, CryptoTest, testVerifyCertificate);
	CppUnit_addTest(pSuite, CryptoTest, testRevokeCertificate);

	return pSuite;
}
