//
// OpenSSLPeerTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OpenSSLPeerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Buffer.h"
#include "Poco/Environment.h"
#include "Poco/Thread.h"
#include "Poco/Timespan.h"
#include "Poco/Exception.h"
#include <iostream>


using Poco::Net::StreamSocket;
using Poco::Net::SecureStreamSocket;
using Poco::Net::SecureServerSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;


namespace
{
	// the scenarios of the tls-peer program, as offsets from its base port
	constexpr Poco::UInt16 SERVER_KEY_UPDATE = 0;
	constexpr Poco::UInt16 SERVER_RENEGOTIATION = 1;
	constexpr Poco::UInt16 CLIENT_KEY_UPDATE = 2;
	constexpr Poco::UInt16 CLIENT_RENEGOTIATION = 3;

	class Relay
		/// Connects to a tls-peer scenario and to a server socket and forwards the
		/// bytes between the two connections in both directions. Each read is
		/// written with one call, so that the server receives the records in the
		/// arrangement in which the peer writes them. The destructor waits until
		/// both directions have ended.
	{
	public:
		Relay(const SocketAddress& peer, const SocketAddress& server):
			_peer(peer),
			_server(server)
		{
			_toServer.startFunc([this]() { forward(_peer, _server); });
			_toPeer.startFunc([this]() { forward(_server, _peer); });
		}

		~Relay()
		{
			try
			{
				_toServer.join();
				_toPeer.join();
			}
			catch (...)
			{
			}
		}

	private:
		Relay(const Relay&) = delete;
		Relay& operator = (const Relay&) = delete;

		static void forward(StreamSocket& from, StreamSocket& to)
		{
			try
			{
				Poco::Buffer<char> buffer(32768);
				int n = from.receiveBytes(buffer.begin(), static_cast<int>(buffer.size()));
				while (n > 0)
				{
					to.sendBytes(buffer.begin(), n);
					n = from.receiveBytes(buffer.begin(), static_cast<int>(buffer.size()));
				}
				to.shutdownSend();
			}
			catch (Poco::Exception&)
			{
			}
		}

		StreamSocket _peer;
		StreamSocket _server;
		Thread _toServer;
		Thread _toPeer;
	};
}


OpenSSLPeerTest::OpenSSLPeerTest(const std::string& name): CppUnit::TestCase(name)
{
}


OpenSSLPeerTest::~OpenSSLPeerTest() = default;


void OpenSSLPeerTest::setUp()
{
}


void OpenSSLPeerTest::tearDown()
{
}


void OpenSSLPeerTest::testPostHandshakeRecordAfterData()
{
	const auto address = peerAddress(SERVER_KEY_UPDATE);
	if (!address.has_value())
		return;

	// the peer sends [one][KeyUpdate][two] in one write
	SecureStreamSocket ss(*address);
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	assertEqual ("onetwo", receiveText(ss, 6));
	ss.sendBytes("three", 5);
	assertEqual ("three", receiveText(ss, 5));
}


void OpenSSLPeerTest::testPostHandshakeRecordAfterDataInSmallReads()
{
	const auto address = peerAddress(SERVER_KEY_UPDATE);
	if (!address.has_value())
		return;

	// the peer sends [one][KeyUpdate][two] in one write; "one" does not fit into one read
	SecureStreamSocket ss(*address);
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	assertEqual ("onetwo", receiveText(ss, 6, 2));
}


void OpenSSLPeerTest::testDataAfterRenegotiation()
{
	const auto address = peerAddress(SERVER_RENEGOTIATION);
	if (!address.has_value())
		return;

	// the peer sends a HelloRequest, and its final flight of the renegotiation together with [two]
	SecureStreamSocket ss(*address);
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	char buffer[256];
	const int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertEqual (3, n);
	assertEqual ("two", std::string(buffer, n));
	ss.sendBytes("three", 5);
	assertEqual ("three", receiveText(ss, 5));
}


std::optional<SocketAddress> OpenSSLPeerTest::peerAddress(Poco::UInt16 scenario)
{
	const std::string peer = Poco::Environment::get("POCO_TLS_PEER", "");
	if (peer.empty())
	{
		std::cout << "POCO_TLS_PEER is not set; test skipped" << std::endl;
		return std::nullopt;
	}
	const SocketAddress base(peer);
	return SocketAddress(base.host(), static_cast<Poco::UInt16>(base.port() + scenario));
}


std::string OpenSSLPeerTest::receiveText(StreamSocket& socket, std::size_t length, std::size_t readSize)
{
	std::string text;
	Poco::Buffer<char> buffer(readSize);
	while (text.size() < length)
	{
		const int n = socket.receiveBytes(buffer.begin(), static_cast<int>(buffer.size()));
		if (n <= 0)
			fail("receiveBytes() returned " + std::to_string(n) + " after '" + text + "'");
		text.append(buffer.begin(), n);
	}
	return text;
}


CppUnit::Test* OpenSSLPeerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OpenSSLPeerTest");

	CppUnit_addTest(pSuite, OpenSSLPeerTest, testPostHandshakeRecordAfterData);
	CppUnit_addTest(pSuite, OpenSSLPeerTest, testPostHandshakeRecordAfterDataInSmallReads);
	CppUnit_addTest(pSuite, OpenSSLPeerTest, testDataAfterRenegotiation);

	return pSuite;
}
