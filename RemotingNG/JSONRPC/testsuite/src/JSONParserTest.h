//
// JSONParserTest.h
//
// Definition of the JSONParserTest class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSONParserTest_INCLUDED
#define JSONParserTest_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "CppUnit/TestCase.h"


class JSONParserTest: public CppUnit::TestCase
{
public:
	JSONParserTest(const std::string& name);
	~JSONParserTest();

	void testNull();
	void testNullWithWhitespace();
	void testInt();
	void testIntWithWhitespace();
	void testBool();
	void testBoolWithWhitespace();
	void testDouble();
	void testDoubleWithWhitespace();
	void testString();
	void testStringWithWhitespace();
	void testEmptyObject();
	void testEmptyObjectWithWhitespace();
	void testObject1();
	void testObject2();
	void testObject3();
	void testEmptyArray();
	void testEmptyArrayWithWhitespace();
	void testArray1();
	void testArray2();
	void testArray3();
	void testComplexObject();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // JSONParserTest_INCLUDED
