//
// RegularExpressionTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RegularExpressionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RegularExpression.h"
#include "Poco/Exception.h"


using Poco::RegularExpression;
using Poco::RegularExpressionException;


RegularExpressionTest::RegularExpressionTest(const std::string& name): CppUnit::TestCase(name)
{
}


RegularExpressionTest::~RegularExpressionTest()
{
}


void RegularExpressionTest::testIndex()
{
	RegularExpression re("[0-9]+");
	RegularExpression::Match match;
	assertTrue (re.match("", 0, match) == 0);
	assertTrue (re.match("123", 3, match) == 0);
}


void RegularExpressionTest::testMatch1()
{
	RegularExpression re("[0-9]+");
	assertTrue (re.match("123"));
	assertTrue (!re.match("123cd"));
	assertTrue (!re.match("abcde"));
	assertTrue (re.match("ab123", 2));
}


void RegularExpressionTest::testMatch2()
{
	RegularExpression re("[0-9]+");
	RegularExpression::Match match;
	assertTrue (re.match("123", 0, match) == 1);
	assertTrue (match.offset == 0);
	assertTrue (match.length == 3);

	assertTrue (re.match("abc123def", 0, match) == 1);
	assertTrue (match.offset == 3);
	assertTrue (match.length == 3);

	assertTrue (re.match("abcdef", 0, match) == 0);
	assertTrue (match.offset == std::string::npos);
	assertTrue (match.length == 0);

	assertTrue (re.match("abc123def", 3, match) == 1);
	assertTrue (match.offset == 3);
	assertTrue (match.length == 3);
}


void RegularExpressionTest::testMatch3()
{
	RegularExpression re("[0-9]+");
	RegularExpression::MatchVec match;
	assertTrue (re.match("123", 0, match) == 1);
	assertTrue (match.size() == 1);
	assertTrue (match[0].offset == 0);
	assertTrue (match[0].length == 3);

	assertTrue (re.match("abc123def", 0, match) == 1);
	assertTrue (match.size() == 1);
	assertTrue (match[0].offset == 3);
	assertTrue (match[0].length == 3);

	assertTrue (re.match("abcdef", 0, match) == 0);
	assertTrue (match.size() == 0);

	assertTrue (re.match("abc123def", 3, match) == 1);
	assertTrue (match.size() == 1);
	assertTrue (match[0].offset == 3);
	assertTrue (match[0].length == 3);
}


void RegularExpressionTest::testMatch4()
{
	RegularExpression re("([0-9]+) ([0-9]+)");
	RegularExpression::MatchVec matches;
	assertTrue (re.match("123 456", 0, matches) == 3);
	assertTrue (matches.size() == 3);
	assertTrue (matches[0].offset == 0);
	assertTrue (matches[0].length == 7);
	assertTrue (matches[1].offset == 0);
	assertTrue (matches[1].length == 3);
	assertTrue (matches[2].offset == 4);
	assertTrue (matches[2].length == 3);

	assertTrue (re.match("abc123 456def", 0, matches) == 3);
	assertTrue (matches.size() == 3);
	assertTrue (matches[0].offset == 3);
	assertTrue (matches[0].length == 7);
	assertTrue (matches[1].offset == 3);
	assertTrue (matches[1].length == 3);
	assertTrue (matches[2].offset == 7);
	assertTrue (matches[2].length == 3);
}


void RegularExpressionTest::testMatch5()
{
	std::string digits = "0123";
	assertTrue (RegularExpression::match(digits, "[0-9]+"));
	std::string alphas = "abcd";
	assertTrue (!RegularExpression::match(alphas, "[0-9]+"));
}


void RegularExpressionTest::testMatch6()
{
	RegularExpression expr("^([a-z]*)?$");
	assertTrue (expr.match("", 0, 0));
	assertTrue (expr.match("abcde", 0, 0));
	assertTrue (!expr.match("123", 0, 0));
}


void RegularExpressionTest::testMatchDateTime()
{
	RegularExpression re(
		R"(([\+-]?\d{4}(?!\d{2}\b))((-?)((0[1-9]|1[0-2])(\3([12]\d|0[1-9]|3[01]))?|W([0-4]\d|5[0-2])(-?[1-7])?|(00[1-9]|0[1-9]\d|[12]\d{2}|3([0-5]\d|6[1-6])))([T\s]((([01]\d|2[0-3])((:?)[0-5]\d)?|24\:?00)([\.,]\d+(?!:))?)?(\17[0-5]\d([\.,]\d+)?)?([zZ]|([\+-])([01]\d|2[0-3]):?([0-5]\d)?)?)?)?)"
	);
	assert (re.match("2005-01-08T12:30:00Z"));
}


void RegularExpressionTest::testExtract()
{
	RegularExpression re("[0-9]+");
	std::string str;
	assertTrue (re.extract("123", str) == 1);
	assertTrue (str == "123");

	assertTrue (re.extract("abc123def", 0, str) == 1);
	assertTrue (str == "123");

	assertTrue (re.extract("abcdef", 0, str) == 0);
	assertTrue (str == "");

	assertTrue (re.extract("abc123def", 3, str) == 1);
	assertTrue (str == "123");
}


void RegularExpressionTest::testSplit1()
{
	RegularExpression re("[0-9]+");
	std::vector<std::string> strings;
	assertTrue (re.split("123", 0, strings) == 1);
	assertTrue (strings.size() == 1);
	assertTrue (strings[0] == "123");

	assertTrue (re.split("abc123def", 0, strings) == 1);
	assertTrue (strings.size() == 1);
	assertTrue (strings[0] == "123");

	assertTrue (re.split("abcdef", 0, strings) == 0);
	assertTrue (strings.empty());

	assertTrue (re.split("abc123def", 3, strings) == 1);
	assertTrue (strings.size() == 1);
	assertTrue (strings[0] == "123");
}


void RegularExpressionTest::testSplit2()
{
	RegularExpression re("([0-9]+) ([0-9]+)");
	std::vector<std::string> strings;
	assertTrue (re.split("123 456", 0, strings) == 3);
	assertTrue (strings.size() == 3);
	assertTrue (strings[0] == "123 456");
	assertTrue (strings[1] == "123");
	assertTrue (strings[2] == "456");

	assertTrue (re.split("abc123 456def", 0, strings) == 3);
	assertTrue (strings.size() == 3);
	assertTrue (strings[0] == "123 456");
	assertTrue (strings[1] == "123");
	assertTrue (strings[2] == "456");
}


void RegularExpressionTest::testSubst1()
{
	RegularExpression re("[0-9]+");
	std::string s = "123";
	assertTrue (re.subst(s, "ABC") == 1);
	assertTrue (s == "ABC");
	assertTrue (re.subst(s, "123") == 0);

	s = "123";
	assertTrue (re.subst(s, "AB$0CD") == 1);
	assertTrue (s == "AB123CD");

	s = "123";
	assertTrue (re.subst(s, "AB$1CD") == 1);
	assertTrue (s == "ABCD");

	s = "123";
	assertTrue (re.subst(s, "AB$2CD") == 1);
	assertTrue (s == "ABCD");

	s = "123";
	assertTrue (re.subst(s, "AB$$CD") == 1);
	assertTrue (s == "AB$$CD");

	s = "123";
	assertTrue (re.subst(s, "AB$0CD", RegularExpression::RE_NO_VARS) == 1);
	assertTrue (s == "AB$0CD");
}


void RegularExpressionTest::testSubst2()
{
	RegularExpression re("([0-9]+) ([0-9]+)");
	std::string s = "123 456";
	assertTrue (re.subst(s, "$2-$1") == 1);
	assertTrue (s == "456-123");
}


void RegularExpressionTest::testSubst3()
{
	RegularExpression re("[0-9]+");
	std::string s = "123 456 789";
	assertTrue (re.subst(s, "n", RegularExpression::RE_GLOBAL) == 3);
	assertTrue (s == "n n n");
}


void RegularExpressionTest::testSubst4()
{
	RegularExpression re("[0-9]+");
	std::string s = "ABC 123 456 789 DEF";
	assertTrue (re.subst(s, "n", RegularExpression::RE_GLOBAL) == 3);
	assertTrue (s == "ABC n n n DEF");
}


void RegularExpressionTest::testError()
{
	try
	{
		RegularExpression re("(0-9]");
		failmsg("bad regexp - must throw exception");
	}
	catch (RegularExpressionException&)
	{
	}
}


void RegularExpressionTest::testGroup()
{
	RegularExpression::MatchVec matches;
	RegularExpression re("(?P<group1>[a-z]+) (?P<group2>[0-9]+)");
	assertTrue (re.match("abcd 1234", 0, matches) == 3);
	assertTrue (matches[0].name == "");
	assertTrue (matches[1].name == "group1");
	assertTrue (matches[2].name == "group2");
}


void RegularExpressionTest::setUp()
{
}


void RegularExpressionTest::tearDown()
{
}


CppUnit::Test* RegularExpressionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RegularExpressionTest");

	CppUnit_addTest(pSuite, RegularExpressionTest, testIndex);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatch1);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatch2);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatch3);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatch4);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatch5);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatch6);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchDateTime);
	CppUnit_addTest(pSuite, RegularExpressionTest, testExtract);
	CppUnit_addTest(pSuite, RegularExpressionTest, testSplit1);
	CppUnit_addTest(pSuite, RegularExpressionTest, testSplit2);
	CppUnit_addTest(pSuite, RegularExpressionTest, testSubst1);
	CppUnit_addTest(pSuite, RegularExpressionTest, testSubst2);
	CppUnit_addTest(pSuite, RegularExpressionTest, testSubst3);
	CppUnit_addTest(pSuite, RegularExpressionTest, testSubst4);
	CppUnit_addTest(pSuite, RegularExpressionTest, testError);
	CppUnit_addTest(pSuite, RegularExpressionTest, testGroup);

	return pSuite;
}
