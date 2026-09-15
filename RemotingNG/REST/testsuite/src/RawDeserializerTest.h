//
// RawDeserializerTest.h
//
// Definition of the RawDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
