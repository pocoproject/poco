//
// HeaderSerializerTest.h
//
// Definition of the HeaderSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
