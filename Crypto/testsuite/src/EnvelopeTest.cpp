//
// EnvelopeTest.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EnvelopeTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/Envelope.h"
#include "Poco/Crypto/EVPPKey.h"
#include <iostream>


using Poco::Crypto::Envelope;
using Poco::Crypto::EVPPKey;


EnvelopeTest::EnvelopeTest(const std::string& name): CppUnit::TestCase(name)
{
}


EnvelopeTest::~EnvelopeTest()
{
}


void EnvelopeTest::testOneKeyRSA()
{
	try
	{
		EVPPKey key(EVP_PKEY_RSA, 1024);
		Envelope env(key, NID_aes_256_cbc);
		assertEqual(env.cipherNID(), NID_aes_256_cbc);
		std::string dec = "let's encrypt some text";
		env.seal(dec);
		assertTrue(env.openAsString(key, env.keys()[0]) == dec);
	}
	catch(const Poco::Exception& e)
	{
		std::cerr << e.displayText() << '\n';
		throw;
	}
}


void EnvelopeTest::testMultiKeyRSA()
{
	try
	{
		Envelope::EVPPKeyVec keyVec;
		keyVec.emplace_back(EVP_PKEY_RSA, 1024);
		keyVec.emplace_back(EVP_PKEY_RSA, 2048);
		keyVec.emplace_back(EVP_PKEY_RSA, 1024);
		Envelope env(keyVec, NID_aes_256_cbc);
		assertEqual(env.cipherNID(), NID_aes_256_cbc);
		std::string dec = "let's encrypt some text";
		env.seal(dec);
		int i = 0;
		for (const auto& key : keyVec)
		{
			assertTrue(env.openAsString(key, env.keys()[i++]) == dec);
		}
	}
	catch(const Poco::Exception& e)
	{
		std::cerr << e.displayText() << '\n';
		throw;
	}
}


void EnvelopeTest::testRSATransfer()
{
	try
	{
		EVPPKey key(EVP_PKEY_RSA, 1024);
		Envelope env1(key, NID_aes_256_cbc);
		std::string dec = "let's encrypt some text";

		Envelope env2(key, NID_aes_256_cbc);
		env2.setContent(env1.seal(dec));
		assertTrue(env2.openAsString(key, env1.keys()[0], env1.iv()) == dec);
	}
	catch(const Poco::Exception& e)
	{
		std::cerr << e.displayText() << '\n';
		throw;
	}
}


void EnvelopeTest::testRSAMultiTransfer()
{
	try
	{
		Envelope::EVPPKeyVec keyVec;
		keyVec.emplace_back(EVP_PKEY_RSA, 1024);
		keyVec.emplace_back(EVP_PKEY_RSA, 2048);
		keyVec.emplace_back(EVP_PKEY_RSA, 1024);

		Envelope env0(keyVec, NID_aes_256_cbc);
		std::string dec = "let's encrypt some text";

		const Envelope::ByteVec enc = env0.seal(dec);

		int i = 0;
		for (const auto& key : keyVec)
		{
			Envelope env(key, NID_aes_256_cbc);
			env.setContent(enc);
			assertTrue(env.openAsString(key, env0.keys()[i++], env0.iv()) == dec);
		}
	}
	catch(const Poco::Exception& e)
	{
		std::cerr << e.displayText() << '\n';
		throw;
	}
}

void EnvelopeTest::setUp()
{
}


void EnvelopeTest::tearDown()
{
}


CppUnit::Test* EnvelopeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EnvelopeTest");

	CppUnit_addTest(pSuite, EnvelopeTest, testOneKeyRSA);
	CppUnit_addTest(pSuite, EnvelopeTest, testMultiKeyRSA);
	CppUnit_addTest(pSuite, EnvelopeTest, testRSATransfer);
	CppUnit_addTest(pSuite, EnvelopeTest, testRSAMultiTransfer);

	return pSuite;
}
