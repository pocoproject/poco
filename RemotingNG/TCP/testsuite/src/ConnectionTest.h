//
// ConnectionTest.h
//
// Definition of the ConnectionTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ConnectionTest_INCLUDED
#define ConnectionTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"


class ConnectionTest: public CppUnit::TestCase
{
public:
	ConnectionTest(const std::string& name);
	~ConnectionTest();

	void testHandshakeFailure();
	void testHandshakeTimeout();
	void testHandshakeTimeoutZero();
	void testStart();
	void testListenerThreadPool();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // ConnectionTest_INCLUDED
