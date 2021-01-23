//
// UTF8StringTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UTF8StringTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/UTF8String.h"


using Poco::UTF8;


UTF8StringTest::UTF8StringTest(const std::string& name): CppUnit::TestCase(name)
{
}


UTF8StringTest::~UTF8StringTest()
{
}


void UTF8StringTest::testCompare()
{
	std::string a1("aaaaa");
	std::string b1("bbbbb");

	assertTrue (UTF8::icompare(a1, b1) < 0);

	std::string a2("aaaaa");
	std::string b2("BBBBB");

	assertTrue (UTF8::icompare(a2, b2) < 0);

	std::string a3("AAAAA");
	std::string b3("bbbbb");

	assertTrue (UTF8::icompare(a3, b3) < 0);

	std::string a4("aaaaa");
	std::string b4("AAAAA");

	assertTrue (UTF8::icompare(a4, b4) == 0);

	std::string a5("AAAAA");
	std::string b5("bbbbb");

	assertTrue (UTF8::icompare(a5, b5) < 0);

	std::string a6("\303\274\303\266\303\244"); // "u"o"a
	std::string b6("\303\234\303\226\303\204"); // "U"O"A

	assertTrue (UTF8::icompare(a6, b6) == 0);
}


void UTF8StringTest::testTransform()
{
	std::string s1("abcde");
	UTF8::toUpperInPlace(s1);
	assertTrue (s1 == "ABCDE");

	std::string s2("aBcDe123");
	UTF8::toUpperInPlace(s2);
	assertTrue (s2 == "ABCDE123");

	std::string s3("\303\274\303\266\303\244"); // "u"o"a
	UTF8::toUpperInPlace(s3);
	assertTrue (s3 == "\303\234\303\226\303\204"); // "U"O"A
	UTF8::toLowerInPlace(s3);
	assertTrue (s3 == "\303\274\303\266\303\244"); // "u"o"a

	// a mix of invalid sequences
	std::string str = "\xC2\xE5\xF0\xF8\xE8\xED\xFB+-++";
	assertTrue ("???+-++" == UTF8::toLower(str));
}


void UTF8StringTest::testEscape()
{
	std::string s1("A \t, a \v, and an \a walk into a |, and the barman says \xD0\x82");

	assertTrue (UTF8::escape(s1) == "A \\t, a \\v, and an \\a walk into a |, and the barman says \\u0402");
	assertTrue (UTF8::escape(s1, true) == "A \\t, a \\u000B, and an \\u0007 walk into a |, and the barman says \\u0402");
}


void UTF8StringTest::testUnescape()
{
	std::string s1("A \\t, a \\u000B, and an \\u0007 walk into a |, and the barman says \\u0402");
	std::string s2("A \\t, a \\v, and an \\a walk into a |, and the barman says \\u0402");
	std::string s3("\\\\");

	assertTrue (UTF8::unescape(s1) == "A \t, a \v, and an \a walk into a |, and the barman says \xD0\x82");
	assertTrue (UTF8::unescape(s2) == "A \t, a \v, and an \a walk into a |, and the barman says \xD0\x82");
	assertTrue (UTF8::unescape(s3) == "\\");
}


void UTF8StringTest::setUp()
{
}


void UTF8StringTest::tearDown()
{
}


CppUnit::Test* UTF8StringTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UTF8StringTest");

	CppUnit_addTest(pSuite, UTF8StringTest, testCompare);
	CppUnit_addTest(pSuite, UTF8StringTest, testTransform);
	CppUnit_addTest(pSuite, UTF8StringTest, testEscape);
	CppUnit_addTest(pSuite, UTF8StringTest, testUnescape);

	return pSuite;
}
