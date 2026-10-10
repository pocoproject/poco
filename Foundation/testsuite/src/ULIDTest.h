//
// ULIDTest.h
//
// Definition of the ULIDTest class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ULIDTest_INCLUDED
#define ULIDTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class ULIDTest: public CppUnit::TestCase
{
public:
	ULIDTest(const std::string& name);
	~ULIDTest();

	void testParse();
	void testBuffer();
	void testCompare();
	void testSwap();
	void testTryParse();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // ULIDTest_INCLUDED
