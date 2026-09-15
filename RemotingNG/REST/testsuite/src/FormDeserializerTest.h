//
// FormDeserializerTest.h
//
// Definition of the FormDeserializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef FormDeserializerTest_INCLUDED
#define FormDeserializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class FormDeserializerTest: public CppUnit::TestCase
{
public:
	FormDeserializerTest(const std::string& name);
	~FormDeserializerTest();

	void testGetRequest();
	void testPostRequest();
	void testPostRequestMultiPart();
	void testResponse();
	void testTypes();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // FormDeserializerTest_INCLUDED
