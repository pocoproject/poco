//
// RegularExpressionTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/RegularExpressionTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RegularExpressionTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/RegularExpression.h"
#include "Poco/Exception.h"


using Poco::RegularExpression;
using Poco::RegularExpressionException;


RegularExpressionTest::RegularExpressionTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


RegularExpressionTest::~RegularExpressionTest()
{
}


void RegularExpressionTest::testIndex()
{
	RegularExpression re("[0-9]+");
	RegularExpression::Match match;
	assert (re.match("", 0, match) == 0);
	assert (re.match("123", 3, match) == 0);
}


void RegularExpressionTest::testMatch1()
{
	RegularExpression re("[0-9]+");
	assert (re.match("123"));
	assert (!re.match("123cd"));
	assert (!re.match("abcde"));
	assert (re.match("ab123", 2));
}


void RegularExpressionTest::testMatch2()
{
	RegularExpression re("[0-9]+");
	RegularExpression::Match match;
	assert (re.match("123", 0, match) == 1);
	assert (match.offset == 0);
	assert (match.length == 3);

	assert (re.match("abc123def", 0, match) == 1);
	assert (match.offset == 3);
	assert (match.length == 3);

	assert (re.match("abcdef", 0, match) == 0);
	assert (match.offset == std::string::npos);
	assert (match.length == 0);

	assert (re.match("abc123def", 3, match) == 1);
	assert (match.offset == 3);
	assert (match.length == 3);
}


void RegularExpressionTest::testMatch3()
{
	RegularExpression re("[0-9]+");
	RegularExpression::MatchVec match;
	assert (re.match("123", 0, match) == 1);
	assert (match.size() == 1);
	assert (match[0].offset == 0);
	assert (match[0].length == 3);

	assert (re.match("abc123def", 0, match) == 1);
	assert (match.size() == 1);
	assert (match[0].offset == 3);
	assert (match[0].length == 3);

	assert (re.match("abcdef", 0, match) == 0);
	assert (match.size() == 0);

	assert (re.match("abc123def", 3, match) == 1);
	assert (match.size() == 1);
	assert (match[0].offset == 3);
	assert (match[0].length == 3);
}


void RegularExpressionTest::testMatch4()
{
	RegularExpression re("([0-9]+) ([0-9]+)");
	RegularExpression::MatchVec matches;
	assert (re.match("123 456", 0, matches) == 3);
	assert (matches.size() == 3);
	assert (matches[0].offset == 0);
	assert (matches[0].length == 7);
	assert (matches[1].offset == 0);
	assert (matches[1].length == 3);
	assert (matches[2].offset == 4);
	assert (matches[2].length == 3);

	assert (re.match("abc123 456def", 0, matches) == 3);
	assert (matches.size() == 3);
	assert (matches[0].offset == 3);
	assert (matches[0].length == 7);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 3);
	assert (matches[2].offset == 7);
	assert (matches[2].length == 3);
}


void RegularExpressionTest::testMatch5()
{
	std::string digits = "0123";
	assert (RegularExpression::match(digits, "[0-9]+"));
	std::string alphas = "abcd";
	assert (!RegularExpression::match(alphas, "[0-9]+"));
}


void RegularExpressionTest::testMatch6()
{
	RegularExpression expr("^([a-z]*)?$");
	assert (expr.match("", 0, 0));
	assert (expr.match("abcde", 0, 0));
	assert (!expr.match("123", 0, 0));
}


void RegularExpressionTest::testMatchAll()
{
	//                     01234567890
	std::string subject = "abracadabra";

	RegularExpression::MatchVec matches;

	// Match at start
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /ab/g
	// data> abracadabra
	//  0: ab
	//  0: ab
	RegularExpression expr1("ab");
	assert (expr1.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);
	assert (matches[0].offset == 0);
	assert (matches[0].length == 2);
	assert (matches[1].offset == 7);
	assert (matches[1].length == 2);

	// Match at end
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /bra/g
	// data> abracadabra
	//  0: bra
	//  0: bra
	RegularExpression expr2("bra");
	assert (expr2.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);
	assert (matches[0].offset == 1);
	assert (matches[0].length == 3);
	assert (matches[1].offset == 8);
	assert (matches[1].length == 3);

	// Non-overlapping matches
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /.a./g
	// data> abracadabra
	//  0: rac
	//  0: dab
	RegularExpression expr3(".a.");
	assert (expr3.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);
	assert (matches[0].offset == 2);
	assert (matches[0].length == 3);
	assert (matches[1].offset == 6);
	assert (matches[1].length == 3);

	// Zero length matches
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> //g
	// data> abracadabra
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	RegularExpression expr4("");
	assert (expr4.matchAll(subject, matches) == 12);
	assert (matches.size() == 12);

	// Look ahead
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /a.(?=..)/g
	// data> abracadabra
	//  0: ab
	//  0: ac
	//  0: ad
	//  0: ab
	RegularExpression expr5("a.(?=..)");
	assert (expr5.matchAll(subject, matches) == 4);
	assert (matches.size() == 4);
	assert (matches[0].offset == 0);
	assert (matches[0].length == 2);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 2);
	assert (matches[2].offset == 5);
	assert (matches[2].length == 2);
	assert (matches[3].offset == 7);
	assert (matches[3].length == 2);

	// Overlapped matches using look ahead
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /(?=(.a.))/g
	// data> abracadabra
	//  0: 
	//  1: rac
	//  0: 
	//  1: cad
	//  0: 
	//  1: dab
	std::vector<RegularExpression::MatchVec> vecmatches;
	RegularExpression expr6("(?=(.a.))");
	assert (expr6.matchAll(subject, 0, vecmatches) == 3);
	assert (vecmatches.size() == 3);
	assert (vecmatches[0][1].offset == 2);
	assert (vecmatches[0][1].length == 3);
	assert (vecmatches[1][1].offset == 4);
	assert (vecmatches[1][1].length == 3);
	assert (vecmatches[2][1].offset == 6);
	assert (vecmatches[2][1].length == 3);

	// Anchored expression
	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /^./g
	// data> abracadabra
	//  0: a
	RegularExpression expr7("^.");
	assert (expr7.matchAll(subject, matches) == 1);
	assert (matches.size() == 1);
	assert (matches[0].offset == 0);
	assert (matches[0].length == 1);
}


void RegularExpressionTest::testMatchAllCapture()
{
	RegularExpression::MatchVec matches;

	// Issue 643 example
	std::string subject = "abc1xyzabc2xyzabc3";
	RegularExpression expr("abc(\\d)");
	assert (expr.matchAll(subject, 0, matches) == 3);
	assert (matches[0].offset == 3);
	assert (matches[0].length == 1);
	assert (matches[1].offset == 10);
	assert (matches[1].length == 1);
	assert (matches[2].offset == 17);
	assert (matches[2].length == 1);
}


void RegularExpressionTest::testMatchAllUtf8()
{
	RegularExpression::MatchVec matches;

	// UTF8 sequence
	std::string valid = "\xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1\x80";
	RegularExpression expr1("\xd0\xbc\xd0\xb8\xd1\x80", RegularExpression::RE_UTF8);
	assert (expr1.matchAll(valid, matches) == 2);
	assert (matches.size() == 2);
	assert (matches[0].offset == 0);
	assert (matches[0].length == 6);
	assert (matches[1].offset == 7);
	assert (matches[1].length == 6);

	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> //8g
	// data> \xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1\x80
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	RegularExpression exprEmpty("", RegularExpression::RE_UTF8);
	assert (exprEmpty.matchAll(valid, matches) == 8);

	// $ ./pcretest -d
	// PCRE version 8.36 2014-09-26
	// 
	//   re> //g
	// ------------------------------------------------------------------
	//   0   3 Bra
	//   3   3 Ket
	//   6     End
	// ------------------------------------------------------------------
	// Capturing subpattern count = 0
	// May match empty string
	// No options
	// No first char
	// No need char
	// data> \xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1\x80
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	//  0: 
	RegularExpression exprEmptyAscii("");
	assert (exprEmptyAscii.matchAll(valid, matches) == 14);

	// $ ./pcretest 
	// PCRE version 8.36 2014-09-26
	//
	//   re> /./8g
	// data> \xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1\x80
	//  0: \x{43c}
	//  0: \x{438}
	//  0: \x{440}
	//  0:  
	//  0: \x{43c}
	//  0: \x{438}
	//  0: \x{440}
	RegularExpression exprDot(".", RegularExpression::RE_UTF8);
	assert (exprDot.matchAll(valid, matches) == 7);

	// $ ./pcretest -d
	// PCRE version 8.36 2014-09-26
	// 
	//   re> /./g
	// ------------------------------------------------------------------
	//   0   4 Bra
	//   3     Any
	//   4   4 Ket
	//   7     End
	// ------------------------------------------------------------------
	// Capturing subpattern count = 0
	// No options
	// No first char
	// No need char
	// data> \xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1\x80
	//  0: \xd0
	//  0: \xbc
	//  0: \xd0
	//  0: \xb8
	//  0: \xd1
	//  0: \x80
	//  0:  
	//  0: \xd0
	//  0: \xbc
	//  0: \xd0
	//  0: \xb8
	//  0: \xd1
	//  0: \x80
	RegularExpression exprDotAscii(".");
	assert (exprDotAscii.matchAll(valid, matches) == 13);

	// Invalid UTF8 sequence
	// $ ./pcretest -d
	// PCRE version 8.36 2014-09-26
	//
	//   re> //8?g
	// ------------------------------------------------------------------
	//   0   3 Bra
	//   3   3 Ket
	//   6     End
	// ------------------------------------------------------------------
	// Capturing subpattern count = 0
	// May match empty string
	// Options: utf no_utf_check
	// No first char
	// No need char
	// data> \xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1
	// Error -10 (bad UTF-8 string) offset=11 reason=1
	std::string invalid = "\xd0\xbc\xd0\xb8\xd1\x80\x20\xd0\xbc\xd0\xb8\xd1";
	bool gotException = false;
	try
	{
		exprEmpty.matchAll(invalid, matches, RegularExpression::RE_NO_UTF8_CHECK);
	}
	catch (const RegularExpressionException&)
	{
		gotException = true;
	}
	assert (gotException);
}


void RegularExpressionTest::testMatchAllPcreTest4()
{
	// NOTES:  The output from pcretest is not all that
	// clear in these examples.  But zero-length matches
	// occur before non-zero length matches at the same
	// offset.  Offset and length are bytes not chars.

	std::vector<RegularExpression::MatchVec> vecMatches;
	RegularExpression::MatchVec matches;

	// $ ./pcretest -q
	//   re> /(|a)/g8
	// data> catac
	//  0: 
	//  1: 
	//  0: 
	//  1: 
	//  0: a
	//  1: a
	//  0: 
	//  1: 
	//  0: 
	//  1: 
	//  0: a
	//  1: a
	//  0: 
	//  1: 
	//  0: 
	//  1: 
	RegularExpression expr1("(|a)", RegularExpression::RE_UTF8);
	std::string subject = "catac";
	assert (expr1.matchAll(subject, 0, vecMatches, 0) == 8);
	assert (vecMatches.size() == 8);
	assert (expr1.matchAll(subject, matches) == 8);
	assert (matches.size() == 8);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 1);
	assert (matches[1].length == 0);
	assert (matches[2].offset == 1);
	assert (matches[2].length == 1);
	assert (matches[3].offset == 2);
	assert (matches[3].length == 0);
	assert (matches[4].offset == 3);
	assert (matches[4].length == 0);
	assert (matches[5].offset == 3);
	assert (matches[5].length == 1);
	assert (matches[6].offset == 4);
	assert (matches[6].length == 0);
	assert (matches[7].offset == 5);
	assert (matches[7].length == 0);

	// \x{256} is U+0256 == c9 96 (hex) (Latin small letter d with tail)
	// $ ./pcretest -q
	//   re> /(|a)/g8
	// data> a\x{256}a
	//  0: 
	//  1: 
	//  0: a
	//  1: a
	//  0: 
	//  1: 
	//  0: 
	//  1: 
	//  0: a
	//  1: a
	//  0: 
	//  1: 
	subject = "a\xc9\x96" "a"; // 'a' is a valid hex digit, so this keeps the compiler happy
	assert (expr1.matchAll(subject, 0, vecMatches, 0) == 6);
	assert (vecMatches.size() == 6);
	assert (expr1.matchAll(subject, matches) == 6);
	assert (matches.size() == 6);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 0);
	assert (matches[1].length == 1);
	assert (matches[2].offset == 1);
	assert (matches[2].length == 0);
	assert (matches[3].offset == 3);
	assert (matches[3].length == 0);
	assert (matches[4].offset == 3);
	assert (matches[4].length == 1);
	assert (matches[5].offset == 4);
	assert (matches[5].length == 0);

	assert (vecMatches[0][0].offset == 0);
	assert (vecMatches[0][0].length == 0);
	assert (vecMatches[1][0].offset == 0);
	assert (vecMatches[1][0].length == 1);
	assert (vecMatches[2][0].offset == 1);
	assert (vecMatches[2][0].length == 0);
	assert (vecMatches[3][0].offset == 3);
	assert (vecMatches[3][0].length == 0);
	assert (vecMatches[4][0].offset == 3);
	assert (vecMatches[4][0].length == 1);
	assert (vecMatches[5][0].offset == 4);
	assert (vecMatches[5][0].length == 0);

	// \x{123} is U+0123 == c4 a3 (hex) (Latin small letter g with cedilla)
	// $ ./pcretest -q 
	//   re> /A*/g8
	// data> AAB\x{123}BAA
	//  0: AA
	//  0: 
	//  0: 
	//  0: 
	//  0: AA
	//  0: 
	RegularExpression expr2("A*", RegularExpression::RE_UTF8);
	subject = "AAB\xc4\xa3" "BAA"; // 'B' is a valid hex digit, so this keeps the compiler happy
	assert (expr2.matchAll(subject, 0, vecMatches, 0) == 6);
	assert (vecMatches.size() == 6);
	assert (expr2.matchAll(subject, matches) == 6);
	assert (matches.size() == 6);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 2);
	assert (matches[1].offset == 2);
	assert (matches[1].length == 0);
	assert (matches[2].offset == 3);
	assert (matches[2].length == 0);
	assert (matches[3].offset == 5);
	assert (matches[3].length == 0);
	assert (matches[4].offset == 6);
	assert (matches[4].length == 2);
	assert (matches[5].offset == 8);
	assert (matches[5].length == 0);
}


void RegularExpressionTest::testMatchAllPcreTest2()
{
	std::vector<RegularExpression::MatchVec> vecMatches;
	RegularExpression::MatchVec matches;

	// $ ./pcretest -q
	//   re> /(*ANY).*/g
	// data> abc\r\ndef
	//  0: abc
	//  0: 
	//  0: def
	//  0: 
	RegularExpression expr1(".*", RegularExpression::RE_NEWLINE_ANY);
	std::string subject = "abc\r\ndef";
	assert (expr1.matchAll(subject, 0, vecMatches, 0) == 4);
	assert (vecMatches.size() == 4);
	assert (expr1.matchAll(subject, matches) == 4);
	assert (matches.size() == 4);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 3);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 0);
	assert (matches[2].offset == 5);
	assert (matches[2].length == 3);
	assert (matches[3].offset == 8);
	assert (matches[3].length == 0);

	// $ ./pcretest -q
	//   re> /(*ANYCRLF).*/g
	// data> abc\r\ndef
	//  0: abc
	//  0: 
	//  0: def
	//  0: 
	RegularExpression expr2(".*", RegularExpression::RE_NEWLINE_ANYCRLF);
	assert (expr2.matchAll(subject, 0, vecMatches, 0) == 4);
	assert (vecMatches.size() == 4);
	assert (expr2.matchAll(subject, matches) == 4);
	assert (matches.size() == 4);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 3);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 0);
	assert (matches[2].offset == 5);
	assert (matches[2].length == 3);
	assert (matches[3].offset == 8);
	assert (matches[3].length == 0);

	// $ ./pcretest -q
	//   re> /(*CRLF).*/g
	// data> abc\r\ndef
	//  0: abc
	//  0: 
	//  0: def
	//  0: 
	RegularExpression expr3(".*", RegularExpression::RE_NEWLINE_CRLF);
	assert (expr3.matchAll(subject, 0, vecMatches, 0) == 4);
	assert (vecMatches.size() == 4);
	assert (expr3.matchAll(subject, matches) == 4);
	assert (matches.size() == 4);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 3);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 0);
	assert (matches[2].offset == 5);
	assert (matches[2].length == 3);
	assert (matches[3].offset == 8);
	assert (matches[3].length == 0);

	// Not a port of a PCRE test but seems an appropriate place to put it.
	// $ ./pcretest -q
	//   re> /(*ANY).*/g
	// data> abc\r
	//  0: abc
	//  0: 
	//  0: 
	RegularExpression expr4(".*", RegularExpression::RE_NEWLINE_ANY);
	subject = "abc\r";
	assert (expr4.matchAll(subject, 0, vecMatches, 0) == 3);
	assert (vecMatches.size() == 3);
	assert (expr4.matchAll(subject, matches) == 3);
	assert (matches.size() == 3);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 3);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 0);
	assert (matches[2].offset == 4);
	assert (matches[2].length == 0);
}


void RegularExpressionTest::testMatchAllPcreTest8()
{
	std::vector<RegularExpression::MatchVec> vecMatches;
	RegularExpression::MatchVec matches;

	// $ ./pcretest -q
	//   re> /.*?/g
	// data> abc
	//  0: 
	//  0: a
	//  0: 
	//  0: b
	//  0: 
	//  0: c
	//  0: 
	RegularExpression expr1(".*?");
	std::string subject = "abc";
	assert (expr1.matchAll(subject, 0, vecMatches, 0) == 7);
	assert (vecMatches.size() == 7);
	assert (expr1.matchAll(subject, matches) == 7);
	assert (matches.size() == 7);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 0);
	assert (matches[1].length == 1);
	assert (matches[2].offset == 1);
	assert (matches[2].length == 0);
	assert (matches[3].offset == 1);
	assert (matches[3].length == 1);
	assert (matches[4].offset == 2);
	assert (matches[4].length == 0);
	assert (matches[5].offset == 2);
	assert (matches[5].length == 1);
	assert (matches[6].offset == 3);
	assert (matches[6].length == 0);

	// NOTE: The '+' option shows the text after the n'th match as n+
	// $ ./pcretest -q
	//   re> /\b/g+
	// data> abc
	//  0: 
	//  0+ abc
	//  0: 
	//  0+ 
	RegularExpression expr2("\\b");
	subject = "abc";
	assert (expr2.matchAll(subject, 0, vecMatches, 0) == 2);
	assert (vecMatches.size() == 2);
	assert (expr2.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 3);
	assert (matches[1].length == 0);

	// \G matches only at the start and
	// immediately after the previous match
	// $ ./pcretest -q
	//   re> /\Gabc./g
	// data> abc1abc2xyzabc3
	//  0: abc1
	//  0: abc2
	RegularExpression expr3("\\Gabc.");
	subject = "abc1abc2xyzabc3";
	assert (expr3.matchAll(subject, 0, vecMatches, 0) == 2);
	assert (vecMatches.size() == 2);
	assert (expr3.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 4);
	assert (matches[1].offset == 4);
	assert (matches[1].length == 4);

	// $ ./pcretest -q
	//   re> /abc./g
	// data> abc1abc2xyzabc3
	//  0: abc1
	//  0: abc2
	//  0: abc3
	RegularExpression expr4("abc.");
	subject = "abc1abc2xyzabc3";
	assert (expr4.matchAll(subject, 0, vecMatches, 0) == 3);
	assert (vecMatches.size() == 3);
	assert (expr4.matchAll(subject, matches) == 3);
	assert (matches.size() == 3);

	assert (matches[0].offset == 0);
	assert (matches[0].length == 4);
	assert (matches[1].offset == 4);
	assert (matches[1].length == 4);
	assert (matches[2].offset == 11);
	assert (matches[2].length == 4);

	// $ ./pcretest -q
	//   re> /(?=C)/g+
	// data> ABCDECBA
	//  0: 
	//  0+ CDECBA
	//  0: 
	//  0+ CBA
	RegularExpression expr5("(?=C)");
	subject = "ABCDECBA";
	assert (expr5.matchAll(subject, 0, vecMatches, 0) == 2);
	assert (vecMatches.size() == 2);
	assert (expr5.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);

	assert (matches[0].offset == 2);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 5);
	assert (matches[1].length == 0);
}


void RegularExpressionTest::testMatchAllPcreTest1()
{
	std::vector<RegularExpression::MatchVec> vecMatches;
	RegularExpression::MatchVec matches;

	// $ ./pcretest -q
	//   re> /\Z/g+
	// data> abc\n
	//  0: 
	//  0+ \x0a
	//  0: 
	//  0+ 
	RegularExpression expr1("\\Z");
	std::string subject = "abc\n";
	assert (expr1.matchAll(subject, 0, vecMatches, 0) == 2);
	assert (vecMatches.size() == 2);
	assert (expr1.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);

	assert (matches[0].offset == 3);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 4);
	assert (matches[1].length == 0);

	// $ ./pcretest -q
	//   re> /abc\K|def\K/g+
	// data> Xabcdefghi
	//  0: 
	//  0+ defghi
	//  0: 
	//  0+ ghi
	RegularExpression expr2("abc\\K|def\\K");
	subject = "Xabcdefghi";
	assert (expr2.matchAll(subject, 0, vecMatches, 0) == 2);
	assert (vecMatches.size() == 2);
	assert (expr2.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);

	assert (matches[0].offset == 4);
	assert (matches[0].length == 0);
	assert (matches[1].offset == 7);
	assert (matches[1].length == 0);

	// $ ./pcretest -q
	//   re> /ab\Kc|de\Kf/g+
	// data> Xabcdefghi
	//  0: c
	//  0+ defghi
	//  0: f
	//  0+ ghi
	RegularExpression expr3("ab\\Kc|de\\Kf");
	subject = "Xabcdefghi";
	assert (expr3.matchAll(subject, 0, vecMatches, 0) == 2);
	assert (vecMatches.size() == 2);
	assert (expr3.matchAll(subject, matches) == 2);
	assert (matches.size() == 2);

	assert (matches[0].offset == 3);
	assert (matches[0].length == 1);
	assert (matches[1].offset == 6);
	assert (matches[1].length == 1);
}


void RegularExpressionTest::testExtract()
{
	RegularExpression re("[0-9]+");
	std::string str;
	assert (re.extract("123", str) == 1);
	assert (str == "123");

	assert (re.extract("abc123def", 0, str) == 1);
	assert (str == "123");

	assert (re.extract("abcdef", 0, str) == 0);
	assert (str == "");

	assert (re.extract("abc123def", 3, str) == 1);
	assert (str == "123");
}


void RegularExpressionTest::testSplit1()
{
	RegularExpression re("[0-9]+");
	std::vector<std::string> strings;
	assert (re.split("123", 0, strings) == 1);
	assert (strings.size() == 1);
	assert (strings[0] == "123");

	assert (re.split("abc123def", 0, strings) == 1);
	assert (strings.size() == 1);
	assert (strings[0] == "123");

	assert (re.split("abcdef", 0, strings) == 0);
	assert (strings.empty());

	assert (re.split("abc123def", 3, strings) == 1);
	assert (strings.size() == 1);
	assert (strings[0] == "123");
}


void RegularExpressionTest::testSplit2()
{
	RegularExpression re("([0-9]+) ([0-9]+)");
	std::vector<std::string> strings;
	assert (re.split("123 456", 0, strings) == 3);
	assert (strings.size() == 3);
	assert (strings[0] == "123 456");
	assert (strings[1] == "123");
	assert (strings[2] == "456");

	assert (re.split("abc123 456def", 0, strings) == 3);
	assert (strings.size() == 3);
	assert (strings[0] == "123 456");
	assert (strings[1] == "123");
	assert (strings[2] == "456");
}


void RegularExpressionTest::testSubst1()
{
	RegularExpression re("[0-9]+");
	std::string s = "123";
	assert (re.subst(s, "ABC") == 1);
	assert (s == "ABC");
	assert (re.subst(s, "123") == 0);

	s = "123";
	assert (re.subst(s, "AB$0CD") == 1);
	assert (s == "AB123CD");

	s = "123";
	assert (re.subst(s, "AB$1CD") == 1);
	assert (s == "ABCD");

	s = "123";
	assert (re.subst(s, "AB$2CD") == 1);
	assert (s == "ABCD");

	s = "123";
	assert (re.subst(s, "AB$$CD") == 1);
	assert (s == "AB$$CD");

	s = "123";
	assert (re.subst(s, "AB$0CD", RegularExpression::RE_NO_VARS) == 1);
	assert (s == "AB$0CD");
}


void RegularExpressionTest::testSubst2()
{
	RegularExpression re("([0-9]+) ([0-9]+)");
	std::string s = "123 456";
	assert (re.subst(s, "$2-$1") == 1);
	assert (s == "456-123");
}


void RegularExpressionTest::testSubst3()
{
	RegularExpression re("[0-9]+");
	std::string s = "123 456 789";
	assert (re.subst(s, "n", RegularExpression::RE_GLOBAL) == 3);
	assert (s == "n n n");
}


void RegularExpressionTest::testSubst4()
{
	RegularExpression re("[0-9]+");
	std::string s = "ABC 123 456 789 DEF";
	assert (re.subst(s, "n", RegularExpression::RE_GLOBAL) == 3);
	assert (s == "ABC n n n DEF");
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
	assert (re.match("abcd 1234", 0, matches) == 3);
	assert (matches[0].name == "");
	assert (matches[1].name == "group1");
	assert (matches[2].name == "group2");
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
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAll);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAllCapture);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAllUtf8);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAllPcreTest4);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAllPcreTest2);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAllPcreTest8);
	CppUnit_addTest(pSuite, RegularExpressionTest, testMatchAllPcreTest1);
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
