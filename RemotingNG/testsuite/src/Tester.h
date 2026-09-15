//
// Tester.h
//
// Definition of the Tester class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef Tester_INCLUDED
#define Tester_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
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
#include "Poco/SharedPtr.h"
#include "Poco/Array.h"
#include "Poco/Optional.h"
#include <vector>
#include <list>
#include <set>
#include <array>
#if __cplusplus >= 201703L
#include <optional>
#endif
#include <memory>


enum Enum1
{
	VALUE_1,
	VALUE_2,
	VALUE_3
};


enum class ScopedEnum: unsigned short
{
	VALUE_A,
	VALUE_B,
	VALUE_C
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
	int anInt = 42;
	double aDouble = 0;
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
	std::vector<std::shared_ptr<Struct2>> anotherComplexVector;
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
struct Struct6
{
	Poco::Array<int, 4> arr;
};


//@ serialize
struct Struct7
{
	std::array<int, 4> arr;
};


//@ serialize
struct Struct8
{
	Poco::Optional<std::string> pocoOptional;
#if __cplusplus >= 201703L
	std::optional<std::string> stdOptional;
#endif
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


struct UnknownStruct
{
};


struct AnotherUnknownStruct
{
};


//@ remote, namespace="http://www.appinf.com/services/tester", synchronized="false"
class Tester
{
public:
	int testInt1(int i);

	void testInt2(int& i);

	//@ $i2={direction=out}
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

	//@ $s2={direction=out}
	void testStruct53(const Struct5& s1, Struct5& s2);

	Struct6 testStruct6(const Struct6& s1);

	Poco::Array<int, 4> testArray(const Poco::Array<int, 4>& arr);

	Struct7 testStruct7(const Struct7& s1);

	std::array<int, 4> testStdArray(const std::array<int, 4>& arr);

	Poco::Optional<std::string> testPocoOptional(const Poco::Optional<std::string>& value);

#if __cplusplus >= 201703L
	std::optional<std::string> testStdOptional(const std::optional<std::string>& value);
#endif

	Struct8 testStruct8(const Struct8& value);

	Class1 testClass11(const Class1& c1);

	void testClass12(Class1& c1);

	//@ $c2={direction=out}
	void testClass13(const Class1& c1, Class1& c2);

	Struct1::Ptr testPtr(Struct1::Ptr p1);

	Struct1Vec testStruct1Vec1(const Struct1Vec& vec);

	void testStruct1Vec2(Struct1Vec& vec);

	//@ $vec2={direction=out}
	void testStruct1Vec3(const Struct1Vec& vec1, Struct1Vec& vec2);

	std::shared_ptr<Struct1> testStdSharedPtr(const std::shared_ptr<Struct1>& pStruct1);

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

	//@ $s31={namespace="http://www.appinf.com/types"}
	//@ $s32={direction=out}
	void testArgNamespace(const Struct3& s31, Struct3& s32);

	//@ permission="perm1"
	void testPermission1();

	//@ permission="perm2"
	void testPermission2();

	//@ oneway
	void fireTestEvent(const std::string& arg);

	//@ oneway
	void fireTestOneWayEvent(const std::string& arg);

	void fireTestVoidEvent();

	void fireTestFilteredEvent(int arg);

	Poco::BasicEvent<std::string> testEvent;

	//@ oneway
	Poco::BasicEvent<std::string> testOneWayEvent;

	Poco::BasicEvent<void> testVoidEvent;

	//@ filter=true
	Poco::BasicEvent<const int> testFilteredEvent;

	Poco::BasicEvent<Enum1> testEnumEvent;

	Poco::BasicEvent<ScopedEnum> testScopedEnumEvent;

	// Make sure RemoteGen ignores the following definitions.

	//@ remote=false
	void localFunction(const UnknownStruct& uks);

	static void localStaticFunction(const AnotherUnknownStruct& uks);

	//@ remote=false
	Poco::BasicEvent<UnknownStruct> localEvent;

private:
	std::string _oneWayResult;
};


#endif // Tester_INCLUDED
