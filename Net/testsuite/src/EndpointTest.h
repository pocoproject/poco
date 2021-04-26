//
// EndpointTest.h
//
// Definition of the EndpointTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef EndpointTest_INCLUDED
#define EndpointTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class EndpointTest: public CppUnit::TestCase
{
public:
	EndpointTest(const std::string& name);
	~EndpointTest();

	void testEndpoint();
	void testSocketRelationals();
	void testEndpoint6();
	void testEndpointUnixLocal();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // EndpointTest_INCLUDED
