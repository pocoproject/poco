//
// CodeGenerationTest.h
//
// Definition of the CodeGenerationTest class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
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

	void testA();
	void testB();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // CodeGenerationTest_INCLUDED
