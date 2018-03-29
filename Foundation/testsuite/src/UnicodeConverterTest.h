//
// UnicodeConverterTest.h
//
// Definition of the UnicodeConverterTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef UnicodeConverterTest_INCLUDED
#define UnicodeConverterTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UTFString.h"
#include <cstring>


class UnicodeConverterTest: public CppUnit::TestCase
{
public:
	UnicodeConverterTest(const std::string& name);
	~UnicodeConverterTest();

	void testUTF16();
	void testUTF32();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	template <typename T>
	void runTests()
	{
		const unsigned char supp[] = {0x41, 0x42, 0xf0, 0x90, 0x82, 0xa4, 0xf0, 0xaf, 0xa6, 0xa0, 0xf0, 0xaf, 0xa8, 0x9d, 0x00};
		std::string text((const char*) supp);

		// Convert from UTF-8 to wide
		T wtext, wtext2, wtext3;
		Poco::UnicodeConverter::convert(text, wtext);
		if (sizeof(typename T::value_type) == 2)
			assertTrue (Poco::UnicodeConverter::UTFStrlen(wtext.data()) == 8);
		else if (sizeof(typename T::value_type) == 4)
			assertTrue (Poco::UnicodeConverter::UTFStrlen(wtext.data()) == 5);
		Poco::UnicodeConverter::convert((const char*) supp, strlen((const char*) supp), wtext2);
		Poco::UnicodeConverter::convert((const char*)supp, wtext3);
		assertTrue (wtext == wtext2);
		assertTrue (wtext == wtext3);

		std::string text2, text3, text4;
		assertTrue (text != text2);
		assertTrue (text != text3);
		assertTrue (text != text4);

		// Convert from wide to UTF-8
		Poco::UnicodeConverter::convert(wtext, text2);
		Poco::UnicodeConverter::convert(wtext2, text3);
		Poco::UnicodeConverter::convert(wtext3, text4);

		assertTrue (text == text2);
		assertTrue (text == text3);
		assertTrue (text == text4);
	}
};


#endif // UnicodeConverterTest_INCLUDED
