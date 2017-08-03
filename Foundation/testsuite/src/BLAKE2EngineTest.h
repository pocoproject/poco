//
// BLAKE2EngineTest.h
//
// $Id: //poco/1.4/Foundation/testsuite/src/BLAKE2EngineTest.h#1 $
//
// Definition of the BLAKE2EngineTest class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef BLAKE2EngineTest_INCLUDED
#define BLAKE2EngineTest_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"

class BLAKE2EngineTest: public CppUnit::TestCase
{
public:
	BLAKE2EngineTest(const std::string& name);
	~BLAKE2EngineTest();

	void testBLAKE2_224();
	void testBLAKE2_256();
	void testBLAKE2_384();
	void testBLAKE2_512();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};

#endif // BLAKE2EngineTest_INCLUDED
