//
// EVPTest.h
//
// Definition of the EVPTest class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef EVPTest_INCLUDED
#define EVPTest_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "CppUnit/TestCase.h"


class EVPTest: public CppUnit::TestCase
{
public:
	EVPTest(const std::string& name);
	~EVPTest();

	void testRSAEVPPKey();
	void testRSAEVPSaveLoadStream();
	void testRSAEVPSaveLoadStreamNoPass();

	void testECEVPPKey();
	void testECEVPSaveLoadStream();
	void testECEVPSaveLoadStreamNoPass();
	void testECEVPSaveLoadFile();
	void testECEVPSaveLoadFileNoPass();
	void testECEVPLoadKeyWrongPassword();

	void testRSAEVPKeyFromX509();
	void testRSAEVPKeyFromPKCS12();

	void testRSAEVPKeyByLength();
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	void testECEVPKeyByLength();
	void testEVPKeyByModulus();
#endif // POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // EVPTest_INCLUDED
