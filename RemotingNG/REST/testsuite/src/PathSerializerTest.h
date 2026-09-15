//
// PathSerializerTest.h
//
// Definition of the PathSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
