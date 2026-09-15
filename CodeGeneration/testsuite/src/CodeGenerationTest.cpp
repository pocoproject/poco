//
// CodeGenerationTest.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CodeGenerationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/Utility.h"
#include <iostream>
#include <fstream>


using namespace Poco::CodeGeneration;
using namespace Poco::CppParser;


std::string linker("cl");
std::string options("/I \"C:\\Program Files\\Microsoft Visual Studio 8\\VC\\INCLUDE\", "
				"/I \"C:\\Program Files\\Microsoft Visual Studio 8\\VC\\PlatformSDK\\include\", "
				"/I \"p:\\poco\\Foundation\\include\", "
				"/I \"p:\\poco\\XML\\include\", "
				"/I \"p:\\poco\\Util\\include\", "
				"/I \"p:\\poco\\Net\\include\", "
				"/D \"WIN32\", "
				"/D \"_DEBUG\", "
				"/D \"_WINDOWS\", "
				"/D \"_MBCS\", "
				"/C, /P, /TP");
std::string path("C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\IDE;C:\\Program Files\\Microsoft Visual Studio 8\\VC\\BIN;C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\Tools;;C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\Tools\\bin");


CodeGenerationTest::CodeGenerationTest(const std::string& name): CppUnit::TestCase(name)
{
}


CodeGenerationTest::~CodeGenerationTest()
{
}


void CodeGenerationTest::testA()
{
}


void CodeGenerationTest::testB()
{
	// add code for second test here
}


void CodeGenerationTest::setUp()
{
}


void CodeGenerationTest::tearDown()
{
}


CppUnit::Test* CodeGenerationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CodeGenerationTest");

	CppUnit_addTest(pSuite, CodeGenerationTest, testA);
	CppUnit_addTest(pSuite, CodeGenerationTest, testB);

	return pSuite;
}
