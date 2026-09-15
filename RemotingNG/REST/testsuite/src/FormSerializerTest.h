//
// FormSerializerTest.h
//
// Definition of the FormSerializerTest class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef FormSerializerTest_INCLUDED
#define FormSerializerTest_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "CppUnit/TestCase.h"


class FormSerializerTest: public CppUnit::TestCase
{
public:
	FormSerializerTest(const std::string& name);
	~FormSerializerTest();

	void testGetRequest();
	void testPostRequest();
	void testPostMultipartRequest();
	void testPostMultipartCharVectorRequest();
	void testPostMultipartCharVectorRequestWithContentType();
	void testResponse();
	void testTypes();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // FormSerializerTest_INCLUDED
