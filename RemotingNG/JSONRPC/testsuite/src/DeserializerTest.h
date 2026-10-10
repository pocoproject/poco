//
// DeserializerTest.h
//
// Definition of the DeserializerTest class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DeserializerTest_INCLUDED
#define DeserializerTest_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "CppUnit/TestCase.h"


class DeserializerTest: public CppUnit::TestCase
{
public:
	DeserializerTest(const std::string& name);
	~DeserializerTest();

	void testEmptyRequest();
	void testOneParamRequest();
	void testTwoParamRequest();
	void testEmptyReply();
	void testOneParamReply();
	void testTwoParamReply();
	void testComplexRequest();
	void testError();
	void testObject();
	void testUnexpectedArray();
	void testArray();
	void testUnexpectedObject();
	void testComplexObject();
	void testComplexArray();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // DeserializerTest_INCLUDED
