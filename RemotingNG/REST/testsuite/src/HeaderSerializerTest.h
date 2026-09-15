//
// HeaderSerializerTest.h
//
// Definition of the HeaderSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef HeaderSerializerTest_INCLUDED
#define HeaderSerializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class HeaderSerializerTest: public CppUnit::TestCase
{
public:
	HeaderSerializerTest(const std::string& name);
	~HeaderSerializerTest();

	void testParams();
	void testTypes();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // HeaderSerializerTest_INCLUDED
