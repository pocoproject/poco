//
// Tester.h
//
// Definition of the Tester class.
//
// Copyright (c) 2009-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Tester_INCLUDED
#define Tester_INCLUDED


#include "Poco/Poco.h"
#include "Poco/SharedPtr.h"
#include "Poco/BinaryWriter.h"
#include "Poco/BinaryReader.h"
#include "Poco/BasicEvent.h"
#include "Poco/Nullable.h"
#include "Poco/URI.h"
#include "Poco/UUID.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timestamp.h"
#include <vector>
#include <list>
#include <set>


enum Enum1
{
	VALUE_1,
	VALUE_2,
	VALUE_3
};


//@ serialize
struct Struct1
{
	typedef Poco::SharedPtr<Struct1> Ptr;

	enum Enum2
	{
		VALUE_1 = 1,
		VALUE_2 = 2,
		VALUE_3 = 4
	};
	
	std::string aString;
	int anInt;
	double aDouble;
	Enum1 anEnum;
	Enum2 anEnum2;
};


typedef Struct1 AnotherStruct1;
typedef std::vector<AnotherStruct1> Struct1Vec;


//@ serialize
struct Struct2
{
	Struct1Vec aVector;
	std::vector<Struct1::Ptr> anotherVector;
	Poco::URI anURI;
	Poco::UUID anUUID;
	Poco::DateTime aDateTime;
	Poco::LocalDateTime aLocalDateTime;
	Poco::Timestamp aTimestamp;
};


//@ serialize
//@ namespace="http://www.appinf.com/types"
struct Struct3
{
	std::vector<char> aCharVector;
	std::vector<Struct2> aComplexVector;
	std::list<Struct2> aList;
	std::set<int> aSet;
	std::multiset<int> aMultiSet;
	Poco::Nullable<std::string> aNullable;
};


//@ serialize
struct Struct4
{
	std::string value;
	std::vector<Struct4> vec;
	//@ namespace="http://www.appinf.com/types"
	Poco::SharedPtr<Struct4> ptr;
};


//@ serialize
struct Struct5
{
	Poco::Timestamp::TimeVal tv;
};


//@ serialize
class Class1
{
public:
	Class1():
		_anEnum(VALUE_1),
		_anInt(0)
	{
	}
	
	void setAnEnum(Enum1 anEnum)
	{
		_anEnum = anEnum;
	}
	
	Enum1 getAnEnum() const
	{
		return _anEnum;
	}
	
	void setAnInt(int anInt)
	{
		_anInt = anInt;
	}
	
	int getAnInt() const
	{
		return _anInt;
	}
	
	void setAString(const std::string& str)
	{
		_aString = str;
	}
	
	const std::string& getAString() const
	{
		return _aString;
	}
	
	void write(Poco::BinaryWriter& writer)
	{
	}
	
	void read(Poco::BinaryReader& reader)
	{
	}
	
	bool operator == (const Class1& cl) const
	{
		return _anEnum == cl._anEnum 
		    && _anInt  == cl._anInt;
	}
	
	static const int A_CONSTANT;
		
private:
	Enum1 _anEnum;
	int _anInt;
	std::string _aString;
};


//@ namespace="urn:appinf.com:service:Tester:1"
//@ remote
class Tester
{
public:
	//@ name=TestInt1
	//@ replyName=TestInt1Response
	//@ $i={name="Value", direction=in}
	int testInt1(int i);
	
	//@ name=TestInt2
	//@ replyName=TestInt2Response
	//@ $i={name="Value", direction=inout}
	void testInt2(int& i);
	
	//@ name=TestInt3
	//@ replyName=TestInt3Response
	//@ $i1={name="Value1", direction=in}
	//@ $i2={name="Value2", direction=out}
	void testInt3(int i1, int& i2);
	
	Enum1 testEnum11(Enum1 e1);
	
	void testEnum12(Enum1& e1);
	
	//@ $e2={direction=out}
	void testEnum13(Enum1 e1, Enum1& e2);

	Struct1::Enum2 testEnum21(Struct1::Enum2 e1);
	
	void testEnum22(Struct1::Enum2& e1);
	
	//@ $e2={direction=out}
	void testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2);
	
	Struct1 testStruct11(const Struct1& s1);
	
	void testStruct12(Struct1& s1);
	
	//@ $s2={direction=out}
	void testStruct13(const Struct1& s1, Struct1& s2);

	Struct2 testStruct21(const Struct2& s1);
	
	void testStruct22(Struct2& s1);
	
	//@ $s2={direction=out}
	void testStruct23(const Struct2& s1, Struct2& s2);

	Struct3 testStruct31(const Struct3& s1);
	
	void testStruct32(Struct3& s1);
	
	//@ $s2={direction=out}
	void testStruct33(const Struct3& s1, Struct3& s2);

	Struct4 testStruct41(const Struct4& s1);
	
	void testStruct42(Struct4& s1);
	
	//@ $s2={direction=out}
	void testStruct43(const Struct4& s1, Struct4& s2);

	Struct5 testStruct51(const Struct5& s1);
	
	void testStruct52(Struct5& s1);
	
	//@ s2={direction=out}
	void testStruct53(const Struct5& s1, Struct5& s2);
	
	Class1 testClass11(const Class1& c1);
	
	void testClass12(Class1& c1);

	//@ $c2={direction=out}
	void testClass13(const Class1& c1, Class1& c2);
	
	Struct1::Ptr testPtr(Struct1::Ptr p1);
	
	Struct1Vec testStruct1Vec1(const Struct1Vec& vec);
	
	void testStruct1Vec2(Struct1Vec& vec);
	
	//@ $vec2={direction=out}
	void testStruct1Vec3(const Struct1Vec& vec1, Struct1Vec& vec2);
	
	//@ oneway
	void testOneWay(const std::string& arg);
	
	std::string testOneWayResult();
	
	Poco::Timestamp::TimeVal testNestedType(Poco::Timestamp::TimeVal arg);
	
	//@ synchronized
	void testSynchronized();
	
	//@ synchronized=proxy
	void testSynchronizedProxy();
	
	//@ synchronized=remote
	void testSynchronizedRemote();
	
	void testFault();
	
	void setCookie();
	
	bool verifyCookie();
	
	//@ authenticated
	void testAuthenticated();

	//@ permission="perm1"
	void testPermission();
	
private:
	std::string _oneWayResult;
};


#endif // Tester_INCLUDED
