//
// HeaderDeserializerTest.h
//
// Definition of the HeaderDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HeaderDeserializerTest_INCLUDED
#define HeaderDeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class HeaderDeserializerTest: public CppUnit::TestCase
{
public:
	HeaderDeserializerTest(const std::string& name);
	~HeaderDeserializerTest();

	void testParams();
	void testTypes();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // HeaderDeserializerTest_INCLUDED
