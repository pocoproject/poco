//
// DoubleByteEncodingTest.cpp
//
// Copyright (c) 2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: Apache-2.0
//


#include "DoubleByteEncodingTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ISO8859_4Encoding.h"
#include "Poco/Windows950Encoding.h"


DoubleByteEncodingTest::DoubleByteEncodingTest(const std::string& name): CppUnit::TestCase(name)
{
}


DoubleByteEncodingTest::~DoubleByteEncodingTest()
{
}


void DoubleByteEncodingTest::testSingleByte()
{
	Poco::ISO8859_4Encoding enc;

	assertTrue (std::string(enc.canonicalName()) == "ISO-8859-4");
	assertTrue (enc.isA("Latin4"));

	unsigned char seq1[] = { 0xF8 }; // 0x00F8 LATIN SMALL LETTER O WITH STROKE
	assertTrue (enc.convert(seq1) == 0x00F8);
	assertTrue (enc.queryConvert(seq1, 1) == 0x00F8);
	assertTrue (enc.sequenceLength(seq1, 1) == 1);

	unsigned char seq2[] = { 0xF9 }; // 0x0173 LATIN SMALL LETTER U WITH OGONEK
	assertTrue (enc.convert(seq2) == 0x0173);
	assertTrue (enc.queryConvert(seq2, 1) == 0x0173);
	assertTrue (enc.sequenceLength(seq2, 1) == 1);
}


void DoubleByteEncodingTest::testSingleByteReverse()
{
	Poco::ISO8859_4Encoding enc;

	unsigned char seq[2];

	assertTrue (enc.convert(0x00F8, seq, 2) == 1);
	assertTrue (seq[0] == 0xF8);

	assertTrue (enc.convert(0x0173, seq, 2) == 1);
	assertTrue (seq[0] == 0xF9);

	assertTrue (enc.convert(0x3000, seq, 2) == 0);
}


void DoubleByteEncodingTest::testDoubleByte()
{
	Poco::Windows950Encoding enc;

	assertTrue (std::string(enc.canonicalName()) == "windows-950");
	assertTrue (enc.isA("Windows-950"));
	assertTrue (enc.isA("cp950"));

	unsigned char seq1[] = { 0x41 }; // 0x0041 LATIN CAPITAL LETTER A
	assertTrue (enc.convert(seq1) == 0x0041);
	assertTrue (enc.queryConvert(seq1, 1) == 0x0041);
	assertTrue (enc.sequenceLength(seq1, 1) == 1);

	unsigned char seq2[] = { 0xA1, 0x40 }; // 0x3000 IDEOGRAPHIC SPACE
	assertTrue (enc.convert(seq2) == 0x3000);
	assertTrue (enc.queryConvert(seq2, 1) == -2);
	assertTrue (enc.queryConvert(seq2, 2) == 0x3000);
	assertTrue (enc.sequenceLength(seq2, 1) == 2);
	assertTrue (enc.sequenceLength(seq2, 2) == 2);

	unsigned char seq3[] = { 0x92 }; // invalid
	assertTrue (enc.convert(seq3) == -1);
	assertTrue (enc.queryConvert(seq3, 1) == -1);
	assertTrue (enc.sequenceLength(seq3, 1) == -1);
}


void DoubleByteEncodingTest::testDoubleByteReverse()
{
	Poco::Windows950Encoding enc;

	unsigned char seq[2];

	assertTrue (enc.convert(0x0041, seq, 2) == 1);
	assertTrue (seq[0] == 0x41);

	assertTrue (enc.convert(0x3000, seq, 2) == 2);
	assertTrue (seq[0] == 0xA1);
	assertTrue (seq[1] == 0x40);

	assertTrue (enc.convert(0x3004, seq, 2) == 0);
}


void DoubleByteEncodingTest::setUp()
{
}


void DoubleByteEncodingTest::tearDown()
{
}


CppUnit::Test* DoubleByteEncodingTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DoubleByteEncodingTest");

	CppUnit_addTest(pSuite, DoubleByteEncodingTest, testSingleByte);
	CppUnit_addTest(pSuite, DoubleByteEncodingTest, testSingleByteReverse);
	CppUnit_addTest(pSuite, DoubleByteEncodingTest, testDoubleByte);
	CppUnit_addTest(pSuite, DoubleByteEncodingTest, testDoubleByteReverse);

	return pSuite;
}
