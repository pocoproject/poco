//
// TokenTest.h
//
// Definition of the TokenTest class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef TokenTest_INCLUDED
#define TokenTest_INCLUDED


#include "Poco/JWT/JWT.h"
#include "CppUnit/TestCase.h"


class TokenTest: public CppUnit::TestCase
{
public:
	TokenTest(const std::string& name);
	~TokenTest();

	void setUp();
	void tearDown();

	void testParse();
	void testParseNoSig();
	void testSerialize();
	void testAssign();
	void testAudience();

	static CppUnit::Test* suite();
};


#endif // TokenTest_INCLUDED
