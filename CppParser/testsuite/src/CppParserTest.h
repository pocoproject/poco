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
	void testExtractNameTemplateHead();
	void testNumberLiterals();
	void testTokenizerRawStrings();
	void testTokenizerStringSuffixes();
	void testParseStaticAssert();
	void testParseAlignas();
	void testParseNoexceptExpr();
	void testParseDeleteDefault();
	void testParseAnonymousNamespace();
	void testParseFriendInline();
	void testParseNestedNamespace();
	void testParseInlineNestedNamespace();
	void testParseRepeatedNestedNamespace();
	void testParseSingleNamespace();
	void testParseDeepNestedNamespace();
	void testLookupOverloadedFunction();
	void testParseConcept();
	void testParseConceptRequiresExpression();
	void testParseConceptConjunction();
	void testParseFunctionTrailingRequires();
	void testParseFunctionTemplateHeadRequires();
	void testParseFunctionBothRequires();
	void testParseParenthesizedRequires();
	void testParseRequiresConjunction();
	void testParseClassTemplateRequires();
	void testParseMemberTemplateRequires();
	void testParseMemberRequiresQualifiers();
	void testParseConstrainedParameters();
	void testParseOperatorInConstraint();
	void testParseRequiresTemplateArgumentComparison();
	void testParseRequiresAlternativeTokens();
	void testParseRequiresTemplateDisambiguator();
	void testParseRequiresLiteral();
	void testParseRequiresExpressionOutsideConcept();
	void testParseConstrainedMembers();
	void testParseConceptForms();
	void testParseRequiresSyntaxErrors();
	void testParseVariableInitializer();
	void testParseFunctionTemplateReturnType();
	void testParseTrailingRequiresOverloads();
	void testParseRequiresDecltype();
	void testParseRequiresExpressionInTemplateArgument();
	void testParseNestedGroupsInTemplateArguments();
	void testParseQualifiedBaseClass();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void testNumberLiteral(const std::string& literal);
	void testRawStringLiteral(const std::string& prefix, const std::string& rawLiteral);
	void testStringSuffix(const std::string& literal, const std::string& expected);
	void parseString(const std::string& code, Poco::CppParser::NameSpace::SymbolTable& st);
	void parseFollowedByPlainDeclarations(const std::string& code, Poco::CppParser::NameSpace::SymbolTable& st);
	void parseExpectingSyntaxError(const std::string& code);

private:
};


#endif // CppParserTest_INCLUDED
