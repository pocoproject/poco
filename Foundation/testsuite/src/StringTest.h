//
// StringTest.h
//
// Definition of the StringTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef StringTest_INCLUDED
#define StringTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/NumericString.h"
#include "Poco/MemoryStream.h"
#include "Poco/NumberFormatter.h"


class StringTest: public CppUnit::TestCase
{
public:
	StringTest(const std::string& name);
	~StringTest();

	void testTrimLeft();
	void testTrimLeftInPlace();
	void testTrimRight();
	void testTrimRightInPlace();
	void testTrim();
	void testTrimInPlace();
	void testToUpper();
	void testToLower();
	void testIstring();
	void testIcompare();
	void testCILessThan();
	void testTranslate();
	void testTranslateInPlace();
	void testReplace();
	void testReplaceInPlace();
	void testCat();
	void testStartsWith();
	void testEndsWith();

	void testStringToInt();
	void testStringToFloat();
	void testStringToDouble();
	void testNumericStringPadding();
	void testNumericStringLimit();
	void testStringToFloatError();
	void testNumericLocale();
	void benchmarkStrToFloat();
	void benchmarkStrToInt();

	void testIntToString();
	void testFloatToString();
	void benchmarkFloatToStr();

	void testJSONString();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

	template <typename T>
	void stringToInt()
	{
		T result = 0;
		if (123 <= std::numeric_limits<T>::max())
			assertTrue (Poco::strToInt("123", result, 10)); assertTrue (result == 123);

		assertTrue (Poco::strToInt("0", result, 10)); assertTrue (result == 0);
		assertTrue (Poco::strToInt("000", result, 10)); assertTrue (result == 0);

		if (std::numeric_limits<T>::is_signed && (-123 > std::numeric_limits<T>::min()))
			{ assertTrue (Poco::strToInt("-123", result, 10)); assertTrue (result == -123); }
		if (0x123 < std::numeric_limits<T>::max())
			{ assertTrue (Poco::strToInt("123", result, 0x10)); assertTrue (result == 0x123); }
		if (0x12ab < std::numeric_limits<T>::max())
			{ assertTrue (Poco::strToInt("12AB", result, 0x10)); assertTrue (result == 0x12ab); }
		if (123 < std::numeric_limits<T>::max())
			{ assertTrue (Poco::strToInt("00", result, 0x10)); assertTrue (result == 0); }
		if (0123 < std::numeric_limits<T>::max())
			{ assertTrue (Poco::strToInt("123", result, 010));  assertTrue (result == 0123); }
		if (0123 < std::numeric_limits<T>::max())
			{ assertTrue (Poco::strToInt("0123", result, 010)); assertTrue (result == 0123); }

		assertTrue (Poco::strToInt("0", result, 010)); assertTrue (result == 0);
		assertTrue (Poco::strToInt("000", result, 010)); assertTrue (result == 0);
	}

	template <typename Larger, typename Smaller>
	void numericStringLimitSameSign()
	{
		Larger l = std::numeric_limits<Smaller>::max();
		std::string str = Poco::NumberFormatter::format(l);

		Smaller s;
		assertTrue(Poco::strToInt<Smaller>(str, s, 10));
		assertTrue(s == std::numeric_limits<Smaller>::max());
		++l; str = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(str, s, 10));
		++l; str = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(str, s, 10));
		++l; str = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(str, s, 10));
		++l; str = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(str, s, 10));
		++l; str = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(str, s, 10));
		++l; str = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(str, s, 10));
	}

	template <typename Larger, typename Smaller>
	void numericStringLowerLimit()
	{
		Larger l = std::numeric_limits<Smaller>::min();
		std::string val = Poco::NumberFormatter::format(l);
		Smaller s = -1;
		assertFalse(s == std::numeric_limits<Smaller>::min());
		assertTrue(Poco::strToInt<Smaller>(val, s, 10));
		assertTrue (s == std::numeric_limits<Smaller>::min());
		assertTrue(s == std::numeric_limits<Smaller>::min());
		--l; val = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(val, s, 10));
		--l; val = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(val, s, 10));
		--l; val = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(val, s, 10));
		--l; val = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(val, s, 10));
		--l; val = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(val, s, 10));
		--l; val = Poco::NumberFormatter::format(l);
		assertFalse(Poco::strToInt<Smaller>(val, s, 10));
	}

	template <typename T>
	bool parseStream(const std::string& s, T& value)
	{
		Poco::MemoryInputStream istr(s.data(), s.size());
#if !defined(POCO_NO_LOCALE)
		istr.imbue(std::locale::classic());
#endif
		istr >> value;
		return istr.eof() && !istr.fail();
	}
};


#endif // StringTest_INCLUDED
