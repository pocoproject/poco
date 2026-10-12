//
// SecureStreamSocketTest.h
//
// Definition of the SecureStreamSocketTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SecureStreamSocketTest_INCLUDED
#define SecureStreamSocketTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "CppUnit/TestCase.h"


class SecureStreamSocketTest: public CppUnit::TestCase
{
public:
	SecureStreamSocketTest(const std::string& name);
	~SecureStreamSocketTest();

	void testSendReceive();
	void testPeek();
	void testBufVecAndBufferOverloadsRefused();
	void testNB();
	void testHandshakeTimeout();
	void testLazyHandshake();
	void testBindClient();
	void testBindClientIPv6();
	void testDefaultCertificateHandler();
	void testLoopbackPeerHostName();
	void testTunneledLoopbackPeerIsVerified();
	void testResolvedLoopbackNameIsVerified();
	void testLocalhostNameOnOtherAddressIsVerified();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void bindAndConnect(const Poco::Net::SocketAddress& loopback, bool reuseAddress);
		/// Binds a client socket to the given loopback address, connects it
		/// to an echo server listening on that address and checks that the
		/// connection uses the bound socket.
};


#endif // SecureStreamSocketTest
