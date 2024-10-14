//
// CppParserTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppParserTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/CppToken.h"
#include <sstream>
#include <iostream>


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
std::string path("C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\IDE;C:\\Program Files\\Microsoft Visual Studio 8\\VC\\BIN;C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\Tools;C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\Tools\\bin");


CppParserTest::CppParserTest(const std::string& name): CppUnit::TestCase(name)
{
}


CppParserTest::~CppParserTest()
{
}


void CppParserTest::testParseDir()
{
	//FIXME: implement some proper tests
	return;
	NameSpace::SymbolTable st;
	std::vector<std::string> inc;
	inc.push_back("./*.h");
	std::vector<std::string> exc;
	Utility::parseDir(inc, exc, st, linker, options, path);

	NameSpace::SymbolTable::const_iterator it = st.begin();
	NameSpace::SymbolTable::const_iterator itEnd = st.end();
	for (; it != itEnd; ++it)
	{
		std::cout << it->first << ": ";
		Symbol* pSym = it->second;
		std::cout << pSym->name() << ", " << pSym->getDocumentation() << "\n";

	}
}


void CppParserTest::testExtractName()
{
	std::string decl("int _var");
	std::string name = Symbol::extractName(decl);
	assertTrue (name == "_var");

	decl = "void func(int arg1, int arg2)";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<bool> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<void(bool)> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<std::vector<int>(std::vector<bool>)> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<void*(std::function<const int*(void)>)> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<bool> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<void(bool)> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<std::vector<int>(std::vector<bool>)> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "std::function<void*(std::function<const int*(void)>)> func";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "const std::vector<NS::MyType>* var";
	name = Symbol::extractName(decl);
	assertTrue (name == "var");

	decl = "const std::vector<NS::MyType>* func(int arg) = 0";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "int (*func)(int, const std::string&)";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "int ( * func )(int, const std::string&)";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "template <typename A, typename B> B func(A a, B b)";
	name = Symbol::extractName(decl);
	assertTrue (name == "func");

	decl = "template <typename A, typename B> class Class";
	name = Symbol::extractName(decl);
	assertTrue (name == "Class");

	decl = "template <> class Class<int, std::string>";
	name = Symbol::extractName(decl);
	assertTrue (name == "Class");

	decl = "template <> class Class <int, std::string>";
	name = Symbol::extractName(decl);
	assertTrue (name == "Class");

	decl = "template <> class Class<int, MyTemplate<int> >";
	name = Symbol::extractName(decl);
	assertTrue (name == "Class");
}


void CppParserTest::testNumberLiterals()
{
	testNumberLiteral("123");
	testNumberLiteral("0123");
	testNumberLiteral("0x123");
	testNumberLiteral("0XABC");
	testNumberLiteral("0b010101");
	testNumberLiteral("0B101010");

	testNumberLiteral("123L");
	testNumberLiteral("0123L");
	testNumberLiteral("0x123L");
	testNumberLiteral("0XABCL");
	testNumberLiteral("0b010101L");
	testNumberLiteral("0B101010L");

	testNumberLiteral("123LL");
	testNumberLiteral("0123LL");
	testNumberLiteral("0x123LL");
	testNumberLiteral("0XABCLL");
	testNumberLiteral("0b010101LL");
	testNumberLiteral("0B101010LL");

	testNumberLiteral("123U");
	testNumberLiteral("0123U");
	testNumberLiteral("0x123U");
	testNumberLiteral("0XABCU");
	testNumberLiteral("0b010101U");
	testNumberLiteral("0B101010U");

	testNumberLiteral("123UL");
	testNumberLiteral("0123UL");
	testNumberLiteral("0x123UL");
	testNumberLiteral("0XABCUL");
	testNumberLiteral("0b010101UL");
	testNumberLiteral("0B101010UL");

	testNumberLiteral("1.23");
	testNumberLiteral(".123");
	testNumberLiteral("1.23e+3");
	testNumberLiteral("1.23E-3");

	testNumberLiteral("0x1fp1");
	testNumberLiteral("0x1f.e3p+12");
	testNumberLiteral("0x1f.e3p-12");

	testNumberLiteral("123'456");
	testNumberLiteral("1'234'567");
	testNumberLiteral("0x1234'5678");
	testNumberLiteral("0xFFFF'FFFFULL");
}


void CppParserTest::testNumberLiteral(const std::string& literal)
{
	NumberLiteralToken tok;

	std::istringstream istr(literal);
	assertTrue (tok.start(istr.get(), istr));
	tok.finish(istr);
	assertEquals (tok.tokenString(), literal);
}


void CppParserTest::setUp()
{
}


void CppParserTest::tearDown()
{
}


CppUnit::Test* CppParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CppParserTest");

	CppUnit_addTest(pSuite, CppParserTest, testParseDir);
	CppUnit_addTest(pSuite, CppParserTest, testExtractName);
	CppUnit_addTest(pSuite, CppParserTest, testNumberLiterals);

	return pSuite;
}
