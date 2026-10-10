//
// PathDeserializerTest.h
//
// Definition of the PathDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PathDeserializerTest_INCLUDED
#define PathDeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class PathDeserializerTest: public CppUnit::TestCase
{
public:
	PathDeserializerTest(const std::string& name);
	~PathDeserializerTest();

	void testParams();
	void testTypes();
	void testAlternation();
	void testAlternationMultiple();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // PathDeserializerTest_INCLUDED
