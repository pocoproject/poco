//
// CodeGenerationTest.h
//
// Definition of the CodeGenerationTest class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGenerationTest_INCLUDED
#define CodeGenerationTest_INCLUDED


#ifndef CppUnit_TestCase_INCLUDED
#include "CppUnit/TestCase.h"
#endif


class CodeGenerationTest: public CppUnit::TestCase
{
public:
	CodeGenerationTest(const std::string& name);
	~CodeGenerationTest();

	void testGenerateClass();
	void testStdHeader();
	void testCreateInclude();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // CodeGenerationTest_INCLUDED
