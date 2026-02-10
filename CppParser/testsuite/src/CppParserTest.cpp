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
#include "Poco/CppParser/Parser.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Function.h"
#include <sstream>
#include <iostream>


using namespace Poco::CppParser;


std::string linker("cl");
std::string options(R"(/I "C:\Program Files\Microsoft Visual Studio 8\VC\INCLUDE", )"
	R"(/I "C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\include", )"
	R"(/I "p:\poco\Foundation\include", )"
	R"(/I "p:\poco\XML\include", )"
	R"(/I "p:\poco\Util\include", )"
	R"(/I "p:\poco\Net\include", )"
	R"(/D "WIN32", )"
	R"(/D "_DEBUG", )"
	R"(/D "_WINDOWS", )"
	R"(/D "_MBCS", )"
	"/C, /P, /TP");
std::string path(R"(C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;)"
	R"(C:\Program Files\Microsoft Visual Studio 8\VC\BIN;)"
	R"(C:\Program Files\Microsoft Visual Studio 8\Common7\Tools;)"
	R"(C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\bin)");


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


void CppParserTest::testRawStringLiteral(const std::string& prefix, const std::string& rawLiteral)
{
	// Raw string literals are consumed by IdentifierToken::finish() when the
	// identifier matches a raw string prefix (R, u8R, uR, UR, LR) and is
	// immediately followed by '"'.
	std::string full = prefix + rawLiteral;
	IdentifierToken tok;

	std::istringstream istr(full);
	assertTrue (tok.start(istr.get(), istr));
	tok.finish(istr);
	assertEquals (tok.tokenString(), full);
}


void CppParserTest::testStringSuffix(const std::string& literal, const std::string& expected)
{
	StringLiteralToken tok;

	std::istringstream istr(literal);
	assertTrue (tok.start(istr.get(), istr));
	tok.finish(istr);
	assertEquals (tok.tokenString(), expected);
}


void CppParserTest::parseString(const std::string& code, NameSpace::SymbolTable& st)
{
	// The parser expects preprocessed input with #line directives that
	// indicate which file the code belongs to.  Without a matching
	// directive the parser considers itself "not in file" and skips
	// all tokens.  We prepend a #line directive matching the filename
	// given to the Parser constructor.
	std::string input = "#line 1 \"test.h\"\n" + code;
	std::istringstream istr(input);
	Parser parser(st, "test.h", istr);
	parser.parse();
}


void CppParserTest::testTokenizerRawStrings()
{
	// Simplest raw string — the pattern that broke ObjectId.h
	testRawStringLiteral("R", R"---("(")")---");

	// Raw string with content
	testRawStringLiteral("R", R"---("(hello world)")---");

	// Custom delimiter
	testRawStringLiteral("R", R"("---(content)---")");

	// All prefix variants
	testRawStringLiteral("u8R", R"---("(text)")---");
	testRawStringLiteral("uR", R"---("(text)")---");
	testRawStringLiteral("UR", R"---("(text)")---");
	testRawStringLiteral("LR", R"---("(text)")---");

	// Content with special characters
	testRawStringLiteral("R", "\"(line1\nline2)\"");  // keep: \n must be a real newline char
	testRawStringLiteral("R", R"---("(has "quotes" inside)")---");

	// Empty content
	testRawStringLiteral("R", R"---("()")---");
}


void CppParserTest::testTokenizerStringSuffixes()
{
	// C++14 std::string literal suffix
	testStringSuffix(R"("hello"s)", R"("hello"s)");

	// C++17 std::string_view literal suffix
	testStringSuffix(R"("hello"sv)", R"("hello"sv)");

	// User-defined literal suffix
	testStringSuffix(R"("hello"_mylit)", R"("hello"_mylit)");

	// No suffix — unchanged behavior
	testStringSuffix(R"("hello" )", R"("hello")");
}


void CppParserTest::testParseStaticAssert()
{
	NameSpace::SymbolTable st;
	parseString(R"(static_assert(sizeof(int) >= 4, "int too small");)", st);
	// static_assert should be skipped without adding symbols
	assertTrue (st.empty());
}


void CppParserTest::testParseAlignas()
{
	// alignas on a member variable — this is the pattern from MPSCQueue.h
	// that caused the original infinite loop.  parseVarFunc() must consume
	// the alignas(...) prefix without misinterpreting it as a function call.
	NameSpace::SymbolTable st;
	parseString(R"(
struct CacheLine
{
    alignas(64) int data;
};
)", st);
	// The parser must not hang on alignas(...) and must find the struct
	assertTrue (st.find("CacheLine") != st.end());
}


void CppParserTest::testParseNoexceptExpr()
{
	NameSpace::SymbolTable st;
	parseString("void func() noexcept(true);\n", st);
	assertTrue (st.find("func") != st.end());

	Symbol* pSym = st.find("func")->second;
	Function* pFunc = dynamic_cast<Function*>(pSym);
	assertTrue (pFunc != nullptr);
	assertTrue (pFunc->flags() & Function::FN_NOEXCEPT);
}


void CppParserTest::testParseDeleteDefault()
{
	NameSpace::SymbolTable st;
	parseString(R"(
class Foo
{
public:
    Foo() = default;
    Foo(const Foo&) = delete;
};
)", st);
	assertTrue (st.find("Foo") != st.end());

	Symbol* pSym = st.find("Foo")->second;
	Struct* pClass = dynamic_cast<Struct*>(pSym);
	assertTrue (pClass != nullptr);

	// Find the default constructor and deleted copy constructor
	bool foundDefault = false;
	bool foundDeleted = false;
	for (auto it = pClass->begin(); it != pClass->end(); ++it)
	{
		Function* pFunc = dynamic_cast<Function*>(it->second);
		if (pFunc && pFunc->name() == "Foo")
		{
			if (pFunc->isDefault())
				foundDefault = true;
			if (pFunc->isDeleted())
				foundDeleted = true;
		}
	}
	assertTrue (foundDefault);
	assertTrue (foundDeleted);
}


void CppParserTest::testParseAnonymousNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace
{
    int hidden = 42;
}
)", st);
	// Anonymous namespace contents should be skipped — no symbols added
	assertTrue (st.empty());
}


void CppParserTest::testParseFriendInline()
{
	NameSpace::SymbolTable st;
	parseString(R"(
class Foo
{
public:
    friend bool operator==(const Foo& a, const Foo& b) { return true; }
    int value() const;
};
)", st);
	assertTrue (st.find("Foo") != st.end());

	Symbol* pSym = st.find("Foo")->second;
	Struct* pClass = dynamic_cast<Struct*>(pSym);
	assertTrue (pClass != nullptr);

	// The member function after the friend declaration should be visible
	bool foundValue = false;
	for (auto it = pClass->begin(); it != pClass->end(); ++it)
	{
		if (it->second->name() == "value")
			foundValue = true;
	}
	assertTrue (foundValue);
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
	CppUnit_addTest(pSuite, CppParserTest, testTokenizerRawStrings);
	CppUnit_addTest(pSuite, CppParserTest, testTokenizerStringSuffixes);
	CppUnit_addTest(pSuite, CppParserTest, testParseStaticAssert);
	CppUnit_addTest(pSuite, CppParserTest, testParseAlignas);
	CppUnit_addTest(pSuite, CppParserTest, testParseNoexceptExpr);
	CppUnit_addTest(pSuite, CppParserTest, testParseDeleteDefault);
	CppUnit_addTest(pSuite, CppParserTest, testParseAnonymousNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseFriendInline);

	return pSuite;
}
