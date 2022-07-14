//
// HMACEngineTest.h
//
// Definition of the HMACEngineTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HMACEngineTest_INCLUDED
#define HMACEngineTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class HMACEngineTest: public CppUnit::TestCase
{
public:
	HMACEngineTest(const std::string& name);
	~HMACEngineTest();

	void testHMAC_MD5();
	void testHMAC_SHA2_224();
	void testHMAC_SHA2_256();
	void testHMAC_SHA2_384();
	void testHMAC_SHA2_512();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // HMACEngineTest_INCLUDED
