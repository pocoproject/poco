//
// FormatTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FormatTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Any.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include <string_view>


using Poco::format;
using Poco::BadCastException;
using Poco::Int64;
using Poco::UInt64;
using Poco::Any;


FormatTest::FormatTest(const std::string& name): CppUnit::TestCase(name)
{
}


FormatTest::~FormatTest()
{
}


void FormatTest::testChar()
{
	char c = 'a';
	std::string s(format("%c", c));
	assertTrue (s == "a");
	s = format("%2c", c);
	assertTrue (s == " a");
	s = format("%-2c", c);
	assertTrue (s == "a ");

	s = format("%*c", 2, c);
	assertTrue (s == " a");
	s = format("%-*c", 2, c);
	assertTrue (s == "a ");

	s = format("%c", std::string("foo"));
	assertTrue (s == "[ERRFMT]");
}


void FormatTest::testInt()
{
	int i = 42;
	std::string s(format("%d", i));
	assertTrue (s == "42");
	s = format("%4d", i);
	assertTrue (s == "  42");
	s = format("%04d", i);
	assertTrue (s == "0042");

	s = format("%*d", 4, i);
	assertTrue (s == "  42");
	s = format("%0*d", 4, i);
	assertTrue (s == "0042");

	short h = 42;
	s = format("%hd", h);
	assertTrue (s == "42");
	s = format("%4hd", h);
	assertTrue (s == "  42");
	s = format("%04hd", h);
	assertTrue (s == "0042");

	s = format("%*hd", 4, h);
	assertTrue (s == "  42");
	s = format("%0*hd", 4, h);
	assertTrue (s == "0042");

	unsigned short hu = 42;
	s = format("%hu", hu);
	assertTrue (s == "42");
	s = format("%4hu", hu);
	assertTrue (s == "  42");
	s = format("%04hu", hu);
	assertTrue (s == "0042");

	s = format("%*hu", 4, hu);
	assertTrue (s == "  42");
	s = format("%0*hu", 4, hu);
	assertTrue (s == "0042");

	unsigned x = 0x42;
	s = format("%x", x);
	assertTrue (s == "42");
	s = format("%4x", x);
	assertTrue (s == "  42");
	s = format("%04x", x);
	assertTrue (s == "0042");

	s = format("%*x", 4, x);
	assertTrue (s == "  42");
	s = format("%0*x", 4, x);
	assertTrue (s == "0042");

	unsigned o = 042;
	s = format("%o", o);
	assertTrue (s == "42");
	s = format("%4o", o);
	assertTrue (s == "  42");
	s = format("%04o", o);
	assertTrue (s == "0042");

	s = format("%*o", 4, o);
	assertTrue (s == "  42");
	s = format("%0*o", 4, o);
	assertTrue (s == "0042");

	unsigned u = 42;
	s = format("%u", u);
	assertTrue (s == "42");
	s = format("%4u", u);
	assertTrue (s == "  42");
	s = format("%04u", u);
	assertTrue (s == "0042");

	s = format("%*u", 4, u);
	assertTrue (s == "  42");
	s = format("%0*u", 4, u);
	assertTrue (s == "0042");

	long l = 42;
	s = format("%ld", l);
	assertTrue (s == "42");
	s = format("%4ld", l);
	assertTrue (s == "  42");
	s = format("%04ld", l);
	assertTrue (s == "0042");

	s = format("%*ld", 4, l);
	assertTrue (s == "  42");
	s = format("%0*ld", 4, l);
	assertTrue (s == "0042");

	unsigned long ul = 42;
	s = format("%lu", ul);
	assertTrue (s == "42");
	s = format("%4lu", ul);
	assertTrue (s == "  42");
	s = format("%04lu", ul);
	assertTrue (s == "0042");

	s = format("%*lu", 4, ul);
	assertTrue (s == "  42");
	s = format("%0*lu", 4, ul);
	assertTrue (s == "0042");

	unsigned long xl = 0x42;
	s = format("%lx", xl);
	assertTrue (s == "42");
	s = format("%4lx", xl);
	assertTrue (s == "  42");
	s = format("%04lx", xl);
	assertTrue (s == "0042");

	s = format("%*lx", 4, xl);
	assertTrue (s == "  42");
	s = format("%0*lx", 4, xl);
	assertTrue (s == "0042");

	Int64 i64 = 42;
	s = format("%Ld", i64);
	assertTrue (s == "42");
	s = format("%4Ld", i64);
	assertTrue (s == "  42");
	s = format("%04Ld", i64);
	assertTrue (s == "0042");

	s = format("%*Ld", 4, i64);
	assertTrue (s == "  42");
	s = format("%0*Ld", 4, i64);
	assertTrue (s == "0042");

	UInt64 ui64 = 42;
	s = format("%Lu", ui64);
	assertTrue (s == "42");
	s = format("%4Lu", ui64);
	assertTrue (s == "  42");
	s = format("%04Lu", ui64);
	assertTrue (s == "0042");

	s = format("%*Lu", 4, ui64);
	assertTrue (s == "  42");
	s = format("%0*Lu", 4, ui64);
	assertTrue (s == "0042");

	x = 0xaa;
	s = format("%x", x);
	assertTrue (s == "aa");
	s = format("%X", x);
	assertTrue (s == "AA");

	i = 42;
	s = format("%+d", i);
	assertTrue (s == "+42");

	i = -42;
	s = format("%+d", i);
	assertTrue (s == "-42");
	s = format("%+04d", i);
	assertTrue (s == "-042");

	s = format("%+0*d", 4, i);
	assertTrue (s == "-042");

	s = format("%d", i);
	assertTrue (s == "-42");

	s = format("%d", i);
	assertTrue (s == "-42");

	x = 0x42;
	s = format("%#x", x);
	assertTrue (s == "0x42");

	s = format("%d", l);
	assertTrue (s == "[ERRFMT]");
}


void FormatTest::testBool()
{
	bool b = true;
	std::string s = format("%b", b);
	assertTrue (s == "1");

	b = false;
	s = format("%b", b);
	assertTrue (s == "0");

	std::vector<Poco::Any> bv;
	bv.push_back(false);
	bv.push_back(true);
	bv.push_back(false);
	bv.push_back(true);
	bv.push_back(false);
	bv.push_back(true);
	bv.push_back(false);
	bv.push_back(true);
	bv.push_back(false);
	bv.push_back(true);

	s.clear();
	format(s, "%b%b%b%b%b%b%b%b%b%b", bv);
	assertTrue (s == "0101010101");
}


void FormatTest::testAnyInt()
{
	char c = 42;
	std::string s(format("%?i", c));
	assertTrue (s == "42");

	c = 43;
	s.clear();
	format(s, "%?i", c);
	assertTrue (s == "43");

	bool b = true;
	s = format("%?i", b);
	assertTrue (s == "1");

	signed char sc = -42;
	s = format("%?i", sc);
	assertTrue (s == "-42");

	unsigned char uc = 65;
	s = format("%?i", uc);
	assertTrue (s == "65");

	short ss = -134;
	s = format("%?i", ss);
	assertTrue (s == "-134");

	unsigned short us = 200;
	s = format("%?i", us);
	assertTrue (s == "200");

	int i = -12345;
	s = format("%?i", i);
	assertTrue (s == "-12345");

	unsigned ui = 12345;
	s = format("%?i", ui);
	assertTrue (s == "12345");

	long l = -54321;
	s = format("%?i", l);
	assertTrue (s == "-54321");

	unsigned long ul = 54321;
	s = format("%?i", ul);
	assertTrue (s == "54321");

	Int64 i64 = -12345678;
	s = format("%?i", i64);
	assertTrue (s == "-12345678");

	UInt64 ui64 = 12345678;
	s = format("%?i", ui64);
	assertTrue (s == "12345678");

	ss = 0x42;
	s = format("%?x", ss);
	assertTrue (s == "42");

	ss = 042;
	s = format("%?o", ss);
	assertTrue (s == "42");
}


void FormatTest::testFloatFix()
{
	double d = 1.5;
	std::string s(format("%f", d));
	assertTrue (s.find("1.50") == 0);

	s = format("%10f", d);
	assertTrue (s.find(" 1.50") != std::string::npos);

	s = format("%*f", 10, d);
	assertTrue (s.find(" 1.50") != std::string::npos);

	s = format("%6.2f", d);
	assertTrue (s == "  1.50");
	s = format("%-6.2f", d);
	assertTrue (s == "1.50  ");

	s = format("%*.*f", 6, 2, d);
	assertTrue (s == "  1.50");
	s = format("%-*.*f", 6,2, d);
	assertTrue (s == "1.50  ");

	float f = 1.5;
	s = format("%hf", f);
	assertTrue (s.find("1.50") == 0);

	s = format("%.0f", 1.0);
	assertTrue (s == "1");

	s = format("%.*f", 0, 1.0);
	assertTrue (s == "1");
}


void FormatTest::testFloatSci()
{
	double d = 1.5;
	std::string s(format("%e", d));
	assertTrue (s.find("1.50") == 0);
	assertTrue (s.find("0e+0") != std::string::npos);

	s = format("%20e", d);
	assertTrue (s.find(" 1.50") != std::string::npos);
	assertTrue (s.find("0e+0") != std::string::npos);

	s = format("%*e", 20, d);
	assertTrue (s.find(" 1.50") != std::string::npos);
	assertTrue (s.find("0e+0") != std::string::npos);

	s = format("%10.2e", d);
	assertTrue (s == " 1.50e+000" || s == "  1.50e+00");
	s = format("%-10.2e", d);
	assertTrue (s == "1.50e+000 " || s == "1.50e+00  ");
	s = format("%-10.2E", d);
	assertTrue (s == "1.50E+000 " || s == "1.50E+00  ");

	s = format("%*.*e", 10, 2, d);
	assertTrue (s == " 1.50e+000" || s == "  1.50e+00");
	s = format("%-*.*e", 10, 2, d);
	assertTrue (s == "1.50e+000 " || s == "1.50e+00  ");
	s = format("%-*.*E", 10, 2, d);
	assertTrue (s == "1.50E+000 " || s == "1.50E+00  ");
}


void FormatTest::testString()
{
	std::string foo("foo");
	std::string s(format("%s", foo));
	assertTrue (s == "foo");

	s = format("%5s", foo);
	assertTrue (s == "  foo");

	s = format("%-5s", foo);
	assertTrue (s == "foo  ");

	s = format("%*s", 5, foo);
	assertTrue (s == "  foo");

	s = format("%-*s", 5, foo);
	assertTrue (s == "foo  ");

	s = format("%s%%a", foo);
	assertTrue (s == "foo%a");

	s = format("'%s%%''%s%%'", foo, foo);
	assertTrue (s == "'foo%''foo%'");
}


void FormatTest::testStringView()
{
	std::string_view foo("foo");
	std::string s(format("%v", foo));
	assertTrue (s == "foo");

	s = format("%5v", foo);
	assertTrue (s == "  foo");

	s = format("%-5v", foo);
	assertTrue (s == "foo  ");

	s = format("%*v", 5, foo);
	assertTrue (s == "  foo");

	s = format("%-*v", 5, foo);
	assertTrue (s == "foo  ");

	s = format("%v%%a", foo);
	assertTrue (s == "foo%a");

	s = format("'%v%%''%v%%'", foo, foo);
	assertTrue (s == "'foo%''foo%'");
}


void FormatTest::testMultiple()
{
	std::string s(format("aaa%dbbb%4dccc", 1, 2));
	assertTrue (s == "aaa1bbb   2ccc");

	s = format("%%%d%%%d%%%d", 1, 2, 3);
	assertTrue (s == "%1%2%3");

	s = format("%d%d%d%d", 1, 2, 3, 4);
	assertTrue (s == "1234");

	s = format("%d%d%d%d%d", 1, 2, 3, 4, 5);
	assertTrue (s == "12345");

	s = format("%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6);
	assertTrue (s == "123456");

	s = format("%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 0);
	assertTrue (s == "1234567890");
}


void FormatTest::testIndex()
{
	std::string s(format("%[1]d%[0]d", 1, 2));
	assertTrue (s == "21");

	s = format("%[5]d%[4]d%[3]d%[2]d%[1]d%[0]d", 1, 2, 3, 4, 5, 6);
	assertTrue (s == "654321");

	s = format("%%%[1]d%%%[2]d%%%d", 1, 2, 3);
	assertTrue (s == "%2%3%1");

	s = format("%%%d%%%d%%%[0]d", 1, 2);
	assertTrue (s == "%1%2%1");
}


void FormatTest::testAny()
{
	Any a = 42;
	std::string s(format("%d", a));
	assertTrue (s == "42");

	a = std::string("42");
	s = format("%s", a);
	assertTrue (s == "42");

	a = 42.;
	s = format("%f", a);
	assertTrue (s.find("42.0") == 0);

	s.clear();
	std::vector<Any> av{ 42, std::string("42"), 42. };
	format(s, "%d '%s' %f", av);
	assertTrue (s.find("42 '42' 42.0") == 0);
}


void FormatTest::setUp()
{
}


void FormatTest::tearDown()
{
}


CppUnit::Test* FormatTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FormatTest");

	CppUnit_addTest(pSuite, FormatTest, testChar);
	CppUnit_addTest(pSuite, FormatTest, testBool);
	CppUnit_addTest(pSuite, FormatTest, testInt);
	CppUnit_addTest(pSuite, FormatTest, testAnyInt);
	CppUnit_addTest(pSuite, FormatTest, testFloatFix);
	CppUnit_addTest(pSuite, FormatTest, testFloatSci);
	CppUnit_addTest(pSuite, FormatTest, testString);
	CppUnit_addTest(pSuite, FormatTest, testStringView);
	CppUnit_addTest(pSuite, FormatTest, testMultiple);
	CppUnit_addTest(pSuite, FormatTest, testIndex);
	CppUnit_addTest(pSuite, FormatTest, testAny);

	return pSuite;
}
