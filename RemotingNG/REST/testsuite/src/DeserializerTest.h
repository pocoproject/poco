//
// DeserializerTest.h
//
// Definition of the DeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DeserializerTest_INCLUDED
#define DeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class DeserializerTest: public CppUnit::TestCase
{
public:
	DeserializerTest(const std::string& name);
	~DeserializerTest() override;

	void testGet();
	void testPostForm();
	void testPostJSONObject();
	void testPostJSONArray();
	void testPostRaw();
	void testPostRawHeader();
	void testMethodOverride();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
};


#endif // DeserializerTest_INCLUDED
