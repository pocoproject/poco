//
// PathSerializerTest.h
//
// Definition of the PathSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PathSerializerTest_INCLUDED
#define PathSerializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class PathSerializerTest: public CppUnit::TestCase
{
public:
	PathSerializerTest(const std::string& name);
	~PathSerializerTest();

	void testParams();
	void testParamsPreserveQuery();
	void testParamsMissing();
	void testTypes();
	void testEscaping();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // PathSerializerTest_INCLUDED
