//
// PageCompilerTest.h
//
// Definition of the PageCompilerTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
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
	void testHandleRequest();
	void testHandleRequestNoForm();
	void testHandleRequestEscape();
	void testHandleRequestBuffered();
	void testHandleRequestCompressed();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // PageCompilerTest_INCLUDED
