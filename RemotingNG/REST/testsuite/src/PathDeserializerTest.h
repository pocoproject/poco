//
// PathDeserializerTest.h
//
// Definition of the PathDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
