//
// RawDeserializerTest.h
//
// Definition of the RawDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RawDeserializerTest_INCLUDED
#define RawDeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class RawDeserializerTest: public CppUnit::TestCase
{
public:
	RawDeserializerTest(const std::string& name);
	~RawDeserializerTest();

	void testInt();
	void testString();
	void testBinary();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // RawDeserializerTest_INCLUDED
