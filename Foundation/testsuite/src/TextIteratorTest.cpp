//
// TextIteratorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TextIteratorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/TextIterator.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"
#include "Poco/UTF32Encoding.h"


using Poco::TextIterator;
using Poco::Latin1Encoding;
using Poco::UTF8Encoding;
using Poco::UTF16Encoding;
using Poco::UTF32Encoding;


TextIteratorTest::TextIteratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


TextIteratorTest::~TextIteratorTest()
{
}


void TextIteratorTest::testEmptyLatin1()
{
	Latin1Encoding encoding;
	std::string text;
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it == end);
}


void TextIteratorTest::testOneLatin1()
{
	Latin1Encoding encoding;
	std::string text("x");
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it == 'x');
	++it;
	assertTrue (it == end);
}


void TextIteratorTest::testLatin1()
{
	Latin1Encoding encoding;
	std::string text("Latin1");
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it++ == 'L');
	assertTrue (it != end);
	assertTrue (*it++ == 'a');
	assertTrue (it != end);
	assertTrue (*it++ == 't');
	assertTrue (it != end);
	assertTrue (*it++ == 'i');
	assertTrue (it != end);
	assertTrue (*it++ == 'n');
	assertTrue (it != end);
	assertTrue (*it++ == '1');
	assertTrue (it == end);

	std::string empty;
	it  = TextIterator(empty, encoding);
	end = TextIterator(empty);
	assertTrue (it == end);
}


void TextIteratorTest::testEmptyUTF8()
{
	UTF8Encoding encoding;
	std::string text;
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it == end);
}


void TextIteratorTest::testOneUTF8()
{
	UTF8Encoding encoding;

	// 1 byte sequence
	std::string text("x");
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it == 'x');
	++it;
	assertTrue (it == end);

	unsigned char data[Poco::TextEncoding::MAX_SEQUENCE_LENGTH];

	// 2 byte sequence
	int n = encoding.convert(0xab, data, sizeof(data));
	assertTrue (n == 2);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0xab);
	assertTrue (it == end);

	// 3 byte sequence
	n = encoding.convert(0xabcd, data, sizeof(data));
	assertTrue (n == 3);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0xabcd);
	assertTrue (it == end);

	// 4 byte sequence
	n = encoding.convert(0xabcde, data, sizeof(data));
	assertTrue (n == 4);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0xabcde);
	assertTrue (it == end);

	// 5 byte sequence - not supported
	n = encoding.convert(0xabcdef, data, sizeof(data));
	assertTrue (n == 0);

	// 6 byte sequence - not supported
	n = encoding.convert(0xfabcdef, data, sizeof(data));
	assertTrue (n == 0);
}


void TextIteratorTest::testUTF8()
{
	UTF8Encoding encoding;
	const unsigned char greek[] = {0x20, 0xce, 0xba, 0xe1, 0xbd, 0xb9, 0xcf, 0x83, 0xce, 0xbc, 0xce, 0xb5, 0x20, 0x00};
	std::string text((const char*) greek);
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0x0020);
	assertTrue (it != end);
	assertTrue (*it++ == 0x03ba);
	assertTrue (it != end);
	assertTrue (*it++ == 0x1f79);
	assertTrue (it != end);
	assertTrue (*it++ == 0x03c3);
	assertTrue (it != end);
	assertTrue (*it++ == 0x03bc);
	assertTrue (it != end);
	assertTrue (*it++ == 0x03b5);
	assertTrue (it != end);
	assertTrue (*it++ == 0x0020);
	assertTrue (it == end);
}


void TextIteratorTest::testUTF8Supplementary()
{
	UTF8Encoding encoding;
	const unsigned char supp[] = {0x41, 0x42, 0xf0, 0x90, 0x82, 0xa4, 0xf0, 0xaf, 0xa6, 0xa0, 0xf0, 0xaf, 0xa8, 0x9d, 0x00};
	std::string text((const char*) supp);
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0x0041);
	assertTrue (it != end);
	assertTrue (*it++ == 0x0042);
	assertTrue (it != end);
	assertTrue (*it++ == 0x100a4);
	assertTrue (it != end);
	assertTrue (*it++ == 0x2f9a0);
	assertTrue (it != end);
	assertTrue (*it++ == 0x2fa1d);
	assertTrue (it == end);
}


void TextIteratorTest::testUTF16Supplementary()
{
	UTF16Encoding encoding;
	const Poco::UInt16 supp [] = { 0x0041, 0x0042, 0xD800, 0xDCA4, 0xD87E, 0xDDA0, 0xD87E, 0xDE1D, 0x00};
	std::string text((const char*) supp, 16);
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0x0041);
	assertTrue (it != end);
	assertTrue (*it++ == 0x0042);
	assertTrue (it != end);
	assertTrue (*it++ == 0x100a4);
	assertTrue (it != end);
	assertTrue (*it++ == 0x2f9a0);
	assertTrue (it != end);
	assertTrue (*it++ == 0x2fa1d);
	assertTrue (it == end);
}


void TextIteratorTest::testSwap()
{
	Latin1Encoding encoding;
	std::string text("x");
	TextIterator it1(text, encoding);
	TextIterator it2(text, encoding);
	TextIterator end(text);

	assertTrue (it1 == it2);
	it2.swap(end);
	assertTrue (it1 != it2);
	it2.swap(end);
	assertTrue (it1 == it2);
}


void TextIteratorTest::testUTF32Invalid1()
{
	UTF32Encoding encoding;
	const Poco::UInt32 data[] = {0x00000041, 0xffffffef, 0x00000041, 0x00000041, 0x00000041, 0x00000041, 0x00};
	std::string text((const char*) data, 24);
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0x41);
	assertTrue (it != end);
	assertTrue (*it++ == -1);
}


void TextIteratorTest::testUTF32Invalid2()
{
	UTF32Encoding encoding;
	const Poco::UInt32 data[] = {0x00000041, 0xfffffffe, 0xfffffffe, 0x00};
	std::string text((const char*) data, 12);
	TextIterator it(text, encoding);
	TextIterator end(text);

	assertTrue (it != end);
	assertTrue (*it++ == 0x41);
	assertTrue (it != end);
	assertTrue (*it++ == -1);
}


void TextIteratorTest::setUp()
{
}


void TextIteratorTest::tearDown()
{
}


CppUnit::Test* TextIteratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TextIteratorTest");

	CppUnit_addTest(pSuite, TextIteratorTest, testEmptyLatin1);
	CppUnit_addTest(pSuite, TextIteratorTest, testOneLatin1);
	CppUnit_addTest(pSuite, TextIteratorTest, testLatin1);
	CppUnit_addTest(pSuite, TextIteratorTest, testEmptyUTF8);
	CppUnit_addTest(pSuite, TextIteratorTest, testOneUTF8);
	CppUnit_addTest(pSuite, TextIteratorTest, testUTF8);
	CppUnit_addTest(pSuite, TextIteratorTest, testUTF8Supplementary);
	CppUnit_addTest(pSuite, TextIteratorTest, testUTF16Supplementary);
	CppUnit_addTest(pSuite, TextIteratorTest, testSwap);
	CppUnit_addTest(pSuite, TextIteratorTest, testUTF32Invalid1);
	CppUnit_addTest(pSuite, TextIteratorTest, testUTF32Invalid2);

	return pSuite;
}
