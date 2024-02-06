//
// VarTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "VarTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Bugcheck.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/Dynamic/Pair.h"
#include "Poco/Dynamic/VarVisitor.h"
#include <map>
#include <utility>
#include <iostream>


#if defined(_MSC_VER) && _MSC_VER < 1400
	#pragma warning(disable:4800)//forcing value to bool 'true' or 'false'
#endif


using namespace Poco;
using namespace Poco::Dynamic;


namespace {


class Dummy
{
public:
	Dummy(): _val(0)
	{
	}

	Dummy(int val): _val(val)
	{
	}

	operator int () const
	{
		return _val;
	}

	bool operator == (int i) const
	{
		return i == _val;
	}

	friend bool operator == (const Dummy &d1, const Dummy &d2)
	{
		return d1._val == d2._val;
	}

private:
	int _val;
};

}


VarTest::VarTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


VarTest::~VarTest()
{
}


void VarTest::testInt8()
{
	Poco::Int8 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::Int8));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	Int8 value = a1.extract<Int8>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testInt16()
{
	Poco::Int16 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::Int16));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	Int16 value = a1.extract<Int16>();
	assertTrue (value == 32);

	try
	{
		Int32 POCO_UNUSED value2; value2 = a1.extract<Int32>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testInt32()
{
	Poco::Int32 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::Int32));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	Int32 value = a1.extract<Int32>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testInt64()
{
	Poco::Int64 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::Int64));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	Int64 value = a1.extract<Int64>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testUInt8()
{
	Poco::UInt8 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::UInt8));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	UInt8 value = a1.extract<UInt8>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testUInt16()
{
	Poco::UInt16 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::UInt16));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	UInt16 value = a1.extract<UInt16>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testUInt32()
{
	Poco::UInt32 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::UInt32));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	UInt32 value = a1.extract<UInt32>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testUInt64()
{
	Poco::UInt64 src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(Poco::UInt64));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	UInt64 value = a1.extract<UInt64>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testBool()
{
	bool src = true;
	Var a1 = src;

	assertTrue (a1.type() == typeid(bool));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 1);
	assertTrue (s15 == 1);
	assertTrue (s16 == 1);
	assertTrue (s17 == 1);
	assertTrue (s1 == "true");
	assertTrue (s2 == 1);
	assertTrue (s3 == 1);
	assertTrue (s4 == 1);
	assertTrue (s5 == 1);
	assertTrue (s6 == 1);
	assertTrue (s7 == 1);
	assertTrue (s8 == 1);
	assertTrue (s9 == 1);
	assertTrue (s10 == 1.0f);
	assertTrue (s11 == 1.0);
	assertTrue (s12);
	assertTrue (s13 == '\x1');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	bool value = a1.extract<bool>();
	assertTrue (value);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}
}


void VarTest::testChar()
{
	char src = ' ';
	Var a1 = src;

	assertTrue (a1.type() == typeid(char));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == " ");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	char value = a1.extract<char>();
	assertTrue (value == ' ');

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}
}


void VarTest::testFloat()
{
	Var any("0");
	float POCO_UNUSED f = any;

	float src = 32.0f;
	Var a1 = src;

	assertTrue (a1.type() == typeid(float));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	float value = a1.extract<float>();
	assertTrue (value == 32.0f);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1.0f;
	assertTrue (a3 == 33.0f);
	a3 = a1 - 1.0f;
	assertTrue (a3 == 31.0f);
	a3 += 1.0f;
	assertTrue (a3 == 32.0f);
	a3 -= 1.0f;
	assertTrue (a3 == 31.0f);
	a3 = a1 / 2.0f;
	assertTrue (a3 == 16.0f);
	a3 = a1 * 2.0f;
	assertTrue (a3 == 64.0f);
	a3 /= 2.0f;
	assertTrue (a3 == 32.0f);
	a3 *= 2.0f;
	assertTrue (a3 == 64.0f);
}


void VarTest::testDouble()
{
	double d = 0;
	Var v(d);
	float POCO_UNUSED f = v;

	double src = 32.0;
	Var a1 = src;

	assertTrue (a1.type() == typeid(double));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	double value = a1.extract<double>();
	assertTrue (value == 32.0);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}


	Var a3 = a1 + 1.0;
	assertTrue (a3 == 33.0);
	a3 = a1 - 1.0;
	assertTrue (a3 == 31.0);
	a3 += 1.0;
	assertTrue (a3 == 32.0);
	a3 -= 1.0;
	assertTrue (a3 == 31.0);
	a3 = a1 / 2.0;
	assertTrue (a3 == 16.0);
	a3 = a1 * 2.0;
	assertTrue (a3 == 64.0);
	a3 /= 2.0;
	assertTrue (a3 == 32.0);
	a3 *= 2.0;
	assertTrue (a3 == 64.0);
}


void VarTest::testString()
{
	Var a1("32");

	assertTrue (a1.type() == typeid(std::string));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12 = false;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == '3');

	const std::string& value = a1.extract<std::string>();
	assertTrue (value == "32");

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a4(123);
	std::string s("456");
	Var a5 = a4 + s;
	assertTrue (a5 == "123456");
	a4 += s;
	assertTrue (a4 == "123456");
	Var a6 = a4 + "789";
	assertTrue (a6 == "123456789");
	a4 += "789";
	assertTrue (a4 == "123456789");

	a4 = "";
	assertTrue (!a4);
	a4 = "0";
	assertTrue (!a4);
	a4 = "FaLsE";
	assertTrue (!a4);
}


void VarTest::testLong()
{
	long src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(long));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	long value = a1.extract<long>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testULong()
{
	unsigned long src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(unsigned long));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	unsigned long value = a1.extract<unsigned long>();
	assertTrue (value == 32);

	try
	{
		Int16 POCO_UNUSED value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}

void VarTest::testLongLong()
{
	long long src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(long long));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	long long value = a1.extract<long long>();
	assertTrue (value == 32);

	try
	{
		POCO_UNUSED Int16 value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testULongLong()
{
	unsigned long long src = 32;
	Var a1 = src;

	assertTrue (a1.type() == typeid(unsigned long long));

	std::string s1;
	Poco::Int8 s2;
	Poco::Int16 s3;
	Poco::Int32 s4;
	Poco::Int64 s5;
	Poco::UInt8 s6;
	Poco::UInt16 s7;
	Poco::UInt32 s8;
	Poco::UInt64 s9;
	float s10;
	double s11;
	bool s12;
	char s13;
	a1.convert(s1);
	a1.convert(s2);
	a1.convert(s3);
	a1.convert(s4);
	a1.convert(s5);
	a1.convert(s6);
	a1.convert(s7);
	a1.convert(s8);
	a1.convert(s9);
	a1.convert(s10);
	a1.convert(s11);
	a1.convert(s12);
	a1.convert(s13);
	long s14;
	unsigned long s15;
	long long s16;
	unsigned long long s17;
	a1.convert(s14);
	a1.convert(s15);
	a1.convert(s16);
	a1.convert(s17);
	assertTrue (s14 == 32);
	assertTrue (s15 == 32);
	assertTrue (s16 == 32);
	assertTrue (s17 == 32);
	assertTrue (s1 == "32");
	assertTrue (s2 == 32);
	assertTrue (s3 == 32);
	assertTrue (s4 == 32);
	assertTrue (s5 == 32);
	assertTrue (s6 == 32);
	assertTrue (s7 == 32);
	assertTrue (s8 == 32);
	assertTrue (s9 == 32);
	assertTrue (s10 == 32.0f);
	assertTrue (s11 == 32.0);
	assertTrue (s12);
	assertTrue (s13 == ' ');
	Var a2(a1);
	std::string t2;
	a2.convert(t2);
	assertTrue (s1 == t2);

	unsigned long long value = a1.extract<unsigned long long>();
	assertTrue (value == 32);

	try
	{
		POCO_UNUSED Int16 value2; value2 = a1.extract<Int16>();
		fail("bad cast - must throw");
	}
	catch (Poco::BadCastException&)
	{
	}

	Var a3 = a1 + 1;
	assertTrue (a3 == 33);
	a3 = a1 - 1;
	assertTrue (a3 == 31);
	a3 += 1;
	assertTrue (a3 == 32);
	a3 -= 1;
	assertTrue (a3 == 31);
	a3 = a1 / 2;
	assertTrue (a3 == 16);
	a3 = a1 * 2;
	assertTrue (a3 == 64);
	a3 /= 2;
	assertTrue (a3 == 32);
	a3 *= 2;
	assertTrue (a3 == 64);
}


void VarTest::testUDT()
{
	Dummy d0;
	assertTrue (d0 == 0);

	Dummy d(1);
	Var da = d;
	assertTrue (da.extract<Dummy>() == 1);

	Dummy d1 = d;
	Var da1 = d1;
	assertTrue (da1.extract<Dummy>() == 1);

	try
	{
		float POCO_UNUSED f = da1;
		fail ("must fail");
	}
	catch (BadCastException&) { }
}


void VarTest::testConversionOperator()
{
	Var any("42");
	int i = any;
	assertTrue (i == 42);
	assertTrue (any == i);

	any = 123;
	std::string s1 = any.convert<std::string>();
	assertTrue (s1 == "123");
	assertTrue (s1 == any);
	assertTrue (any == s1);
	assertTrue ("123" == any);

	any = 321;
	s1 = any.convert<std::string>();
	assertTrue (s1 == "321");

	any = "456";
	assertTrue (any == "456");
	assertTrue ("456" == any);

	any = 789;
	std::string s2 = any.convert<std::string>();
	assertTrue (s2 == "789");
	assertTrue (s2 == any);
	assertTrue (any == s2);
	assertTrue ("789" == any);

	Var any2 = "1.5";
	double d = any2;
	assertTrue (d == 1.5);
	assertTrue (any2 == d);
}


void VarTest::testComparisonOperators()
{
	Var any1 = 1;
	Var any2 = "1";
	assertTrue (any1 == any2);
	assertTrue (any1 == 1);
	assertTrue (1 == any1);
	assertTrue (any1 == "1");
	assertTrue ("1" == any1);
	assertTrue (any1 <= 1);
	assertTrue (1 >= any1);
	assertTrue (any1 <= 2);
	assertTrue (2 >= any1);
	assertTrue (any1 < 2);
	assertTrue (2 > any1);
	assertTrue (any1 > 0);
	assertTrue (0 < any1);
	assertTrue (any1 >= 1);
	assertTrue (1 <= any1);
	assertTrue (any1 >= 0);
	assertTrue (0 <= any1);

	any1 = 1L;
	assertTrue (any1 == any2);
	assertTrue (any1 == 1L);
	assertTrue (1L == any1);
	assertTrue (any1 == "1");
	assertTrue ("1" == any1);
	assertTrue (any1 != 2L);
	assertTrue (2L != any1);
	assertTrue (any1 != "2");
	assertTrue ("2" != any1);
	assertTrue (any1 <= 1L);
	assertTrue (1L >= any1);
	assertTrue (any1 <= 2L);
	assertTrue (2L >= any1);
	assertTrue (any1 < 2L);
	assertTrue (2L > any1);
	assertTrue (any1 > 0);
	assertTrue (0 < any1);
	assertTrue (any1 >= 1L);
	assertTrue (1L <= any1);
	assertTrue (any1 >= 0);
	assertTrue (0 <= any1);

	any1 = 0x31;
	assertTrue (any1 == '1');
	assertTrue ('1' == any1);
	assertTrue (any1 <= '1');
	assertTrue ('1' >= any1);
	assertTrue (any1 <= '2');
	assertTrue ('2' >= any1);
	assertTrue (any1 < '2');
	assertTrue ('2' > any1);
	assertTrue (any1 > 0);
	assertTrue (0 < any1);
	assertTrue (any1 >= '1');
	assertTrue ('1' <= any1);
	assertTrue (any1 >= 0);
	assertTrue (0 <= any1);

	any1 = "2";
	assertTrue (any1 != any2);
	assertTrue (any1 != 1);
	assertTrue (1 != any1);
	assertTrue (any1 != "1");
	assertTrue ("1" != any1);

	any1 = 1.5;
	assertTrue (any1 == 1.5);
	assertTrue (1.5 == any1);
	assertTrue (any1 == "1.5");
	assertTrue ("1.5" == any1);
	assertTrue (any1 != 2.5);
	assertTrue (2.5 != any1);
	assertTrue (any1 != "2.5");
	assertTrue ("2.5" != any1);
	assertTrue (any1 <= 1.5);
	assertTrue (1.5 >= any1);
	assertTrue (any1 <= 2.5);
	assertTrue (2.5 >= any1);
	assertTrue (any1 < 2.5);
	assertTrue (2.5 > any1);
	assertTrue (any1 > 0);
	assertTrue (0 < any1);
	assertTrue (any1 >= 1.5);
	assertTrue (1.5 <= any1);
	assertTrue (any1 >= 0);
	assertTrue (0 <= any1);

	any1 = 1.5f;
	assertTrue (any1 == 1.5f);
	assertTrue (1.5f == any1);
	assertTrue (any1 == "1.5");
	assertTrue ("1.5" == any1);
	assertTrue (any1 != 2.5f);
	assertTrue (2.5f != any1);
	assertTrue (any1 != "2.5");
	assertTrue ("2.5" != any1);
	assertTrue (any1 <= 1.5f);
	assertTrue (1.5f >= any1);
	assertTrue (any1 <= 2.5f);
	assertTrue (2.5f >= any1);
	assertTrue (any1 < 2.5f);
	assertTrue (2.5f > any1);
	assertTrue (any1 > 0);
	assertTrue (0 < any1);
	assertTrue (any1 >= 1.5f);
	assertTrue (1.5f <= any1);
	assertTrue (any1 >= 0);
	assertTrue (0 <= any1);
}


void VarTest::testArithmeticOperators()
{
	Var any1 = 1;
	Var any2 = 2;
	Var any3 = any1 + any2;
	assertTrue (any3 == 3);
	int i = 1;
	i += any1;
	assertTrue (2 == i);

	any1 = 3;
	assertTrue ((5 - any1) == 2);
	any2 = 5;
	any3 = any2 - any1;
	assertTrue (any3 == 2);
	any3 -= 1;
	assertTrue (any3 == 1);
	i = 5;
	i -= any1;
	assertTrue (2 == i);

	any1 = 3;
	assertTrue ((5 * any1) == 15);
	any2 = 5;
	any3 = any1 * any2;
	assertTrue (any3 == 15);
	any3 *= 3;
	assertTrue (any3 == 45);
	i = 5;
	i *= any1;
	assertTrue (15 == i);

	any1 = 3;
	assertTrue ((9 / any1) == 3);
	any2 = 9;
	any3 = any2 / any1;
	assertTrue (any3 == 3);
	any3 /= 3;
	assertTrue (any3 == 1);
	i = 9;
	i /= any1;
	assertTrue (3 == i);

	any1 = 1.0f;
	any2 = .5f;
	any3 = .0f;
	any3 = any1 + any2;
	assertTrue (any3 == 1.5f);
	any3 += .5f;
	assertTrue (any3 == 2.0f);

	any1 = 1.0;
	any2 = .5;
	any3 = 0.0;
	any3 = any1 + any2;
	assertTrue (any3 == 1.5);
	any3 += .5;
	assertTrue (any3 == 2.0);

	any1 = 1;
	any2 = "2";
	any3 = any1 + any2;
	assertTrue (any3 == 3);
	any2 = "4";
	any3 += any2;
	assertTrue (any3 == 7);
	assertTrue (1 + any3 == 8);

	any1 = "123";
	any2 = "456";
	any3 = any1 + any2;
	assertTrue (any3 == "123456");
	any2 = "789";
	any3 += any2;
	assertTrue (any3 == "123456789");
	assertTrue (("xyz" + any3) == "xyz123456789");

	try	{ any3 = any1 - any2; fail ("must fail"); }
	catch (InvalidArgumentException&){}

	try	{ any3 -= any2;	fail ("must fail");	}
	catch (InvalidArgumentException&){}

	try	{ any3 = any1 * any2; fail ("must fail"); }
	catch (InvalidArgumentException&){}

	try { any3 *= any2;	fail ("must fail");	}
	catch (InvalidArgumentException&){}

	try { any3 = any1 / any2; fail ("must fail"); }
	catch (InvalidArgumentException&){}

	try { any3 /= any2; fail ("must fail");	}
	catch (InvalidArgumentException&){}

	any1 = 10;

	assertTrue (any1++ == 10);
	assertTrue (any1 == 11);
	assertTrue (++any1 == 12);

	assertTrue (any1-- == 12);
	assertTrue (any1 == 11);
	assertTrue (--any1 == 10);

	any1 = 1.23;

	try { ++any1; fail ("must fail"); }
	catch (InvalidArgumentException&){}

	try { any1++; fail ("must fail"); }
	catch (InvalidArgumentException&){}

	try { --any1; fail ("must fail"); }
	catch (InvalidArgumentException&){}

	try { any1--; fail ("must fail"); }
	catch (InvalidArgumentException&){}
}


void VarTest::testLimitsInt()
{
	testLimitsSigned<Int16, Int8>();
	testLimitsSigned<Int32, Int8>();
	testLimitsSigned<Int64, Int8>();
	testLimitsFloatToInt<float, Int8>();
	testLimitsFloatToInt<double, Int8>();

	testLimitsSigned<Int32, Int16>();
	testLimitsSigned<Int64, Int16>();
	testLimitsFloatToInt<float, Int16>();
	testLimitsFloatToInt<double, Int16>();

	testLimitsSigned<Int64, Int32>();
	testLimitsFloatToInt<float, Int32>();
	testLimitsFloatToInt<double, Int32>();

	testLimitsSignedUnsigned<Int8, UInt8>();
	testLimitsSignedUnsigned<Int16, UInt8>();
	testLimitsSignedUnsigned<Int32, UInt8>();
	testLimitsSignedUnsigned<Int64, UInt8>();
	testLimitsFloatToInt<float, UInt8>();
	testLimitsFloatToInt<double, UInt8>();

	testLimitsSignedUnsigned<Int8, UInt16>();
	testLimitsSignedUnsigned<Int16, UInt16>();
	testLimitsSignedUnsigned<Int32, UInt16>();
	testLimitsSignedUnsigned<Int64, UInt16>();
	testLimitsFloatToInt<float, UInt16>();
	testLimitsFloatToInt<double, UInt16>();

	testLimitsSignedUnsigned<Int8, UInt32>();
	testLimitsSignedUnsigned<Int16, UInt32>();
	testLimitsSignedUnsigned<Int32, UInt32>();
	testLimitsSignedUnsigned<Int64, UInt32>();
	testLimitsFloatToInt<float, UInt32>();
	testLimitsFloatToInt<double, UInt32>();

	testLimitsSignedUnsigned<Int8, UInt64>();
	testLimitsSignedUnsigned<Int16, UInt64>();
	testLimitsSignedUnsigned<Int32, UInt64>();
	testLimitsSignedUnsigned<Int64, UInt64>();
	testLimitsFloatToInt<float, UInt64>();
	testLimitsFloatToInt<double, UInt64>();

	testLimitsUnsigned<UInt16, UInt8>();
	testLimitsUnsigned<UInt32, UInt8>();
	testLimitsUnsigned<UInt64, UInt8>();

	testLimitsUnsigned<UInt32, UInt16>();
	testLimitsUnsigned<UInt64, UInt16>();

	testLimitsUnsigned<UInt64, UInt32>();
}


void VarTest::testLimitsFloat()
{
	if (std::numeric_limits<double>::max() != std::numeric_limits<float>::max())
	{
		constexpr double iMin = -1 * std::numeric_limits<float>::max();
		Var da = iMin * 10;
		try { float POCO_UNUSED f; f = da; fail("must throw", __LINE__, __FILE__); }
		catch (RangeException&) {}

		constexpr double iMax = std::numeric_limits<float>::max();
		da = iMax * 10;
		try { float POCO_UNUSED f; f = da; fail("must throw", __LINE__, __FILE__); }
		catch (RangeException&) {}
	}

	int64_t i = std::numeric_limits<int>::max();
	Var anyInt = i;
	try { anyInt.convert<float>(); fail("must throw", __LINE__, __FILE__); }
	catch (RangeException&) {}

	Var anyFloat = 1.0f;
	anyFloat = i;
	anyFloat.convert<int>();
	assertTrue (anyFloat.convert<int64_t>() == i);

	try { float POCO_UNUSED fl = anyFloat; fail("must throw", __LINE__, __FILE__); }
	catch (Poco::RangeException&) {}

	i = std::numeric_limits<int64_t>::max();
	anyInt = i;

	float f = 0.f;
	try { f = anyInt.convert<float>(); fail("must throw", __LINE__, __FILE__); }
	catch (Poco::RangeException&) {}
	i = static_cast<int64_t>(f);
	assertTrue (0 == i);

	double d = 0.;
	try { d = anyInt.convert<double>(); fail("must throw", __LINE__, __FILE__); }
	catch (Poco::RangeException&) {}
	i = static_cast<int64_t>(d);
	assertTrue (0 == i);
}


void VarTest::testCtor()
{
	// this is mainly to test a reported compiler error with assignment on HP aCC.
	// (SF# 1733964)

	Var a1(42);
	Var a2(a1);
	Var a3;

	a3 = a1;

	assertTrue (a2 == 42);
	assertTrue (a3 == 42);
}


void VarTest::testIsStruct()
{
	std::string s1("string");
	Poco::Int8 s2(-23);
	Poco::Int16 s3(-33);
	Poco::Int32 s4(-388);
	Poco::Int64 s5(-23823838);
	Poco::UInt8 s6(32u);
	Poco::UInt16 s7(16000u);
	Poco::UInt32 s8(334234u);
	Poco::UInt64 s9(2328328382u);
	float s10(13.333f);
	double s11(13.555);
	bool s12(true);
	char s13('c');
	long s14(232323);
	unsigned long s15(21233232u);
	long long s16(-23823838);
	unsigned long s17(2328328382u);
	std::vector<Var> s18;
	Struct<std::string> s19;
	Struct<int> s20;

	Var d1(s1);
	Var d2(s2);
	Var d3(s3);
	Var d4(s4);
	Var d5(s5);
	Var d6(s6);
	Var d7(s7);
	Var d8(s8);
	Var d9(s9);
	Var d10(s10);
	Var d11(s11);
	Var d12(s12);
	Var d13(s13);
	Var d14(s14);
	Var d15(s15);
	Var d16(s16);
	Var d17(s17);
	Var d18(s18);
	Var d19(s19);
	Var d20(s20);

	assertTrue (!d1.isStruct());
	assertTrue (!d2.isStruct());
	assertTrue (!d3.isStruct());
	assertTrue (!d4.isStruct());
	assertTrue (!d5.isStruct());
	assertTrue (!d6.isStruct());
	assertTrue (!d7.isStruct());
	assertTrue (!d8.isStruct());
	assertTrue (!d9.isStruct());
	assertTrue (!d10.isStruct());
	assertTrue (!d11.isStruct());
	assertTrue (!d12.isStruct());
	assertTrue (!d13.isStruct());
	assertTrue (!d14.isStruct());
	assertTrue (!d15.isStruct());
	assertTrue (!d16.isStruct());
	assertTrue (!d17.isStruct());
	assertTrue (!d18.isStruct());
	assertTrue (d19.isStruct());
	assertTrue (d20.isStruct());
}


void VarTest::testIsArray()
{
	std::string s1("string");
	Poco::Int8 s2(-23);
	Poco::Int16 s3(-33);
	Poco::Int32 s4(-388);
	Poco::Int64 s5(-23823838);
	Poco::UInt8 s6(32u);
	Poco::UInt16 s7(16000u);
	Poco::UInt32 s8(334234u);
	Poco::UInt64 s9(2328328382u);
	float s10(13.333f);
	double s11(13.555);
	bool s12(true);
	char s13('c');
	long s14(232323);
	unsigned long s15(21233232u);
	long long s16(-23823838);
	unsigned long long s17(2328328382u);
	std::vector<Var> s18;
	DynamicStruct s19;

	Var d0;
	Var d1(s1);
	Var d2(s2);
	Var d3(s3);
	Var d4(s4);
	Var d5(s5);
	Var d6(s6);
	Var d7(s7);
	Var d8(s8);
	Var d9(s9);
	Var d10(s10);
	Var d11(s11);
	Var d12(s12);
	Var d13(s13);
	Var d14(s14);
	Var d15(s15);
	Var d16(s16);
	Var d17(s17);
	Var d18(s18);
	Var d19(s19);

	assertTrue (!d0.isArray());
	assertTrue (!d1.isArray());
	assertTrue (!d2.isArray());
	assertTrue (!d3.isArray());
	assertTrue (!d4.isArray());
	assertTrue (!d5.isArray());
	assertTrue (!d6.isArray());
	assertTrue (!d7.isArray());
	assertTrue (!d8.isArray());
	assertTrue (!d9.isArray());
	assertTrue (!d10.isArray());
	assertTrue (!d11.isArray());
	assertTrue (!d12.isArray());
	assertTrue (!d13.isArray());
	assertTrue (!d14.isArray());
	assertTrue (!d15.isArray());
	assertTrue (!d16.isArray());
	assertTrue (!d17.isArray());
	assertTrue (d18.isArray());
	assertTrue (!d19.isArray());
}


void VarTest::testArrayIdxOperator()
{
	std::string s1("string");
	Poco::Int8 s2(-23);
	Poco::Int16 s3(-33);
	Poco::Int32 s4(-388);
	Poco::Int64 s5(-23823838);
	Poco::UInt8 s6(32u);
	Poco::UInt16 s7(16000u);
	Poco::UInt32 s8(334234u);
	Poco::UInt64 s9(2328328382u);
	float s10(13.333f);
	double s11(13.555);
	bool s12(true);
	char s13('c');
	long s14(232323);
	unsigned long s15(21233232u);
	long long s16(-23823838);
	unsigned long long s17(2328328382u);
	std::vector<Var> s18;
	s18.push_back(s1);
	s18.push_back(s2);
	DynamicStruct s19;

	Var d1(s1);
	Var d2(s2);
	Var d3(s3);
	Var d4(s4);
	Var d5(s5);
	Var d6(s6);
	Var d7(s7);
	Var d8(s8);
	Var d9(s9);
	Var d10(s10);
	Var d11(s11);
	Var d12(s12);
	Var d13(s13);
	Var d14(s14);
	Var d15(s15);
	Var d16(s16);
	Var d17(s17);
	Var d18(s18);
	Var d19(s19);

	testGetIdxMustThrow(d1, 0);
	testGetIdxNoThrow(d2, 0);
	testGetIdxNoThrow(d3, 0);
	testGetIdxNoThrow(d4, 0);
	testGetIdxNoThrow(d5, 0);
	testGetIdxNoThrow(d6, 0);
	testGetIdxNoThrow(d7, 0);
	testGetIdxNoThrow(d8, 0);
	testGetIdxNoThrow(d9, 0);
	testGetIdxNoThrow(d10, 0);
	testGetIdxNoThrow(d11, 0);
	testGetIdxNoThrow(d12, 0);
	testGetIdxNoThrow(d13, 0);
	testGetIdxNoThrow(d14, 0);
	testGetIdxNoThrow(d15, 0);
	testGetIdxNoThrow(d16, 0);
	testGetIdxNoThrow(d17, 0);
	testGetIdx(d18, 0, s1);
	testGetIdx(d18, 1, s2);

	testGetIdxMustThrow(d1, 1);
	testGetIdxMustThrow(d2, 1);
	testGetIdxMustThrow(d3, 1);
	testGetIdxMustThrow(d4, 1);
	testGetIdxMustThrow(d5, 1);
	testGetIdxMustThrow(d6, 1);
	testGetIdxMustThrow(d7, 1);
	testGetIdxMustThrow(d8, 1);
	testGetIdxMustThrow(d9, 1);
	testGetIdxMustThrow(d10, 1);
	testGetIdxMustThrow(d11, 1);
	testGetIdxMustThrow(d12, 1);
	testGetIdxMustThrow(d13, 1);
	testGetIdxMustThrow(d14, 1);
	testGetIdxMustThrow(d15, 1);
	testGetIdxMustThrow(d16, 1);
	testGetIdxMustThrow(d17, 1);
	testGetIdxMustThrow(d19, 1);
}


void VarTest::testDynamicStructBasics()
{
	DynamicStruct aStruct;
	assertTrue (aStruct.empty());
	assertTrue (aStruct.size() == 0);
	assertTrue (aStruct.members().empty());

	aStruct.insert("First Name", "Little");
	assertTrue (!aStruct.empty());
	assertTrue (aStruct.size() == 1);
	assertTrue (*(aStruct.members().begin()) == "First Name");
	assertTrue (aStruct["First Name"] == "Little");
	aStruct.insert("Last Name", "POCO");
	assertTrue (aStruct.members().size() == 2);
	aStruct.erase("First Name");
	assertTrue (aStruct.size() == 1);
	assertTrue (*(aStruct.members().begin()) == "Last Name");
	aStruct.clear();
	assertTrue (aStruct.size() == 0);
}


void VarTest::testOrderedDynamicStructBasics()
{
	OrderedDynamicStruct aStruct;
	assertTrue(aStruct.empty());
	assertTrue(aStruct.size() == 0);
	assertTrue(aStruct.members().empty());

	aStruct.insert("First Name", "Little");
	assertTrue(!aStruct.empty());
	assertTrue(aStruct.size() == 1);
	assertTrue(*(aStruct.members().begin()) == "First Name");
	assertTrue(aStruct["First Name"] == "Little");
	aStruct.insert("Last Name", "POCO");
	assertTrue(aStruct.members().size() == 2);
	aStruct.erase("First Name");
	assertTrue(aStruct.size() == 1);
	assertTrue(*(aStruct.members().begin()) == "Last Name");
	aStruct.insert("Age", 1);
	assertTrue(aStruct["Age"] == 1);
	assertTrue(aStruct.members().size() == 2);
	assertTrue(*(aStruct.members().begin()) == "Last Name");
	aStruct.clear();
	assertTrue(aStruct.size() == 0);
}


void VarTest::testDynamicStructEmptyString()
{
	DynamicStruct aStruct;
	aStruct["Empty"] = "";
	aStruct["Space"] = " ";
	assertEqual(aStruct.toString(true), "{ \"Empty\": \"\", \"Space\": \" \" }");
}


void VarTest::testDynamicStructNoEscapeString()
{
	DynamicStruct aStruct;
	aStruct["Birthday"] = "{ \"Day\": 12, \"Month\": \"May\", \"Year\": 2005 }";
	assertEqual(aStruct.toString(false), "{ \"Birthday\": { \"Day\": 12, \"Month\": \"May\", \"Year\": 2005 } }");
}


void VarTest::testDynamicStructString()
{
	DynamicStruct aStruct;
	aStruct["First Name"] = "Junior";
	aStruct["Last Name"] = "POCO";
	Var a1(aStruct);
	assertTrue (a1["First Name"] == "Junior");
	assertTrue (a1["Last Name"] == "POCO");
	a1["First Name"] = "Senior";
	assertTrue (a1["First Name"] == "Senior");
	testGetIdxMustThrow(a1, 0);

	Struct<std::string> s1;
	s1["1"] = 1;
	s1["2"] = 2;
	s1["3"] = 3;

	Struct<std::string> s2(s1);
	assertTrue (s2["1"] == 1);
	assertTrue (s2["2"] == 2);
	assertTrue (s2["3"] == 3);

	std::map<std::string, int> m1;
	m1["1"] = 1;
	m1["2"] = 2;
	m1["3"] = 3;

	Struct<std::string> m2(m1);
	assertTrue (m2["1"] == 1);
	assertTrue (m2["2"] == 2);
	assertTrue (m2["3"] == 3);
}


void VarTest::testOrderedDynamicStructString()
{
	OrderedDynamicStruct aStruct;
	aStruct["First Name"] = "Junior";
	aStruct["Last Name"] = "POCO";
	Var a1(aStruct);
	assertTrue(a1["First Name"] == "Junior");
	assertTrue(a1["Last Name"] == "POCO");
	a1["First Name"] = "Senior";
	assertTrue(a1["First Name"] == "Senior");
	testGetIdxMustThrow(a1, 0);

	using OrderedStruct = OrderedDynamicStruct;
	OrderedStruct s1;
	s1["1"] = 1;
	s1["2"] = 2;
	s1["3"] = 3;

	OrderedStruct s2(s1);
	assertTrue(s2["1"] == 1);
	assertTrue(s2["2"] == 2);
	assertTrue(s2["3"] == 3);

	OrderedMap<std::string, int> m1;
	m1["2"] = 2;
	m1["1"] = 1;
	m1["3"] = 3;
	assertTrue (m1.begin()->first == "2");
	assertTrue(m1.begin()->second == 2);

	OrderedStruct m2(m1);
	assertTrue(m2["1"] == 1);
	assertTrue(m2["2"] == 2);
	assertTrue(m2["3"] == 3);
}


void VarTest::testDynamicStructInt()
{
	Dynamic::Struct<int> aStruct;
	aStruct[0] = "Junior";
	aStruct[1] = "POCO";
	aStruct[2] = 10;
	Var a1(aStruct);
	assertTrue (a1[0]== "Junior");
	assertTrue (a1[1]== "POCO");
	assertTrue (a1[2]== 10);
	a1[0] = "Senior";
	assertTrue (a1[0] == "Senior");

	Struct<int> s1;
	s1[1] = "1";
	s1[2] = "2";
	s1[3] = "3";

	Struct<int> s2(s1);
	assertTrue (s2[1] == "1");
	assertTrue (s2[2] == "2");
	assertTrue (s2[3] == "3");

	std::map<int, std::string> m1;
	m1[1] = "1";
	m1[2] = "2";
	m1[3] = "3";

	Struct<int> m2(m1);
	assertTrue (m2[1] == "1");
	assertTrue (m2[2] == "2");
	assertTrue (m2[3] == "3");
}


void VarTest::testOrderedDynamicStructInt()
{
	typedef Struct<int, OrderedMap<int, Var>, OrderedSet<int> > OrderedStruct;
	OrderedStruct aStruct;
	aStruct[0] = "POCO";
	aStruct[1] = "Junior";
	aStruct[2] = 100;
	aStruct[3] = 10;

	Var a1(aStruct);
	assertTrue(a1[0] == "POCO");
	assertTrue(a1[1] == "Junior");
	assertTrue(a1[2] == 100);
	assertTrue(a1[3] == 10);
	a1[0] = "Senior";
	assertTrue(a1[0] == "Senior");

	OrderedStruct s1;
	s1[1] = "1";
	s1[2] = "2";
	s1[3] = "3";

	OrderedStruct s2(s1);
	assertTrue(s2[1] == "1");
	assertTrue(s2[2] == "2");
	assertTrue(s2[3] == "3");

	OrderedMap<int, std::string> m1;
	m1[1] = "2";
	m1[2] = "1";
	m1[3] = "3";

	OrderedStruct m2(m1);
	assertTrue(m2[1] == "2");
	assertTrue(m2[2] == "1");
	assertTrue(m2[3] == "3");
}


void VarTest::testDynamicPair()
{
	Pair<int> aPair;
	assertTrue (0 == aPair.first());
	try
	{
		std::string s = aPair.second().convert<std::string>();
		fail ("must fail");
	}
	catch (InvalidAccessException&) { }

	Var va(aPair);
	assertTrue ("{ \"0\": null }" == va.convert<std::string>());
	assertTrue (aPair.toString() == va.convert<std::string>());

	aPair = Pair<int>(4, "123");
	assertTrue ("123" == aPair.second());

	va = aPair;
	assertTrue ("{ \"4\": \"123\" }" == va.convert<std::string>());
	assertTrue (aPair.toString() == va.convert<std::string>());

	int i = 1;
	std::string s = "2";
	Pair<int> iPair(i, s);
	assertTrue (1 == iPair.first());
	assertTrue ("2" == iPair.second());

	Pair<std::string> sPair(s, i);
	assertTrue ("2" == sPair.first());
	assertTrue (1 == sPair.second());

	std::pair<int, std::string> p = std::make_pair(i, s);
	Pair<int> pPair(p);
	assertTrue (1 == pPair.first());
	assertTrue ("2" == pPair.second());

	Var vp(pPair);
	assertTrue ("{ \"1\": \"2\" }" == vp.convert<std::string>());
	assertTrue (pPair.toString() == vp.convert<std::string>());

	Var vs(sPair);
	assertTrue ("{ \"2\": 1 }" == vs.convert<std::string>());
	assertTrue (sPair.toString() == vs.convert<std::string>());
}


void VarTest::testArrayToString()
{
	std::string s1("string");
	Poco::Int8 s2(23);
	std::vector<Var> s16;
	s16.push_back(s1);
	s16.push_back(s2);
	Var a1(s16);
	std::string res = a1.convert<std::string>();
	std::string expected("[ \"string\", 23 ]");
	assertTrue (res == expected);
}


void VarTest::testArrayToStringEscape()
{
	std::string s1("\"quoted string\"");
	Poco::Int8 s2(23);
	std::vector<Var> s16;
	s16.push_back(s1);
	s16.push_back(s2);
	Var a1(s16);
	std::string res = a1.convert<std::string>();
	std::string expected("[ \"\\\"quoted string\\\"\", 23 ]");
	assertTrue (res == expected);
}


void VarTest::testStructToString()
{
	DynamicStruct aStruct;
	aStruct["First Name"] = "Junior";
	aStruct["Last Name"] = "POCO";
	aStruct["Age"] = 1;
	Var a1(aStruct);
	std::string res = a1.convert<std::string>();
	std::string expected = "{ \"Age\": 1, \"First Name\": \"Junior\", \"Last Name\": \"POCO\" }";
	assertTrue (res == expected);
	assertTrue (aStruct.toString() == res);
}


void VarTest::testOrderedStructToString()
{
	OrderedDynamicStruct aStruct;
	aStruct["First Name"] = "Junior";
	aStruct["Last Name"] = "POCO";
	aStruct["Age"] = 1;
	Var a1(aStruct);
	std::string res = a1.convert<std::string>();
	std::string expected = "{ \"First Name\": \"Junior\", \"Last Name\": \"POCO\", \"Age\": 1 }";
	assertTrue(res == expected);
	assertTrue(aStruct.toString() == res);
}


void VarTest::testStructToStringEscape()
{
	DynamicStruct aStruct;
	aStruct["Value"] = "Value with \" and \n";
	Var a1(aStruct);
	std::string res = a1.convert<std::string>();
	std::string expected = "{ \"Value\": \"Value with \\\" and \\n\" }";
	assertTrue (res == expected);
	assertTrue (aStruct.toString() == res);
}


void VarTest::testArrayOfStructsToString()
{
	std::vector<Var> s16;
	DynamicStruct aStruct;
	aStruct["First Name"] = "Junior";
	aStruct["Last Name"] = "POCO";
	aStruct["Age"] = 1;
	s16.push_back(aStruct);
	aStruct["First Name"] = "Senior";
	aStruct["Last Name"] = "POCO";
	aStruct["Age"] = 100;
	s16.push_back(aStruct);
	std::vector<Var> s16Cpy = s16;
	// recursive arrays!
	s16Cpy.push_back(s16);
	s16.push_back(s16Cpy);
	Var a1(s16);
	std::string res = a1.convert<std::string>();
	std::string expected = "[ "
						"{ \"Age\": 1, \"First Name\": \"Junior\", \"Last Name\": \"POCO\" }, "
						"{ \"Age\": 100, \"First Name\": \"Senior\", \"Last Name\": \"POCO\" }, "
							"[ "
							"{ \"Age\": 1, \"First Name\": \"Junior\", \"Last Name\": \"POCO\" }, "
							"{ \"Age\": 100, \"First Name\": \"Senior\", \"Last Name\": \"POCO\" }, "
								"[ "
								"{ \"Age\": 1, \"First Name\": \"Junior\", \"Last Name\": \"POCO\" }, "
								"{ \"Age\": 100, \"First Name\": \"Senior\", \"Last Name\": \"POCO\" } "
								"] ] ]";

	assertTrue (res == expected);
	assertTrue (a1.toString() == res);
}


void VarTest::testStructWithArraysToString()
{
	std::string s1("string");
	Poco::Int8 s2(23);
	std::vector<Var> s16;
	s16.push_back(s1);
	s16.push_back(s2);
	Var a1(s16);
	DynamicStruct addr;
	addr["Number"] = 4;
	addr["Street"] = "Unknown";
	addr["Country"] = "Carinthia";
	DynamicStruct aStruct;
	aStruct["First Name"] = "Junior";
	aStruct["Last Name"] = a1;
	aStruct["Age"] = 1;
	aStruct["Address"] = addr;
	Var a2(aStruct);
	std::string res = a2.convert<std::string>();
	std::string expected = "{ \"Address\": { \"Country\": \"Carinthia\", \"Number\": 4, \"Street\": \"Unknown\" }, "
								"\"Age\": 1, \"First Name\": \"Junior\", \"Last Name\": [ \"string\", 23 ] }";

	assertTrue (res == expected);
	assertTrue (aStruct.toString() == res);
}


void VarTest::testJSONDeserializeString()
{
	Var a("test");
	std::string tst = Var::toString(a);
	Var b = Var::parse(tst);
	assertTrue (b.convert<std::string>() == "test");

	Var c('c');
	tst = Var::toString(c);
	Var b2 = Var::parse(tst);
	char cc = b2.convert<char>();
	assertTrue (cc == 'c');

	tst = "{ \"a\": \"1\", \"b\": \"2\" \n}";
	a = Var::parse(tst);
	assertTrue (a.toString() == "{ \"a\": \"1\", \"b\": \"2\" }");

	tst = "{ \"a\": \"1\", \"b\": \"2\"\n}";
	a = Var::parse(tst);
	assertTrue (a.toString() == "{ \"a\": \"1\", \"b\": \"2\" }");

	tst = "{ \"message\": \"escape\\b\\f\\n\\r\\t\", \"path\": \"\\/dev\\/null\", \"zero\": null }";
	a = Var::parse(tst);
	assertTrue(a.toString() == "{ \"message\": \"escape\\b\\f\\n\\r\\t\", \"path\": \"/dev/null\", \"zero\": null }");
}


void VarTest::testJSONDeserializePrimitives()
{
	Poco::Int8 i8(-12);
	Poco::UInt16 u16(2345);
	Poco::Int32 i32(-24343);
	Poco::UInt64 u64(1234567890);
	u64 *= u64;
	bool b = false;
	float f = 3.1415f;
	double d = 3.1415;

	std::string s8 = Var::toString(i8);
	std::string s16 = Var::toString(u16);
	std::string s32 = Var::toString(i32);
	std::string s64 = Var::toString(u64);
	std::string sb = Var::toString(b);
	std::string sf = Var::toString(f);
	std::string sd = Var::toString(d);
	Var a8 = Var::parse(s8);
	Var a16 = Var::parse(s16);
	Var a32 = Var::parse(s32);
	Var a64 = Var::parse(s64);
	Var ab = Var::parse(sb);
	Var af = Var::parse(sf);
	Var ad = Var::parse(sd);
	assertTrue (a8 == i8);
	assertTrue (a16 == u16);
	assertTrue (a32 == i32);
	assertTrue (a64 == u64);
	assertTrue (ab == b);
	assertTrue (af == f);
	assertTrue (ad == d);
}


void VarTest::testJSONDeserializeArray()
{
	Poco::Int8 i8(-12);
	Poco::UInt16 u16(2345);
	Poco::Int32 i32(-24343);
	Poco::UInt64 u64(1234567890);
	u64 *= u64;
	bool b = false;
	float f = 3.1415f;
	double d = 3.1415;
	std::string s("test string");
	char c('x');
	std::vector<Var> aVec;
	aVec.push_back(i8);
	aVec.push_back(u16);
	aVec.push_back(i32);
	aVec.push_back(u64);
	aVec.push_back(b);
	aVec.push_back(f);
	aVec.push_back(d);
	aVec.push_back(s);
	aVec.push_back(c);

	std::string sVec = Var::toString(aVec);
	Var a = Var::parse(sVec);
	assertTrue (a[0] == i8);
	assertTrue (a[1] == u16);
	assertTrue (a[2] == i32);
	assertTrue (a[3] == u64);
	assertTrue (a[4] == b);
	assertTrue (a[5] == f);
	assertTrue (a[6] == d);
	assertTrue (a[7] == s);
	assertTrue (a[8] == c);
}


void VarTest::testJSONDeserializeComplex()
{
	Poco::Int8 i8(-12);
	Poco::UInt16 u16(2345);
	Poco::Int32 i32(-24343);
	Poco::UInt64 u64(1234567890);
	u64 *= u64;
	bool b = false;
	float f = 3.1415f;
	double d = 3.1415;
	std::string s("test string");
	char c('x');
	DynamicStruct aStr;
	aStr["i8"] = i8;
	aStr["u16"] = u16;
	aStr["i32"] = i32;
	aStr["u64"] = u64;
	aStr["b"] = b;
	aStr["f"] = f;
	aStr["d"] = d;
	aStr["s"] = s;
	aStr["c"] = c;
	std::vector<Var> aVec;
	aVec.push_back(i8);
	aVec.push_back(u16);
	aVec.push_back(i32);
	aVec.push_back(u64);
	aVec.push_back(b);
	aVec.push_back(f);
	aVec.push_back(d);
	aVec.push_back(s);
	aVec.push_back(c);
	aVec.push_back(aStr);
	aStr["vec"] = aVec;

	std::string sStr = Var::toString(aStr);
	Var a = Var::parse(sStr);
	assertTrue (a.isStruct());
	assertTrue (aStr["i8"] == i8);
	assertTrue (aStr["u16"] == u16);
	assertTrue (aStr["i32"] == i32);
	assertTrue (aStr["u64"] == u64);
	assertTrue (aStr["b"] == b);
	assertTrue (aStr["f"] == f);
	assertTrue (aStr["d"] == d);
	assertTrue (aStr["s"] == s);
	assertTrue (aStr["c"] == c);
	Var vecRet = a["vec"];
	assertTrue (vecRet.isArray());
	assertTrue (vecRet[0] == i8);
	assertTrue (vecRet[1] == u16);
	assertTrue (vecRet[2] == i32);
	assertTrue (vecRet[3] == u64);
	assertTrue (vecRet[4] == b);
	assertTrue (vecRet[5] == f);
	assertTrue (vecRet[6] == d);
	assertTrue (vecRet[7] == s);
	assertTrue (vecRet[8] == c);
	Var strRet = vecRet[9];
	assertTrue (strRet.isStruct());
}


void VarTest::testJSONDeserializeStruct()
{
	Poco::Int8 i8(-12);
	Poco::UInt16 u16(2345);
	Poco::Int32 i32(-24343);
	Poco::UInt64 u64(1234567890);
	u64 *= u64;
	bool b = false;
	float f = 3.1415f;
	double d = 3.1415;
	std::string s("test string");
	char c('x');
	DynamicStruct aStr;
	aStr["i8"] = i8;
	aStr["u16"] = u16;
	aStr["i32"] = i32;
	aStr["u64"] = u64;
	aStr["b"] = b;
	aStr["f"] = f;
	aStr["d"] = d;
	aStr["s"] = s;
	aStr["c"] = c;

	std::string sStr = Var::toString(aStr);
	Var a = Var::parse(sStr);
	assertTrue (a["i8"] == i8);
	assertTrue (a["u16"] == u16);
	assertTrue (a["i32"] == i32);
	assertTrue (a["u64"] == u64);
	assertTrue (a["b"] == b);
	assertTrue (a["f"] == f);
	assertTrue (a["d"] == d);
	assertTrue (a["s"] == s);
	assertTrue (a["c"] == c);
}


void VarTest::testJSONRoundtripStruct()
{
	Poco::Int64 i64(-1234567890);
	Poco::UInt64 u64(1234567890);
	u64 *= u64;
	bool b = false;
	double d = 3.1415;
	std::string s("test string");
	DynamicStruct aStr;
	aStr["i64"] = i64;
	aStr["u64"] = u64;
	aStr["b"] = b;
	aStr["d"] = d;
	aStr["s"] = s;

	std::string sStr = Var::toString(aStr);
	Var a = Var::parse(sStr);
	assertTrue (a["i64"].isInteger());
	assertTrue (!a["u64"].isSigned());
	assertTrue (a["b"].isBoolean());
	assertTrue (a["d"].isNumeric());
	assertTrue (a["s"].isString());

	std::string serialized = Var::toString(a);
	assertTrue (sStr == serialized);
}


void VarTest::testDate()
{
	Poco::DateTime dtNow(2007, 3, 13, 8, 12, 15);

	Poco::Timestamp tsNow = dtNow.timestamp();
	Poco::LocalDateTime ldtNow(dtNow.timestamp());
	Var dt(dtNow);
	assertTrue (dt.isDate());
	assertTrue (dt.isTime());
	assertTrue (dt.isDateTime());
	Var ts(tsNow);
	assertTrue (ts.isDate());
	assertTrue (ts.isTime());
	assertTrue (ts.isDateTime());
	Var ldt(ldtNow);
	assertTrue (ldt.isDate());
	assertTrue (ldt.isTime());
	assertTrue (ldt.isDateTime());
	Var dtStr(dt.convert<std::string>());
	Var tsStr(ts.convert<std::string>());
	Var ldtStr(ldt.convert<std::string>());
	DateTime dtRes = dtStr.convert<DateTime>();
	LocalDateTime ldtRes = ldtStr.convert<LocalDateTime>();
	Timestamp tsRes = tsStr.convert<Timestamp>();
	assertTrue (dtNow == dtRes);
	assertTrue (ldtNow == ldtRes);
	assertTrue (tsNow == tsRes);
}


void VarTest::testUUID()
{
	Poco::UUID uuid("f1881be4-c3b7-4a47-9619-5169db5108a7");

	Var vuuid(uuid);
	assertTrue (vuuid.isUUID());

	assert (vuuid.convert<std::string>() == "f1881be4-c3b7-4a47-9619-5169db5108a7");

	assert (vuuid.extract<Poco::UUID>() == uuid);

	Var vstr(std::string("f1881be4-c3b7-4a47-9619-5169db5108a7"));
	assert (vstr.convert<Poco::UUID>() == uuid);

	Var vstr2(std::string("notAnUUID"));
	try
	{
		Poco::UUID uuid2 = vstr2.convert<Poco::UUID>();
		fail("not a valid UUID, must fail");
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void VarTest::testGetIdxNoThrow(Var& a1, std::vector<Var>::size_type n)
{
	Var val1 = a1[n];
}


void VarTest::testGetIdxMustThrow(Var& a1, std::vector<Var>::size_type n)
{
	try
	{
		Var val1 = a1[n];
		fail("bad cast - must throw");
		val1 = 0; // silence the compiler
	}
	catch (Poco::InvalidAccessException&)
	{
	}
	catch (Poco::RangeException&)
	{
	}

	try
	{
		const Var& c1 = a1;
		const Var& cval1 = c1[n];
		fail("bad const cast - must throw");
		assertTrue (cval1 == c1); // silence the compiler
	}
	catch (Poco::InvalidAccessException&)
	{
	}
	catch (Poco::RangeException&)
	{
	}
}


void VarTest::testEmpty()
{
	Var da;
	assertTrue (da.isEmpty());
	assertTrue (da.type() == typeid(void));
	assertTrue (!da.isArray());
	assertTrue (!da.isInteger());
	assertTrue (!da.isNumeric());
	assertTrue (!da.isSigned());
	assertTrue (!da.isString());
	assertTrue (da == da);
	assertTrue (!(da != da));

	assertTrue (da != Var(1));
	assertTrue (!(da == Var(1)));
	assertTrue (Var(1) != da);
	assertTrue (!(Var(1) == da));

	da = "123";
	int i = da.convert<int>();
	assertTrue (123 == i);
	std::string s = da.extract<std::string>();
	assertTrue ("123" == s);
	assertTrue (!da.isEmpty());
	da.empty();
	assertTrue (da.isEmpty());
	assertTrue (da.type() == typeid(void));
	assertTrue (!da.isArray());
	assertTrue (!da.isInteger());
	assertTrue (!da.isNumeric());
	assertTrue (!da.isSigned());
	assertTrue (!da.isString());
	assertTrue (da == da);
	assertTrue (!(da != da));

	assertTrue (da != "");
	assertTrue ("" != da);
	assertTrue (!(da == ""));
	assertTrue (!("" == da));

	testEmptyComparisons<unsigned char>();
	testEmptyComparisons<char>();
	testEmptyComparisons<Poco::UInt8>();
	testEmptyComparisons<Poco::Int8>();
	testEmptyComparisons<Poco::UInt16>();
	testEmptyComparisons<Poco::Int16>();
	testEmptyComparisons<Poco::UInt32>();
	testEmptyComparisons<Poco::Int32>();
	testEmptyComparisons<Poco::UInt64>();
	testEmptyComparisons<Poco::Int64>();
#ifdef POCO_LONG_IS_64_BIT
	testEmptyComparisons<unsigned long>();
	testEmptyComparisons<long>();
#else
	testEmptyComparisons<unsigned long long>();
	testEmptyComparisons<long long>();
#endif
	testEmptyComparisons<float>();
	testEmptyComparisons<double>();

	try
	{
		int POCO_UNUSED i = da;
		fail ("must fail");
	} catch (InvalidAccessException&) { }

	try
	{
		int POCO_UNUSED i = da.extract<int>();
		fail ("must fail");
	} catch (InvalidAccessException&) { }
}


void VarTest::testIterator()
{
	Var da;
	try
	{
		auto it = da.begin();
		fail("calling begin() on empty Var must throw");
	}
	catch (const InvalidAccessException&) { }

	da = Poco::Dynamic::Array();
	assertTrue(da.begin() == da.end());

	da = 1;
	assertTrue (!da.isEmpty());
	assertTrue (da == 1);
	assertTrue (da[0] == 1);
	try
	{
		da[1] = 2;
	}
	catch (RangeException&) {}
	assertTrue (da.begin() != da.end());

	Var::Iterator it = da.begin();
	Var::Iterator end = da.end();
	assertTrue (it != end);
	assertTrue (++it == end);
	assertTrue (--it == da.begin());
	it++;
	assertTrue (it == end);
	try
	{
		++it;
		fail ("must fail");
	}
	catch (RangeException&) {}
	assertTrue (it == end);

	da = "abc";
	assertTrue (da.size() == 3);
	assertTrue (!da.isArray());
	assertTrue (da.isString());
	//assertTrue (da[0] == 'a');
	assertTrue (da.at(0) == 'a');
	//assertTrue (da[1] = 'b');
	assertTrue (da.at(1) == 'b');
	//assertTrue (da[2] = 'c');
	assertTrue (da.at(2) == 'c');

	da.at(0) = 'b';
	assertTrue (da.at(0) == 'b');
	// TODO: allow treatment of strings like arrays
	//da[1] = 'c';
	da.at(1) = 'c';
	assertTrue (da.at(1) == 'c');
	//da[2] = 'a';
	da.at(2) = 'a';
	assertTrue (da.at(2) == 'a');

	it = da.begin();
	end = da.end();
	assertTrue (it != end);
	assertTrue (++it != end);
	assertTrue (--it == da.begin());

	testContainerIterator<std::vector<Var> >();
	testContainerIterator<std::list<Var> >();
	testContainerIterator<std::deque<Var> >();
}


void VarTest::testSharedPtr()
{
	Poco::SharedPtr<int> p = new int(42);
	{
		Var v;
		v = p;
		Var v1;
		v = v1;
		v1 = v;
	}
	assertTrue(p.referenceCount() == 1);
}

struct ProcessDummy
{
	Var &v;
	ProcessDummy(Var &var) : v(var) {}
	void operator()(const Dummy &d)
	{
		v = d;
	}
};

#define ADD_HANDLER_FOR_TYPE_WITH_VALUE(Type, Handler, Value) \
visitor.addHandler<Type>(Handler); \
if (accepted) \
{ \
	var.emplace_back(Type(Value));\
} \
else \
{ \
	warn("handler already exists for " #Type "", __LINE__, __FILE__); \
} void(0)

void VarTest::testVarVisitor()
{
	Visitor visitor;
	Var processedVar;
	auto processInt8 = [&processedVar](const Poco::Int8 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::Int8 ";
	};
	auto processInt16 = [&processedVar](const Poco::Int16 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::Int16 ";
	};
	auto processInt32 = [&processedVar](const Poco::Int32 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::Int32 ";
	};
	auto processInt64 = [&processedVar](const Poco::Int64 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::Int64 ";
	};
	auto processUInt8 = [&processedVar](const Poco::UInt8 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::UInt8 ";
	};
	auto processUInt16 = [&processedVar](const Poco::UInt16 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::UInt16 ";
	};
	auto processUInt32 = [&processedVar](const Poco::UInt32 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::UInt32 ";
	};
	auto processUInt64 = [&processedVar](const Poco::UInt64 &v) -> void
	{
		processedVar = v;
		std::cout << " -> Poco::UInt64 ";
	};
	auto processBool = [&processedVar](const bool &v) -> void
	{
		processedVar = v;
		std::cout << " -> bool ";
	};
	auto processChar = [&processedVar](const char &v) -> void
	{
		processedVar = v;
		std::cout << " -> char ";
	};
	auto processFloat = [&processedVar](const float &v) -> void
	{
		processedVar = v;
		std::cout << " -> float ";
	};
	auto processDouble = [&processedVar](const double &v) -> void
	{
		processedVar = v;
		std::cout << " -> double ";
	};
	auto processLong = [&processedVar](const long &v) -> void
	{
		processedVar = v;
		std::cout << " -> long ";
	};
	auto processLongLong = [&processedVar](const long long &v) -> void
	{
		processedVar = v;
		std::cout << " -> long long ";
	};
	auto processULong = [&processedVar](const unsigned long &v) -> void
	{
		processedVar = v;
		std::cout << " -> unsigned long ";
	};
	auto processULongLong = [&processedVar](const unsigned long long &v) -> void
	{
		processedVar = v;
		std::cout << " -> unsigned long long ";
	};
	auto processString = [&processedVar](const std::string &v) -> void
	{
		processedVar = v;
		std::cout << " -> string ";
	};

	std::vector<Var> var;

	using ulong = unsigned long;
	using longlong = long long;
	using ulonglong = unsigned long long;

	ProcessDummy processDummy(processedVar);

	bool accepted = false;
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::Int8,   processInt8,      -8);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::Int16,  processInt16,     -16);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::Int32,  processInt32,     -32);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::Int64,  processInt64,     -64);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::UInt8,  processUInt8,     8);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::UInt16, processUInt16,    16);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::UInt32, processUInt32,    32);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Poco::UInt64, processUInt64,    64);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(bool,         processBool,      true);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(char,         processChar,      'f');
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(float,        processFloat,     1.2f);
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(double,       processDouble,    2.4);
	if (typeid(long) != typeid(Poco::Int64))
	{accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(long,        processLong,      123L);}
	if (typeid(ulong) != typeid(Poco::UInt64))
	{accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(ulong,       processULong,     124UL);}
	if (typeid(longlong) != typeid(Poco::Int64))
	{accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(longlong,    processLongLong,  123123LL);}
	if (typeid(ulonglong) != typeid(Poco::UInt64))
	{accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(ulonglong,   processULongLong, 124124ULL);}
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(std::string,  processString,    "hello world");
	accepted = ADD_HANDLER_FOR_TYPE_WITH_VALUE(Dummy,        processDummy,     42);

	for (const auto &v : var)
	{
		std::cout << "handle type : " << v.typeName();
		if (visitor.visit(v))
		{
			if (v.type() != typeid(Dummy))
			{
				std::cout  << " [" << v.toString() << "] ... ";
				assertTrue(v == processedVar);
			}
			else
			{
				std::cout  << " [" << v.extract<Dummy>() << "] ... ";
				assertTrue(v.extract<Dummy>() == processedVar.extract<Dummy>());
			}
			std::cout << " ok" << '\n';
		}
		else
		{
			std::cout << " fail" << '\n';
			fail(Poco::format("failed type handle : %s", v.typeName()), __LINE__, __FILE__);
		}
	}
}


void VarTest::setUp()
{
}


void VarTest::tearDown()
{
}


CppUnit::Test* VarTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("VarTest");

	CppUnit_addTest(pSuite, VarTest, testInt8);
	CppUnit_addTest(pSuite, VarTest, testInt16);
	CppUnit_addTest(pSuite, VarTest, testInt32);
	CppUnit_addTest(pSuite, VarTest, testInt64);
	CppUnit_addTest(pSuite, VarTest, testUInt8);
	CppUnit_addTest(pSuite, VarTest, testUInt16);
	CppUnit_addTest(pSuite, VarTest, testUInt32);
	CppUnit_addTest(pSuite, VarTest, testUInt64);
	CppUnit_addTest(pSuite, VarTest, testBool);
	CppUnit_addTest(pSuite, VarTest, testChar);
	CppUnit_addTest(pSuite, VarTest, testFloat);
	CppUnit_addTest(pSuite, VarTest, testDouble);
	CppUnit_addTest(pSuite, VarTest, testLong);
	CppUnit_addTest(pSuite, VarTest, testULong);
	CppUnit_addTest(pSuite, VarTest, testLongLong);
	CppUnit_addTest(pSuite, VarTest, testULongLong);
	CppUnit_addTest(pSuite, VarTest, testString);
	CppUnit_addTest(pSuite, VarTest, testUDT);
	CppUnit_addTest(pSuite, VarTest, testConversionOperator);
	CppUnit_addTest(pSuite, VarTest, testComparisonOperators);
	CppUnit_addTest(pSuite, VarTest, testArithmeticOperators);
	CppUnit_addTest(pSuite, VarTest, testLimitsInt);
	CppUnit_addTest(pSuite, VarTest, testLimitsFloat);
	CppUnit_addTest(pSuite, VarTest, testCtor);
	CppUnit_addTest(pSuite, VarTest, testIsStruct);
	CppUnit_addTest(pSuite, VarTest, testIsArray);
	CppUnit_addTest(pSuite, VarTest, testArrayIdxOperator);
	CppUnit_addTest(pSuite, VarTest, testDynamicPair);
	CppUnit_addTest(pSuite, VarTest, testDynamicStructBasics);
	CppUnit_addTest(pSuite, VarTest, testOrderedDynamicStructBasics);
	CppUnit_addTest(pSuite, VarTest, testDynamicStructEmptyString);
	CppUnit_addTest(pSuite, VarTest, testDynamicStructNoEscapeString);
	CppUnit_addTest(pSuite, VarTest, testDynamicStructString);
	CppUnit_addTest(pSuite, VarTest, testOrderedDynamicStructString);
	CppUnit_addTest(pSuite, VarTest, testDynamicStructInt);
	CppUnit_addTest(pSuite, VarTest, testOrderedDynamicStructInt);
	CppUnit_addTest(pSuite, VarTest, testSharedPtr);
	CppUnit_addTest(pSuite, VarTest, testArrayToString);
	CppUnit_addTest(pSuite, VarTest, testArrayToStringEscape);
	CppUnit_addTest(pSuite, VarTest, testStructToString);
	CppUnit_addTest(pSuite, VarTest, testOrderedStructToString);
	CppUnit_addTest(pSuite, VarTest, testStructToStringEscape);
	CppUnit_addTest(pSuite, VarTest, testArrayOfStructsToString);
	CppUnit_addTest(pSuite, VarTest, testStructWithArraysToString);
	CppUnit_addTest(pSuite, VarTest, testJSONDeserializeString);
	CppUnit_addTest(pSuite, VarTest, testJSONDeserializePrimitives);
	CppUnit_addTest(pSuite, VarTest, testJSONDeserializeArray);
	CppUnit_addTest(pSuite, VarTest, testJSONDeserializeStruct);
	CppUnit_addTest(pSuite, VarTest, testJSONDeserializeComplex);
	CppUnit_addTest(pSuite, VarTest, testJSONRoundtripStruct);
	CppUnit_addTest(pSuite, VarTest, testDate);
	CppUnit_addTest(pSuite, VarTest, testUUID);
	CppUnit_addTest(pSuite, VarTest, testEmpty);
	CppUnit_addTest(pSuite, VarTest, testIterator);
	CppUnit_addTest(pSuite, VarTest, testVarVisitor);

	return pSuite;
}
