//
// SerializerTest.h
//
// Definition of the SerializerTest class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef SerializerTest_INCLUDED
#define SerializerTest_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "CppUnit/TestCase.h"


class SerializerTest: public CppUnit::TestCase
{
public:
	SerializerTest(const std::string& name);
	~SerializerTest();

	void testEmptyRequest();
	void testOneParamRequest();
	void testTwoParamRequest();
	void testEmptyReply();
	void testOneParamReply();
	void testTwoParamReply();
	void testComplexRequest();
	void testError();
	void testObject();
	void testArray();
	void testComplexObject();
	void testComplexArray();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SerializerTest_INCLUDED
