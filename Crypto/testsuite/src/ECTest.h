//
// ECTest.h
//
// $Id: //poco/1.4/Crypto/testsuite/src/ECTest.h#1 $
//
// Definition of the ECTest class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ECTest_INCLUDED
#define ECTest_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/CppUnit/TestCase.h"


class ECTest: public CppUnit::TestCase
{
public:
	ECTest(const std::string& name);
	~ECTest();

	void testECNewKeys();
	void testECNewKeysNoPassphrase();
	void testECSign();
	void testECSignSha256();
	void testECSignManipulated();
	void testECCipher();
	void testECCipherLarge();
	void testECCertificate();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // ECTest_INCLUDED
