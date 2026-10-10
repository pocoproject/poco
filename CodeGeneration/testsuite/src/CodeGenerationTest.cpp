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
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/NameSpace.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/Variable.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/Utility.h"
#include <sstream>


using namespace Poco::CodeGeneration;
using namespace Poco::CppParser;


CodeGenerationTest::CodeGenerationTest(const std::string& name): CppUnit::TestCase(name)
{
}


CodeGenerationTest::~CodeGenerationTest()
{
}


void CodeGenerationTest::testGenerateClass()
{
	// Every symbol registers itself with the namespace it is constructed in and is
	// owned by it, so the whole model is released with the root namespace.
	NameSpace* pNameSpace = new NameSpace("MyLib", NameSpace::root());
	Struct* pStruct = new Struct("class Foo", true, pNameSpace);
	pStruct->setLibrary("MyLib");
	pStruct->setPackage("MyPackage");
	pStruct->setFile("include/MyLib/Foo.h");

	Function* pFunc = new Function("void bar", pStruct);
	pFunc->setAccess(Symbol::ACC_PUBLIC);
	pFunc->addParameter(new Parameter("int x", pFunc));

	Variable* pVar = new Variable("int _x", pStruct);
	pVar->setAccess(Symbol::ACC_PRIVATE);

	std::ostringstream hOut;
	std::ostringstream cppOut;
	CppGenerator gen("MyLib", "MyLib", true, "Copyright (c) 2026, Test", hOut, cppOut);
	gen.enableTimestamps(false);
	GeneratorEngine engine;
	engine.generate(pStruct, gen);

	const std::string includeGuard = Poco::CodeGeneration::Utility::createIncludeGuard("MyLib", "Foo");
	const std::string header = hOut.str();
	assertTrue (header.find("#ifndef " + includeGuard) != std::string::npos);
	assertTrue (header.find("#define " + includeGuard) != std::string::npos);
	assertTrue (header.find("#endif // " + includeGuard) != std::string::npos);
	assertTrue (header.find("// Copyright (c) 2026, Test") != std::string::npos);
	assertTrue (header.find("namespace MyLib {") != std::string::npos);
	assertTrue (header.find("} // namespace MyLib") != std::string::npos);
	assertTrue (header.find("class MyLib_API Foo") != std::string::npos);
	assertTrue (header.find("public:") != std::string::npos);
	assertTrue (header.find("void bar(int x);") != std::string::npos);
	assertTrue (header.find("private:") != std::string::npos);
	assertTrue (header.find("int _x;") != std::string::npos);
	// Timestamps are disabled, so the generated-on line must carry no date.
	assertTrue (header.find("// This file has been generated.") != std::string::npos);

	const std::string source = cppOut.str();
	assertTrue (source.find("// Copyright (c) 2026, Test") != std::string::npos);
	assertTrue (source.find("#include \"MyLib/Foo.h\"") != std::string::npos);
	assertTrue (source.find("namespace MyLib {") != std::string::npos);
	assertTrue (source.find("} // namespace MyLib") != std::string::npos);
	assertTrue (source.find("void Foo::bar(int x)") != std::string::npos);
}


void CodeGenerationTest::testStdHeader()
{
	std::ostringstream ostr;
	Poco::CodeGeneration::Utility::writeStdHeader(ostr, "Foo.h", "MyLib", "MyPackage", "Foo", "Copyright (c) 2026, Test", false);

	const std::string stdHeader = ostr.str();
	assertTrue (stdHeader.find("// Foo.h") != std::string::npos);
	assertTrue (stdHeader.find("// Library: MyLib") != std::string::npos);
	assertTrue (stdHeader.find("// Package: MyPackage") != std::string::npos);
	assertTrue (stdHeader.find("// Module:  Foo") != std::string::npos);
	assertTrue (stdHeader.find("// This file has been generated.") != std::string::npos);
	assertTrue (stdHeader.find("// Copyright (c) 2026, Test") != std::string::npos);
}


void CodeGenerationTest::testCreateInclude()
{
	assertTrue (Poco::CodeGeneration::Utility::createInclude("include/MyLib/Foo.h", false) == "MyLib/Foo.h");
	assertTrue (Poco::CodeGeneration::Utility::createInclude("include/MyLib/Foo.h", true) == "Foo.h");
	assertTrue (Poco::CodeGeneration::Utility::createInclude("Foo.h", false) == "Foo.h");
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

	CppUnit_addTest(pSuite, CodeGenerationTest, testGenerateClass);
	CppUnit_addTest(pSuite, CodeGenerationTest, testStdHeader);
	CppUnit_addTest(pSuite, CodeGenerationTest, testCreateInclude);

	return pSuite;
}
