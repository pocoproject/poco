//
// TuplesTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TuplesTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Tuple.h"
#include "Poco/Void.h"
#include <algorithm>
#include <map>


using Poco::TypeList;
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
using Poco::Tuple;
using Poco::Void;
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


TuplesTest::TuplesTest(const std::string& name): CppUnit::TestCase(name)
{
}


TuplesTest::~TuplesTest()
{
}


void TuplesTest::testTuple1()
{
	Tuple<int> t;
	assertTrue (t.length == 1);
	t.set<0>(-1);
	assertTrue (t.get<0>() == -1);
	
	Tuple<int> t2(-1);
	assertTrue (t2.get<0>() == -1);

	Tuple<int> tCpy (t2);
	assertTrue (t2.get<0>() == tCpy.get<0>());
	assertTrue (t2 == tCpy);

	Tuple<int> tCpy2 = t2;
	assertTrue (t2.get<0>() == tCpy2.get<0>());
	assertTrue (t2 == tCpy2);
	tCpy2.set<0>(888);
	assertTrue (t2 != tCpy2);
	assertTrue (t < tCpy2);

	Tuple<int, std::string> t3(0);
	assertTrue (t3.get<1>().empty());

	Tuple<int, std::string, std::vector<std::string> > t4(0);
	assertTrue (t4.get<1>().empty());
	assertTrue (t4.get<2>().empty());
}


void TuplesTest::testTuple2()
{
	Tuple<int, Void> dummy1;
	assertTrue (dummy1.length == 2);
	dummy1.set<0>(-1);
	assertTrue (dummy1.get<0>() == -1);
	//test partial init
	Tuple<int, Void> dummy11(0);

	Tuple<Void, int> dummy2;
	dummy2.set<1>(-1);
	assertTrue (dummy2.get<1>() == -1);

	Tuple<Void, int> dummy3;
	dummy3.set<1>(-1);
	assertTrue (dummy3.get<1>() == -1);

	Tuple<std::string, int> aTuple;
	aTuple.set<0>("str");
	aTuple.set<1>(1);
	assertTrue (aTuple.get<0>() == "str");
	assertTrue (aTuple.get<1>() == 1);

	Tuple<std::string, int> aTuple2(aTuple);
	assertTrue (aTuple == aTuple2);
	aTuple2.set<1>(99);
	assertTrue (aTuple != aTuple2);
	aTuple2.set<1>(100000);
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple3()
{
	Tuple<int, Void, Void> dummy1;
	assertTrue (dummy1.length == 3);
	dummy1.set<0>(-1);
	assertTrue (dummy1.get<0>() == -1);
	//test partial init
	Tuple<int, Void, Void> dummy11(0);

	Tuple<Void, int, Void> dummy2;
	dummy2.set<1>(-1);
	assertTrue (dummy2.get<1>() == -1);

	Tuple<Void, Void, int> dummy3;
	dummy3.set<2>(-1);
	assertTrue (dummy3.get<2>() == -1);

	Tuple<std::string, int, bool> aTuple;
	aTuple.set<0>("str");
	aTuple.set<1>(1);
	aTuple.set<2>(true);
	assertTrue (aTuple.get<0>() == "str");
	assertTrue (aTuple.get<1>() == 1);
	assertTrue (aTuple.get<2>());

	Tuple<std::string, int, bool> aTuple2(aTuple);
	assertTrue (aTuple == aTuple2);
	aTuple2.set<2>(false);
	assertTrue (aTuple != aTuple2);
	aTuple2.set<1>(100000);
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple4()
{
	Tuple<int, Void, Void, Void> dummy1;
	assertTrue (dummy1.length == 4);
	dummy1.set<0>(-1);
	assertTrue (dummy1.get<0>() == -1);
	//test partial init
	Tuple<int, Void, Void, Void> dummy11(0);

	Tuple<Void, int, Void, Void> dummy2;
	dummy2.set<1>(-1);
	assertTrue (dummy2.get<1>() == -1);

	Tuple<Void, Void, Void, int> dummy3;
	dummy3.set<3>(-1);
	assertTrue (dummy3.get<3>() == -1);

	Tuple<std::string, int, bool, float> aTuple;
	aTuple.set<0>("str");
	aTuple.set<1>(1);
	aTuple.set<2>(true);
	aTuple.set<3>(3.14f);
	assertTrue (aTuple.get<0>() == "str");
	assertTrue (aTuple.get<1>() == 1);
	assertTrue (aTuple.get<2>());
	assertTrue (aTuple.get<3>() >= 3.13f && aTuple.get<3>() <= 3.15f);

	Tuple<std::string, int, bool, float> aTuple2(aTuple);
	assertTrue (aTuple == aTuple2);
	aTuple2.set<2>(false);
	assertTrue (aTuple != aTuple2);
	aTuple2.set<1>(100000);
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple5()
{
	Tuple<int, Void, Void, Void, Void> dummy1;
	assertTrue (dummy1.length == 5);
	dummy1.set<0>(-1);
	assertTrue (dummy1.get<0>() == -1);
	//test partial init
	Tuple<int, Void, Void, Void, Void> dummy11(0);

	Tuple<Void, int, Void, Void, Void> dummy2;
	dummy2.set<1>(-1);
	assertTrue (dummy2.get<1>() == -1);

	Tuple<Void, Void, Void, Void, int> dummy3;
	dummy3.set<4>(-1);
	assertTrue (dummy3.get<4>() == -1);

	Tuple<std::string, int, bool, float, char> aTuple;
	aTuple.set<0>("str");
	aTuple.set<1>(1);
	aTuple.set<2>(true);
	aTuple.set<3>(3.14f);
	aTuple.set<4>(' ');
	assertTrue (aTuple.get<0>() == "str");
	assertTrue (aTuple.get<1>() == 1);
	assertTrue (aTuple.get<2>());
	assertTrue (aTuple.get<3>() >= 3.13f && aTuple.get<3>() <= 3.15f);
	assertTrue (aTuple.get<4>() == ' ');

	Tuple<std::string, int, bool, float, char> aTuple2(aTuple);
	assertTrue (aTuple == aTuple2);
	aTuple2.set<2>(false);
	assertTrue (aTuple != aTuple2);
	aTuple2.set<1>(100000);
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple6()
{
	Tuple<std::string, int, bool, float, char, long> aTuple;
	assertTrue (aTuple.length == 6);
	Tuple<std::string, int, bool, float, char, long> 
		aTuple2("1", 1, true, 3.14f, 'c', 999);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple7()
{
	Tuple<std::string, int, bool, float, char, long, double> aTuple;
	assertTrue (aTuple.length == 7);
	Tuple<std::string, int, bool, float, char, long, double> 
		aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple8()
{
	Tuple<std::string, int, bool, float, char, long, double, short> aTuple;
	assertTrue (aTuple.length == 8);
	Tuple<std::string, int, bool, float, char, long, double, short> 
		aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}

void TuplesTest::testTuple9()
{
	Tuple<std::string, int, bool, float, char, long, double, short, std::string> aTuple;
	assertTrue (aTuple.length == 9);
	Tuple<std::string, int, bool, float, char, long, double, short, std::string> 
		aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2");
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple10()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 10);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple11()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 11);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1");
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple12()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 12);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple13()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 13);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple14()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 14);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple15()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 15);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f, 'c');
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple16()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char, long> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 16);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f, 'c', 999);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple17()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char, long, double> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 17);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f, 'c', 999, 33.14);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple18()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char, long, double, short> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 18);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f, 'c', 999, 33.14, 32700);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple19()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char, long, double, short, std::string> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 19);
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2");
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple20()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char, long, double, short, std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 20);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2,
					  "1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple21()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 21);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5");
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple22()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 22);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple23()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 23);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple24()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 24);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple25()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 25);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c');
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple26()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 26);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple27()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 27);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple28()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 28);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple29()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 29);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6");
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple30()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 30);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple31()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 31);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7");
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple32()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 32);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple33()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 33);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple34()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 34);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple35()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 35);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f, 'd');
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple36()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 36);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f, 'd', 999);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple37()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 37);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f, 'd', 999, 33.14);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple38()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 38);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f, 'd', 999, 33.14, 32700);
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple39()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 39);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f, 'd', 999, 33.14, 32700, "8");
	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3 == aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTuple40()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int,
		std::string, int, bool, float, char, long, double, short, std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 40);

	assertTrue (aTuple.get<0>() == "");
	assertTrue (aTuple.get<1>() == 0);
	assertTrue (aTuple.get<2>() == false);
	assertTrue (aTuple.get<4>() == 0);
	assertTrue (aTuple.get<5>() == 0);
	assertTrue (aTuple.get<7>() == 0);
	assertTrue (aTuple.get<8>() == "");
	assertTrue (aTuple.get<9>() == 0);

	assertTrue (aTuple.get<10>() == "");
	assertTrue (aTuple.get<11>() == 0);
	assertTrue (aTuple.get<12>() == false);
	assertTrue (aTuple.get<14>() == 0);
	assertTrue (aTuple.get<15>() == 0);
	assertTrue (aTuple.get<17>() == 0);
	assertTrue (aTuple.get<18>() == "");
	assertTrue (aTuple.get<19>() == 0);

	assertTrue (aTuple.get<20>() == "");
	assertTrue (aTuple.get<21>() == 0);
	assertTrue (aTuple.get<22>() == false);
	assertTrue (aTuple.get<24>() == 0);
	assertTrue (aTuple.get<25>() == 0);
	assertTrue (aTuple.get<27>() == 0);
	assertTrue (aTuple.get<28>() == "");
	assertTrue (aTuple.get<29>() == 0);

	assertTrue (aTuple.get<30>() == "");
	assertTrue (aTuple.get<31>() == 0);
	assertTrue (aTuple.get<32>() == false);
	assertTrue (aTuple.get<34>() == 0);
	assertTrue (aTuple.get<35>() == 0);
	assertTrue (aTuple.get<37>() == 0);
	assertTrue (aTuple.get<38>() == "");
	assertTrue (aTuple.get<39>() == 0);

	
	TupleType aTuple2("1", 1, true, 3.14f, 'a', 999, 33.14, 32700, "2", 2,
					  "3", 3, true, 3.14f, 'b', 999, 33.14, 32700, "4", 4,
					  "5", 5, true, 3.14f, 'c', 999, 33.14, 32700, "6", 6,
					  "7", 7, true, 3.14f, 'd', 999, 33.14, 32700, "8", 8);

	assertTrue (aTuple2.get<0>() == "1");
	assertTrue (aTuple2.get<1>() == 1);
	assertTrue (aTuple2.get<2>() == true);
	assertTrue (aTuple2.get<4>() == 'a');
	assertTrue (aTuple2.get<5>() == 999);
	assertTrue (aTuple2.get<7>() == 32700);
	assertTrue (aTuple2.get<8>() == "2");
	assertTrue (aTuple2.get<9>() == 2);

	assertTrue (aTuple2.get<10>() == "3");
	assertTrue (aTuple2.get<11>() == 3);
	assertTrue (aTuple2.get<12>() == true);
	assertTrue (aTuple2.get<14>() == 'b');
	assertTrue (aTuple2.get<15>() == 999);
	assertTrue (aTuple2.get<17>() == 32700);
	assertTrue (aTuple2.get<18>() == "4");
	assertTrue (aTuple2.get<19>() == 4);

	assertTrue (aTuple2.get<20>() == "5");
	assertTrue (aTuple2.get<21>() == 5);
	assertTrue (aTuple2.get<22>() == true);
	assertTrue (aTuple2.get<24>() == 'c');
	assertTrue (aTuple2.get<25>() == 999);
	assertTrue (aTuple2.get<27>() == 32700);
	assertTrue (aTuple2.get<28>() == "6");
	assertTrue (aTuple2.get<29>() == 6);

	assertTrue (aTuple2.get<30>() == "7");
	assertTrue (aTuple2.get<31>() == 7);
	assertTrue (aTuple2.get<32>() == true);
	assertTrue (aTuple2.get<34>() == 'd');
	assertTrue (aTuple2.get<35>() == 999);
	assertTrue (aTuple2.get<37>() == 32700);
	assertTrue (aTuple2.get<38>() == "8");
	assertTrue (aTuple2.get<39>() == 8);

	assertTrue (aTuple != aTuple2);
	TupleType aTuple3(aTuple2);
	assertTrue (aTuple3.get<0>() == "1");
	assertTrue (aTuple3.get<1>() == 1);
	assertTrue (aTuple3.get<2>() == true);
	assertTrue (aTuple3.get<4>() == 'a');
	assertTrue (aTuple3.get<5>() == 999);
	assertTrue (aTuple3.get<7>() == 32700);
	assertTrue (aTuple3.get<8>() == "2");
	assertTrue (aTuple3.get<9>() == 2);

	assertTrue (aTuple3.get<10>() == "3");
	assertTrue (aTuple3.get<11>() == 3);
	assertTrue (aTuple3.get<12>() == true);
	assertTrue (aTuple3.get<14>() == 'b');
	assertTrue (aTuple3.get<15>() == 999);
	assertTrue (aTuple3.get<17>() == 32700);
	assertTrue (aTuple3.get<18>() == "4");
	assertTrue (aTuple3.get<19>() == 4);

	assertTrue (aTuple3.get<20>() == "5");
	assertTrue (aTuple3.get<21>() == 5);
	assertTrue (aTuple3.get<22>() == true);
	assertTrue (aTuple3.get<24>() == 'c');
	assertTrue (aTuple3.get<25>() == 999);
	assertTrue (aTuple3.get<27>() == 32700);
	assertTrue (aTuple3.get<28>() == "6");
	assertTrue (aTuple3.get<29>() == 6);

	assertTrue (aTuple3.get<30>() == "7");
	assertTrue (aTuple3.get<31>() == 7);
	assertTrue (aTuple3.get<32>() == true);
	assertTrue (aTuple3.get<34>() == 'd');
	assertTrue (aTuple3.get<35>() == 999);
	assertTrue (aTuple3.get<37>() == 32700);
	assertTrue (aTuple3.get<38>() == "8");
	assertTrue (aTuple3.get<39>() == 8);
	assertTrue (aTuple3 == aTuple2);

	aTuple = aTuple2;
	assertTrue (aTuple.get<0>() == "1");
	assertTrue (aTuple.get<1>() == 1);
	assertTrue (aTuple.get<2>() == true);
	assertTrue (aTuple.get<4>() == 'a');
	assertTrue (aTuple.get<5>() == 999);
	assertTrue (aTuple.get<7>() == 32700);
	assertTrue (aTuple.get<8>() == "2");
	assertTrue (aTuple.get<9>() == 2);

	assertTrue (aTuple.get<10>() == "3");
	assertTrue (aTuple.get<11>() == 3);
	assertTrue (aTuple.get<12>() == true);
	assertTrue (aTuple.get<14>() == 'b');
	assertTrue (aTuple.get<15>() == 999);
	assertTrue (aTuple.get<17>() == 32700);
	assertTrue (aTuple.get<18>() == "4");
	assertTrue (aTuple.get<19>() == 4);

	assertTrue (aTuple.get<20>() == "5");
	assertTrue (aTuple.get<21>() == 5);
	assertTrue (aTuple.get<22>() == true);
	assertTrue (aTuple.get<24>() == 'c');
	assertTrue (aTuple.get<25>() == 999);
	assertTrue (aTuple.get<27>() == 32700);
	assertTrue (aTuple.get<28>() == "6");
	assertTrue (aTuple.get<29>() == 6);

	assertTrue (aTuple.get<30>() == "7");
	assertTrue (aTuple.get<31>() == 7);
	assertTrue (aTuple.get<32>() == true);
	assertTrue (aTuple.get<34>() == 'd');
	assertTrue (aTuple.get<35>() == 999);
	assertTrue (aTuple.get<37>() == 32700);
	assertTrue (aTuple.get<38>() == "8");
	assertTrue (aTuple.get<39>() == 8);
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);
}


void TuplesTest::testTupleOrder()
{
	typedef Tuple<std::string, int, bool, float, char, long, double, short, std::string, int, 
		std::string, int, bool, float, char, long, double, short, std::string, int> TupleType;

	TupleType aTuple;
	assertTrue (aTuple.length == 20);
	
	TupleType aTuple2("1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2, "1", 1, true, 3.14f, 'c', 999, 33.14, 32700, "2", 2);
	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3;
	aTuple3 = aTuple2;
	aTuple3.get<1>()++;
	assertTrue (aTuple2 < aTuple3);

	testTupleStrictWeak(aTuple, aTuple2, aTuple3);

	std::vector<TupleType> tv;
	tv.push_back(aTuple3);
	tv.push_back(aTuple);
	tv.push_back(aTuple2);

	assertTrue (tv[0] == aTuple3);
	assertTrue (tv[1] == aTuple);
	assertTrue (tv[2] == aTuple2);

	std::sort(tv.begin(), tv.end());
	
	assertTrue (tv[0] == aTuple);
	assertTrue (tv[1] == aTuple2);
	assertTrue (tv[2] == aTuple3);

	std::map<TupleType, int> tm;
	std::pair<std::map<TupleType, int>::iterator, bool> mIt = tm.insert(std::map<TupleType, int>::value_type(aTuple2, 2));
	assertTrue (mIt.second);
	mIt = tm.insert(std::map<TupleType, int>::value_type(aTuple, 1));
	assertTrue (mIt.second);
	mIt = tm.insert(std::map<TupleType, int>::value_type(aTuple3, 3));
	assertTrue (mIt.second);

	std::map<TupleType, int>::iterator fIt = tm.find(aTuple2);
	assertTrue (2 == fIt->second);

	typedef Tuple<std::string, std::string, std::string, std::string> StrTup;
	typedef std::map<StrTup, int> StrTupMap;

	StrTup st1("123", "456", "789", "101112");
	StrTup st2("123", "456", "101112", "789");
	StrTup st3("123", "789", "789", "101112");
	StrTup st4("123", "101112", "456", "789");

	testTupleStrictWeak(st2, st1, st3);

	StrTupMap strMap;
	strMap.insert(StrTupMap::value_type(st1, 1));
	strMap.insert(StrTupMap::value_type(st2, 2));
	strMap.insert(StrTupMap::value_type(st3, 3));
	strMap.insert(StrTupMap::value_type(st4, 4));

	assertTrue (1 == strMap[st1]);
	assertTrue (2 == strMap[st2]);
	assertTrue (3 == strMap[st3]);
	assertTrue (4 == strMap[st4]);

	StrTupMap::iterator it = strMap.begin();
	assertTrue (st4 == it->first); ++it;
	assertTrue (st2 == it->first); ++it;
	assertTrue (st1 == it->first); ++it;
	assertTrue (st3 == it->first); ++it;
	assertTrue (it == strMap.end());
}


void TuplesTest::testMemOverhead()
{
	Tuple<short> smallOne(0);
	int sz = sizeof(smallOne);
	assertTrue (sz == 4 || sz == 8); //depending on architecture and alignment
	Tuple<long> notSoSmall(0);
	sz = sizeof(notSoSmall);
	assertTrue (sz == 8 || sz == 16); //depending on architecture and alignment
}


void TuplesTest::setUp()
{
}


void TuplesTest::tearDown()
{
}


CppUnit::Test* TuplesTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TuplesTest");

	CppUnit_addTest(pSuite, TuplesTest, testTuple1);
	CppUnit_addTest(pSuite, TuplesTest, testTuple2);
	CppUnit_addTest(pSuite, TuplesTest, testTuple3);
	CppUnit_addTest(pSuite, TuplesTest, testTuple4);
	CppUnit_addTest(pSuite, TuplesTest, testTuple5);
	CppUnit_addTest(pSuite, TuplesTest, testTuple6);
	CppUnit_addTest(pSuite, TuplesTest, testTuple7);
	CppUnit_addTest(pSuite, TuplesTest, testTuple8);
	CppUnit_addTest(pSuite, TuplesTest, testTuple9);
	CppUnit_addTest(pSuite, TuplesTest, testTuple10);
	CppUnit_addTest(pSuite, TuplesTest, testTuple11);
	CppUnit_addTest(pSuite, TuplesTest, testTuple12);
	CppUnit_addTest(pSuite, TuplesTest, testTuple13);
	CppUnit_addTest(pSuite, TuplesTest, testTuple14);
	CppUnit_addTest(pSuite, TuplesTest, testTuple15);
	CppUnit_addTest(pSuite, TuplesTest, testTuple16);
	CppUnit_addTest(pSuite, TuplesTest, testTuple17);
	CppUnit_addTest(pSuite, TuplesTest, testTuple18);
	CppUnit_addTest(pSuite, TuplesTest, testTuple19);
	CppUnit_addTest(pSuite, TuplesTest, testTuple20);
	CppUnit_addTest(pSuite, TuplesTest, testTuple21);
	CppUnit_addTest(pSuite, TuplesTest, testTuple22);
	CppUnit_addTest(pSuite, TuplesTest, testTuple23);
	CppUnit_addTest(pSuite, TuplesTest, testTuple24);
	CppUnit_addTest(pSuite, TuplesTest, testTuple25);
	CppUnit_addTest(pSuite, TuplesTest, testTuple26);
	CppUnit_addTest(pSuite, TuplesTest, testTuple27);
	CppUnit_addTest(pSuite, TuplesTest, testTuple28);
	CppUnit_addTest(pSuite, TuplesTest, testTuple29);
	CppUnit_addTest(pSuite, TuplesTest, testTuple30);
	CppUnit_addTest(pSuite, TuplesTest, testTuple31);
	CppUnit_addTest(pSuite, TuplesTest, testTuple32);
	CppUnit_addTest(pSuite, TuplesTest, testTuple33);
	CppUnit_addTest(pSuite, TuplesTest, testTuple34);
	CppUnit_addTest(pSuite, TuplesTest, testTuple35);
	CppUnit_addTest(pSuite, TuplesTest, testTuple36);
	CppUnit_addTest(pSuite, TuplesTest, testTuple37);
	CppUnit_addTest(pSuite, TuplesTest, testTuple38);
	CppUnit_addTest(pSuite, TuplesTest, testTuple39);
	CppUnit_addTest(pSuite, TuplesTest, testTuple40);
	CppUnit_addTest(pSuite, TuplesTest, testTupleOrder);
	CppUnit_addTest(pSuite, TuplesTest, testMemOverhead);

	return pSuite;
}
