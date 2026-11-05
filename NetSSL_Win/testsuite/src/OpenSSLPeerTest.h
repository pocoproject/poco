//
// OpenSSLPeerTest.h
//
// Definition of the OpenSSLPeerTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OpenSSLPeerTest_INCLUDED
#define OpenSSLPeerTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "CppUnit/TestCase.h"
#include <optional>
#include <string>


class OpenSSLPeerTest: public CppUnit::TestCase
	/// Tests that need a peer that sends post-handshake messages after application
	/// data, which Schannel does not do. They connect to the tls-peer program
	/// (NetSSL_Win/testsuite/tls-peer), which the environment variable POCO_TLS_PEER
	/// names as host:baseport, and are skipped when it is not set.
{
public:
	OpenSSLPeerTest() = delete;
	explicit OpenSSLPeerTest(const std::string& name);
	~OpenSSLPeerTest() override;

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
	static std::optional<Poco::Net::SocketAddress> peerAddress(Poco::UInt16 scenario);
		/// Returns the address of the tls-peer scenario, given as the offset from
		/// the base port. Returns no address, and prints that the test is skipped,
		/// when POCO_TLS_PEER is not set.

	std::string receiveText(Poco::Net::StreamSocket& socket, std::size_t length);
		/// Receives length bytes. Fails the test when a call returns no data.
};


#endif // OpenSSLPeerTest_INCLUDED
