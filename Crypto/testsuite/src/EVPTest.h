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

	void testRSAEVPKeyFromX509();
	void testRSAEVPKeyFromPKCS12();

#if OPENSSL_VERSION_NUMBER >= 0x10000000L
	void testRSAEVPKeyByLength();
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	void testECEVPKeyByLength();
	void testEVPKeyByModulus();
#endif // OPENSSL_VERSION_NUMBER >= 0x30000000L
#endif // OPENSSL_VERSION_NUMBER >= 0x10000000L

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // EVPTest_INCLUDED
