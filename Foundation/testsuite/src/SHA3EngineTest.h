//
// SHA3EngineTest.h
//
// $Id: //poco/1.4/Foundation/testsuite/src/SHA3EngineTest.h#1 $
//
// Definition of the SHA3EngineTest class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef SHA3EngineTest_INCLUDED
#define SHA3EngineTest_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/CppUnit/TestCase.h"

class SHA3EngineTest: public CppUnit::TestCase
{
public:
	SHA3EngineTest(const std::string& name);
	~SHA3EngineTest();

	void testSHA3_224();
	void testSHA3_256();
	void testSHA3_384();
	void testSHA3_512();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};

#endif // SHA3EngineTest_INCLUDED
