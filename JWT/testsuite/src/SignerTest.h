//
// SignerTest.h
//
// Definition of the SignerTest class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SignerTest_INCLUDED
#define SignerTest_INCLUDED


#include "Poco/JWT/JWT.h"
#include "CppUnit/TestCase.h"


class SignerTest: public CppUnit::TestCase
{
public:
	SignerTest(const std::string& name);
	~SignerTest();

	void setUp();
	void tearDown();

	void testSignHS256();
	void testSignHS384();
	void testSignHS512();
	void testVerifyHS256();
	void testVerifyHS384();
	void testVerifyHS512();
	void testVerifyFailSignature();
	void testVerifyFailKey();
	void testSignRS256();
	void testSignRS384();
	void testSignRS512();
	void testVerifyRS256();
	void testVerifyRS384();
	void testVerifyRS512();
	void testSignVerifyES256();
	void testVerifyES256();

	static CppUnit::Test* suite();

	static const std::string RSA_PRIVATE_KEY;
	static const std::string RSA_PUBLIC_KEY;

	static const std::string ECDSA_PRIVATE_KEY;
	static const std::string ECDSA_PUBLIC_KEY;
};


#endif // SignerTest_INCLUDED
