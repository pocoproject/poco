//
// SocketProactorTest.h
//
// Definition of the SocketProactorTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SocketProactorTest_INCLUDED
#define SocketProactorTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Thread.h"
#include "Poco/Observer.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketProactor.h"
#include "Poco/Net/SocketNotification.h"
#include "CppUnit/TestCase.h"
#include "EchoServer.h"


class SocketProactorTest: public CppUnit::TestCase
{
public:
	SocketProactorTest(const std::string& name);
	~SocketProactorTest();

	void testTCPSocketProactor();
	void testUDPSocketProactor();
	void testSocketProactorStartStop();

	void testWork();
	void testTimedWork();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SocketProactorTest_INCLUDED
