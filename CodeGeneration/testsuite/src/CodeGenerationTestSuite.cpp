//
// CodeGenerationTestSuite.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CodeGenerationTestSuite.h"
#include "CodeGenerationTest.h"


CppUnit::Test* CodeGenerationTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CodeGenerationTestSuite");

	pSuite->addTest(CodeGenerationTest::suite());

	return pSuite;
}
