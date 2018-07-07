//
// TypeListTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TypeListTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Tuple.h"
#include "Poco/TypeList.h"
#include "Poco/Void.h"
#include <iostream>


#if defined(_MSC_VER)
#	pragma warning(disable:4800) // forcing value to bool 'true' or 'false' on MSVC 71
#endif


using Poco::TypeList;
using Poco::Tuple;
using Poco::NullTypeList;
using Poco::TypeListType;
using Poco::TypeGetter;
using Poco::TypeLocator;
using Poco::TypeAppender;
using Poco::TypeOneEraser;
using Poco::TypeAllEraser;
using Poco::TypeDuplicateEraser;
using Poco::TypeOneReplacer;
using Poco::TypeAllReplacer;
using Poco::Int8;
using Poco::UInt8;
using Poco::Int16;
using Poco::UInt16;
using Poco::Int32;
using Poco::UInt32;
using Poco::Int8;
using Poco::UInt8;
using Poco::Int16;
using Poco::UInt16;
using Poco::Int32;
using Poco::UInt32;
using Poco::Void;


TypeListTest::TypeListTest(const std::string& name): CppUnit::TestCase(name)
{
}


TypeListTest::~TypeListTest()
{
}


void TypeListTest::testTypeList()
{
	typedef TypeListType<Int8,
		UInt8,
		Int16,
		UInt16,
		Int32,
		UInt32,
		float,
		double,
		Int8,
		UInt8,
		Int16,
		UInt16,
		Int32,
		UInt32,
		float>::HeadType Type15;
const int i = 0;
	Tuple<TypeGetter<i, Type15>::HeadType,
		TypeGetter<1, Type15>::HeadType,
		TypeGetter<2, Type15>::HeadType,
		TypeGetter<3, Type15>::HeadType,
		TypeGetter<4, Type15>::HeadType,
		TypeGetter<5, Type15>::HeadType,
		TypeGetter<6, Type15>::HeadType,
		TypeGetter<7, Type15>::HeadType,
		TypeGetter<8, Type15>::HeadType,
		TypeGetter<9, Type15>::HeadType> tuple;

	static POCO_UNUSED TypeLocator<Type15, Int8> pos0;
	static POCO_UNUSED TypeLocator<Type15, UInt8> pos1;
	static POCO_UNUSED TypeLocator<Type15, Int16> pos2;
	static POCO_UNUSED TypeLocator<Type15, UInt16> pos3;
	static POCO_UNUSED TypeLocator<Type15, Int32> pos4;
	static POCO_UNUSED TypeLocator<Type15, UInt32> pos5;
	static POCO_UNUSED TypeLocator<Type15, float> pos6;
	static POCO_UNUSED TypeLocator<Type15, double> pos7;
	static POCO_UNUSED TypeLocator<Type15, Int8> pos8;
	static POCO_UNUSED TypeLocator<Type15, std::string> posUnknown;

	assertTrue (pos0.value == 0);
	assertTrue (pos1.value == 1);
	assertTrue (pos2.value == 2);
	assertTrue (pos3.value == 3);
	assertTrue (pos4.value == 4);
	assertTrue (pos5.value == 5);
	assertTrue (pos6.value == 6);
	assertTrue (pos7.value == 7);
	assertTrue (pos8.value == 0);
	assertTrue (posUnknown.value == -1);

	tuple.set<TypeLocator<Type15, Int32>::value >(-123);
	assertTrue (-123 == tuple.get<4>());

	assertTrue (typeid(TypeGetter<0, Type15>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<0, Type15>::ConstHeadType) == typeid(const Int8));
	assertTrue (typeid(TypeGetter<1, Type15>::HeadType) == typeid(UInt8));
	assertTrue (typeid(TypeGetter<1, Type15>::ConstHeadType) == typeid(const UInt8));
	assertTrue (typeid(TypeGetter<2, Type15>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<2, Type15>::ConstHeadType) == typeid(const Int16));
	assertTrue (typeid(TypeGetter<3, Type15>::HeadType) == typeid(UInt16));
	assertTrue (typeid(TypeGetter<3, Type15>::ConstHeadType) == typeid(const UInt16));
	assertTrue (typeid(TypeGetter<4, Type15>::HeadType) == typeid(Int32));
	assertTrue (typeid(TypeGetter<4, Type15>::ConstHeadType) == typeid(const Int32));
	assertTrue (typeid(TypeGetter<5, Type15>::HeadType) == typeid(UInt32));
	assertTrue (typeid(TypeGetter<5, Type15>::ConstHeadType) == typeid(const UInt32));
	assertTrue (typeid(TypeGetter<6, Type15>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<6, Type15>::ConstHeadType) == typeid(const float));
	assertTrue (typeid(TypeGetter<7, Type15>::HeadType) == typeid(double));
	assertTrue (typeid(TypeGetter<7, Type15>::ConstHeadType) == typeid(const double));
	assertTrue (typeid(TypeGetter<8, Type15>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<8, Type15>::ConstHeadType) == typeid(const Int8));
	assertTrue (typeid(TypeGetter<9, Type15>::HeadType) == typeid(UInt8));
	assertTrue (typeid(TypeGetter<9, Type15>::ConstHeadType) == typeid(const UInt8));
	assertTrue (typeid(TypeGetter<10, Type15>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<10, Type15>::ConstHeadType) == typeid(const Int16));
	assertTrue (typeid(TypeGetter<11, Type15>::HeadType) == typeid(UInt16));
	assertTrue (typeid(TypeGetter<11, Type15>::ConstHeadType) == typeid(const UInt16));
	assertTrue (typeid(TypeGetter<12, Type15>::HeadType) == typeid(Int32));
	assertTrue (typeid(TypeGetter<12, Type15>::ConstHeadType) == typeid(const Int32));
	assertTrue (typeid(TypeGetter<13, Type15>::HeadType) == typeid(UInt32));
	assertTrue (typeid(TypeGetter<13, Type15>::ConstHeadType) == typeid(const UInt32));
	assertTrue (typeid(TypeGetter<14, Type15>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<14, Type15>::ConstHeadType) == typeid(const float));

	typedef TypeListType<Int8>::HeadType Type1;
	assertTrue (1 == Type1::length);
	typedef TypeListType<Int16, Int32>::HeadType Type2;
	assertTrue (2 == Type2::length);
	typedef TypeAppender<Type1, Type2>::HeadType Type3;
	assertTrue (3 == Type3::length);

	assertTrue (typeid(TypeGetter<0, Type3>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, Type3>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<2, Type3>::HeadType) == typeid(Int32));

	static POCO_UNUSED TypeLocator<Type3, Int8> posNo1;
	static POCO_UNUSED TypeLocator<Type3, Int16> posNo2;
	static POCO_UNUSED TypeLocator<Type3, Int32> posNo3;

	assertTrue (posNo1.value == 0);
	assertTrue (posNo2.value == 1);
	assertTrue (posNo3.value == 2);

	typedef TypeOneEraser<Type3, Int8>::HeadType TypeEraser1;
	assertTrue (2 == TypeEraser1::length);
	assertTrue (typeid(TypeGetter<0, TypeEraser1>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<1, TypeEraser1>::HeadType) == typeid(Int32));

	typedef TypeOneEraser<Type3, Int16>::HeadType TypeEraser2;
	assertTrue (2 == TypeEraser2::length);
	assertTrue (typeid(TypeGetter<0, TypeEraser2>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeEraser2>::HeadType) == typeid(Int32));

	typedef TypeOneEraser<Type3, Int32>::HeadType TypeEraser3;
	assertTrue (2 == TypeEraser3::length);
	assertTrue (typeid(TypeGetter<0, TypeEraser3>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeEraser3>::HeadType) == typeid(Int16));

	typedef TypeListType<Int8,Int16,Int8,Int16,Int8>::HeadType Type5;
	typedef TypeAllEraser<Type5, Int8>::HeadType TypeAllEraser3;
	assertTrue (2 == TypeAllEraser3::length);
	assertTrue (typeid(TypeGetter<0, TypeAllEraser3>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<1, TypeAllEraser3>::HeadType) == typeid(Int16));

	typedef TypeDuplicateEraser<Type5>::HeadType TypeDuplicateEraser1;
	assertTrue (2 == TypeDuplicateEraser1::length);
	assertTrue (typeid(TypeGetter<0, TypeDuplicateEraser1>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeDuplicateEraser1>::HeadType) == typeid(Int16));

	typedef TypeOneReplacer<Type5, Int8, Int32>::HeadType TypeOneReplacer1;
	assertTrue (5 == TypeOneReplacer1::length);
	assertTrue (typeid(TypeGetter<0, TypeOneReplacer1>::HeadType) == typeid(Int32));
	assertTrue (typeid(TypeGetter<1, TypeOneReplacer1>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<2, TypeOneReplacer1>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<3, TypeOneReplacer1>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<4, TypeOneReplacer1>::HeadType) == typeid(Int8));

	typedef TypeAllReplacer<Type5, Int8, Int32>::HeadType TypeAllReplacer1;
	assertTrue (5 == TypeAllReplacer1::length);
	assertTrue (typeid(TypeGetter<0, TypeAllReplacer1>::HeadType) == typeid(Int32));
	assertTrue (typeid(TypeGetter<1, TypeAllReplacer1>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<2, TypeAllReplacer1>::HeadType) == typeid(Int32));
	assertTrue (typeid(TypeGetter<3, TypeAllReplacer1>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<4, TypeAllReplacer1>::HeadType) == typeid(Int32));

	typedef TypeListType<Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,
						Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,Void>::HeadType TypeVoid;
	assertTrue (typeid(TypeGetter<0, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<1, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<2, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<3, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<4, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<5, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<6, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<7, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<8, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<9, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<10, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<11, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<12, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<13, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<14, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<15, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<16, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<17, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<18, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<19, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<20, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<21, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<22, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<23, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<24, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<25, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<26, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<27, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<28, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<29, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<30, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<31, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<32, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<33, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<34, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<35, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<36, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<37, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<38, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<39, TypeVoid>::HeadType) == typeid(Void));
	

	typedef TypeOneReplacer<TypeVoid, Void, Int8>::HeadType TypeFirstReplacer;
	assertTrue (typeid(TypeGetter<0, TypeFirstReplacer>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<2, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<3, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<4, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<5, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<6, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<7, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<8, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<9, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<10, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<11, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<12, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<13, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<14, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<15, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<16, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<17, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<18, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<19, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<20, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<21, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<22, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<23, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<24, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<25, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<26, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<27, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<28, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<29, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<30, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<31, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<32, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<33, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<34, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<35, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<36, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<37, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<38, TypeFirstReplacer>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<39, TypeFirstReplacer>::HeadType) == typeid(Void));

	typedef TypeOneReplacer<TypeFirstReplacer, Void, Int16>::HeadType TypeSecondReplacer;
	assertTrue (typeid(TypeGetter<0, TypeSecondReplacer>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeSecondReplacer>::HeadType) == typeid(Int16));

	typedef TypeOneReplacer<TypeSecondReplacer, Void, Int32>::HeadType TypeThirdReplacer;
	assertTrue (typeid(TypeGetter<0, TypeThirdReplacer>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeThirdReplacer>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<2, TypeThirdReplacer>::HeadType) == typeid(Int32));

	typedef TypeAllReplacer<TypeThirdReplacer, Void, float>::HeadType TypeFourthReplacer;
	assertTrue (typeid(TypeGetter<0, TypeFourthReplacer>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeFourthReplacer>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<2, TypeFourthReplacer>::HeadType) == typeid(Int32));
	assertTrue (typeid(TypeGetter<3, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<4, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<5, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<6, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<7, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<8, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<9, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<10, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<11, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<12, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<13, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<14, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<15, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<16, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<17, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<18, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<19, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<20, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<21, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<22, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<23, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<24, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<25, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<26, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<27, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<28, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<29, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<30, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<31, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<32, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<33, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<34, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<35, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<36, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<37, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<38, TypeFourthReplacer>::HeadType) == typeid(float));
	assertTrue (typeid(TypeGetter<39, TypeFourthReplacer>::HeadType) == typeid(float));
}


void TypeListTest::setUp()
{
}


void TypeListTest::tearDown()
{
}


CppUnit::Test* TypeListTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TypeListTest");

	CppUnit_addTest(pSuite, TypeListTest, testTypeList);

	return pSuite;
}
