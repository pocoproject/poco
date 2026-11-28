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
#include "Poco/CppParser/Decl.h"
#include "Poco/CppParser/CppToken.h"
#include "Poco/CppParser/Parser.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Variable.h"
#include "Poco/Exception.h"
#include <sstream>
#include <iostream>
#include <vector>


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


void CppParserTest::testExtractNameTemplateHead()
{
	std::string decl("template <std::size_t S = (64 * 1024), class M = Mutex> class Handler");
	std::string name = Symbol::extractName(decl);
	assertTrue (name == "Handler");

	decl = "template <typename T> requires (sizeof(T) > 4) && Integral<T> T wide";
	name = Symbol::extractName(decl);
	assertTrue (name == "wide");

	decl = "template <typename T> requires requires (T t) { t.x = 1; } class Point";
	name = Symbol::extractName(decl);
	assertTrue (name == "Point");

	decl = "template <typename T> concept Big = sizeof(T) > 4";
	name = Symbol::extractName(decl);
	assertTrue (name == "Big");

	decl = "template <typename T> requires (Integral<T>) && true T flag";
	name = Symbol::extractName(decl);
	assertTrue (name == "flag");

	decl = "template <typename T> requires Outer < int >::Inner < T >::value && (sizeof(T) > 4) T deep";
	name = Symbol::extractName(decl);
	assertTrue (name == "deep");

	decl = "template <typename F> requires requires { & F::operator (); } void callIt";
	name = Symbol::extractName(decl);
	assertTrue (name == "callIt");

	decl = "template <typename F> concept Indexable = requires (F f) { f.operator[](0); }";
	name = Symbol::extractName(decl);
	assertTrue (name == "Indexable");

	decl = "template <typename T> requires std::bool_constant < (sizeof(T) > sizeof(int)) >::value void large";
	name = Symbol::extractName(decl);
	assertTrue (name == "large");

	decl = "template <typename T> requires Integral < T > and (sizeof(T) > 1) void both";
	name = Symbol::extractName(decl);
	assertTrue (name == "both");

	decl = "template <typename T> requires T::template Pred < int >::value && (sizeof(T) > 1) void head";
	name = Symbol::extractName(decl);
	assertTrue (name == "head");

	decl = "template <typename T> requires (sizeof(\"\\\")\") > 1 && ')' != '(') void escaped";
	name = Symbol::extractName(decl);
	assertTrue (name == "escaped");

	decl = R"decl(template <typename T> requires (sizeof(R"tag(()tag") > 1) int value = 0)decl";
	name = Symbol::extractName(decl);
	assertTrue (name == "value");

	decl = "template <class T> requires T :: template Pred < int >::value && (true) int answer";
	name = Symbol::extractName(decl);
	assertTrue (name == "answer");
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


void CppParserTest::parseFollowedByPlainDeclarations(const std::string& code, NameSpace::SymbolTable& st)
{
	// A construct that the parser does not understand ends parsing or makes
	// it skip the declarations after it, so the code is followed by a plain
	// class and a plain function that must be found.
	parseString(code + "\nclass After\n{\npublic:\n\tint value() const;\n};\n\nint afterFunc(int x);\n", st);
	assertTrue (st.find("After") != st.end());
	assertTrue (st.find("value") != st.end());
	assertTrue (st.find("afterFunc") != st.end());
}


void CppParserTest::parseExpectingSyntaxError(const std::string& code)
{
	NameSpace::SymbolTable st;
	try
	{
		parseString(code, st);
		fail("invalid code must throw: " + code);
	}
	catch (Poco::SyntaxException&)
	{
	}
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

	// The class and its constructors have the same name; the class is declared first.
	Symbol* pSym = st.lower_bound("Foo")->second;
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


void CppParserTest::testParseNestedNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace A::B::C
{
    class Foo
    {
    public:
        void bar();
    };
}
)", st);
	
	// Check that namespace A exists
	assertTrue (st.find("A") != st.end());
	Symbol* pSymA = st.find("A")->second;
	NameSpace* pNSA = dynamic_cast<NameSpace*>(pSymA);
	assertTrue (pNSA != nullptr);
	
	// Check that namespace B exists within A
	Symbol* pSymB = pNSA->lookup("B");
	assertTrue (pSymB != nullptr);
	NameSpace* pNSB = dynamic_cast<NameSpace*>(pSymB);
	assertTrue (pNSB != nullptr);
	
	// Check that namespace C exists within B
	Symbol* pSymC = pNSB->lookup("C");
	assertTrue (pSymC != nullptr);
	NameSpace* pNSC = dynamic_cast<NameSpace*>(pSymC);
	assertTrue (pNSC != nullptr);
	
	// Check that class Foo exists within C
	Symbol* pSymFoo = pNSC->lookup("Foo");
	assertTrue (pSymFoo != nullptr);
	Struct* pFoo = dynamic_cast<Struct*>(pSymFoo);
	assertTrue (pFoo != nullptr);
	
	// Check the full name
	assertTrue (pFoo->fullName() == "A::B::C::Foo");
}


void CppParserTest::testParseInlineNestedNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace A::inline B::C
{
    void func();
}
)", st);
	
	// Check that namespace A exists
	assertTrue (st.find("A") != st.end());
	Symbol* pSymA = st.find("A")->second;
	NameSpace* pNSA = dynamic_cast<NameSpace*>(pSymA);
	assertTrue (pNSA != nullptr);
	assertTrue (!pNSA->isInline());
	
	// Check that namespace B exists within A and is inline
	Symbol* pSymB = pNSA->lookup("B");
	assertTrue (pSymB != nullptr);
	NameSpace* pNSB = dynamic_cast<NameSpace*>(pSymB);
	assertTrue (pNSB != nullptr);
	assertTrue (pNSB->isInline());
	
	// Check that namespace C exists within B and is not inline
	Symbol* pSymC = pNSB->lookup("C");
	assertTrue (pSymC != nullptr);
	NameSpace* pNSC = dynamic_cast<NameSpace*>(pSymC);
	assertTrue (pNSC != nullptr);
	assertTrue (!pNSC->isInline());
	
	// Check that func exists within C
	Symbol* pSymFunc = pNSC->lookup("func");
	assertTrue (pSymFunc != nullptr);
	assertTrue (pSymFunc->fullName() == "A::B::C::func");
}


void CppParserTest::testParseRepeatedNestedNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace A::B
{
	class X {};
}
namespace A::B
{
	class Y {};
}
)", st);

	assertTrue (st.find("A") != st.end());
	NameSpace* pNSA = dynamic_cast<NameSpace*>(st.find("A")->second);
	assertTrue (pNSA != nullptr);

	NameSpace* pNSB = dynamic_cast<NameSpace*>(pNSA->lookup("B"));
	assertTrue (pNSB != nullptr);

	Symbol* pSymX = pNSB->lookup("X");
	assertTrue (pSymX != nullptr);
	Struct* pX = dynamic_cast<Struct*>(pSymX);
	assertTrue (pX != nullptr);

	Symbol* pSymY = pNSB->lookup("Y");
	assertTrue (pSymY != nullptr);
	Struct* pY = dynamic_cast<Struct*>(pSymY);
	assertTrue (pY != nullptr);

	assertTrue (pX->fullName() == "A::B::X");
	assertTrue (pY->fullName() == "A::B::Y");
}


void CppParserTest::testParseNamespaceShadowingOuterNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace Shadow {
namespace Detail {
void outerFunc();
}
namespace Inner {
namespace Detail {
void innerFunc();
}
}
namespace Detail {
void outerFunc2();
}
}
namespace Shadow::Inner::Detail {
void innerFunc2();
}
)", st);

	const auto fullNameOf = [this, &st](const std::string& name)
	{
		const auto it = st.find(name);
		assertTrue (it != st.end());
		return it->second->fullName();
	};

	assertTrue (fullNameOf("outerFunc") == "Shadow::Detail::outerFunc");
	assertTrue (fullNameOf("outerFunc2") == "Shadow::Detail::outerFunc2");
	assertTrue (fullNameOf("innerFunc") == "Shadow::Inner::Detail::innerFunc");
	assertTrue (fullNameOf("innerFunc2") == "Shadow::Inner::Detail::innerFunc2");
}


void CppParserTest::testParseSingleNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace Solo
{
	class Only {};
}
)", st);

	assertTrue (st.find("Solo") != st.end());
	NameSpace* pNSSolo = dynamic_cast<NameSpace*>(st.find("Solo")->second);
	assertTrue (pNSSolo != nullptr);

	Symbol* pSymOnly = pNSSolo->lookup("Only");
	assertTrue (pSymOnly != nullptr);
	Struct* pOnly = dynamic_cast<Struct*>(pSymOnly);
	assertTrue (pOnly != nullptr);
	assertTrue (pOnly->fullName() == "Solo::Only");
}


void CppParserTest::testParseDeepNestedNamespace()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace A::B::C::D::E
{
	class Deep {};
}
class GlobalAfter {};
)", st);

	assertTrue (st.find("A") != st.end());
	NameSpace* pNSA = dynamic_cast<NameSpace*>(st.find("A")->second);
	assertTrue (pNSA != nullptr);

	NameSpace* pNSB = dynamic_cast<NameSpace*>(pNSA->lookup("B"));
	assertTrue (pNSB != nullptr);
	NameSpace* pNSC = dynamic_cast<NameSpace*>(pNSB->lookup("C"));
	assertTrue (pNSC != nullptr);
	NameSpace* pNSD = dynamic_cast<NameSpace*>(pNSC->lookup("D"));
	assertTrue (pNSD != nullptr);
	NameSpace* pNSE = dynamic_cast<NameSpace*>(pNSD->lookup("E"));
	assertTrue (pNSE != nullptr);

	Symbol* pSymDeep = pNSE->lookup("Deep");
	assertTrue (pSymDeep != nullptr);
	Struct* pDeep = dynamic_cast<Struct*>(pSymDeep);
	assertTrue (pDeep != nullptr);
	assertTrue (pDeep->fullName() == "A::B::C::D::E::Deep");

	// Verify parser state after namespace pop loop by checking a following global symbol.
	assertTrue (st.find("GlobalAfter") != st.end());
	Symbol* pSymGlobalAfter = st.find("GlobalAfter")->second;
	assertTrue (pSymGlobalAfter != nullptr);
	Struct* pGlobalAfter = dynamic_cast<Struct*>(pSymGlobalAfter);
	assertTrue (pGlobalAfter != nullptr);
	assertTrue (pGlobalAfter->fullName() == "GlobalAfter");
}


void CppParserTest::testLookupOverloadedFunction()
{
	NameSpace::SymbolTable st;
	parseString(R"(
namespace Overloaded
{
	void func();
	void func(int a);
	void func(int a, int b);
}
)", st);

	const auto itNS = st.find("Overloaded");
	assertTrue (itNS != st.end());
	auto* pNS = dynamic_cast<NameSpace*>(itNS->second);
	assertTrue (pNS != nullptr);

	auto* pFunc = dynamic_cast<Function*>(pNS->lookup("func"));
	assertTrue (pFunc != nullptr);
	assertTrue (pFunc->fullName() == "Overloaded::func");
	assertTrue (pFunc->countParameters() == 0);
}


void CppParserTest::testParseConcept()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
concept Integral = std::is_integral_v<T>;
	/// Integral documentation.

template <typename T>
concept Wide = sizeof(T) > 4;
)", st);

	const auto itIntegral = st.find("Integral");
	assertTrue (itIntegral != st.end());
	assertTrue (itIntegral->second->kind() == Symbol::SYM_CONCEPT);
	const auto* pIntegral = dynamic_cast<const Decl*>(itIntegral->second);
	assertTrue (pIntegral != nullptr);
	assertTrue (pIntegral->declaration().find("concept Integral") != std::string::npos);
	assertTrue (pIntegral->declaration().find("is_integral_v") != std::string::npos);
	assertTrue (pIntegral->getDocumentation().find("Integral documentation.") != std::string::npos);

	const auto itWide = st.find("Wide");
	assertTrue (itWide != st.end());
	assertTrue (itWide->second->kind() == Symbol::SYM_CONCEPT);
}


void CppParserTest::testParseConceptRequiresExpression()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
concept HasSize = requires (const T& t)
{
	{ t.size() } -> std::convertible_to<std::size_t>;
	t.empty();
};
	/// HasSize documentation.
)", st);

	const auto it = st.find("HasSize");
	assertTrue (it != st.end());
	assertTrue (it->second->kind() == Symbol::SYM_CONCEPT);
	const auto* pConcept = dynamic_cast<const Decl*>(it->second);
	assertTrue (pConcept != nullptr);
	const std::string& decl = pConcept->declaration();
	assertTrue (decl.find("convertible_to") != std::string::npos);
	assertTrue (decl.find("empty") != std::string::npos);
	assertTrue (decl.back() == '}');
	assertTrue (pConcept->getDocumentation().find("HasSize documentation.") != std::string::npos);
}


void CppParserTest::testParseConceptConjunction()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
concept SizedIntegral = Integral<T> && (HasSize<T> || std::is_enum_v<T>);
)", st);

	const auto it = st.find("SizedIntegral");
	assertTrue (it != st.end());
	assertTrue (it->second->kind() == Symbol::SYM_CONCEPT);
	const auto* pConcept = dynamic_cast<const Decl*>(it->second);
	assertTrue (pConcept != nullptr);
	assertTrue (pConcept->declaration().find("is_enum_v") != std::string::npos);
}


void CppParserTest::testParseFunctionTrailingRequires()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
T twice(T v) requires Integral<T>;
)", st);

	const auto it = st.find("twice");
	assertTrue (it != st.end());
	const auto* pFunc = dynamic_cast<const Function*>(it->second);
	assertTrue (pFunc != nullptr);
	assertTrue (pFunc->countParameters() == 1);
	assertTrue (pFunc->getRequiresClause() == "requires Integral < T >");
}


void CppParserTest::testParseFunctionTemplateHeadRequires()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires Integral<T>
T half(T v);

template <typename T> requires requires (const T& t) { t.size(); }
std::size_t count(const T& t);
)", st);

	assertTrue (st.find("requires") == st.end());

	const auto itHalf = st.find("half");
	assertTrue (itHalf != st.end());
	const auto* pHalf = dynamic_cast<const Function*>(itHalf->second);
	assertTrue (pHalf != nullptr);
	assertTrue (pHalf->declaration().find("requires Integral") != std::string::npos);
	assertTrue (pHalf->countParameters() == 1);
	assertTrue (pHalf->getReturnParameter() == "T");

	const auto itCount = st.find("count");
	assertTrue (itCount != st.end());
	const auto* pCount = dynamic_cast<const Function*>(itCount->second);
	assertTrue (pCount != nullptr);
	assertTrue (pCount->declaration().find("requires requires") != std::string::npos);
	assertTrue (pCount->countParameters() == 1);
}


void CppParserTest::testParseFunctionBothRequires()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires Integral<T>
T both(T v) requires HasSize<T>;
)", st);

	const auto it = st.find("both");
	assertTrue (it != st.end());
	const auto* pFunc = dynamic_cast<const Function*>(it->second);
	assertTrue (pFunc != nullptr);
	assertTrue (pFunc->countParameters() == 1);
}


void CppParserTest::testParseParenthesizedRequires()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires (!std::is_pointer_v<T>)
void store(T v);

template <typename T>
void keep(T v) requires (!std::is_pointer_v<T> && sizeof(T) > 1);
)", st);

	assertTrue (st.find("requires") == st.end());

	const auto itStore = st.find("store");
	assertTrue (itStore != st.end());
	const auto* pStore = dynamic_cast<const Function*>(itStore->second);
	assertTrue (pStore != nullptr);
	assertTrue (pStore->declaration().find("is_pointer_v") != std::string::npos);
	assertTrue (pStore->countParameters() == 1);

	const auto itKeep = st.find("keep");
	assertTrue (itKeep != st.end());
	assertTrue (dynamic_cast<const Function*>(itKeep->second) != nullptr);
}


void CppParserTest::testParseRequiresConjunction()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
T sum(T a, T b) requires Integral<T> && HasSize<T>;

template <typename T> requires std::is_integral<T>::value || std::is_same_v<T, std::vector<int>>
T pick(T v);

template <typename T> requires true
T always(T v) requires (Integral<T>) && false;

template <typename T> requires (Integral<T>) && false
T never(T v) requires true;

template <typename T> requires std::is_integral<T>::type::value && Outer<int>::Inner<T>::value
T nested(T v) requires std::is_integral<T>::type::value && Outer<int>::Inner<T>::value;
)", st);

	assertTrue (st.find("requires") == st.end());

	const auto itAlways = st.find("always");
	assertTrue (itAlways != st.end());
	const auto* pAlways = dynamic_cast<const Function*>(itAlways->second);
	assertTrue (pAlways != nullptr);
	assertTrue (pAlways->declaration().find("requires true") != std::string::npos);
	assertTrue (pAlways->countParameters() == 1);

	const auto itNever = st.find("never");
	assertTrue (itNever != st.end());
	const auto* pNever = dynamic_cast<const Function*>(itNever->second);
	assertTrue (pNever != nullptr);
	assertTrue (pNever->declaration().find("&& false") != std::string::npos);
	assertTrue (pNever->countParameters() == 1);

	const auto itNested = st.find("nested");
	assertTrue (itNested != st.end());
	const auto* pNested = dynamic_cast<const Function*>(itNested->second);
	assertTrue (pNested != nullptr);
	assertTrue (pNested->declaration().find(">::type::value &&") != std::string::npos);
	assertTrue (pNested->declaration().find(">::Inner") != std::string::npos);
	assertTrue (pNested->countParameters() == 1);

	const auto itSum = st.find("sum");
	assertTrue (itSum != st.end());
	const auto* pSum = dynamic_cast<const Function*>(itSum->second);
	assertTrue (pSum != nullptr);
	assertTrue (pSum->countParameters() == 2);

	const auto itPick = st.find("pick");
	assertTrue (itPick != st.end());
	const auto* pPick = dynamic_cast<const Function*>(itPick->second);
	assertTrue (pPick != nullptr);
	assertTrue (pPick->declaration().find(">::value ||") != std::string::npos);
	assertTrue (pPick->countParameters() == 1);
}


void CppParserTest::testParseClassTemplateRequires()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires Integral<T>
class Counter
	/// Counter documentation.
{
public:
	explicit Counter(T start);
	T count() const;
};
)", st);

	// The class and its constructor have the same name; the class is declared first.
	const auto it = st.lower_bound("Counter");
	assertTrue (it != st.end());
	const auto* pClass = dynamic_cast<const Struct*>(it->second);
	assertTrue (pClass != nullptr);
	assertTrue (pClass->declaration().find("requires Integral") != std::string::npos);
	assertTrue (pClass->getDocumentation().find("Counter documentation.") != std::string::npos);
	assertTrue (pClass->lookup("count") != nullptr);
}


void CppParserTest::testParseMemberTemplateRequires()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
class Holder
{
public:
	template <typename T>
	void set(T&& v) requires (!std::is_pointer_v<std::remove_cvref_t<T>>);
	int size() const;
};
)", st);

	const auto it = st.find("Holder");
	assertTrue (it != st.end());
	const auto* pClass = dynamic_cast<const Struct*>(it->second);
	assertTrue (pClass != nullptr);
	assertTrue (dynamic_cast<const Function*>(pClass->lookup("set")) != nullptr);
	assertTrue (dynamic_cast<const Function*>(pClass->lookup("size")) != nullptr);
}


void CppParserTest::testParseMemberRequiresQualifiers()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
class Box
{
public:
	Box(const Box& other) requires std::copyable<T> = default;
	T value() const requires Integral<T>;
	void swap(Box& other) noexcept requires std::swappable<T>;
	void clear() requires HasSize<T> { }
	int plain() const;
};
)", st);

	// The class and its constructor have the same name; the class is declared first.
	const auto it = st.lower_bound("Box");
	assertTrue (it != st.end());
	const auto* pClass = dynamic_cast<const Struct*>(it->second);
	assertTrue (pClass != nullptr);

	bool foundCopy = false;
	for (const auto& [name, pSym]: *pClass)
	{
		const auto* pFunc = dynamic_cast<const Function*>(pSym);
		if (pFunc != nullptr && pFunc->isConstructor() && pFunc->isDefault())
			foundCopy = true;
	}
	assertTrue (foundCopy);

	const auto* pValue = dynamic_cast<const Function*>(pClass->lookup("value"));
	assertTrue (pValue != nullptr);
	assertTrue (pValue->isConst());

	const auto* pSwap = dynamic_cast<const Function*>(pClass->lookup("swap"));
	assertTrue (pSwap != nullptr);
	assertTrue (pSwap->flags() & Function::FN_NOEXCEPT);

	const auto* pClear = dynamic_cast<const Function*>(pClass->lookup("clear"));
	assertTrue (pClear != nullptr);
	assertTrue (pClear->flags() & Function::FN_INLINE);

	assertTrue (pClass->lookup("plain") != nullptr);
}


void CppParserTest::testParseConstrainedParameters()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <Integral T>
T half(T v);

void print(const Integral auto& v);
)", st);

	const auto itHalf = st.find("half");
	assertTrue (itHalf != st.end());
	assertTrue (dynamic_cast<const Function*>(itHalf->second) != nullptr);

	const auto itPrint = st.find("print");
	assertTrue (itPrint != st.end());
	assertTrue (dynamic_cast<const Function*>(itPrint->second) != nullptr);
}


void CppParserTest::testParseOperatorInConstraint()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename F>
concept HasCallOp = requires { &F::operator(); };

template <typename F>
concept Indexable = requires (F f) { f.operator[](0); };

template <typename F> requires requires { &F::operator(); }
void callIt(F f);

template <typename F> requires requires (F f) { f.operator bool(); }
void convertIt(F f);

template <typename T> requires Integral<T>
bool operator == (const T& a, const T& b);
)", st);

	assertTrue (st.find("operator ()") == st.end());
	assertTrue (st.find("operator []") == st.end());

	const auto itCallOp = st.find("HasCallOp");
	assertTrue (itCallOp != st.end());
	assertTrue (itCallOp->second->kind() == Symbol::SYM_CONCEPT);
	const auto itIndexable = st.find("Indexable");
	assertTrue (itIndexable != st.end());
	assertTrue (itIndexable->second->kind() == Symbol::SYM_CONCEPT);

	const auto itCallIt = st.find("callIt");
	assertTrue (itCallIt != st.end());
	assertTrue (dynamic_cast<const Function*>(itCallIt->second) != nullptr);
	const auto itConvertIt = st.find("convertIt");
	assertTrue (itConvertIt != st.end());
	assertTrue (dynamic_cast<const Function*>(itConvertIt->second) != nullptr);

	const auto itEqual = st.find("operator ==");
	assertTrue (itEqual != st.end());
	assertTrue (dynamic_cast<const Function*>(itEqual->second) != nullptr);
}


void CppParserTest::testParseRequiresTemplateArgumentComparison()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
void small(T v) requires std::bool_constant<(sizeof(T) > 1)>::value;

template <typename T> requires std::bool_constant<(sizeof(T) > sizeof(int))>::value
void large(T v);
)", st);

	assertTrue (st.find("sizeof") == st.end());

	const auto itSmall = st.find("small");
	assertTrue (itSmall != st.end());
	assertTrue (dynamic_cast<const Function*>(itSmall->second) != nullptr);

	const auto itLarge = st.find("large");
	assertTrue (itLarge != st.end());
	const auto* pLarge = dynamic_cast<const Function*>(itLarge->second);
	assertTrue (pLarge != nullptr);
	assertTrue (pLarge->countParameters() == 1);
}


void CppParserTest::testParseRequiresAlternativeTokens()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires Integral<T> and (sizeof(T) > 1)
void both(T v);

template <typename T>
void either(T v) requires (Integral<T>) or HasSize<T>;
)", st);

	assertTrue (st.find("and") == st.end());

	const auto itBoth = st.find("both");
	assertTrue (itBoth != st.end());
	const auto* pBoth = dynamic_cast<const Function*>(itBoth->second);
	assertTrue (pBoth != nullptr);
	assertTrue (pBoth->declaration().find(" and ") != std::string::npos);

	const auto itEither = st.find("either");
	assertTrue (itEither != st.end());
	assertTrue (dynamic_cast<const Function*>(itEither->second) != nullptr);
}


void CppParserTest::testParseRequiresTemplateDisambiguator()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires T::template Pred<int>::value && (sizeof(T) > 1)
void head(T v);

template <typename T>
void trailing(T v) requires T::template Pred<int>::value;
)", st);

	const auto itHead = st.find("head");
	assertTrue (itHead != st.end());
	const auto* pHead = dynamic_cast<const Function*>(itHead->second);
	assertTrue (pHead != nullptr);
	assertTrue (pHead->declaration().find("T::template Pred") != std::string::npos);

	const auto itTrailing = st.find("trailing");
	assertTrue (itTrailing != st.end());
	assertTrue (dynamic_cast<const Function*>(itTrailing->second) != nullptr);
}


void CppParserTest::testParseRequiresLiteral()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"code(
template <typename T> requires (sizeof(")") == sizeof(char[2]))
void quoted(T v);

template <typename T> requires (sizeof("\")") > 1 && ')' != '(')
void escaped(T v);
)code", st);

	assertTrue (st.find("sizeof") == st.end());

	const auto itQuoted = st.find("quoted");
	assertTrue (itQuoted != st.end());
	assertTrue (dynamic_cast<const Function*>(itQuoted->second) != nullptr);

	const auto itEscaped = st.find("escaped");
	assertTrue (itEscaped != st.end());
	assertTrue (dynamic_cast<const Function*>(itEscaped->second) != nullptr);
}


void CppParserTest::testParseRequiresExpressionOutsideConcept()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
inline constexpr bool hasX = requires { typename T::x; };

template <typename T>
inline constexpr bool hasF = requires (T t) { t.f(); };

static const int SHIFTED = (1 << 3);

template <typename T>
struct Checked
{
	static_assert(requires (T t) { t.f(); }, "needs f");
	static constexpr bool hasG = requires (T t) { t.f(); t.g(); };
	int afterChecks();
};
)", st);

	for (const auto* pName: {"hasX", "hasF", "SHIFTED", "hasG"})
	{
		const auto it = st.find(pName);
		assertTrue (it != st.end());
		assertTrue (it->second->kind() == Symbol::SYM_VARIABLE);
	}
	assertTrue (st.find("afterChecks") != st.end());
}


void CppParserTest::testParseConstrainedMembers()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
class Holder
{
public:
	explicit Holder(T v) requires Integral<T>: _v(v), _w(0) { }
	~Holder() requires std::is_trivially_destructible_v<T> = default;
	~Holder() requires (!std::is_trivially_destructible_v<T>);
	T get() const;
	int afterHolder();

private:
	T _v;
	int _w;
};

template <typename T> requires Integral<T>
T Holder<T>::get() const
{
	return _v;
}

template <typename T> requires Integral<T>
struct Pair
{
	T first;
	T second;
};

template <typename T> requires Integral<T>
union Either
{
	T value;
	char bytes[4];
};
)", st);

	// The class and its constructor have the same name; the class is declared first.
	const auto itHolder = st.lower_bound("Holder");
	assertTrue (itHolder != st.end());
	const auto* pHolder = dynamic_cast<const Struct*>(itHolder->second);
	assertTrue (pHolder != nullptr);
	int destructors = 0;
	bool foundConstructor = false;
	for (const auto& [name, pSym]: *pHolder)
	{
		const auto* pFunc = dynamic_cast<const Function*>(pSym);
		if (pFunc != nullptr && pFunc->isDestructor())
			++destructors;
		if (pFunc != nullptr && pFunc->isConstructor() && pFunc->countParameters() == 1)
			foundConstructor = true;
	}
	assertTrue (destructors == 2);
	assertTrue (foundConstructor);
	assertTrue (pHolder->lookup("afterHolder") != nullptr);

	const auto itPair = st.find("Pair");
	assertTrue (itPair != st.end());
	assertTrue (dynamic_cast<const Struct*>(itPair->second) != nullptr);
	const auto itEither = st.find("Either");
	assertTrue (itEither != st.end());
	assertTrue (dynamic_cast<const Struct*>(itEither->second) != nullptr);
}


void CppParserTest::testParseConceptForms()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename... Ts>
concept AllIntegral = (Integral<Ts> && ...);

template <typename T>
concept Deep = requires (T t)
{
	requires Integral<T>;
	requires requires (T x) { x.inner(); };
	{ t.f() } noexcept -> std::same_as<int>;
	typename T::value_type;
} && std::copyable<T>;
	/// Deep documentation.
)", st);

	const auto itAll = st.find("AllIntegral");
	assertTrue (itAll != st.end());
	assertTrue (itAll->second->kind() == Symbol::SYM_CONCEPT);

	const auto itDeep = st.find("Deep");
	assertTrue (itDeep != st.end());
	assertTrue (itDeep->second->kind() == Symbol::SYM_CONCEPT);
	const auto* pDeep = dynamic_cast<const Decl*>(itDeep->second);
	assertTrue (pDeep != nullptr);
	assertTrue (pDeep->declaration().find("copyable") != std::string::npos);
	assertTrue (pDeep->getDocumentation().find("Deep documentation.") != std::string::npos);
}


void CppParserTest::testParseRequiresSyntaxErrors()
{
	parseExpectingSyntaxError("template <typename T> requires");
	parseExpectingSyntaxError("template <typename T> requires Integral<T> &&");
	parseExpectingSyntaxError("template <typename T> void f(T v) requires 42;");
	parseExpectingSyntaxError("template <typename T> requires requires (T t) void f(T v);");
	parseExpectingSyntaxError("template <typename T> concept = true;");
	parseExpectingSyntaxError("template <typename T> concept");
	parseExpectingSyntaxError("template <typename T> concept Open = true");
}


void CppParserTest::testParseVariableInitializer()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
static constexpr bool A2 = sizeof(long) > 4;
inline const int VALUE = compute(*DEFAULT_PTR);
static const std::size_t N = sizeof(T) * N_ELEMENTS;
)", st);

	const auto itA2 = st.find("A2");
	assertTrue (itA2 != st.end());
	const auto* pA2 = dynamic_cast<const Variable*>(itA2->second);
	assertTrue (pA2 != nullptr);
	assertTrue (pA2->declaration().find("sizeof") != std::string::npos);
	assertTrue (pA2->declaration().find("> 4") != std::string::npos);

	const auto itValue = st.find("VALUE");
	assertTrue (itValue != st.end());
	assertTrue (itValue->second->kind() == Symbol::SYM_VARIABLE);
	assertTrue (st.find("DEFAULT_PTR") == st.end());

	const auto itN = st.find("N");
	assertTrue (itN != st.end());
	const auto* pN = dynamic_cast<const Variable*>(itN->second);
	assertTrue (pN != nullptr);
	assertTrue (pN->declType() == "std::size_t");
	assertTrue (!pN->isPointer());
}


void CppParserTest::testParseFunctionTemplateReturnType()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T, bool B = (sizeof(T) > 4)>
T pick(T v);

template <typename T, bool B = requires (T t) { { t.f() } -> std::same_as<int>; }>
void probe(T v);
)", st);

	const auto itPick = st.find("pick");
	assertTrue (itPick != st.end());
	const auto* pPick = dynamic_cast<const Function*>(itPick->second);
	assertTrue (pPick != nullptr);
	assertTrue (pPick->getReturnParameter() == "T");

	const auto itProbe = st.find("probe");
	assertTrue (itProbe != st.end());
	const auto* pProbe = dynamic_cast<const Function*>(itProbe->second);
	assertTrue (pProbe != nullptr);
	assertTrue (pProbe->getReturnParameter() == "void");
}


void CppParserTest::testParseTrailingRequiresOverloads()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T>
class Chooser
{
public:
	void f(T v) requires A<T>;
	void f(T v) requires B<T>;
};
)", st);

	const auto it = st.find("Chooser");
	assertTrue (it != st.end());
	const auto* pClass = dynamic_cast<const Struct*>(it->second);
	assertTrue (pClass != nullptr);
	std::vector<std::string> clauses;
	for (const auto& [name, pSym]: *pClass)
	{
		const auto* pFunc = dynamic_cast<const Function*>(pSym);
		if (pFunc != nullptr && name == "f")
			clauses.push_back(pFunc->getRequiresClause());
	}
	assertTrue (clauses.size() == 2);
	assertTrue (clauses[0].find("A") != std::string::npos);
	assertTrue (clauses[1].find("B") != std::string::npos);
}


void CppParserTest::testParseRequiresDecltype()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires decltype(T{})::value
void dt(T v);
)", st);

	const auto it = st.find("dt");
	assertTrue (it != st.end());
	const auto* pFunc = dynamic_cast<const Function*>(it->second);
	assertTrue (pFunc != nullptr);
	assertTrue (pFunc->getReturnParameter() == "void");
}


void CppParserTest::testParseRequiresExpressionInTemplateArgument()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
template <typename T> requires C<requires (T t) { t > 0; }>
void gt(T v);
)", st);

	const auto it = st.find("gt");
	assertTrue (it != st.end());
	const auto* pFunc = dynamic_cast<const Function*>(it->second);
	assertTrue (pFunc != nullptr);
	assertTrue (pFunc->countParameters() == 1);
}


void CppParserTest::testParseNestedGroupsInTemplateArguments()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
void fill(std::array<int, (N < 2 ? 1 : 2)> a, int b);
void g(std::vector<int> v = {1, 2}, int c);
std::array<int, (N < 2 ? 1 : 2)> make();
)", st);

	const auto itFill = st.find("fill");
	assertTrue (itFill != st.end());
	const auto* pFill = dynamic_cast<const Function*>(itFill->second);
	assertTrue (pFill != nullptr);
	assertTrue (pFill->countParameters() == 2);

	const auto itG = st.find("g");
	assertTrue (itG != st.end());
	const auto* pG = dynamic_cast<const Function*>(itG->second);
	assertTrue (pG != nullptr);
	assertTrue (pG->countParameters() == 2);

	const auto itMake = st.find("make");
	assertTrue (itMake != st.end());
	const auto* pMake = dynamic_cast<const Function*>(itMake->second);
	assertTrue (pMake != nullptr);
	assertTrue (pMake->countParameters() == 0);
}


void CppParserTest::testParseQualifiedBaseClass()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
class D: public Outer<int>::Inner
{
};

class E: public ns::Base<int>
{
};
)", st);

	const auto itD = st.find("D");
	assertTrue (itD != st.end());
	const auto* pD = dynamic_cast<const Struct*>(itD->second);
	assertTrue (pD != nullptr);
	assertTrue (pD->baseEnd() - pD->baseBegin() == 1);
	assertTrue (pD->baseBegin()->name.find("Inner") != std::string::npos);

	const auto itE = st.find("E");
	assertTrue (itE != st.end());
	const auto* pE = dynamic_cast<const Struct*>(itE->second);
	assertTrue (pE != nullptr);
	assertTrue (pE->baseEnd() - pE->baseBegin() == 1);
	assertTrue (pE->baseBegin()->name == "ns::Base < int >");

	NameSpace::SymbolTable st2;
	parseFollowedByPlainDeclarations(R"(
struct G: ::ns::Base<int>
{
};

struct H: public decltype(makeBase())
{
};
)", st2);

	const auto itG = st2.find("G");
	assertTrue (itG != st2.end());
	const auto* pG = dynamic_cast<const Struct*>(itG->second);
	assertTrue (pG != nullptr);
	assertTrue (pG->baseEnd() - pG->baseBegin() == 1);
	assertTrue (pG->baseBegin()->name == "::ns::Base < int >");

	const auto itH = st2.find("H");
	assertTrue (itH != st2.end());
	const auto* pH = dynamic_cast<const Struct*>(itH->second);
	assertTrue (pH != nullptr);
	assertTrue (pH->baseEnd() - pH->baseBegin() == 1);
	assertTrue (pH->baseBegin()->name.find("makeBase") != std::string::npos);
}


void CppParserTest::testParseFreeFunctionAttributes()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
namespace Poco::Impl {
[[noreturn]] void Foundation_API throwAnyCastError(const char* function, const std::type_info* pHeld, const std::type_info& requested);
[[nodiscard]] bool Foundation_API isJSONString(const Var& any);
[[nodiscard]] bool isEmpty(const Var& any);
[[nodiscard]] [[deprecated("use isEmpty")]] bool empty(const Var& any);
[[nodiscard, gnu::pure]] int combined(int x);
[[nodiscard]] inline int inlined(int x)
{
	return x;
}
[[nodiscard]] constexpr int doubled(int x)
{
	return 2*x;
}
[[maybe_unused]] inline constexpr int limit = 8;

class Holder
{
public:
	[[nodiscard]] [[deprecated]] int held() const;
};
}

[[nodiscard]] int globalFunc(int x)
{
	return x;
}
)", st);

	const auto findFunc = [this, &st](const std::string& name) -> const Function*
	{
		const auto it = st.find(name);
		assertTrue (it != st.end());
		const auto* pFunc = dynamic_cast<const Function*>(it->second);
		assertTrue (pFunc != nullptr);
		return pFunc;
	};

	const auto* pThrow = findFunc("throwAnyCastError");
	assertTrue (pThrow->fullName() == "Poco::Impl::throwAnyCastError");
	assertTrue (pThrow->getAttributeList() == "[[noreturn]]");
	assertTrue (pThrow->countParameters() == 3);

	const auto* pIsJSON = findFunc("isJSONString");
	assertTrue (pIsJSON->fullName() == "Poco::Impl::isJSONString");
	assertTrue (pIsJSON->getAttributeList() == "[[nodiscard]]");
	assertTrue (pIsJSON->countParameters() == 1);

	assertTrue (findFunc("isEmpty")->getAttributeList() == "[[nodiscard]]");
	assertTrue (findFunc("empty")->getAttributeList() == R"([[nodiscard]] [[deprecated ("use isEmpty")]])");
	assertTrue (findFunc("combined")->getAttributeList() == "[[nodiscard, gnu::pure]]");

	const auto* pInlined = findFunc("inlined");
	assertTrue (pInlined->getAttributeList() == "[[nodiscard]]");
	assertTrue ((pInlined->flags() & Function::FN_INLINE) != 0);

	const auto* pDoubled = findFunc("doubled");
	assertTrue (pDoubled->getAttributeList() == "[[nodiscard]]");
	assertTrue (pDoubled->countParameters() == 1);

	const auto itLimit = st.find("limit");
	assertTrue (itLimit != st.end());
	const auto* pLimit = dynamic_cast<const Variable*>(itLimit->second);
	assertTrue (pLimit != nullptr);
	assertTrue (pLimit->getAttributeList() == "[[maybe_unused]]");

	assertTrue (findFunc("held")->getAttributeList() == "[[nodiscard]] [[deprecated]]");

	const auto* pGlobal = findFunc("globalFunc");
	assertTrue (pGlobal->fullName() == "globalFunc");
	assertTrue (pGlobal->getAttributeList() == "[[nodiscard]]");
}


void CppParserTest::testParseFunctionTemplateAttributes()
{
	NameSpace::SymbolTable st;
	parseFollowedByPlainDeclarations(R"(
namespace N {
template <typename T>
[[nodiscard]] T pickFirst(T a, T b);

template <typename T> requires Integral<T>
[[nodiscard]] constexpr T half(T v)
{
	return v / 2;
}
}
)", st);

	const auto itPick = st.find("pickFirst");
	assertTrue (itPick != st.end());
	const auto* pPick = dynamic_cast<const Function*>(itPick->second);
	assertTrue (pPick != nullptr);
	assertTrue (pPick->getAttributeList() == "[[nodiscard]]");
	assertTrue (pPick->getReturnParameter() == "T");
	assertTrue (pPick->countParameters() == 2);

	const auto itHalf = st.find("half");
	assertTrue (itHalf != st.end());
	const auto* pHalf = dynamic_cast<const Function*>(itHalf->second);
	assertTrue (pHalf != nullptr);
	assertTrue (pHalf->getAttributeList() == "[[nodiscard]]");
	assertTrue (pHalf->declaration().find("requires Integral") != std::string::npos);
	assertTrue (pHalf->countParameters() == 1);
}


void CppParserTest::testParseUnterminatedAttribute()
{
	parseExpectingSyntaxError("[[nodiscard int f();");
	parseExpectingSyntaxError("namespace N { [[nodiscard");
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
	CppUnit_addTest(pSuite, CppParserTest, testExtractNameTemplateHead);
	CppUnit_addTest(pSuite, CppParserTest, testNumberLiterals);
	CppUnit_addTest(pSuite, CppParserTest, testTokenizerRawStrings);
	CppUnit_addTest(pSuite, CppParserTest, testTokenizerStringSuffixes);
	CppUnit_addTest(pSuite, CppParserTest, testParseStaticAssert);
	CppUnit_addTest(pSuite, CppParserTest, testParseAlignas);
	CppUnit_addTest(pSuite, CppParserTest, testParseNoexceptExpr);
	CppUnit_addTest(pSuite, CppParserTest, testParseDeleteDefault);
	CppUnit_addTest(pSuite, CppParserTest, testParseAnonymousNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseFriendInline);
	CppUnit_addTest(pSuite, CppParserTest, testParseNestedNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseInlineNestedNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseRepeatedNestedNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseNamespaceShadowingOuterNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseSingleNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testParseDeepNestedNamespace);
	CppUnit_addTest(pSuite, CppParserTest, testLookupOverloadedFunction);
	CppUnit_addTest(pSuite, CppParserTest, testParseConcept);
	CppUnit_addTest(pSuite, CppParserTest, testParseConceptRequiresExpression);
	CppUnit_addTest(pSuite, CppParserTest, testParseConceptConjunction);
	CppUnit_addTest(pSuite, CppParserTest, testParseFunctionTrailingRequires);
	CppUnit_addTest(pSuite, CppParserTest, testParseFunctionTemplateHeadRequires);
	CppUnit_addTest(pSuite, CppParserTest, testParseFunctionBothRequires);
	CppUnit_addTest(pSuite, CppParserTest, testParseParenthesizedRequires);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresConjunction);
	CppUnit_addTest(pSuite, CppParserTest, testParseClassTemplateRequires);
	CppUnit_addTest(pSuite, CppParserTest, testParseMemberTemplateRequires);
	CppUnit_addTest(pSuite, CppParserTest, testParseMemberRequiresQualifiers);
	CppUnit_addTest(pSuite, CppParserTest, testParseConstrainedParameters);
	CppUnit_addTest(pSuite, CppParserTest, testParseOperatorInConstraint);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresTemplateArgumentComparison);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresAlternativeTokens);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresTemplateDisambiguator);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresLiteral);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresExpressionOutsideConcept);
	CppUnit_addTest(pSuite, CppParserTest, testParseConstrainedMembers);
	CppUnit_addTest(pSuite, CppParserTest, testParseConceptForms);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresSyntaxErrors);
	CppUnit_addTest(pSuite, CppParserTest, testParseVariableInitializer);
	CppUnit_addTest(pSuite, CppParserTest, testParseFunctionTemplateReturnType);
	CppUnit_addTest(pSuite, CppParserTest, testParseTrailingRequiresOverloads);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresDecltype);
	CppUnit_addTest(pSuite, CppParserTest, testParseRequiresExpressionInTemplateArgument);
	CppUnit_addTest(pSuite, CppParserTest, testParseNestedGroupsInTemplateArguments);
	CppUnit_addTest(pSuite, CppParserTest, testParseQualifiedBaseClass);
	CppUnit_addTest(pSuite, CppParserTest, testParseFreeFunctionAttributes);
	CppUnit_addTest(pSuite, CppParserTest, testParseFunctionTemplateAttributes);
	CppUnit_addTest(pSuite, CppParserTest, testParseUnterminatedAttribute);

	return pSuite;
}
