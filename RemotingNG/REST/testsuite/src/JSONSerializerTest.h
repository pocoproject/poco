//
// JSONSerializerTest.h
//
// Definition of the JSONSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSONSerializerTest_INCLUDED
#define JSONSerializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class JSONSerializerTest: public CppUnit::TestCase
{
public:
	JSONSerializerTest(const std::string& name);
	~JSONSerializerTest();

	void testError();
	void testObject();
	void testArray();
	void testComplexObject();
	void testComplexArray();
	void testScalarBool();
	void testScalarInt();
	void testScalarString();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // JSONSerializerTest_INCLUDED
