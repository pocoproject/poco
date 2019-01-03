//
// RSATest.h
//
// Definition of the RSATest class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RSATest_INCLUDED
#define RSATest_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/CppUnit/TestCase.h"


class RSATest: public CppUnit::TestCase
{
public:
	RSATest(const std::string& name);
	~RSATest();

	void testRSANewKeys();
	void testRSANewKeysNoPassphrase();
	void testRSASign();
	void testRSASignSha256();
	void testRSASignManipulated();
	void testRSACipher();
	void testRSACipherLarge();
	void testRSACertificate();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // RSATest_INCLUDED
