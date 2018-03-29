//
// TextBufferIteratorTest.cpp
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TextBufferIteratorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/TextBufferIterator.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"


using Poco::TextBufferIterator;
using Poco::Latin1Encoding;
using Poco::UTF8Encoding;
using Poco::UTF16Encoding;


TextBufferIteratorTest::TextBufferIteratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


TextBufferIteratorTest::~TextBufferIteratorTest()
{
}


void TextBufferIteratorTest::testEmptyLatin1()
{
	Latin1Encoding encoding;
	const char* text = "";
	TextBufferIterator it(text, encoding);
	TextBufferIterator end(it.end());
	
	assertTrue (it == end);
}


void TextBufferIteratorTest::testOneLatin1()
{
	Latin1Encoding encoding;
	const char* text = "x";
	TextBufferIterator it(text, encoding);
	TextBufferIterator end(it.end());
	
	assertTrue (it != end);
	assertTrue (*it == 'x');
	++it;
	assertTrue (it == end);
}


void TextBufferIteratorTest::testLatin1()
{
	Latin1Encoding encoding;
	const char* text = "Latin1";
	TextBufferIterator it(text, encoding);
	TextBufferIterator end(it.end());
	
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
	
	const char* empty = "";
	it  = TextBufferIterator(empty, encoding);
	end = it.end();
	assertTrue (it == end);
}


void TextBufferIteratorTest::testEmptyUTF8()
{
	UTF8Encoding encoding;
	const char* text = "";
	TextBufferIterator it(text, encoding);
	TextBufferIterator end(text);
	
	assertTrue (it == end);
}


void TextBufferIteratorTest::testOneUTF8()
{
	UTF8Encoding encoding;
	
	// 1 byte sequence
	const char* text = "x";
	TextBufferIterator it(text, encoding);
	TextBufferIterator end(it.end());
	
	assertTrue (it != end);
	assertTrue (*it == 'x');
	++it;
	assertTrue (it == end);
	
	unsigned char data[Poco::TextEncoding::MAX_SEQUENCE_LENGTH];
	
	// 2 byte sequence
	int n = encoding.convert(0xab, data, sizeof(data));
	assertTrue (n == 2);
	it  = TextBufferIterator(reinterpret_cast<const char*>(data), n, encoding);
	end = it.end();
	
	assertTrue (it != end);
	assertTrue (*it++ == 0xab);
	assertTrue (it == end);

	// 3 byte sequence
	n = encoding.convert(0xabcd, data, sizeof(data));
	assertTrue (n == 3);
	it  = TextBufferIterator(reinterpret_cast<const char*>(data), n, encoding);
	end = it.end();
	
	assertTrue (it != end);
	assertTrue (*it++ == 0xabcd);
	assertTrue (it == end);

	// 4 byte sequence
	n = encoding.convert(0xabcde, data, sizeof(data));
	assertTrue (n == 4);
	it  = TextBufferIterator(reinterpret_cast<const char*>(data), n, encoding);
	end = it.end();
	
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


void TextBufferIteratorTest::testUTF8()
{
	UTF8Encoding encoding;
	const unsigned char text[] = {0x20, 0xce, 0xba, 0xe1, 0xbd, 0xb9, 0xcf, 0x83, 0xce, 0xbc, 0xce, 0xb5, 0x20, 0x00};
	TextBufferIterator it(reinterpret_cast<const char*>(text), encoding);
	TextBufferIterator end(it.end());
	
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


void TextBufferIteratorTest::testUTF8Supplementary()
{
	UTF8Encoding encoding; 
	const unsigned char text[] = {0x41, 0x42, 0xf0, 0x90, 0x82, 0xa4, 0xf0, 0xaf, 0xa6, 0xa0, 0xf0, 0xaf, 0xa8, 0x9d, 0x00};
	TextBufferIterator it(reinterpret_cast<const char*>(text), encoding);
	TextBufferIterator end(it.end());
	
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


void TextBufferIteratorTest::testUTF16Supplementary()
{
	UTF16Encoding encoding; 
	const Poco::UInt16 text[] = { 0x0041, 0x0042, 0xD800, 0xDCA4, 0xD87E, 0xDDA0, 0xD87E, 0xDE1D};
	TextBufferIterator it(reinterpret_cast<const char*>(text), sizeof(text), encoding);
	TextBufferIterator end(it.end());
	
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


void TextBufferIteratorTest::testSwap()
{
	Latin1Encoding encoding;
	const char* text = "x";
	TextBufferIterator it1(text, encoding);
	TextBufferIterator it2(text, encoding);
	TextBufferIterator end(it1.end());
	
	assertTrue (it1 == it2);
	it2.swap(end);
	assertTrue (it1 != it2);
	it2.swap(end);
	assertTrue (it1 == it2);
}


void TextBufferIteratorTest::setUp()
{
}


void TextBufferIteratorTest::tearDown()
{
}


CppUnit::Test* TextBufferIteratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TextBufferIteratorTest");

	CppUnit_addTest(pSuite, TextBufferIteratorTest, testEmptyLatin1);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testOneLatin1);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testLatin1);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testEmptyUTF8);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testOneUTF8);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testUTF8);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testUTF8Supplementary);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testUTF16Supplementary);
	CppUnit_addTest(pSuite, TextBufferIteratorTest, testSwap);

	return pSuite;
}
