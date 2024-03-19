//
// UDPServerTest.h
//
// Definition of the UDPServerTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef UDPServerTest_INCLUDED
#define UDPServerTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"

class UDPServerTest: public CppUnit::TestCase
{
public:
	UDPServerTest(const std::string& name);
	~UDPServerTest();

	void testUDPSingleSocket();
	void testUDPMultiSocket();
	void testUDPSingleSocketMultipleHandlers();
	void testUDPMultiSocketMultipleHandlers();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // UDPServerTest_INCLUDED
