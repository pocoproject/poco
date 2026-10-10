//
// SerializerTest.h
//
// Definition of the SerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SerializerTest_INCLUDED
#define SerializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class SerializerTest: public CppUnit::TestCase
{
public:
	SerializerTest(const std::string& name);
	~SerializerTest();

	void testGet();
	void testPostForm();
	void testPostFormMultiPart();
	void testPostJSONObject();
	void testPostJSONArray();
	void testPostRaw();
	void testPostRawHeader();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SerializerTest_INCLUDED
