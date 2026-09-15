//
// Tester.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Tester.h"
#include "Poco/Exception.h"


const int Class1::A_CONSTANT(1);


int Tester::testInt1(int i)
{
	return i;
}


void Tester::testInt2(int& i)
{
	i = 2112;
}


void Tester::testInt3(int i1, int& i2)
{
	i2 = i1;
}


Enum1 Tester::testEnum11(Enum1 e1)
{
	return e1;
}


void Tester::testEnum12(Enum1& e1)
{
	e1 = VALUE_2;
}


void Tester::testEnum13(Enum1 e1, Enum1& e2)
{
	e2 = e1;
}


Struct1::Enum2 Tester::testEnum21(Struct1::Enum2 e1)
{
	return e1;
}


void Tester::testEnum22(Struct1::Enum2& e1)
{
	e1 = Struct1::VALUE_2;
}


void Tester::testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2)
{
	e2 = e1;
}


Struct1 Tester::testStruct11(const Struct1& s1)
{
	return s1;
}


void Tester::testStruct12(Struct1& s1)
{
}


void Tester::testStruct13(const Struct1& s1, Struct1& s2)
{
	s2 = s1;
}


Struct2 Tester::testStruct21(const Struct2& s1)
{
	return s1;
}


void Tester::testStruct22(Struct2& s1)
{
}


void Tester::testStruct23(const Struct2& s1, Struct2& s2)
{
	s2 = s1;
}


Struct3 Tester::testStruct31(const Struct3& s1)
{
	return s1;
}


void Tester::testStruct32(Struct3& s1)
{
}


void Tester::testStruct33(const Struct3& s1, Struct3& s2)
{
	s2 = s1;
}


Struct4 Tester::testStruct41(const Struct4& s1)
{
	return s1;
}


void Tester::testStruct42(Struct4& s1)
{
}


void Tester::testStruct43(const Struct4& s1, Struct4& s2)
{
	s2 = s1;
}


Struct5 Tester::testStruct51(const Struct5& s1)
{
	return s1;
}


void Tester::testStruct52(Struct5& s1)
{
}


void Tester::testStruct53(const Struct5& s1, Struct5& s2)
{
	s2 = s1;
}


Struct6 Tester::testStruct6(const Struct6& s1)
{
	return s1;
}


Poco::Array<int, 4> Tester::testArray(const Poco::Array<int, 4>& arr)
{
	return arr;
}


Struct7 Tester::testStruct7(const Struct7& s1)
{
	return s1;
}


std::array<int, 4> Tester::testStdArray(const std::array<int, 4>& arr)
{
	return arr;
}


Poco::Optional<std::string> Tester::testPocoOptional(const Poco::Optional<std::string>& value)
{
	return value;
}


#if __cplusplus >= 201703L
std::optional<std::string> Tester::testStdOptional(const std::optional<std::string>& value)
{
	return value;
}
#endif


Struct8 Tester::testStruct8(const Struct8& value)
{
	return value;
}


Class1 Tester::testClass11(const Class1& c1)
{
	return c1;
}


void Tester::testClass12(Class1& c1)
{
}


void Tester::testClass13(const Class1& c1, Class1& c2)
{
	c2 = c1;
}


Struct1::Ptr Tester::testPtr(Struct1::Ptr p1)
{
	return p1;
}


Struct1Vec Tester::testStruct1Vec1(const Struct1Vec& vec)
{
	return vec;
}


void Tester::testStruct1Vec2(Struct1Vec& vec)
{
}


void Tester::testStruct1Vec3(const Struct1Vec& vec1, Struct1Vec& vec2)
{
	vec2 = vec1;
}


std::shared_ptr<Struct1> Tester::testStdSharedPtr(const std::shared_ptr<Struct1>& pStruct1)
{
	return pStruct1;
}


void Tester::testOneWay(const std::string& arg)
{
	_oneWayResult = arg;
}


std::string Tester::testOneWayResult()
{
	return _oneWayResult;
}


Poco::Timestamp::TimeVal Tester::testNestedType(Poco::Timestamp::TimeVal arg)
{
	return arg;
}


void Tester::testArgNamespace(const Struct3& s31, Struct3& s32)
{
	s32 = s31;
}


void Tester::testSynchronized()
{
}


void Tester::testSynchronizedProxy()
{
}


void Tester::testSynchronizedRemote()
{
}


void Tester::fireTestEvent(const std::string& arg)
{
	std::string mutableArg(arg);
	testEvent(this, mutableArg);
}


void Tester::fireTestOneWayEvent(const std::string& arg)
{
	std::string mutableArg(arg);
	testOneWayEvent(this, mutableArg);
}


void Tester::fireTestVoidEvent()
{
	testVoidEvent(this);
}


void Tester::fireTestFilteredEvent(int arg)
{
	testFilteredEvent(this, arg);
}


void Tester::testPermission1()
{
}


void Tester::testPermission2()
{
}
