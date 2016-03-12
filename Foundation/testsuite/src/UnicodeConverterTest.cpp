//
// UnicodeConverterTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/UnicodeConverterTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef POCO_NO_WSTRING


#include "Poco/UnicodeConverter.h"
#include "UnicodeConverterTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/UTFString.h"


UnicodeConverterTest::UnicodeConverterTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


UnicodeConverterTest::~UnicodeConverterTest()
{
}


void UnicodeConverterTest::testUTF16()
{
	runTests<Poco::UTF16String>();
}


void UnicodeConverterTest::testUTF32()
{
	runTests<Poco::UTF32String>();
}


void UnicodeConverterTest::setUp()
{
}


void UnicodeConverterTest::tearDown()
{
}


CppUnit::Test* UnicodeConverterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UnicodeConverterTest");

	CppUnit_addTest(pSuite, UnicodeConverterTest, testUTF16);
	CppUnit_addTest(pSuite, UnicodeConverterTest, testUTF32);

	return pSuite;
}


#endif

