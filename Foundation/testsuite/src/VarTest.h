//
// VarTest.h
//
// Tests for Any types
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef VarTest_INCLUDED
#define VarTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/VarIterator.h"
#include "Poco/Exception.h"
#include "CppUnit/TestCase.h"


class VarTest: public CppUnit::TestCase
{
public:
	VarTest(const std::string& name);
	~VarTest();

	void testInt8();
	void testInt16();
	void testInt32();
	void testInt64();
	void testUInt8();
	void testUInt16();
	void testUInt32();
	void testUInt64();
	void testBool();
	void testChar();
	void testFloat();
	void testDouble();
	void testLong();
	void testULong();
	void testLongLong();
	void testULongLong();
	void testString();
	void testUDT();
	void testConversionOperator();
	void testComparisonOperators();
	void testArithmeticOperators();
	void testLimitsInt();
	void testLimitsFloat();
	void testCtor();
	void testIsStruct();
	void testIsArray();
	void testArrayIdxOperator();
	void testDynamicPair();
	void testDynamicStructBasics();
	void testOrderedDynamicStructBasics();
	void testDynamicStructString();
	void testDynamicStructEmptyString();
	void testDynamicStructNoEscapeString();
	void testOrderedDynamicStructString();
	void testDynamicStructInt();
	void testOrderedDynamicStructInt();
	void testArrayToString();
	void testArrayToStringEscape();
	void testStructToString();
	void testOrderedStructToString();
	void testStructToStringEscape();
	void testArrayOfStructsToString();
	void testStructWithArraysToString();
	void testJSONDeserializeString();
	void testJSONDeserializePrimitives();
	void testJSONDeserializeArray();
	void testJSONDeserializeStruct();
	void testJSONRoundtripStruct();
	void testJSONDeserializeComplex();
	void testDate();
	void testUUID();
	void testEmpty();
	void testIterator();
	void testSharedPtr();
	void testVarVisitor();

	void setUp();
	void tearDown();
	static CppUnit::Test* suite();

private:
	void testGetIdxMustThrow(Poco::Dynamic::Var& a1, std::vector<Poco::Dynamic::Var>::size_type n);
	void testGetIdxNoThrow(Poco::Dynamic::Var& a1, std::vector<Poco::Dynamic::Var>::size_type n);
	template<typename T>
	void testGetIdx(const Poco::Dynamic::Var& a1, std::vector<Poco::Dynamic::Var>::size_type n, const T& expectedResult)
	{
		Poco::Dynamic::Var val1 = a1[n];
		assertTrue (val1 == expectedResult);

		const Poco::Dynamic::Var c1 = a1;
		assertTrue (a1 == c1); // silence the compiler
		const Poco::Dynamic::Var cval1 = a1[n];
		assertTrue (cval1 == expectedResult);
	}


	template<typename TL, typename TS>
	void testLimitsSigned()
	{
		TL iMin = std::numeric_limits<TS>::min();
		Poco::Dynamic::Var da = iMin - 1;
		try { TS POCO_UNUSED i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}

		TL iMax = std::numeric_limits<TS>::max();
		da = iMax + 1;
		try { TS POCO_UNUSED i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template<typename TL, typename TS>
	void testLimitsFloatToInt()
	{
		Poco::Dynamic::Var da;

		if (std::numeric_limits<TS>::is_signed)
		{
			TL iMin = static_cast<TL>(std::numeric_limits<TS>::min());
			da = iMin * 10;
			try { TS POCO_UNUSED i; i = da.convert<TS>(); fail("must fail"); }
			catch (Poco::RangeException&) {}
		}

		TL iMax = static_cast<TL>(std::numeric_limits<TS>::max());
		da = iMax * 10;
		try { TS POCO_UNUSED i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template<typename TS, typename TU>
	void testLimitsSignedUnsigned()
	{
		assertTrue (std::numeric_limits<TS>::is_signed);
		assertTrue (!std::numeric_limits<TU>::is_signed);

		TS iMin = std::numeric_limits<TS>::min();
		Poco::Dynamic::Var dMin = iMin;
		try { TU POCO_UNUSED i; i = dMin.convert<TU>(); fail("must fail"); }
		catch (Poco::RangeException&) {}

		if constexpr (sizeof(TS) == sizeof(TU))
		{
			TU iMax = std::numeric_limits<TU>::max();
			Poco::Dynamic::Var dMax = iMax;
			try { TS POCO_UNUSED i; i = dMax.convert<TS>(); fail("must fail"); }
			catch (Poco::RangeException&) {}
		}
	}

	template<typename TL, typename TS>
	void testLimitsUnsigned()
	{
		TL iMax = std::numeric_limits<TS>::max();
		Poco::Dynamic::Var da = iMax + 1;
		try { TS POCO_UNUSED i; i = da.convert<TS>(); fail("must fail"); }
		catch (Poco::RangeException&) {}
	}

	template <typename T>
	void testEmptyComparisons()
	{
		Poco::Dynamic::Var da;
		T val = 0;

		assertTrue (da != val);
		assertTrue (val != da);
		assertTrue (!(val == da));
		assertTrue (!(da == val));
		assertTrue (!(da < val));
		assertTrue (!(val < da));
		assertTrue (!(da > val));
		assertTrue (!(val > da));
		assertTrue (!(da <= val));
		assertTrue (!(val <= da));
		assertTrue (!(da >= val));
		assertTrue (!(val >= da));
	}

	template <typename C>
	void testContainerIterator()
	{
		C cont;
		Poco::Dynamic::Var arr(cont);
		int counter = 0;

		// test empty
		assertTrue (arr.size() == 0);
		Poco::Dynamic::Var::Iterator it = arr.begin();
		Poco::Dynamic::Var::Iterator end = arr.end();

		for (; it != end; ++it)
		{
			*it = ++counter;
		}
		assertTrue(counter == 0);

		// test non-empty
		cont.push_back(1);
		cont.push_back("2");
		cont.push_back(3.5);
		arr = cont;
		assertTrue (arr.size() == 3);
		it = arr.begin();
		end = arr.end();
		counter = 0;
		for (; it != end; ++it)
		{
			switch (++counter)
			{
			case 1:
				assertTrue (*it == 1);
				break;
			case 2:
				assertTrue (*it == 2);
				break;
			case 3:
				assertTrue (*it == 3.5);
				break;
			default:
				fail ("must fail - wrong size");
			}
		}
	}
};


#endif // VarTest_INCLUDED
