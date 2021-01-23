//
// NamedTuplesTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NamedTuplesTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NamedTuple.h"
#include "Poco/Exception.h"


using Poco::NamedTuple;
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
using Poco::NotFoundException;
using Poco::InvalidArgumentException;


NamedTuplesTest::NamedTuplesTest(const std::string& name): CppUnit::TestCase(name)
{
}


NamedTuplesTest::~NamedTuplesTest()
{
}


void NamedTuplesTest::testNamedTuple1()
{
	typedef NamedTuple<std::string> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 1);
	
	TupleType aTuple2("string1", "1");
	assertTrue (aTuple2["string1"] == "1");

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.set<0>("2");
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3.length == 1);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple2()
{
	typedef NamedTuple<std::string, int> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 2);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3.length == 2);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple3()
{
	typedef NamedTuple<std::string, 
		int, 
		bool> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 3);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3.length == 3);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple4()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 4);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3.length == 4);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple5()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 5);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c');
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3.length == 5);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple6()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 6);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3.length == 6);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple7()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 7);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3.length == 7);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple8()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 8);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3.length == 8);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}



void NamedTuplesTest::testNamedTuple9()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 9);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2");
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3.length == 9);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple10()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 10);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2 );
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3.length == 10);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple11()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 11);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3");
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3.length == 11);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple12()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 12);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3.length == 12);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple13()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 13);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3.length == 13);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple14()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 14);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3.length == 14);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple15()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 15);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c');
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3.length == 15);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple16()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char, 
		long> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 16);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2.length == 16);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0); 
	assertTrue (aTuple3.length == 16);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple17()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 17);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2.length == 17);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0); 
	assertTrue (aTuple3.length == 17);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple18()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 18);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2.length == 18);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3.length == 18);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple19()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 19);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4");
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2.length == 19);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3.length == 19);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple20()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 20);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2.length == 20);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3.length == 20);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(20, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple21()
{
	typedef NamedTuple<std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		std::string, 
		int, 
		bool, 
		float, 
		char, 
		long, 
		double, 
		short, 
		std::string, 
		int,
		bool> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0); 
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == false);
	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 21);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"bool3", true);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2.length == 21);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["bool3"] == false);
	assertTrue (aTuple3.length == 21);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(21, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple22()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 22);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5);
	assertTrue (aTuple2.length == 22);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3.length == 22);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple23()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int, // 21, V
		bool> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 23);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2.length == 23);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3["bool3"] == false);
	assertTrue (aTuple3.length == 23);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple24()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int, // 21, V
		bool, // 22, W
		float> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["X"] == 0); 

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 24);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2.length == 24);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3.length == 24);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple25()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int, // 21, V
		bool, // 22, W
		float, //23, X
		char // 24, Y
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["Y"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 25);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f,
		"char3", 'c');
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2["char3"] == 'c');
	assertTrue (aTuple2.length == 25);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3.length == 25);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple26()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int, // 21, V
		bool, // 22, W
		float, //23, X
		char, // 24, Y
		long // 25, Z
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["Y"] == 0);
	assertTrue (aTuple["Z"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 26);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f,
		"char3", 'c');
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2["char3"] == 'c');
	assertTrue (aTuple2.length == 26);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3.length == 26);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple27()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int, // 21, V
		bool, // 22, W
		float, //23, X
		char, // 24, Y
		long, // 25, Z
		double // 26, A1
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["X"] == 0); 
	assertTrue (aTuple["Y"] == 0);
	assertTrue (aTuple["Z"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 27);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f,
		"char3", 'c',
		"long3", 999,
		"double3", 3.5);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5);
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2["char3"] == 'c');
	assertTrue (aTuple2["long3"] == 999); 
	assertTrue (aTuple2["double3"] == 3.5);
	assertTrue (aTuple2.length == 27);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3.length == 27);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple28()
{
	typedef NamedTuple<std::string, // 0,A
		int, // 1, B
		bool, // 2, C
		float, // 3, D
		char, // 4, E
		long, // 5, F
		double, // 6, G
		short, // 7, H
		std::string, // 8, I
		int, // 9, J
		std::string, // 10, K
		int, // 11, L
		bool, // 12, M
		float, // 13, N
		char, // 14, O
		long, // 15, P
		double, // 16, Q
		short, // 17, R
		std::string, // 18, S
		int, // 19, T
		std::string, //20, U
		int, // 21, V
		bool, // 22, W
		float, //23, X
		char, // 24, Y
		long, // 25, Z
		double, // 26, A1
		short // 27, B1
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["X"] == 0); 
	assertTrue (aTuple["Y"] == 0);
	assertTrue (aTuple["Z"] == 0);
	assertTrue (aTuple["B1"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 28);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f,
		"char3", 'c',
		"long3", 999,
		"double3", 3.5,
		"short3", 3);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5);
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2["char3"] == 'c');
	assertTrue (aTuple2["long3"] == 999); 
	assertTrue (aTuple2["double3"] == 3.5);
	assertTrue (aTuple2["short3"] == 3);
	assertTrue (aTuple2.length == 28);

	assertTrue (aTuple != aTuple2);
	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3.length == 28);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple29()
{
	typedef NamedTuple<std::string, // A
		int, // B
		bool, // C
		float, // D
		char, // E
		long, // F
		double, // G
		short, // H
		std::string, // I
		int, // J
		std::string, // K
		int, // L
		bool, // M
		float, // N
		char, // O
		long, // P
		double, // Q
		short, // R
		std::string, // S
		int, // T
		std::string, // U
		int, // V
		bool, // W
		float, //X
		char, // Y
		long, // Z
		double, // A1
		short, // B1
		std::string // C1
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["X"] == 0); 
	assertTrue (aTuple["Y"] == 0);
	assertTrue (aTuple["Z"] == 0);
	assertTrue (aTuple["B1"] == 0); 
	assertTrue (aTuple["C1"] == "");

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 29);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f,
		"char3", 'c',
		"long3", 999,
		"double3", 3.5,
		"short3", 32700, 
		"string6", "6");
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2["char3"] == 'c');
	assertTrue (aTuple2["long3"] == 999); 
	assertTrue (aTuple2["double3"] == 3.5);
	assertTrue (aTuple2["short3"] == 32700);
	assertTrue (aTuple2["string6"] == "6");
	assertTrue (aTuple2.length == 29);

	assertTrue (aTuple != aTuple2);

	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3["string6"] == "");
	assertTrue (aTuple3.length == 29);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::testNamedTuple30()
{
	typedef NamedTuple<std::string, // A
		int, // B
		bool, // C
		float, // D
		char, // E
		long, // F
		double, // G
		short, // H
		std::string, // I
		int, // J
		std::string, // K
		int, // L
		bool, // M
		float, // N
		char, // O
		long, // P
		double, // Q
		short, // R
		std::string, // S
		int, // T
		std::string, // U
		int, // V
		bool, // W
		float, //X
		char, // Y
		long, // Z
		double, // A1
		short, // B1
		std::string, // C1
		int // D1
	> TupleType;

	TupleType aTuple; 

	assertTrue (aTuple["A"] == "");
	assertTrue (aTuple["B"] == 0);
	assertTrue (aTuple["C"] == false);
	assertTrue (aTuple["E"] == 0);
	assertTrue (aTuple["F"] == 0);
	assertTrue (aTuple["H"] == 0);
	assertTrue (aTuple["I"] == "");
	assertTrue (aTuple["J"] == 0); 
	assertTrue (aTuple["K"] == "");
	assertTrue (aTuple["L"] == 0);
	assertTrue (aTuple["M"] == false);
	assertTrue (aTuple["O"] == 0); 
	assertTrue (aTuple["P"] == 0);
	assertTrue (aTuple["R"] == 0);
	assertTrue (aTuple["S"] == "");
	assertTrue (aTuple["T"] == 0);
	assertTrue (aTuple["U"] == "");
	assertTrue (aTuple["V"] == 0);
	assertTrue (aTuple["W"] == false);
	assertTrue (aTuple["X"] == 0); 
	assertTrue (aTuple["Y"] == 0);
	assertTrue (aTuple["Z"] == 0);
	assertTrue (aTuple["B1"] == 0); 
	assertTrue (aTuple["C1"] == "");
	assertTrue (aTuple["D1"] == 0);

	try { int POCO_UNUSED xyz; xyz = aTuple["XYZ"]; fail ("must fail"); }
	catch (NotFoundException&) { }
	assertTrue (aTuple.length == 30);
	
	TupleType aTuple2("string1", "1", 
		"int1", 1,
		"bool1", true,
		"float1", 1.5f,
		"char1", 'c',
		"long1", 999,
		"double1", 1.5,
		"short1", 32700, 
		"string2", "2",
		"int2", 2,
		"string3", "3", 
		"int3", 3, 
		"bool2", true,
		"float2", 2.5,
		"char2", 'c',
		"long2", 999,
		"double2", 2.5,
		"short2", 32700, 
		"string4", "4",
		"int4", 4,
		"string5", "5", 
		"int5", 5,
		"bool3", true,
		"float3", 3.5f,
		"char3", 'c',
		"long3", 999,
		"double3", 3.5,
		"short3", 32700, 
		"string6", "6",
		"int6", 6);
	assertTrue (aTuple2["string1"] == "1");
	assertTrue (aTuple2["int1"] == 1); 
	assertTrue (aTuple2["bool1"] == true);
	assertTrue (aTuple2["float1"] == 1.5);
	assertTrue (aTuple2["char1"] == 'c');
	assertTrue (aTuple2["long1"] == 999); 
	assertTrue (aTuple2["double1"] == 1.5);
	assertTrue (aTuple2["short1"] == 32700);
	assertTrue (aTuple2["string2"] == "2");
	assertTrue (aTuple2["int2"] == 2); 
	assertTrue (aTuple2["string3"] == "3");
	assertTrue (aTuple2["int3"] == 3); 
	assertTrue (aTuple2["bool2"] == true);
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["char2"] == 'c');
	assertTrue (aTuple2["long2"] == 999); 
	assertTrue (aTuple2["float2"] == 2.5);
	assertTrue (aTuple2["short2"] == 32700);
	assertTrue (aTuple2["string4"] == "4");
	assertTrue (aTuple2["int4"] == 4); 
	assertTrue (aTuple2["string5"] == "5");
	assertTrue (aTuple2["int5"] == 5); 
	assertTrue (aTuple2["bool3"] == true);
	assertTrue (aTuple2["float3"] == 3.5);
	assertTrue (aTuple2["char3"] == 'c');
	assertTrue (aTuple2["long3"] == 999); 
	assertTrue (aTuple2["double3"] == 3.5);
	assertTrue (aTuple2["short3"] == 32700);
	assertTrue (aTuple2["string6"] == "6");
	assertTrue (aTuple2["int6"] == 6);
	assertTrue (aTuple2.length == 30);

	assertTrue (aTuple != aTuple2);

	aTuple = aTuple2;
	assertTrue (aTuple == aTuple2);
	aTuple2.get<1>()++;
	assertTrue (aTuple < aTuple2);

	TupleType aTuple3(aTuple2.names());
	assertTrue (aTuple3.names() == aTuple2.names());
	assertTrue (aTuple3["string1"] == "");
	assertTrue (aTuple3["int1"] == 0); 
	assertTrue (aTuple3["bool1"] == false);
	assertTrue (aTuple3["char1"] == 0);
	assertTrue (aTuple3["long1"] == 0);
	assertTrue (aTuple3["short1"] == 0); 
	assertTrue (aTuple3["string2"] == "");
	assertTrue (aTuple3["int2"] == 0);
	assertTrue (aTuple3["string3"] == "");
	assertTrue (aTuple3["int3"] == 0); 
	assertTrue (aTuple3["bool2"] == false);
	assertTrue (aTuple3["char2"] == 0);
	assertTrue (aTuple3["long2"] == 0);
	assertTrue (aTuple3["short2"] == 0); 
	assertTrue (aTuple3["string4"] == "");
	assertTrue (aTuple3["int4"] == 0);
	assertTrue (aTuple3["string5"] == "");
	assertTrue (aTuple3["int5"] == 0);
	assertTrue (aTuple3["string6"] == "");
	assertTrue (aTuple3["int6"] == 0);
	assertTrue (aTuple3.length == 30);

	assertTrue (aTuple.getName(0) == "string1");
	aTuple.setName(0, "New Name");
	assertTrue (aTuple.getName(0) == "New Name");

	try { aTuple.setName(30, ""); fail("must fail"); }
	catch (InvalidArgumentException&) { }
}


void NamedTuplesTest::setUp()
{
}


void NamedTuplesTest::tearDown()
{
}


CppUnit::Test* NamedTuplesTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NamedTuplesTest");

	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple1);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple2);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple3);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple4);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple5);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple6);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple7);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple8);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple9);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple10);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple11);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple12);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple13);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple14);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple15);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple16);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple17);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple18);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple19);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple20);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple21);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple22);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple23);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple24);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple25);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple26);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple27);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple28);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple29);
	CppUnit_addTest(pSuite, NamedTuplesTest, testNamedTuple30);

	return pSuite;
}
