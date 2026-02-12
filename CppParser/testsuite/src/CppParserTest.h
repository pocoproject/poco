//
// CppParserTest.h
//
// Definition of the CppParserTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CppParserTest_INCLUDED
#define CppParserTest_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "Poco/CppParser/NameSpace.h"
#include "CppUnit/TestCase.h"


class CppParserTest: public CppUnit::TestCase
{
public:
	CppParserTest(const std::string& name);
	~CppParserTest();

	void testParseDir();
	void testExtractName();
	void testNumberLiterals();
	void testTokenizerRawStrings();
	void testTokenizerStringSuffixes();
	void testParseStaticAssert();
	void testParseAlignas();
	void testParseNoexceptExpr();
	void testParseDeleteDefault();
	void testParseAnonymousNamespace();
	void testParseFriendInline();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void testNumberLiteral(const std::string& literal);
	void testRawStringLiteral(const std::string& prefix, const std::string& rawLiteral);
	void testStringSuffix(const std::string& literal, const std::string& expected);
	void parseString(const std::string& code, Poco::CppParser::NameSpace::SymbolTable& st);

private:
};


#endif // CppParserTest_INCLUDED
