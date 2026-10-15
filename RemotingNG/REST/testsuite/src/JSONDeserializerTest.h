//
// JSONDeserializerTest.h
//
// Definition of the JSONDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSONDeserializerTest_INCLUDED
#define JSONDeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class JSONDeserializerTest: public CppUnit::TestCase
{
public:
	JSONDeserializerTest(const std::string& name);
	~JSONDeserializerTest() override;

	void testError();
	void testObject();
	void testUnexpectedArray();
	void testArray();
	void testUnexpectedObject();
	void testComplexObject();
	void testComplexArray();
	void testScalarBool();
	void testScalarInt();
	void testScalarString();
	void testNullValue();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
};


#endif // JSONDeserializerTest_INCLUDED
