//
// DeserializerTest.h
//
// Definition of the DeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef DeserializerTest_INCLUDED
#define DeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class DeserializerTest: public CppUnit::TestCase
{
public:
	DeserializerTest(const std::string& name);
	~DeserializerTest();

	void testGet();
	void testPostForm();
	void testPostJSONObject();
	void testPostJSONArray();
	void testPostRaw();
	void testPostRawHeader();
	void testMethodOverride();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // DeserializerTest_INCLUDED
