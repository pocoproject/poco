//
// CodeGenerationTestSuite.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "CodeGenerationTestSuite.h"
#include "CodeGenerationTest.h"


CppUnit::Test* CodeGenerationTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CodeGenerationTestSuite");

	pSuite->addTest(CodeGenerationTest::suite());

	return pSuite;
}
