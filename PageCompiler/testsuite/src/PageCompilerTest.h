//
// PageCompilerTest.h
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PageCompilerTest_INCLUDED
#define PageCompilerTest_INCLUDED


#include "CppUnit/TestCase.h"


class PageCompilerTest: public CppUnit::TestCase
{
public:
	PageCompilerTest(const std::string& name);
	~PageCompilerTest();

	void testHandler();
	void testHandlerNoForm();
	void testStringify();
	void testStringifyNoForm();
	void testStringifyEscape();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // PageCompilerTest_INCLUDED
