//
// StringTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "StringTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/JSONString.h"
#include "Poco/Format.h"
#include "Poco/MemoryStream.h"
#include "Poco/Stopwatch.h"
#include "Poco/FPEnvironment.h"
#include "Poco/Exception.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <climits>
#include <map>
#include <set>


using Poco::trimLeft;
using Poco::trimLeftInPlace;
using Poco::trimRight;
using Poco::trimRightInPlace;
using Poco::trim;
using Poco::trimInPlace;
using Poco::toUpper;
using Poco::toUpperInPlace;
using Poco::toLower;
using Poco::toLowerInPlace;
using Poco::icompare;
using Poco::istring;
using Poco::isubstr;
using Poco::translate;
using Poco::translateInPlace;
using Poco::replace;
using Poco::replaceInPlace;
using Poco::remove;
using Poco::removeInPlace;
using Poco::cat;
using Poco::startsWith;
using Poco::endsWith;
using Poco::strToInt;
using Poco::strToFloat;
using Poco::strToDouble;
using Poco::intToStr;
using Poco::intToStr;
using Poco::floatToStr;
using Poco::doubleToStr;
using Poco::thousandSeparator;
using Poco::decimalSeparator;
using Poco::format;
using Poco::toJSON;
using Poco::CILess;
using Poco::MemoryInputStream;
using Poco::Stopwatch;
using Poco::RangeException;
using Poco::isIntOverflow;
using Poco::safeMultiply;
using Poco::isSafeIntCast;
using Poco::safeIntCast;
using Poco::FPEnvironment;


StringTest::StringTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


StringTest::~StringTest()
{
}


void StringTest::testTrimLeft()
{
	{
		std::string s = "abc";
		assertTrue (trimLeft(s) == "abc");
	}
		std::string s = " abc ";
		assertTrue (trimLeft(s) == "abc ");
	{
		std::string s = "  ab c ";
		assertTrue (trimLeft(s) == "ab c ");
	}
	{
		std::string s;
		assertTrue (trimLeft(s) == "");
	}
}


void StringTest::testTrimLeftInPlace()
{
	{
		std::string s = "abc";
		assertTrue (trimLeftInPlace(s) == "abc");
	}
	{
		std::string s = " abc ";
		assertTrue (trimLeftInPlace(s) == "abc ");
	}
	{
		std::string s = "  ab c ";
		assertTrue (trimLeftInPlace(s) == "ab c ");
	}
	{
		std::string s;
		assertTrue (trimLeftInPlace(s) == "");
	}
}


void StringTest::testTrimRight()
{
	{
		std::string s = "abc";
		assertTrue (trimRight(s) == "abc");
	}
	{
		std::string s = " abc ";
		assertTrue (trimRight(s) == " abc");
	}
	{
		std::string s = "  ab c  ";
		assertTrue (trimRight(s) == "  ab c");
	}
	{
		std::string s;
		assertTrue (trimRight(s) == "");
	}
}


void StringTest::testTrimRightInPlace()
{
	{
		std::string s = "abc";
		assertTrue (trimRightInPlace(s) == "abc");
	}
	{
		std::string s = " abc ";
		assertTrue (trimRightInPlace(s) == " abc");
	}
	{
		std::string s = "  ab c  ";
		assertTrue (trimRightInPlace(s) == "  ab c");
	}
	{
		std::string s;
		assertTrue (trimRightInPlace(s) == "");
	}
}


void StringTest::testTrim()
{
	{
		std::string s = "abc";
		assertTrue (trim(s) == "abc");
	}
	{
		std::string s = "abc ";
		assertTrue (trim(s) == "abc");
	}
	{
		std::string s = "  ab c  ";
		assertTrue (trim(s) == "ab c");
	}
	{
		std::string s;
		assertTrue (trim(s) == "");
	}
}


void StringTest::testTrimInPlace()
{
	{
		std::string s = "abc";
		assertTrue (trimInPlace(s) == "abc");
	}
	{
		std::string s = " abc ";
		assertTrue (trimInPlace(s) == "abc");
	}
	{
		std::string s = "  ab c  ";
		assertTrue (trimInPlace(s) == "ab c");
	}
	{
		std::string s;
		assertTrue (trimInPlace(s) == "");
	}
}


void StringTest::testToUpper()
{
	{
		std::string s = "abc";
		assertTrue (toUpper(s) == "ABC");
	}
	{
		std::string s = "Abc";
		assertTrue (toUpper(s) == "ABC");
	}
	{
		std::string s = "abc";
		assertTrue (toUpperInPlace(s) == "ABC");
	}
	{
		std::string s = "Abc";
		assertTrue (toUpperInPlace(s) == "ABC");
	}
}


void StringTest::testToLower()
{
	{
		std::string s = "ABC";
		assertTrue (toLower(s) == "abc");
	}
	{
		std::string s = "aBC";
		assertTrue (toLower(s) == "abc");
	}
	{
		std::string s = "ABC";
		assertTrue (toLowerInPlace(s) == "abc");
	}
	{
		std::string s = "aBC";
		assertTrue (toLowerInPlace(s) == "abc");
	}
}


void StringTest::testIstring()
{
	istring is1 = "AbC";
	istring is2 = "aBc";
	assertTrue (is1 == is2);

	const char c1[] = { 'G', 0, (char) 0xFC, 'n', 't', 'e', 'r', '\0' };
	const char c2[] = { 'g', 0, (char) 0xDC, 'N', 'T', 'E', 'R', '\0' };
	is1 = c1;
	is2 = c2;
	assertTrue (is1 == is2);
	is1[0] = 'f';
	assertTrue (is1 < is2);
	is1[0] = 'F';
	assertTrue (is1 < is2);
	is1[0] = 'H';
	assertTrue (is1 > is2);
	is1[0] = 'h';
	assertTrue (is1 > is2);

	is1 = "aAaBbBcCc";
	is2 = "bbb";
	assertTrue (isubstr(is1, is2) == 3);
	is2 = "bC";
	assertTrue (isubstr(is1, is2) == 5);
	is2 = "xxx";
	assertTrue (isubstr(is1, is2) == istring::npos);
}


void StringTest::testIcompare()
{
	std::string s1 = "AAA";
	std::string s2 = "aaa";
	std::string s3 = "bbb";
	std::string s4 = "cCcCc";
	std::string s5;
	assertTrue (icompare(s1, s2) == 0);
	assertTrue (icompare(s1, s3) < 0);
	assertTrue (icompare(s1, s4) < 0);
	assertTrue (icompare(s3, s1) > 0);
	assertTrue (icompare(s4, s2) > 0);
	assertTrue (icompare(s2, s4) < 0);
	assertTrue (icompare(s1, s5) > 0);
	assertTrue (icompare(s5, s4) < 0);

	std::string ss1 = "xxAAAzz";
	std::string ss2 = "YaaaX";
	std::string ss3 = "YbbbX";
	assertTrue (icompare(ss1, 2, 3, ss2, 1, 3) == 0);
	assertTrue (icompare(ss1, 2, 3, ss3, 1, 3) < 0);
	assertTrue (icompare(ss1, 2, 3, ss2, 1) == 0);
	assertTrue (icompare(ss1, 2, 3, ss3, 1) < 0);
	assertTrue (icompare(ss1, 2, 2, ss2, 1, 3) < 0);
	assertTrue (icompare(ss1, 2, 2, ss2, 1, 2) == 0);
	assertTrue (icompare(ss3, 1, 3, ss1, 2, 3) > 0);

	assertTrue (icompare(s1, s2.c_str()) == 0);
	assertTrue (icompare(s1, s3.c_str()) < 0);
	assertTrue (icompare(s1, s4.c_str()) < 0);
	assertTrue (icompare(s3, s1.c_str()) > 0);
	assertTrue (icompare(s4, s2.c_str()) > 0);
	assertTrue (icompare(s2, s4.c_str()) < 0);
	assertTrue (icompare(s1, s5.c_str()) > 0);
	assertTrue (icompare(s5, s4.c_str()) < 0);

	assertTrue (icompare(ss1, 2, 3, "aaa") == 0);
	assertTrue (icompare(ss1, 2, 2, "aaa") < 0);
	assertTrue (icompare(ss1, 2, 3, "AAA") == 0);
	assertTrue (icompare(ss1, 2, 2, "bb") < 0);

	assertTrue (icompare(ss1, 2, "aaa") > 0);
}


void StringTest::testCILessThan()
{
	typedef std::map<std::string, int, CILess> CIMapType;
	CIMapType ciMap;

	ciMap["z"] = 1;
	ciMap["b"] = 2;
	ciMap["A"] = 3;
	ciMap["Z"] = 4;

	assertTrue (ciMap.size() == 3);
	CIMapType::iterator it = ciMap.begin();
	assertTrue (it->first == "A"); ++it;
	assertTrue (it->first == "b"); ++it;
	assertTrue (it->first == "z");
	assertTrue (it->second == 4);

	typedef std::set<std::string, CILess> CISetType;

	CISetType ciSet;
	ciSet.insert("z");
	ciSet.insert("b");
	ciSet.insert("A");
	ciSet.insert("Z");

	assertTrue (ciSet.size() == 3);
	CISetType::iterator sIt = ciSet.begin();
	assertTrue (*sIt == "A"); ++sIt;
	assertTrue (*sIt == "b"); ++sIt;
	assertTrue (*sIt == "z");
}


void StringTest::testTranslate()
{
	std::string s = "aabbccdd";
	assertTrue (translate(s, "abc", "ABC") == "AABBCCdd");
	assertTrue (translate(s, "abc", "AB") == "AABBdd");
	assertTrue (translate(s, "abc", "") == "dd");
	assertTrue (translate(s, "cba", "CB") == "BBCCdd");
	assertTrue (translate(s, "", "CB") == "aabbccdd");
}


void StringTest::testTranslateInPlace()
{
	std::string s = "aabbccdd";
	translateInPlace(s, "abc", "ABC");
	assertTrue (s == "AABBCCdd");
}


void StringTest::testReplace()
{
	std::string s("aabbccdd");

	assertTrue (replace(s, std::string("aa"), std::string("xx")) == "xxbbccdd");
	assertTrue (replace(s, std::string("bb"), std::string("xx")) == "aaxxccdd");
	assertTrue (replace(s, std::string("dd"), std::string("xx")) == "aabbccxx");
	assertTrue (replace(s, std::string("bbcc"), std::string("xx")) == "aaxxdd");
	assertTrue (replace(s, std::string("b"), std::string("xx")) == "aaxxxxccdd");
	assertTrue (replace(s, std::string("bb"), std::string("")) == "aaccdd");
	assertTrue (replace(s, std::string("b"), std::string("")) == "aaccdd");
	assertTrue (replace(s, std::string("ee"), std::string("xx")) == "aabbccdd");
	assertTrue (replace(s, std::string("dd"), std::string("")) == "aabbcc");

	assertTrue (replace(s, "aa", "xx") == "xxbbccdd");
	assertTrue (replace(s, "bb", "xx") == "aaxxccdd");
	assertTrue (replace(s, "dd", "xx") == "aabbccxx");
	assertTrue (replace(s, "bbcc", "xx") == "aaxxdd");
	assertTrue (replace(s, "bb", "") == "aaccdd");
	assertTrue (replace(s, "b", "") == "aaccdd");
	assertTrue (replace(s, "ee", "xx") == "aabbccdd");
	assertTrue (replace(s, "dd", "") == "aabbcc");

	s = "aabbaabb";
	assertTrue (replace(s, std::string("aa"), std::string("")) == "bbbb");
	assertTrue (replace(s, std::string("a"), std::string("")) == "bbbb");
	assertTrue (replace(s, std::string("a"), std::string("x")) == "xxbbxxbb");
	assertTrue (replace(s, std::string("a"), std::string("xx")) == "xxxxbbxxxxbb");
	assertTrue (replace(s, std::string("aa"), std::string("xxx")) == "xxxbbxxxbb");

	assertTrue (replace(s, std::string("aa"), std::string("xx"), 2) == "aabbxxbb");

	assertTrue (replace(s, "aa", "") == "bbbb");
	assertTrue (replace(s, "a", "") == "bbbb");
	assertTrue (replace(s, "a", "x") == "xxbbxxbb");
	assertTrue (replace(s, "a", "xx") == "xxxxbbxxxxbb");
	assertTrue (replace(s, "aa", "xxx") == "xxxbbxxxbb");

	assertTrue (replace(s, "aa", "xx", 2) == "aabbxxbb");
	assertTrue (replace(s, 'a', 'x', 2) == "aabbxxbb");
	assertTrue (remove(s, 'a', 2) == "aabbbb");
	assertTrue (remove(s, 'a') == "bbbb");
	assertTrue (remove(s, 'b', 2) == "aaaa");
}


void StringTest::testReplaceInPlace()
{
	std::string s("aabbccdd");

	replaceInPlace(s, std::string("aa"), std::string("xx"));
	assertTrue (s == "xxbbccdd");

	s = "aabbccdd";
	replaceInPlace(s, 'a', 'x');
	assertTrue (s == "xxbbccdd");
	replaceInPlace(s, 'x');
	assertTrue (s == "bbccdd");
	removeInPlace(s, 'b', 1);
	assertTrue (s == "bccdd");
	removeInPlace(s, 'd');
	assertTrue (s == "bcc");
}


void StringTest::testCat()
{
	std::string s1("one");
	std::string s2("two");
	std::string s3("three");
	std::string s4("four");
	std::string s5("five");
	std::string s6("six");

	assertTrue (cat(s1, s2) == "onetwo");
	assertTrue (cat(s1, s2, s3) == "onetwothree");
	assertTrue (cat(s1, s2, s3, s4) == "onetwothreefour");
	assertTrue (cat(s1, s2, s3, s4, s5) == "onetwothreefourfive");
	assertTrue (cat(s1, s2, s3, s4, s5, s6) == "onetwothreefourfivesix");

	std::vector<std::string> vec;
	assertTrue (cat(std::string(), vec.begin(), vec.end()) == "");
	assertTrue (cat(std::string(","), vec.begin(), vec.end()) == "");
	vec.push_back(s1);
	assertTrue (cat(std::string(","), vec.begin(), vec.end()) == "one");
	vec.push_back(s2);
	assertTrue (cat(std::string(","), vec.begin(), vec.end()) == "one,two");
	vec.push_back(s3);
	assertTrue (cat(std::string(","), vec.begin(), vec.end()) == "one,two,three");
}


void StringTest::testStartsWith()
{
	std::string s1("o");

	assertTrue (startsWith(s1, std::string("o")));
	assertTrue (startsWith(s1, std::string("")));

	assertTrue (!startsWith(s1, std::string("O")));
	assertTrue (!startsWith(s1, std::string("1")));

	std::string s2("");

	assertTrue (startsWith(s2, std::string("")));

	assertTrue (!startsWith(s2, std::string("o")));

	std::string s3("oO");

	assertTrue (startsWith(s3, std::string("o")));

	assertTrue (!startsWith(s3, std::string(" o")));
}


void StringTest::testEndsWith()
{
	std::string s1("o");

	assertTrue (endsWith(s1, std::string("o")));
	assertTrue (endsWith(s1, std::string("")));

	assertTrue (!endsWith(s1, std::string("O")));
	assertTrue (!endsWith(s1, std::string("1")));


	std::string s2("");

	assertTrue (endsWith(s2, std::string("")));

	assertTrue (!endsWith(s2, std::string("o")));

	std::string s3("Oo");

	assertTrue (endsWith(s3, std::string("o")));

	assertTrue (!endsWith(s3, std::string("o ")));
}


void StringTest::testStringToInt()
{
//gcc on Mac emits warnings that cannot be suppressed
#ifndef POCO_OS_FAMILY_BSD
	stringToInt<Poco::Int8>();
	stringToInt<Poco::UInt8>();
	stringToInt<Poco::Int16>();
	stringToInt<Poco::UInt16>();
#endif
	stringToInt<Poco::Int32>();
	stringToInt<Poco::UInt32>();
#if defined(POCO_HAVE_INT64)
	stringToInt<Poco::Int64>();
	stringToInt<Poco::UInt64>();
#endif
}


void StringTest::testStringToFloat()
{
	float result;
	std::string sep(".,");

	for (int i = 0; i < 2; ++i)
	{
		char ds = sep[i];
		for (int j = 0; j < 2; ++j)
		{
			char ts = sep[j];
			if (ts == ds) continue;

			assertTrue (strToFloat("1", result, ds, ts));
			assertEqualDelta(1.0, result, 0.01);
			assertTrue (strToFloat(format("%c1", ds), result, ds, ts));
			assertEqualDelta(.1, result, 0.01);
			assertTrue (strToFloat(format("1%c", ds), result, ds, ts));
			assertEqualDelta(1., result, 0.01);
			assertTrue (strToFloat("0", result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToFloat(format("0%c", ds), result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToFloat(format("%c0", ds), result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToFloat(format("0%c0", ds), result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToFloat(format("0%c0", ds), result, ds, ts));
			assertEqualDelta(0., result, 0.01);
			assertTrue (strToFloat(format("0%c0", ds), result, ds, ts));
			assertEqualDelta(.0, result, 0.01);
			assertTrue (strToFloat(format("12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToFloat(format("12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToFloat(format("-12%c34", ds), result, ds, ts));
			assertEqualDelta(-12.34, result, 0.01);
			assertTrue (strToFloat(format("%c34", ds), result, ds, ts));
			assertEqualDelta(.34, result, 0.01);
			assertTrue (strToFloat(format("-%c34", ds), result, ds, ts));
			assertEqualDelta(-.34, result, 0.01);
			assertTrue (strToFloat(format("12%c", ds), result, ds, ts));
			assertEqualDelta(12., result, 0.01);
			assertTrue (strToFloat(format("-12%c", ds), result, ds, ts));
			assertEqualDelta(-12., result, 0.01);
			assertTrue (strToFloat("12", result, ds, ts));
			assertEqualDelta(12, result, 0.01);
			assertTrue (strToFloat("-12", result, ds, ts));
			assertEqualDelta(-12, result, 0.01);
			assertTrue (strToFloat(format("12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);

			assertTrue (strToFloat(format("1%c234%c34", ts, ds), result, ds, ts));
			assertEqualDelta(1234.34, result, 0.01);
			assertTrue (strToFloat(format("12%c345%c34", ts, ds), result, ds, ts));
			assertEqualDelta(12345.34, result, 0.01);
			assertTrue (strToFloat(format("123%c456%c34", ts, ds), result, ds, ts));
			assertEqualDelta(123456.34, result, 0.01);
			assertTrue (strToFloat(format("1%c234%c567%c34", ts, ts, ds), result, ds, ts));

			if ((std::numeric_limits<double>::max() / 10) < 1.23456e10)
				fail ("test value larger than max value for this platform");
			else
			{
				float d = 12e34f;
				assertTrue (strToFloat(format("12e34", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01e34);

				d = 1.234e30f;
				assertTrue (strToFloat(format("1%c234e30", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				assertTrue (strToFloat(format("1%c234E+30", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
			}

			float d = 12.34e-10f;
			assertTrue (strToFloat(format("12%c34e-10", ds), result, ds, ts));
			assertEqualDelta(d, result, 0.01);
			assertTrue (strToFloat(format("-12%c34", ds), result, ds, ts));
			assertEqualDelta(-12.34, result, 0.01);

			assertTrue (strToFloat(format("   12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToFloat(format("12%c34   ", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToFloat(format(" 12%c34  ", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
		}
	}

	assertTrue (FPEnvironment::isNaN(strToFloat("nan")));
	assertTrue (FPEnvironment::isNaN(strToFloat("xNaNy")));
	assertTrue (!FPEnvironment::isNaN(strToFloat("inf")));
	assertTrue (!FPEnvironment::isNaN(strToFloat("-inf")));
	assertTrue (FPEnvironment::isNaN(strToFloat("infinity")));
	assertTrue (!FPEnvironment::isNaN(strToFloat("infinity", "infinity")));
	assertTrue (!FPEnvironment::isNaN(strToFloat("-infinity", "infinity")));
	assertTrue (!FPEnvironment::isNaN(strToFloat("1.23")));
	assertTrue (FPEnvironment::isNaN(strToFloat("Inf")));
	assertTrue (!FPEnvironment::isNaN(strToFloat("Inf", "Inf")));

	assertTrue (FPEnvironment::isInfinite(strToFloat("inf")));
	assertTrue (FPEnvironment::isInfinite(strToFloat("-inf")));
	assertTrue (FPEnvironment::isInfinite(strToFloat("infinity", "infinity")));
	assertTrue (FPEnvironment::isInfinite(strToFloat("-infinity", "infinity")));
	assertTrue (!FPEnvironment::isInfinite(strToFloat("1.23")));
	assertTrue (!FPEnvironment::isInfinite(strToFloat("abc")));
	assertTrue (FPEnvironment::isInfinite(strToFloat("Inf", "Inf")));
}


void StringTest::testStringToDouble()
{
	double result;
	std::string sep(".,");

	for (int i = 0; i < 2; ++i)
	{
		char ds = sep[i];
		for (int j = 0; j < 2; ++j)
		{
			char ts = sep[j];
			if (ts == ds) continue;

			assertTrue (strToDouble("1", result, ds, ts));
			assertEqualDelta(1.0, result, 0.01);
			assertTrue (strToDouble(format("%c1", ds), result, ds, ts));
			assertEqualDelta(.1, result, 0.01);
			assertTrue (strToDouble(format("1%c", ds), result, ds, ts));
			assertEqualDelta(1., result, 0.01);
			assertTrue (strToDouble("0", result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToDouble(format("0%c", ds), result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToDouble(format("%c0", ds), result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToDouble(format("0%c0", ds), result, ds, ts));
			assertEqualDelta(0.0, result, 0.01);
			assertTrue (strToDouble(format("0%c0", ds), result, ds, ts));
			assertEqualDelta(0., result, 0.01);
			assertTrue (strToDouble(format("0%c0", ds), result, ds, ts));
			assertEqualDelta(.0, result, 0.01);
			assertTrue (strToDouble(format("12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToDouble(format("12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToDouble(format("-12%c34", ds), result, ds, ts));
			assertEqualDelta(-12.34, result, 0.01);
			assertTrue (strToDouble(format("%c34", ds), result, ds, ts));
			assertEqualDelta(.34, result, 0.01);
			assertTrue (strToDouble(format("-%c34", ds), result, ds, ts));
			assertEqualDelta(-.34, result, 0.01);
			assertTrue (strToDouble(format("12%c", ds), result, ds, ts));
			assertEqualDelta(12., result, 0.01);
			assertTrue (strToDouble(format("-12%c", ds), result, ds, ts));
			assertEqualDelta(-12., result, 0.01);
			assertTrue (strToDouble("12", result, ds, ts));
			assertEqualDelta(12, result, 0.01);
			assertTrue (strToDouble("-12", result, ds, ts));
			assertEqualDelta(-12, result, 0.01);
			assertTrue (strToDouble(format("12%c3456789012345678901234567890", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);

			assertTrue (strToDouble("1234345", result, ds, ts));
			assertEqualDelta(1234345, result, 0.00000001);
			assertTrue (strToDouble(format("1%c234%c345", ts, ts), result, ds, ts));
			assertEqualDelta(1234345, result, 0.00000001);
			assertTrue (strToDouble(format("1%c234%c3456789012345678901234567890", ts, ds), result, ds, ts));
			assertEqualDelta(1234.3456789, result, 0.00000001);
			assertTrue (strToDouble(format("12%c345%c3456789012345678901234567890", ts, ds), result, ds, ts));
			assertEqualDelta(12345.3456789, result, 0.00000001);
			assertTrue (strToDouble(format("123%c456%c3456789012345678901234567890", ts, ds), result, ds, ts));
			assertEqualDelta(123456.3456789, result, 0.00000001);
			assertTrue (strToDouble(format("1%c234%c567%c3456789012345678901234567890", ts, ts, ds), result, ds, ts));
			assertEqualDelta(1234567.3456789, result, 0.00000001);
			assertTrue (strToDouble(format("12%c345%c678%c3456789012345678901234567890", ts, ts, ds), result, ds, ts));
			assertEqualDelta(12345678.3456789, result, 0.00000001);
			assertTrue (strToDouble(format("123%c456%c789%c3456789012345678901234567890", ts, ts, ds), result, ds, ts));
			assertEqualDelta(123456789.3456789, result, 0.00000001);

			if ((std::numeric_limits<double>::max() / 10) < 1.23456e10)
				fail ("test value larger than max value for this platform");
			else
			{
				double d = 12e34;
				assertTrue (strToDouble(format("12e34", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01e34);

				d = 1.234e100;
				assertTrue (strToDouble(format("1%c234e100", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				assertTrue (strToDouble(format("1%c234E+100", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);

				d = 1.234e-100;
				assertTrue (strToDouble(format("1%c234E-100", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);

				d = -1.234e100;
				assertTrue (strToDouble(format("-1%c234e+100", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				assertTrue (strToDouble(format("-1%c234E100", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);

				d = 1.234e-100;
				assertTrue (strToDouble(format(" 1%c234e-100 ", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				assertTrue (strToDouble(format(" 1%c234e-100 ", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				assertTrue (strToDouble(format("  1%c234e-100 ", ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);

				d = 1234.234e-100;
				assertTrue (strToDouble(format(" 1%c234%c234e-100 ", ts, ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				d = 12345.234e-100;
				assertTrue (strToDouble(format(" 12%c345%c234e-100 ", ts, ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				d = 123456.234e-100;
				assertTrue (strToDouble(format("  123%c456%c234e-100 ", ts, ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);

				d = -1234.234e-100;
				assertTrue (strToDouble(format(" -1%c234%c234e-100 ", ts, ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				d = -12345.234e-100;
				assertTrue (strToDouble(format(" -12%c345%c234e-100 ", ts, ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				d = -123456.234e-100;
				char ou = 0;
				assertTrue (strToDouble(format("  -123%c456%c234e-100 ", ts, ds), result, ds, ts));
				assertEqualDelta(d, result, 0.01);
				assertTrue (ou == 0);
			}

			double d = 12.34e-10;
			assertTrue (strToDouble(format("12%c34e-10", ds), result, ds, ts));
			assertEqualDelta(d, result, 0.01);
			assertTrue (strToDouble(format("-12%c34", ds), result, ds, ts));
			assertEqualDelta(-12.34, result, 0.01);

			assertTrue (strToDouble(format("   12%c34", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToDouble(format("12%c34   ", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
			assertTrue (strToDouble(format(" 12%c34  ", ds), result, ds, ts));
			assertEqualDelta(12.34, result, 0.01);
		}
	}

	assertTrue (FPEnvironment::isNaN(strToDouble("nan")));
	assertTrue (FPEnvironment::isNaN(strToDouble("xNaNy")));
	assertTrue (!FPEnvironment::isNaN(strToDouble("inf")));
	assertTrue (!FPEnvironment::isNaN(strToDouble("-inf")));
	assertTrue (FPEnvironment::isNaN(strToDouble("infinity")));
	assertTrue (!FPEnvironment::isNaN(strToDouble("infinity", "infinity")));
	assertTrue (!FPEnvironment::isNaN(strToDouble("-infinity", "infinity")));
	assertTrue (!FPEnvironment::isNaN(strToDouble("1.23")));
	assertTrue (FPEnvironment::isNaN(strToDouble("Inf")));
	assertTrue (!FPEnvironment::isNaN(strToDouble("Inf", "Inf")));

	assertTrue (FPEnvironment::isInfinite(strToDouble("inf")));
	assertTrue (FPEnvironment::isInfinite(strToDouble("-inf")));
	assertTrue (FPEnvironment::isInfinite(strToDouble("infinity", "infinity")));
	assertTrue (FPEnvironment::isInfinite(strToDouble("-infinity", "infinity")));
	assertTrue (!FPEnvironment::isInfinite(strToDouble("1.23")));
	assertTrue (!FPEnvironment::isInfinite(strToDouble("abc")));
	assertTrue (FPEnvironment::isInfinite(strToDouble("Inf", "Inf")));
}


void StringTest::testNumericStringPadding()
{
	std::string str;
	assertTrue (floatToStr(str, 0.999f, 2, 4) == "1.00");
	assertTrue (floatToStr(str, 0.999f, 2, 4, '.', ',') == "1,00");
	assertTrue (floatToStr(str, 0.945f, 2, 4) == "0.95");
	assertTrue (floatToStr(str, 0.944f, 2, 4) == "0.94");
	assertTrue (floatToStr(str, 12.45f, 2, 5) == "12.45");
	assertTrue (floatToStr(str, 12.45f, 1, 4) == "12.5");
	assertTrue (floatToStr(str, 12.45f, 2, 6) == " 12.45");
	assertTrue (floatToStr(str, 12.455f, 3, 7) == " 12.455");
	assertTrue (floatToStr(str, 12.455f, 2, 6) == " 12.46");
	assertTrue (floatToStr(str, 1.23556E-16f, 2, 6) == "1.24e-16");

	assertTrue (doubleToStr(str, 0.999, 2, 4) == "1.00");
	assertTrue (doubleToStr(str, 0.945, 2, 4) == "0.95");
	assertTrue (doubleToStr(str, 0.944, 2, 4) == "0.94");
	assertTrue (doubleToStr(str, 12.45, 2, 5) == "12.45");
	assertTrue (doubleToStr(str, 12.45, 1, 4) == "12.5");
	assertTrue (doubleToStr(str, 12.45, 2, 6) == " 12.45");
	assertTrue (doubleToStr(str, 12.455, 3, 7) == " 12.455");
	assertTrue (doubleToStr(str, 12.455, 2, 6) == " 12.46");
	assertTrue (doubleToStr(str, 12345.678, 3, 6, '.', ',') == "12.345,678");
	assertTrue (doubleToStr(str, 1.23556E-16, 2, 6) == "1.24e-16");
}

void StringTest::testStringToFloatError()
{
	char ds = decimalSeparator();
	if (ds == 0) ds = '.';
	char ts = thousandSeparator();
	if (ts == 0) ts = ',';
	assertTrue (ds != ts);

	double result = 0.0;
	assertTrue (!strToDouble(format("a12%c3", ds), result));
	assertTrue (!strToDouble(format("1b2%c3", ds), result));
	assertTrue (!strToDouble(format("12c%c3", ds), result));
	assertTrue (!strToDouble(format("12%cx3", ds), result));

	assertTrue (!strToDouble(format("123%c456%c234e1000000", ts, ds), result));
	assertTrue (!strToDouble(format("123%c456%c234e+1000000", ts, ds), result));
	//assertTrue (!strToDouble(0, result, ou)); // strToDouble is resilient to null pointers
	assertTrue (!strToDouble("", result));
}


void StringTest::testNumericLocale()
{
#if !defined(POCO_NO_LOCALE) && POCO_OS == POCO_OS_WINDOWS_NT
	try
	{
		char buffer[POCO_MAX_FLT_STRING_LEN];

		char dp = decimalSeparator();
		char ts = thousandSeparator();
		std::locale loc;
		std::cout << "Original locale: '" << loc.name() << '\'' << std::endl;
		std::cout << "Decimal point: '" << decimalSeparator() << '\'' << std::endl;
		std::cout << "Thousand separator: '" << ts << '\'' << std::endl;
		doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, 1.23);
		assertTrue (std::strncmp(buffer, "1.23", 4) == 0);
		std::cout << "1.23 == '" << buffer << '\'' << std::endl;

		std::locale::global(std::locale("German"));
		std::locale locGerman;
		assertTrue (',' == decimalSeparator());
		assertTrue ('.' == thousandSeparator());
		std::cout << "New locale: '" << locGerman.name() << '\'' << std::endl;
		std::cout << "Decimal point: '" << decimalSeparator() << '\'' << std::endl;
		std::cout << "Thousand separator: '" << thousandSeparator() << '\'' << std::endl;
		doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, 1.23);
		assertTrue (std::strncmp(buffer, "1.23", 4) == 0);
		std::cout << "1.23 == '" << buffer << '\'' << std::endl;

		std::locale::global(std::locale("US"));
		std::locale locUS;
		assertTrue ('.' == decimalSeparator());
		assertTrue (',' == thousandSeparator());
		std::cout << "New locale: '" << locUS.name() << '\'' << std::endl;
		std::cout << "Decimal point: '" << decimalSeparator() << '\'' << std::endl;
		std::cout << "Thousand separator: '" << thousandSeparator() << '\'' << std::endl;
		doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, 1.23);
		assertTrue (std::strncmp(buffer, "1.23", 4) == 0);
		std::cout << "1.23 == '" << buffer << '\'' << std::endl;

		std::locale::global(loc);
		dp = decimalSeparator();
		ts = thousandSeparator();
		std::cout << "Final locale: '" << loc.name() << '\'' << std::endl;
		std::cout << "Decimal point: '" << decimalSeparator() << '\'' << std::endl;
		std::cout << "Thousand separator: '" << thousandSeparator() << '\'' << std::endl;
		doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, 1.23);
		assertTrue (std::strncmp(buffer, "1.23", 4) == 0);
		std::cout << "1.23 == '" << buffer << '\'' << std::endl;

		assertTrue (dp == decimalSeparator());
		assertTrue (ts == thousandSeparator());
	} catch (std::runtime_error& ex)
	{
		std::cout << ex.what() << std::endl;
		warnmsg ("Locale not found, skipping test");
	}
#endif
}


void StringTest::testIntToString()
{
	//intToStr(T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)

	// decimal
	std::string result;
	assertTrue (intToStr(0, 10, result));
	assertTrue (result == "0");
	assertTrue (intToStr(0, 10, result, false, 10, '0'));
	assertTrue (result == "0000000000");
	assertTrue (intToStr(1234567890, 10, result));
	assertTrue (result == "1234567890");
	assertTrue (intToStr(-1234567890, 10, result));
	assertTrue (result == "-1234567890");
	assertTrue (intToStr(-1234567890, 10, result, false, 15, '0'));
	assertTrue (result == "-00001234567890");
	assertTrue (intToStr(-1234567890, 10, result, false, 15));
	assertTrue (result == "    -1234567890");
	assertTrue (intToStr(-1234567890, 10, result, false, 0, 0, ','));
	assertTrue (result == "-1,234,567,890");

	// binary
	assertTrue (intToStr(1234567890, 2, result));
	assertTrue (result == "1001001100101100000001011010010");
	assertTrue (intToStr(1234567890, 2, result, true));
	assertTrue (result == "1001001100101100000001011010010");
	assertTrue (intToStr(1234567890, 2, result, true, 35, '0'));
	assertTrue (result == "00001001001100101100000001011010010");
	assertTrue (intToStr(0xFF, 2, result));
	assertTrue (result == "11111111");
	assertTrue (intToStr(0x0F, 2, result, false, 8, '0'));
	assertTrue (result == "00001111");
	assertTrue (intToStr(0x0F, 2, result));
	assertTrue (result == "1111");
	assertTrue (intToStr(0xF0, 2, result));
	assertTrue (result == "11110000");
	assertTrue (intToStr(0xFFFF, 2, result));
	assertTrue (result == "1111111111111111");
	assertTrue (intToStr(0xFF00, 2, result));
	assertTrue (result == "1111111100000000");
	assertTrue (intToStr(0xFFFFFFFF, 2, result));
	assertTrue (result == "11111111111111111111111111111111");
	assertTrue (intToStr(0xFF00FF00, 2, result));
	assertTrue (result == "11111111000000001111111100000000");
	assertTrue (intToStr(0xF0F0F0F0, 2, result));
	assertTrue (result == "11110000111100001111000011110000");
#if defined(POCO_HAVE_INT64)
	assertTrue (intToStr(0xFFFFFFFFFFFFFFFF, 2, result));
	assertTrue (result == "1111111111111111111111111111111111111111111111111111111111111111");
	assertTrue (intToStr(0xFF00000FF00000FF, 2, result));
	assertTrue (result == "1111111100000000000000000000111111110000000000000000000011111111");
#endif

	// octal
	assertTrue (intToStr(1234567890, 010, result));
	assertTrue (result == "11145401322");
	assertTrue (intToStr(1234567890, 010, result, true));
	assertTrue (result == "011145401322");
	assertTrue (intToStr(1234567890, 010, result, true, 15, '0'));
	assertTrue (result == "000011145401322");
	assertTrue (intToStr(012345670, 010, result, true));
	assertTrue (result == "012345670");
	assertTrue (intToStr(012345670, 010, result));
	assertTrue (result == "12345670");

	// hexadecimal
	// uppercase
	assertTrue (intToStr(0, 0x10, result, true));
	assertTrue (result == "0x0");
	assertTrue (intToStr(0, 0x10, result, true, 4, '0'));
	assertTrue (result == "0x00");
	assertTrue (intToStr(0, 0x10, result, false, 4, '0'));
	assertTrue (result == "0000");
	assertTrue (intToStr(1234567890, 0x10, result));
	assertTrue (result == "499602D2");
	assertTrue (intToStr(1234567890, 0x10, result, true));
	assertTrue (result == "0x499602D2");
	assertTrue (intToStr(1234567890, 0x10, result, true, 15, '0'));
	assertTrue (result == "0x00000499602D2");
	assertTrue (intToStr(0x1234567890ABCDEF, 0x10, result, true));
	assertTrue (result == "0x1234567890ABCDEF");
	assertTrue (intToStr(0xDEADBEEF, 0x10, result));
	assertTrue (result == "DEADBEEF");
#if defined(POCO_HAVE_INT64)
	assertTrue (intToStr(0xFFFFFFFFFFFFFFFF, 0x10, result));
	assertTrue (result == "FFFFFFFFFFFFFFFF");
	assertTrue (intToStr(0xFFFFFFFFFFFFFFFF, 0x10, result, true));
	assertTrue (result == "0xFFFFFFFFFFFFFFFF");
#endif

	// lowercase
	assertTrue (intToStr(0, 0x10, result, true, -1, (char)32, 0, true));
	assertTrue (result == "0x0");
	assertTrue (intToStr(0, 0x10, result, true, 4, '0', 0, true));
	assertTrue (result == "0x00");
	assertTrue (intToStr(0, 0x10, result, false, 4, '0', 0, true));
	assertTrue (result == "0000");
	assertTrue (intToStr(1234567890, 0x10, result, false, -1, (char)32, 0, true));
	assertTrue (result == "499602d2");
	assertTrue (intToStr(1234567890, 0x10, result, true, -1, (char)32, 0, true));
	assertTrue (result == "0x499602d2");
	assertTrue (intToStr(1234567890, 0x10, result, true, 15, '0', 0, true));
	assertTrue (result == "0x00000499602d2");
	assertTrue (intToStr(0x1234567890ABCDEF, 0x10, result, true, -1, (char)32, 0, true));
	assertTrue (result == "0x1234567890abcdef");
	assertTrue (intToStr(0xDEADBEEF, 0x10, result, false, -1, (char)32, 0, true));
	assertTrue (result == "deadbeef");
#if defined(POCO_HAVE_INT64)
	assertTrue (intToStr(0xFFFFFFFFFFFFFFFF, 0x10, result, false, -1, (char)32, 0, true));
	assertTrue (result == "ffffffffffffffff");
	assertTrue (intToStr(0xFFFFFFFFFFFFFFFF, 0x10, result, true, -1, (char)32, 0, true));
	assertTrue (result == "0xffffffffffffffff");
#endif

	try
	{
		char pResult[POCO_MAX_INT_STRING_LEN];
		std::size_t sz = POCO_MAX_INT_STRING_LEN;
		intToStr(0, 10, pResult, sz, false, (int) sz + 1, ' ');
		fail ("must throw RangeException");
	} catch (RangeException&) { }
}


void StringTest::testFloatToString()
{
	double val = 1.03721575516329e-112;
	std::string str;

	assertTrue (doubleToStr(str, val, 14, 21) == "1.03721575516329e-112");
	assertTrue (doubleToStr(str, val, 14, 22) == " 1.03721575516329e-112");
	val = -val;
	assertTrue (doubleToStr(str, val, 14, 22) == "-1.03721575516329e-112");
	assertTrue (doubleToStr(str, val, 14, 23) == " -1.03721575516329e-112");

	val = -10372157551632.9;
	assertTrue (doubleToStr(str, val, 1, 21, ',') == "-10,372,157,551,632.9");
	assertTrue (doubleToStr(str, val, 1, 22, ',') == " -10,372,157,551,632.9");
	assertTrue (doubleToStr(str, val, 2, 22, ',') == "-10,372,157,551,632.90");
	assertTrue (doubleToStr(str, val, 2, 22, '.', ',') == "-10.372.157.551.632,90");
	assertTrue (doubleToStr(str, val, 2, 22, ' ', ',') == "-10 372 157 551 632,90");

	int ival = 1234567890;
	assertTrue (doubleToStr(str, ival, 1, 15, ',') == "1,234,567,890.0");
	ival = -123456789;
	assertTrue (doubleToStr(str, ival, 1, 14, ',') == "-123,456,789.0");
}


void StringTest::testNumericStringLimit()
{
	char c = 0, t = -1;
	assertTrue(!isIntOverflow<char>(c));
	assertTrue(safeIntCast<char>(c, t) == c);
	assertTrue(t == c);

	short s = SHRT_MAX;
	assertTrue(isIntOverflow<char>(s));
	try
	{
		safeIntCast(s, t);
		fail("cast must fail");
	}
	catch(Poco::BadCastException&){}

	s = SHRT_MIN;
	assertTrue(isIntOverflow<char>(s));
	try
	{
		safeIntCast(s, t);
		fail("short => char cast must fail");
	}
	catch(Poco::BadCastException&){}

	signed char sc = 0, st = -1;
	assertTrue(!isIntOverflow<signed char>(sc));
	assertTrue(safeIntCast<char>(sc, st) == sc);
	assertTrue(st == sc);

	short ss = SHRT_MAX;
	assertTrue(isIntOverflow<signed char>(ss));
	assertTrue(isIntOverflow<char>(ss));
	try
	{
		safeIntCast(ss, st);
		fail("short => signed char  cast must fail");
	}
	catch(Poco::BadCastException&){}

	ss = SHRT_MIN;
	assertTrue(isIntOverflow<signed char>(ss));
	assertTrue(isIntOverflow<char>(ss));
	try
	{
		safeIntCast(ss, st);
		fail("short => signed char cast must fail");
	}
	catch(Poco::BadCastException&){}

	assertTrue(safeIntCast<signed char>(sc, st) == c);
	assertTrue(st == sc);

	unsigned char uc = 0, ut = -1;
	assertTrue(!isIntOverflow<unsigned char>(uc));
	assertTrue(safeIntCast<char>(uc, ut) == uc);
	assertTrue(ut == uc);

	ss = SHRT_MAX;
	assertTrue(isIntOverflow<unsigned char>(ss));
	try
	{
		safeIntCast(ss, st);
		fail("cast must fail");
	}
	catch(Poco::BadCastException&){}

	ss = -1;
	assertTrue(isIntOverflow<unsigned char>(ss));
	try
	{
		safeIntCast(ss, uc);
		fail("unsigned short => unsigned char cast must fail");
	}
	catch(Poco::BadCastException&){}

	int i = 0;
	assertTrue(!isIntOverflow<int>(i));
	assertTrue(!isIntOverflow<unsigned>(i));
	i = -1;
	unsigned int ti = -1;
	assertTrue(isIntOverflow<unsigned>(i));
	try
	{
		safeIntCast(i, ti);
		fail("unsigned int => int cast must fail");
	}
	catch(Poco::BadCastException&){}

	if (sizeof(long) > sizeof(int))
	{
		long l = LONG_MAX;
		assertTrue(isIntOverflow<int>(l));
		l = -1L;
		assertTrue(isIntOverflow<unsigned>(l));
		i = -1;
		assertTrue(!isIntOverflow<long>(i));
		long tl = 0;
		assertTrue(safeIntCast(i, tl) == i);
		unsigned long ul = ULONG_MAX, tul = 0;
		assertTrue(isIntOverflow<long>(ul));
		try
		{
			safeIntCast(ul, tl);
			fail("unsigned long => long cast must fail");
		}
		catch(Poco::BadCastException&){}
		assertTrue(!isIntOverflow<unsigned long>(ul));
		tl = 0;
		assertTrue(safeIntCast(ul, tul) == ul);
		l = LONG_MIN;
		assertTrue(isIntOverflow<unsigned long>(l));
		try
		{
			safeIntCast(l, ul);
			fail("unsigned long => long cast must fail");
		}
		catch(Poco::BadCastException&){}
		ul = LONG_MAX;
		assertTrue(!isIntOverflow<long>(ul));
		assertTrue(safeIntCast(ul, l) == ul);
	}

	numericStringLimitSameSign<unsigned short, unsigned char>();
	numericStringLimitSameSign<short, char>();
	numericStringLimitSameSign<unsigned int, unsigned short>();
	numericStringLimitSameSign<int, short>();

	if (sizeof(long) > sizeof(int))
	{
		numericStringLimitSameSign<unsigned long, unsigned int>();
		numericStringLimitSameSign<long, int>();
	}

	numericStringLowerLimit<short, char>();
	numericStringLowerLimit<int, short>();

	if (sizeof(long) > sizeof(int))
	{
		numericStringLowerLimit<Poco::Int64, Poco::Int32>();
	}

	assertTrue(!isIntOverflow<int8_t>(0));
	assertTrue(isIntOverflow<int8_t>(std::numeric_limits<int16_t>::max()));
	assertTrue(isIntOverflow<int8_t>(std::numeric_limits<int16_t>::min()));
	assertTrue(!isIntOverflow<uint8_t>(0));
	assertTrue(isIntOverflow<uint8_t>(std::numeric_limits<int16_t>::max()));
	assertTrue(isIntOverflow<uint8_t>(-1));
	assertTrue(!isIntOverflow<int32_t>(0));
	assertTrue(isIntOverflow<int32_t>(std::numeric_limits<int64_t>::max()));
	assertTrue(!isIntOverflow<uint32_t>(0));
	assertTrue(isIntOverflow<uint32_t>(-1));
	assertTrue(isIntOverflow<uint32_t>(-1L));
	assertTrue(isIntOverflow<uint32_t>(-1LL));
	assertTrue(!isIntOverflow<int64_t>(-1));
	assertTrue(isIntOverflow<int64_t>(std::numeric_limits<uint64_t>::max()));
	assertTrue(!isIntOverflow<uint64_t>(std::numeric_limits<uint64_t>::max()));
	assertTrue(isIntOverflow<uint64_t>(std::numeric_limits<int64_t>::min()));
	assertTrue(!isIntOverflow<uint64_t>(std::numeric_limits<uint64_t>::min()));
	assertTrue(!isIntOverflow<int64_t>(std::numeric_limits<int64_t>::max()));

	numericStringLimitSameSign<uint16_t, uint8_t>();
	numericStringLimitSameSign<int16_t, int8_t>();
	numericStringLimitSameSign<uint32_t, uint16_t>();
	numericStringLimitSameSign<int32_t, int16_t>();
	numericStringLimitSameSign<uint64_t, uint32_t>();
	numericStringLimitSameSign<int64_t, int32_t>();

	numericStringLowerLimit<int16_t, int8_t>();
	numericStringLowerLimit<int32_t, int16_t>();
	numericStringLowerLimit<int64_t, int32_t>();

	multiplyOverflow<int8_t>();
	multiplyOverflow<uint8_t>();
	multiplyOverflow<int16_t>();
	multiplyOverflow<uint16_t>();
	multiplyOverflow<int32_t>();
	multiplyOverflow<uint32_t>();
	multiplyOverflow<int64_t>();
	multiplyOverflow<uint64_t>();
}


void formatStream(double value, std::string& str)
{
	char buffer[128];
	Poco::MemoryOutputStream ostr(buffer, sizeof(buffer));
#if !defined(POCO_NO_LOCALE)
	ostr.imbue(std::locale::classic());
#endif
	ostr << std::setprecision(16) << value;
	str.assign(buffer, static_cast<std::string::size_type>(ostr.charsWritten()));
}


void formatSprintf(double value, std::string& str)
{
	char buffer[128];
	std::snprintf(buffer, sizeof(buffer), "%.*g", 16, value);
	str = buffer;
}


void StringTest::testJSONString()
{
	assertTrue (toJSON("\\", 0) == "\\\\");
	assertTrue (toJSON("\"", 0) == "\\\"");
	assertTrue (toJSON("\a", 0) == "\\u0007");
	assertTrue (toJSON("\b", 0) == "\\b");
	assertTrue (toJSON("\f", 0) == "\\f");
	assertTrue (toJSON("\n", 0) == "\\n");
	assertTrue (toJSON("\r", 0) == "\\r");
	assertTrue (toJSON("\t", 0) == "\\t");
	assertTrue (toJSON("\v", 0) == "\\u000B");
	assertTrue (toJSON("\v", Poco::JSON_LOWERCASE_HEX) == "\\u000b");
	assertTrue (toJSON("a", 0) == "a");
	assertTrue (toJSON("\xD0\x82", 0) == "\xD0\x82");
	assertTrue (toJSON("\xD0\x82", Poco::JSON_ESCAPE_UNICODE) == "\\u0402");

	// ??? on MSVC, the assert macro expansion
	// fails to compile when this string is inline ???
	std::string str = "\"foo\\\\\"";
	assertTrue (toJSON("foo\\") == str);

	assertTrue (toJSON("bar/") == "\"bar/\"");
	assertTrue (toJSON("baz") == "\"baz\"");
	assertTrue (toJSON("q\"uote\"d") == "\"q\\\"uote\\\"d\"");
	assertTrue (toJSON("bs\b") == "\"bs\\b\"");
	assertTrue (toJSON("nl\n") == "\"nl\\n\"");
	assertTrue (toJSON("tb\t") == "\"tb\\t\"");
	assertTrue (toJSON("\xD0\x82") == "\"\xD0\x82\"");
	assertTrue (toJSON("\xD0\x82", Poco::JSON_WRAP_STRINGS) == "\"\xD0\x82\"");
	assertTrue (toJSON("\xD0\x82",
			Poco::JSON_WRAP_STRINGS | Poco::JSON_ESCAPE_UNICODE) == "\"\\u0402\"");

	std::ostringstream ostr;
	toJSON("foo\\", ostr);
	assertTrue (ostr.str() == str);
	ostr.str("");

	toJSON("foo\\", ostr);
	assertTrue (toJSON("bar/") == "\"bar/\"");
	ostr.str("");
	toJSON("baz", ostr);
	assertTrue (ostr.str() == "\"baz\"");
	ostr.str("");
	toJSON("q\"uote\"d", ostr);
	assertTrue (ostr.str() == "\"q\\\"uote\\\"d\"");
	ostr.str("");
	toJSON("bs\b", ostr);
	assertTrue (ostr.str() == "\"bs\\b\"");
	ostr.str("");
	toJSON("nl\n", ostr);
	assertTrue (ostr.str() == "\"nl\\n\"");
	ostr.str("");
	toJSON("tb\t", ostr);
	assertTrue (ostr.str() == "\"tb\\t\"");

	ostr.str("");
	toJSON("\xD0\x82", ostr);
	assertTrue (ostr.str() == "\"\xD0\x82\"");
	ostr.str("");
	toJSON("\xD0\x82", ostr, Poco::JSON_WRAP_STRINGS);
	assertTrue (ostr.str() == "\"\xD0\x82\"");

	// wrap
	// uppercase (default)
	ostr.str("");
	toJSON("\v", ostr);
	assertTrue (ostr.str() == "\"\\u000B\"");
	// lowercase
	ostr.str("");
	toJSON("\v", ostr, Poco::JSON_WRAP_STRINGS | Poco::JSON_LOWERCASE_HEX);
	assertTrue (ostr.str() == "\"\\u000b\"");

	// no wrap
	// uppercase
	ostr.str("");
	toJSON("\v", ostr, 0);
	assertTrue (ostr.str() == "\\u000B");
	// lowercase
	ostr.str("");
	toJSON("\v", ostr, Poco::JSON_LOWERCASE_HEX);
	assertTrue (ostr.str() == "\\u000b");

	ostr.str("");
	toJSON("\xD0\x82", ostr, Poco::JSON_WRAP_STRINGS | Poco::JSON_ESCAPE_UNICODE);
	assertTrue (ostr.str() == "\"\\u0402\"");
	ostr.str("");
}



void StringTest::conversionBenchmarks()
{
	std::cout << std::endl << "===================" << std::endl;
	benchmarkFloatToStr();
	std::cout << "===================" << std::endl << std::endl;
	std::cout << "===================" << std::endl;
	benchmarkStrToFloat();
	std::cout << "===================" << std::endl << std::endl;
	std::cout << "===================" << std::endl;
	benchmarkStrToInt();
	std::cout << "===================" << std::endl << std::endl;
}


void StringTest::benchmarkFloatToStr()
{
	Poco::Stopwatch sw;
	double val = 1.0372157551632929e-112;
	std::cout << "The Number: " << std::setprecision(std::numeric_limits<double>::digits10) << val << std::endl;
	std::string str;
	sw.start();
	for (int i = 0; i < 1000000; ++i) formatStream(val, str);
	sw.stop();
	std::cout << "formatStream Number: " << str << std::endl;
	double timeStream = sw.elapsed() / 1000.0;

	// standard sprintf
	str = "";
	sw.restart();
	for (int i = 0; i < 1000000; ++i) formatSprintf(val, str);
	sw.stop();
	std::cout << "std::sprintf Number: " << str << std::endl;
	double timeSprintf = sw.elapsed() / 1000.0;

	// POCO Way (via double-conversion)
	// no padding
	sw.restart();
	char buffer[POCO_MAX_FLT_STRING_LEN];
	for (int i = 0; i < 1000000; ++i) doubleToStr(buffer, POCO_MAX_FLT_STRING_LEN, val);
	sw.stop();
	std::cout << "doubleToStr(char) Number: " << buffer << std::endl;
	double timeDoubleToStrChar = sw.elapsed() / 1000.0;

	// with padding
	str = "";
	sw.restart();
	for (int i = 0; i < 1000000; ++i) doubleToStr(str, val);
	sw.stop();
	std::cout << "doubleToStr(std::string) Number: " << str << std::endl;
	double timeDoubleToStrString = sw.elapsed() / 1000.0;

	int graph;
	std::cout << std::endl << "Timing and speedup relative to I/O stream:" << std::endl << std::endl;
	std::cout << std::setw(14) << "Stream:\t" << std::setw(10) << std::setfill(' ') << std::setprecision(4) << timeStream << "[ms]" << std::endl;

	std::cout << std::setw(14) << "sprintf:\t" << std::setw(10) << std::setfill(' ') << timeSprintf << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeSprintf) << '\t' ;
	graph = (int) (timeStream / timeSprintf); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl << std::setw(14) << "doubleToChar:\t" << std::setw(10) << std::setfill(' ') << timeDoubleToStrChar << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeDoubleToStrChar) << '\t' ;
	graph = (int) (timeStream / timeDoubleToStrChar); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl << std::setw(14) << "doubleToString:\t" << std::setw(10) << std::setfill(' ') << timeDoubleToStrString << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeDoubleToStrString) << '\t' ;
	graph = (int) (timeStream / timeDoubleToStrString); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl;
}


void StringTest::benchmarkStrToInt()
{
	Poco::Stopwatch sw;
	std::string num = "123456789";
	int res[4] = {};
	sw.start();
	for (int i = 0; i < 1000000; ++i) parseStream(num, res[0]);
	sw.stop();
	std::cout << "parseStream Number: " << res[0] << std::endl;
	double timeStream = sw.elapsed() / 1000.0;

	char* pC = 0;
	sw.restart();
	for (int i = 0; i < 1000000; ++i) res[1] = std::strtol(num.c_str(), &pC, 10);
	sw.stop();
	std::cout << "std::strtol Number: " << res[1] << std::endl;
	double timeStrtol = sw.elapsed() / 1000.0;

	sw.restart();
	for (int i = 0; i < 1000000; ++i) strToInt(num.c_str(), res[2], 10);
	sw.stop();
	std::cout << "strToInt Number: " << res[2] << std::endl;
	double timeStrToInt = sw.elapsed() / 1000.0;

	sw.restart();
	for (int i = 0; i < 1000000; ++i) std::sscanf(num.c_str(), "%d", &res[3]);
	sw.stop();
	std::cout << "sscanf Number: " << res[3] << std::endl;
	double timeScanf = sw.elapsed() / 1000.0;

	assertEqual (res[0], res[1]);
	assertEqual (res[1], res[2]);
	assertEqual (res[2], res[3]);

	int graph;
	std::cout << std::endl << "Timing and speedup relative to I/O stream:" << std::endl << std::endl;
	std::cout << std::setw(14) << "Stream:\t" << std::setw(10) << std::setfill(' ') << timeStream << "[ms]" << std::endl;

	std::cout << std::setw(14) << "std::strtol:\t" << std::setw(10) << std::setfill(' ') << timeStrtol << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeStrtol) << '\t' ;
	graph = (int) (timeStream / timeStrtol); for (int i = 0; i < graph; ++i) std::cout << '|';

	std::cout << std::endl << std::setw(14) << "strToInt:\t" << std::setw(10) << std::setfill(' ') << timeStrToInt << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeStrToInt) << '\t' ;
	graph = (int) (timeStream / timeStrToInt); for (int i = 0; i < graph; ++i) std::cout << '|';

	std::cout << std::endl << std::setw(14) << "std::sscanf:\t" << std::setw(10) << std::setfill(' ')  << timeScanf << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeScanf) << '\t' ;
	graph = (int) (timeStream / timeScanf); for (int i = 0; i < graph; ++i) std::cout << '|';
	std::cout << std::endl;
}


void StringTest::benchmarkStrToFloat()
{
	double res[5] = {};
	Poco::Stopwatch sw;
	std::string num = "1.0372157551632929e-112";
	std::cout << "The Number: " << num << std::endl;
	sw.start();
	for (int i = 0; i < 1000000; ++i) parseStream(num, res[0]);
	sw.stop();
	std::cout << "parseStream Number: " << std::setprecision(std::numeric_limits<double>::digits10) << res[0] << std::endl;
	double timeStream = sw.elapsed() / 1000.0;

	// standard strtod
	char* pC = 0;
	sw.restart();
	for (int i = 0; i < 1000000; ++i) res[1] = std::strtod(num.c_str(), &pC);
	sw.stop();
	std::cout << "std::strtod Number: " << res[1] << std::endl;
	double timeStdStrtod = sw.elapsed() / 1000.0;

	// POCO Way
	sw.restart();
	char ou = 0;
	for (int i = 0; i < 1000000; ++i) strToDouble(num, res[2], ou);
	sw.stop();
	std::cout << "Poco::strToDouble(const string&, double&) Number: " << res[2] << std::endl;
	double timeStrToDouble = sw.elapsed() / 1000.0;

	sw.restart();
	for (int i = 0; i < 1000000; ++i) res[3] = strToDouble(num.c_str());
	sw.stop();
	std::cout << "Poco::strToDouble(const char*) Number: " << res[3] << std::endl;
	double timeStrtoD = sw.elapsed() / 1000.0;

	// standard sscanf
	sw.restart();
	for (int i = 0; i < 1000000; ++i) std::sscanf(num.c_str(), "%lf", &res[4]);
	sw.stop();
	std::cout << "sscanf Number: " << res[4] << std::endl;
	double timeScanf = sw.elapsed() / 1000.0;

	assertEqual (res[0], res[1]);
	assertEqual (res[1], res[2]);
	assertEqual (res[2], res[3]);
	assertEqual (res[3], res[4]);

	int graph;
	std::cout << std::endl << "Timing and speedup relative to I/O stream:" << std::endl << std::endl;
	std::cout << std::setw(14) << "Stream:\t" << std::setw(10) << std::setfill(' ') << std::setprecision(4) << timeStream << "[ms]" << std::endl;

	std::cout << std::setw(14) << "std::strtod:\t" << std::setw(10) << std::setfill(' ') << timeStdStrtod << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeStdStrtod) << '\t' ;
	graph = (int) (timeStream / timeStdStrtod); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl << std::setw(14) << "strToDouble:\t" << std::setw(10) << std::setfill(' ') << timeStrToDouble << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeStrToDouble) << '\t' ;
	graph = (int) (timeStream / timeStrToDouble); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl << std::setw(14) << "strToDouble:\t" << std::setw(10) << std::setfill(' ')  << timeScanf << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeStrtoD) << '\t' ;
	graph = (int) (timeStream / timeStrtoD); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl << std::setw(14) << "std::sscanf:\t" << std::setw(10) << std::setfill(' ')  << timeScanf << "[ms]" <<
	std::setw(10) << std::setfill(' ')  << "Speedup: " << (timeStream / timeScanf) << '\t' ;
	graph = (int) (timeStream / timeScanf); for (int i = 0; i < graph; ++i) std::cout << '#';

	std::cout << std::endl;
}


void StringTest::setUp()
{
}


void StringTest::tearDown()
{
}


CppUnit::Test* StringTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StringTest");

	CppUnit_addTest(pSuite, StringTest, testTrimLeft);
	CppUnit_addTest(pSuite, StringTest, testTrimLeftInPlace);
	CppUnit_addTest(pSuite, StringTest, testTrimRight);
	CppUnit_addTest(pSuite, StringTest, testTrimInPlace);
	CppUnit_addTest(pSuite, StringTest, testTrim);
	CppUnit_addTest(pSuite, StringTest, testTrimRightInPlace);
	CppUnit_addTest(pSuite, StringTest, testToUpper);
	CppUnit_addTest(pSuite, StringTest, testToLower);
	CppUnit_addTest(pSuite, StringTest, testIstring);
	CppUnit_addTest(pSuite, StringTest, testIcompare);
	CppUnit_addTest(pSuite, StringTest, testCILessThan);
	CppUnit_addTest(pSuite, StringTest, testTranslate);
	CppUnit_addTest(pSuite, StringTest, testTranslateInPlace);
	CppUnit_addTest(pSuite, StringTest, testReplace);
	CppUnit_addTest(pSuite, StringTest, testReplaceInPlace);
	CppUnit_addTest(pSuite, StringTest, testCat);
	CppUnit_addTest(pSuite, StringTest, testStartsWith);
	CppUnit_addTest(pSuite, StringTest, testEndsWith);
	CppUnit_addTest(pSuite, StringTest, testStringToInt);
	CppUnit_addTest(pSuite, StringTest, testStringToFloat);
	CppUnit_addTest(pSuite, StringTest, testStringToDouble);
	CppUnit_addTest(pSuite, StringTest, testNumericStringPadding);
	CppUnit_addTest(pSuite, StringTest, testNumericStringLimit);
	CppUnit_addTest(pSuite, StringTest, testStringToFloatError);
	CppUnit_addTest(pSuite, StringTest, testNumericLocale);
	CppUnit_addTest(pSuite, StringTest, testIntToString);
	CppUnit_addTest(pSuite, StringTest, testFloatToString);
	CppUnit_addTest(pSuite, StringTest, testJSONString);
	CppUnit_addTest(pSuite, StringTest, conversionBenchmarks);

	return pSuite;
}
