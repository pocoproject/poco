//
// SHA2EngineTest.h
//
// Definition of the SHA2EngineTest class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SHA2EngineTest_INCLUDED
#define SHA2EngineTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class SHA2EngineTest: public CppUnit::TestCase
{
public:
	SHA2EngineTest(const std::string& name);
	~SHA2EngineTest();

	void testSHA224();
	void testSHA256();
	void testSHA384();
	void testSHA512();
	void testSHA512_224();
	void testSHA512_256();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SHA2EngineTest_INCLUDED
