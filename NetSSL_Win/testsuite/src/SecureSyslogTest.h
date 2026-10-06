//
// SecureSyslogTest.h
//
// Definition of the SecureSyslogTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SecureSyslogTest_INCLUDED
#define SecureSyslogTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class SecureSyslogTest: public CppUnit::TestCase
{
public:
	SecureSyslogTest(const std::string& name);
	~SecureSyslogTest();

	void testRoundTrip();
	void testSuppliedSocket();
	void testNewlineFraming();
	void testLargeAndCoalesced();
	void testPlainClient();
	void testStalledHandshake();
	void testCloseWithClients();
	void testChannelReconnect();
	void testChannelHandshakeTimeout();
	void testDefaultPort();
	void testProperties();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // SecureSyslogTest_INCLUDED
