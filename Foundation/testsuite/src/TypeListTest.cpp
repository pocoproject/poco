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
#include "Poco/Void.h"


using Poco::TypeList;
using Poco::Tuple;
using Poco::NullTypeList;
using Poco::TypeListType;
using Poco::TypeGetter;
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
	// Test TypeListType: create a TypeList from individual type parameters
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

	// Test TypeList::length
	assertTrue (15 == Type15::length);

	// Test TypeGetter: retrieve types by index
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

	tuple.set<4>(-123);
	assertTrue (-123 == tuple.get<4>());

	// Test TypeGetter::HeadType and TypeGetter::ConstHeadType
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

	// Test single-element TypeList
	typedef TypeListType<Int8>::HeadType Type1;
	assertTrue (1 == Type1::length);
	assertTrue (typeid(TypeGetter<0, Type1>::HeadType) == typeid(Int8));

	// Test two-element TypeList
	typedef TypeListType<Int16, Int32>::HeadType Type2;
	assertTrue (2 == Type2::length);
	assertTrue (typeid(TypeGetter<0, Type2>::HeadType) == typeid(Int16));
	assertTrue (typeid(TypeGetter<1, Type2>::HeadType) == typeid(Int32));

	// Test NullTypeList filtering - NullTypeList entries are removed
	typedef TypeListType<Int8, NullTypeList, Int16>::HeadType TypeFiltered;
	assertTrue (2 == TypeFiltered::length);
	assertTrue (typeid(TypeGetter<0, TypeFiltered>::HeadType) == typeid(Int8));
	assertTrue (typeid(TypeGetter<1, TypeFiltered>::HeadType) == typeid(Int16));

	// Test maximum 40-element TypeList
	typedef TypeListType<Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,
						Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,
						Void,Void,Void,Void,Void,Void,Void,Void,Void,Void,
						Void,Void,Void,Void,Void,Void,Void,Void,Void,Void>::HeadType TypeVoid;
	assertTrue (40 == TypeVoid::length);
	assertTrue (typeid(TypeGetter<0, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<19, TypeVoid>::HeadType) == typeid(Void));
	assertTrue (typeid(TypeGetter<39, TypeVoid>::HeadType) == typeid(Void));

	// Test NullTypeList properties
	NullTypeList nl1, nl2;
	assertTrue (nl1 == nl2);
	assertTrue (!(nl1 != nl2));
	assertTrue (!(nl1 < nl2));
	assertTrue (NullTypeList::length == 0);
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
