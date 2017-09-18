//
// PKCS12ContainerTest.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PKCS12ContainerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/PKCS12Container.h"
#include "Poco/Environment.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include <iostream>
#include <sstream>
#include <fstream>


using namespace Poco::Crypto;
using Poco::Environment;
using Poco::Path;
using Poco::File;


PKCS12ContainerTest::PKCS12ContainerTest(const std::string& name): CppUnit::TestCase(name)
{
}


PKCS12ContainerTest::~PKCS12ContainerTest()
{
}


void PKCS12ContainerTest::testFullPKCS12()
{
	try
	{
		std::string file = getTestFilesPath("full");
		full(PKCS12Container(file.c_str(), "crypto"));

		std::ifstream ifs(file.c_str(), std::ios::binary);
		full(PKCS12Container(ifs, "crypto"));
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void PKCS12ContainerTest::full(const PKCS12Container& pkcs12)
{
	assert ("vally" == pkcs12.getFriendlyName());

	assert (pkcs12.hasKey());
	EVPPKey pKey = pkcs12.getKey();
	assert (EVP_PKEY_RSA == pKey.type());

	assert (pkcs12.hasX509Certificate());
	X509Certificate x509 = pkcs12.getX509Certificate();

	std::string subjectName(x509.subjectName());
	std::string issuerName(x509.issuerName());
	std::string commonName(x509.commonName());
	std::string country(x509.subjectName(X509Certificate::NID_COUNTRY));
	std::string localityName(x509.subjectName(X509Certificate::NID_LOCALITY_NAME));
	std::string stateOrProvince(x509.subjectName(X509Certificate::NID_STATE_OR_PROVINCE));
	std::string organizationName(x509.subjectName(X509Certificate::NID_ORGANIZATION_NAME));
	std::string organizationUnitName(x509.subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME));
	std::string emailAddress(x509.subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS));
	std::string serialNumber(x509.subjectName(X509Certificate::NID_SERIAL_NUMBER));

	assert (subjectName == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Server");
	assert (issuerName == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Intermediate CA v3");
	assert (commonName == "CV Server");
	assert (country == "CH");
	assert (localityName.empty());
	assert (stateOrProvince == "Zug");
	assert (organizationName == "Crypto Vally");
	assert (organizationUnitName.empty());
	assert (emailAddress.empty());
	assert (serialNumber.empty());

	PKCS12Container::CAList caList = pkcs12.getCACerts();
	assert (2 == caList.size());

	assert (caList[0].subjectName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Root CA v3");
	assert (caList[0].issuerName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Root CA v3");
	assert (caList[0].commonName() == "CV Root CA v3");
	assert (caList[0].subjectName(X509Certificate::NID_COUNTRY) == "CH");
	assert (caList[0].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[0].subjectName(X509Certificate::NID_STATE_OR_PROVINCE) == "Zug");
	assert (caList[0].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Crypto Vally");
	assert (caList[0].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[0].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[0].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());

	assert (caList[1].subjectName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Intermediate CA v3");
	assert (caList[1].issuerName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Root CA v3");
	assert (caList[1].commonName() == "CV Intermediate CA v3");
	assert (caList[1].subjectName(X509Certificate::NID_COUNTRY) == "CH");
	assert (caList[1].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[1].subjectName(X509Certificate::NID_STATE_OR_PROVINCE) == "Zug");
	assert (caList[1].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Crypto Vally");
	assert (caList[1].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[1].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[1].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());
}


void PKCS12ContainerTest::testCertsOnlyPKCS12()
{
	try
	{
		std::string file = getTestFilesPath("certs-only");
		certsOnly(PKCS12Container(file.c_str(), "crypto"));

		std::ifstream ifs(file.c_str(), std::ios::binary);
		certsOnly(PKCS12Container(ifs, "crypto"));
	}
	catch (Poco::Exception& ex)
	{
		std::cerr << ex.displayText() << std::endl;
		throw;
	}
}


void PKCS12ContainerTest::certsOnly(const PKCS12Container& pkcs12)
{
	assert (!pkcs12.hasKey());
	assert (!pkcs12.hasX509Certificate());

	PKCS12Container::CAList caList = pkcs12.getCACerts();

	assert (5 == caList.size());

	assert (caList[0].subjectName() == "/C=US/O=Let's Encrypt/CN=Let's Encrypt Authority X3");
	assert (caList[0].issuerName() == "/C=US/O=Internet Security Research Group/CN=ISRG Root X1");
	assert (caList[0].commonName() == "Let's Encrypt Authority X3");
	assert (caList[0].subjectName(X509Certificate::NID_COUNTRY) == "US");
	assert (caList[0].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[0].subjectName(X509Certificate::NID_STATE_OR_PROVINCE).empty());
	assert (caList[0].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Let's Encrypt");
	assert (caList[0].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[0].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[0].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());

	assert (caList[1].subjectName() == "/C=US/O=Let's Encrypt/CN=Let's Encrypt Authority X3");
	assert (caList[1].issuerName() == "/O=Digital Signature Trust Co./CN=DST Root CA X3");
	assert (caList[1].commonName() == "Let's Encrypt Authority X3");
	assert (caList[1].subjectName(X509Certificate::NID_COUNTRY) == "US");
	assert (caList[1].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[1].subjectName(X509Certificate::NID_STATE_OR_PROVINCE).empty());
	assert (caList[1].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Let's Encrypt");
	assert (caList[1].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[1].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[1].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());

	assert (caList[2].subjectName() == "/C=US/O=Internet Security Research Group/CN=ISRG Root X1");
	assert (caList[2].issuerName() == "/C=US/O=Internet Security Research Group/CN=ISRG Root X1");
	assert (caList[2].commonName() == "ISRG Root X1");
	assert (caList[2].subjectName(X509Certificate::NID_COUNTRY) == "US");
	assert (caList[2].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[2].subjectName(X509Certificate::NID_STATE_OR_PROVINCE).empty());
	assert (caList[2].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Internet Security Research Group");
	assert (caList[2].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[2].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[2].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());

	assert (caList[3].subjectName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Root CA v3");
	assert (caList[3].issuerName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Root CA v3");
	assert (caList[3].commonName() == "CV Root CA v3");
	assert (caList[3].subjectName(X509Certificate::NID_COUNTRY) == "CH");
	assert (caList[3].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[3].subjectName(X509Certificate::NID_STATE_OR_PROVINCE) == "Zug");
	assert (caList[3].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Crypto Vally");
	assert (caList[3].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[3].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[3].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());

	assert (caList[4].subjectName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Intermediate CA v3");
	assert (caList[4].issuerName() == "/C=CH/ST=Zug/O=Crypto Vally/CN=CV Root CA v3");
	assert (caList[4].commonName() == "CV Intermediate CA v3");
	assert (caList[4].subjectName(X509Certificate::NID_COUNTRY) == "CH");
	assert (caList[4].subjectName(X509Certificate::NID_LOCALITY_NAME).empty());
	assert (caList[4].subjectName(X509Certificate::NID_STATE_OR_PROVINCE) == "Zug");
	assert (caList[4].subjectName(X509Certificate::NID_ORGANIZATION_NAME) == "Crypto Vally");
	assert (caList[4].subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME).empty());
	assert (caList[4].subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS).empty());
	assert (caList[4].subjectName(X509Certificate::NID_SERIAL_NUMBER).empty());

}


void PKCS12ContainerTest::setUp()
{
}


void PKCS12ContainerTest::tearDown()
{
}


std::string PKCS12ContainerTest::getTestFilesPath(const std::string& name)
{
	std::ostringstream ostr;
	ostr << "data/" << name << ".p12";
	std::string fileName(ostr.str());
	Poco::Path path(fileName);
	if (Poco::File(path).exists())
	{
		return fileName;
	}

	ostr.str("");
	ostr << "/Crypto/testsuite/data/" << name << ".p12";
	fileName = Poco::Environment::get("POCO_BASE") + ostr.str();
	path = fileName;

	if (!Poco::File(path).exists())
	{
		std::cerr << "Can't find " << fileName << std::endl;
		throw Poco::NotFoundException("cannot locate directory containing valid Crypto test files");
	}
	return fileName;
}


CppUnit::Test* PKCS12ContainerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PKCS12ContainerTest");

	CppUnit_addTest(pSuite, PKCS12ContainerTest, testFullPKCS12);
	CppUnit_addTest(pSuite, PKCS12ContainerTest, testCertsOnlyPKCS12);

	return pSuite;
}
