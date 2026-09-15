//
// RawSerializerTest.h
//
// Definition of the RawSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RawSerializerTest_INCLUDED
#define RawSerializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class RawSerializerTest: public CppUnit::TestCase
{
public:
	RawSerializerTest(const std::string& name);
	~RawSerializerTest();

	void testInt();
	void testString();
	void testBinary();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // RawSerializerTest_INCLUDED
