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
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Exception.h"
#include <array>
#include <cstddef>


using Poco::Net::SecureSMTPClientSession;
using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;


SecureSMTPClientSessionTest::SecureSMTPClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


SecureSMTPClientSessionTest::~SecureSMTPClientSessionTest() = default;


void SecureSMTPClientSessionTest::testStartTLSSendsHostName()
{
	// The server confirms STARTTLS and ends the connection, so the handshake
	// fails after the client has sent its ClientHello, whose server name
	// indication carries the host name that the certificate is verified against.
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket socket(server.address());
	StreamSocket peer = server.acceptConnection();
	const std::string ready("220 Ready to start TLS\r\n");
	peer.sendBytes(ready.data(), static_cast<int>(ready.size()));
	peer.shutdownSend();

	SecureSMTPClientSession session(socket, "mail.example.com");
	try
	{
		session.startTLS();
		fail("server ends the connection during the handshake - must throw");
	}
	catch (Poco::Exception&)
	{
	}
	socket.close();

	std::string received;
	constexpr std::size_t BUFFER_SIZE = 1024;
	std::array<char, BUFFER_SIZE> buffer{};
	int n = 0;
	while ((n = peer.receiveBytes(buffer.data(), static_cast<int>(buffer.size()))) > 0)
	{
		received.append(buffer.data(), n);
	}
	assertTrue (received.find("mail.example.com") != std::string::npos);
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

	CppUnit_addTest(pSuite, SecureSMTPClientSessionTest, testStartTLSSendsHostName);

	return pSuite;
}
