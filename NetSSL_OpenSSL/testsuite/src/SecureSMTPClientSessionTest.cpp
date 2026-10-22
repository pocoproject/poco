//
// SecureSMTPClientSessionTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SecureSMTPClientSessionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "DialogServer.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"


using Poco::Net::SecureSMTPClientSession;
using Poco::Net::CertificateValidationException;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;


SecureSMTPClientSessionTest::SecureSMTPClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


SecureSMTPClientSessionTest::~SecureSMTPClientSessionTest() = default;


void SecureSMTPClientSessionTest::testStartTLSAcceptsHostName()
{
	// The test certificate is issued for www.appinf.com.
	DialogServer server(true, true);
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 Hello localhost");
	server.addResponse("221 Bye");
	StreamSocket socket(SocketAddress("127.0.0.1", server.port()));
	SecureSMTPClientSession session(socket, "www.appinf.com");
	session.login("localhost");
	assertTrue (server.popCommand() == "EHLO localhost");
	assertTrue (session.startTLS());
	session.login("localhost");
	assertTrue (server.popCommand() == "EHLO localhost");
	session.close();
	assertTrue (server.popCommand() == "QUIT");
}


void SecureSMTPClientSessionTest::testStartTLSRejectsOtherHostName()
{
	// The peer address is a loopback address, for which the host name check
	// is left out, so the check fails only if the given host name is used.
	DialogServer server(true, true);
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("221 Bye");
	StreamSocket socket(SocketAddress("127.0.0.1", server.port()));
	SecureSMTPClientSession session(socket, "mail.example.com");
	session.login("localhost");
	try
	{
		session.startTLS();
		fail("certificate is not issued for mail.example.com - must throw");
	}
	catch (CertificateValidationException&)
	{
	}
}


void SecureSMTPClientSessionTest::setUp()
{
}


void SecureSMTPClientSessionTest::tearDown()
{
}


CppUnit::Test* SecureSMTPClientSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SecureSMTPClientSessionTest");

	CppUnit_addTest(pSuite, SecureSMTPClientSessionTest, testStartTLSAcceptsHostName);
	CppUnit_addTest(pSuite, SecureSMTPClientSessionTest, testStartTLSRejectsOtherHostName);

	return pSuite;
}
