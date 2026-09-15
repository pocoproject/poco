//
// JSONDeserializerTest.h
//
// Definition of the JSONDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef JSONDeserializerTest_INCLUDED
#define JSONDeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class JSONDeserializerTest: public CppUnit::TestCase
{
public:
	JSONDeserializerTest(const std::string& name);
	~JSONDeserializerTest();

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

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // JSONDeserializerTest_INCLUDED
