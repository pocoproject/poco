//
// TuplesTest.h
//
// Definition of the TuplesTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef TuplesTest_INCLUDED
#define TuplesTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class TuplesTest: public CppUnit::TestCase
{
public:
	TuplesTest(const std::string& name);
	~TuplesTest();

	void testTuple1();
	void testTuple2();
	void testTuple3();
	void testTuple4();
	void testTuple5();
	void testTuple6();
	void testTuple7();
	void testTuple8();
	void testTuple9();
	void testTuple10();
	void testTuple11();
	void testTuple12();
	void testTuple13();
	void testTuple14();
	void testTuple15();
	void testTuple16();
	void testTuple17();
	void testTuple18();
	void testTuple19();
	void testTuple20();
	void testTuple21();
	void testTuple22();
	void testTuple23();
	void testTuple24();
	void testTuple25();
	void testTuple26();
	void testTuple27();
	void testTuple28();
	void testTuple29();
	void testTuple30();
	void testTuple31();
	void testTuple32();
	void testTuple33();
	void testTuple34();
	void testTuple35();
	void testTuple36();
	void testTuple37();
	void testTuple38();
	void testTuple39();
	void testTuple40();
	void testTupleOrder();
	void testMemOverhead();
	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

	template <class T>
	void testTupleStrictWeak(const T& t1, const T& t2, const T& t3)
	{
		assertTrue (t1 < t2 && !(t2 < t1)); // antisymmetric
		assertTrue (t1 < t2 && t2 < t3 && t1 < t3); // transitive
		assertTrue (!(t1 < t1)); // irreflexive
	}
};


#endif // TuplesTest_INCLUDED
