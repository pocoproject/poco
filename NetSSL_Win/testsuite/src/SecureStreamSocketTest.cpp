//
// SecureStreamSocketTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SecureStreamSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/RejectCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Buffer.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/Exception.h"
#include <iostream>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SecureStreamSocket;
using Poco::Net::SecureServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::Context;
using Poco::Net::RejectCertificateHandler;
using Poco::Net::Session;
using Poco::Net::SSLManager;
using Poco::Thread;
using Poco::Util::AbstractConfiguration;
using Poco::Util::Application;
using Poco::Util::LayeredConfiguration;


namespace
{
	bool acceptsConnections(const Poco::Net::IPAddress& address)
		/// Returns true if a socket bound to the address can be connected to;
		/// the address of a bridge or a similar interface cannot.
	{
		try
		{
			Poco::Net::ServerSocket server(SocketAddress(address, 0));
			StreamSocket client;
			client.connect(server.address(), Poco::Timespan(1, 0));
			return true;
		}
		catch (Poco::Exception&)
		{
			return false;
		}
	}

	class EchoConnection: public TCPServerConnection
	{
	public:
		EchoConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			StreamSocket& ss = socket();
			ss.setReceiveTimeout(Poco::Timespan(10, 0));
			try
			{
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0)
				{
					ss.sendBytes(buffer, n);
					n = ss.receiveBytes(buffer, sizeof(buffer));
				}
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
			}
		}
	};

	class HeldBackServer: public Poco::Runnable
		/// Takes one connection and answers the TLS handshake only when
		/// it is told to. Keeps what the client then sends.
	{
	public:
		HeldBackServer():
			_socket(SocketAddress("127.0.0.1", 0))
		{
		}

		Poco::UInt16 port() const
		{
			return _socket.address().port();
		}

		void proceed()
		{
			_proceed.set();
		}

		std::string received()
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _received;
		}

		void run() override
		{
			try
			{
				// the handshake of an accepted socket is made with its first data
				StreamSocket socket = _socket.acceptConnection();
				_proceed.wait(30000);
				socket.setReceiveTimeout(Poco::Timespan(10, 0));
				char buffer[256];
				int n = socket.receiveBytes(buffer, sizeof(buffer));
				if (n > 0)
				{
					Poco::FastMutex::ScopedLock lock(_mutex);
					_received.assign(buffer, n);
				}
			}
			catch (Poco::Exception&)
			{
			}
		}

	private:
		SecureServerSocket _socket;
		Poco::Event _proceed;
		Poco::FastMutex _mutex;
		std::string _received;
	};
}


SecureStreamSocketTest::SecureStreamSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


SecureStreamSocketTest::~SecureStreamSocketTest()
{
}


void SecureStreamSocketTest::testSendReceive()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	ss1.setReceiveTimeout(Poco::Timespan(10, 0));
	std::string data("hello, world");
	ss1.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[8192];
	int rc = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (rc > 0);
	assertTrue (std::string(buffer, rc) == data);

	const std::vector<std::size_t> sizes = {67, 467, 7883, 19937};
	for (const auto n: sizes)
	{
		data.assign(n, 'X');
		ss1.sendBytes(data.data(), static_cast<int>(data.size()));
		std::string received;
		while (received.size() < n)
		{
			rc = ss1.receiveBytes(buffer, sizeof(buffer));
			if (rc > 0)
			{
				received.append(buffer, rc);
			}
			else if (rc == 0)
			{
				break;
			}
		}
		assertTrue (received == data);
	}

	ss1.close();
}


void SecureStreamSocketTest::testPeek()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss(sa);
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	
	int n = ss.sendBytes("hello, world!", 13);
	assertTrue (n == 13);
	char buffer[256];
	n = ss.receiveBytes(buffer, 5, MSG_PEEK);
	assertTrue (n == 5);
	assertTrue (std::string(buffer, n) == "hello");
	n = ss.receiveBytes(buffer, sizeof(buffer), MSG_PEEK);
	assertTrue (n == 13);
	assertTrue (std::string(buffer, n) == "hello, world!");
	n = ss.receiveBytes(buffer, 7);
	assertTrue (n == 7);
	assertTrue (std::string(buffer, n) == "hello, ");
	n = ss.receiveBytes(buffer, 6);
	assertTrue (n == 6);
	assertTrue (std::string(buffer, n) == "world!");
	ss.close();
}


void SecureStreamSocketTest::testBufVecAndBufferOverloadsRefused()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss(sa);
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	char data[] = "cleartext";
	Poco::Net::SocketBufVec vec{Socket::makeBuffer(data, sizeof(data) - 1)};
	try
	{
		ss.sendBytes(vec);
		fail("sendBytes() of a SocketBufVec bypasses TLS - must throw");
	}
	catch (Poco::InvalidAccessException&)
	{
	}
	try
	{
		ss.receiveBytes(vec);
		fail("receiveBytes() into a SocketBufVec bypasses TLS - must throw");
	}
	catch (Poco::InvalidAccessException&)
	{
	}
	Poco::Buffer<char> pocoBuffer(0);
	try
	{
		ss.receiveBytes(pocoBuffer);
		fail("receiveBytes() into a Poco::Buffer bypasses TLS - must throw");
	}
	catch (Poco::InvalidAccessException&)
	{
	}

	// the echo arrives intact only if the refused calls did not touch the connection
	int n = ss.sendBytes("hello, world!", 13);
	assertTrue (n == 13);
	char buffer[256];
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n == 13);
	assertTrue (std::string(buffer, n) == "hello, world!");
	ss.close();
}


void SecureStreamSocketTest::testNB()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	ss1.setBlocking(false);
	ss1.setSendBufferSize(32000);

	char buffer[8192];
	const std::vector<std::size_t> sizes = {67, 467, 7883, 19937};
	for (const auto s: sizes)
	{
		std::string data(s, 'X');
		ss1.sendBytes(data.data(), static_cast<int>(data.size()));

		int rc;
		do
		{
			rc = ss1.receiveBytes(buffer, sizeof(buffer), MSG_PEEK);
		}
		while (rc < 0);
		assertTrue (rc > 0 && rc <= s);
		assertTrue (data.compare(0, rc, buffer, rc) == 0);

		std::string received;
		while (received.size() < s)
		{
			rc = ss1.receiveBytes(buffer, sizeof(buffer));
			if (rc > 0)
			{
				received.append(buffer, rc);
			}
			else if (rc == 0)
			{
				break;
			}
		}
		assertTrue (received == data);
	}

	ss1.close();
}


void SecureStreamSocketTest::testHandshakeTimeout()
{
	// A server that does not answer the handshake within the time allowed
	// for connecting: there is no connection then, whatever the server
	// does afterwards.
	HeldBackServer server;
	Thread thread;
	thread.start(server);

	SecureStreamSocket ss;
	std::string outcome("connected");
	try
	{
		ss.connect(SocketAddress("127.0.0.1", server.port()), Poco::Timespan(0, 250000));
	}
	catch (Poco::TimeoutException&)
	{
		outcome = "timeout";
	}
	catch (Poco::Exception& exc)
	{
		outcome = exc.displayText();
	}
	// the server is let go before anything is asserted: its thread uses what is on this stack
	server.proceed();
	ss.close();
	thread.join();
	assertEqual (std::string("timeout"), outcome);
	assertTrue (server.received().empty());
}


void SecureStreamSocketTest::testLazyHandshake()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setLazyHandshake(true);
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));

	// the handshake is made with the first data
	std::string data("hello, world");
	ss.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	ss.close();
}


void SecureStreamSocketTest::testBindClient()
{
	bindAndConnect(SocketAddress("127.0.0.1", 0), false);
}


void SecureStreamSocketTest::testBindClientIPv6()
{
#if defined(POCO_HAVE_IPv6)
	bindAndConnect(SocketAddress("::1", 0), true);
#endif
}


void SecureStreamSocketTest::bindAndConnect(const SocketAddress& loopback, bool reuseAddress)
{
	SecureServerSocket svs(loopback);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.bind(loopback, reuseAddress);
	const SocketAddress boundAddress = ss.address();
	const poco_socket_t boundSocket = ss.impl()->sockfd();
	assertTrue (boundAddress.port() != 0);

	ss.connect(svs.address());
	assertTrue (ss.impl()->sockfd() == boundSocket);
	assertTrue (ss.address() == boundAddress);
	ss.setReceiveTimeout(Poco::Timespan(10, 0));

	std::string data("hello, world");
	ss.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	ss.close();
}


void SecureStreamSocketTest::testDefaultCertificateHandler()
{
	SSLManager& manager = SSLManager::instance();
	const SSLManager::PrivateKeyPassphraseHandlerPtr pPassphraseHandler = manager.clientPassphraseHandler();
	const SSLManager::InvalidCertificateHandlerPtr pConfiguredHandler = manager.clientCertificateHandler();
	const Context::Ptr pContext = manager.defaultClientContext();

	// The configuration file names a handler in a read-only layer, so the layers before the
	// system properties are removed while the SSLManager creates the handler. Both are
	// unlabeled: first the writeable layer of the Application, then the configuration file.
	LayeredConfiguration& config = Application::instance().config();
	const AbstractConfiguration::Ptr pApplicationLayer = config.find("");
	config.removeConfiguration(pApplicationLayer);
	const AbstractConfiguration::Ptr pFileLayer = config.find("");
	config.removeConfiguration(pFileLayer);
	const bool configured = config.has(SSLManager::CFG_CLIENT_PREFIX + "invalidCertificateHandler.name");

	SSLManager::InvalidCertificateHandlerPtr pDefaultHandler;
	manager.initializeClient(pPassphraseHandler, nullptr, pContext);
	try
	{
		pDefaultHandler = manager.clientCertificateHandler();
	}
	catch (Poco::Exception&)
	{
	}
	manager.initializeClient(pPassphraseHandler, pConfiguredHandler, pContext);
	config.add(pFileLayer, Application::PRIO_DEFAULT, false);
	config.add(pApplicationLayer, Application::PRIO_APPLICATION, true);

	assertFalse (configured);
	assertFalse (pDefaultHandler.isNull());
	// a handler that asks on the console would block the thread performing the handshake
	assertNotNullPtr (dynamic_cast<RejectCertificateHandler*>(pDefaultHandler.get()));

	assertTrue (manager.certificateHandlerFactoryMgr().hasFactory("ConsoleCertificateHandler"));
}


void SecureStreamSocketTest::testLoopbackPeerHostName()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	const SocketAddress address("127.0.0.1", svs.address().port());

	// the certificate of the test server is issued by a CA that is not trusted:
	// verifyPeerCertificate() throws unless it leaves the verification out
	const std::vector<std::string> names = {"localhost", "LocalHost", "127.0.0.1", "::1"};
	for (const auto& name: names)
	{
		SecureStreamSocket ss;
		ss.setPeerHostName(name);
		ss.connect(address, Poco::Timespan(10, 0));
		// The handshake accepts the certificate, as the configured AcceptCertificateHandler
		// ignores the errors; this handler is notified after it and makes them fail.
		RejectCertificateHandler rejectHandler(false);
		ss.verifyPeerCertificate();
	}
}


void SecureStreamSocketTest::testTunneledLoopbackPeerIsVerified()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	const SocketAddress address("127.0.0.1", svs.address().port());

	// not tunneled: the peer is on this host, so the verification is left out
	StreamSocket direct;
	direct.connect(address, Poco::Timespan(10, 0));
	SecureStreamSocket ss = SecureStreamSocket::attach(direct, "localhost", SSLManager::instance().defaultClientContext(), nullptr, false);
	{
		// The handshake accepts the certificate, as the configured AcceptCertificateHandler
		// ignores the errors; this handler is notified after it and makes them fail.
		RejectCertificateHandler rejectHandler(false);
		ss.verifyPeerCertificate();
	}
	ss.close();

	// tunneled: the peer address is the one of a proxy and says nothing about
	// the named host, so the certificate is verified
	StreamSocket tunnel;
	tunnel.connect(address, Poco::Timespan(10, 0));
	SecureStreamSocket tunneled = SecureStreamSocket::attach(tunnel, "localhost", SSLManager::instance().defaultClientContext(), nullptr, true);
	RejectCertificateHandler rejectHandler(false);
	try
	{
		// the certificate of the test server (CN=*) matches no host name and is issued by a CA that is not trusted
		tunneled.verifyPeerCertificate();
		fail("certificate not trusted, tunneled connection - must throw");
	}
	catch (Poco::Net::SSLException&)
	{
	}
}


void SecureStreamSocketTest::testResolvedLoopbackNameIsVerified()
{
	// names that a host may resolve to a loopback address without being "localhost"
	const std::vector<std::string> candidates = {"localhost.", "ip6-localhost", "localhost.localdomain", "poco-test.localhost"};
	std::string name;
	for (const auto& candidate: candidates)
	{
		try
		{
			if (SocketAddress(candidate, 0).host().isLoopback())
			{
				name = candidate;
				break;
			}
		}
		catch (Poco::Exception&)
		{
		}
	}
	if (name.empty())
	{
		std::cout << "no name other than localhost resolves to a loopback address; test skipped" << std::endl;
		return;
	}

	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setPeerHostName(name);
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
	RejectCertificateHandler rejectHandler(false);
	try
	{
		// the certificate of the test server (CN=*) is issued by a CA that is not trusted
		ss.verifyPeerCertificate();
		fail("certificate not trusted, name resolves to loopback - must throw");
	}
	catch (Poco::Net::SSLException&)
	{
	}
}


void SecureStreamSocketTest::testLocalhostNameOnOtherAddressIsVerified()
{
	// an address of this host that is not a loopback address and accepts connections
	Poco::Net::IPAddress host;
	for (const auto& interfc: Poco::Net::NetworkInterface::list())
	{
		for (const auto& entry: interfc.addressList())
		{
			const Poco::Net::IPAddress& candidate = entry.get<Poco::Net::NetworkInterface::IP_ADDRESS>();
			if (host.isWildcard() && candidate.family() == Poco::Net::IPAddress::IPv4 && !candidate.isLoopback() && !candidate.isWildcard() && acceptsConnections(candidate))
			{
				host = candidate;
			}
		}
	}
	if (host.isWildcard())
	{
		std::cout << "this host has no IPv4 address other than loopback that accepts connections; test skipped" << std::endl;
		return;
	}

	SecureServerSocket svs(SocketAddress(host, 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setPeerHostName("localhost");
	ss.connect(SocketAddress(host, svs.address().port()), Poco::Timespan(10, 0));
	RejectCertificateHandler rejectHandler(false);
	try
	{
		// the certificate of the test server (CN=*) is issued by a CA that is not trusted
		ss.verifyPeerCertificate();
		fail("certificate not trusted, peer not on loopback - must throw");
	}
	catch (Poco::Net::SSLException&)
	{
	}
}


void SecureStreamSocketTest::setUp()
{
}


void SecureStreamSocketTest::tearDown()
{
}


CppUnit::Test* SecureStreamSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SecureStreamSocketTest");

	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendReceive);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testPeek);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testBufVecAndBufferOverloadsRefused);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testNB);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testHandshakeTimeout);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLazyHandshake);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testBindClient);
#if defined(POCO_HAVE_IPv6)
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testBindClientIPv6);
#endif
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testDefaultCertificateHandler);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLoopbackPeerHostName);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testTunneledLoopbackPeerIsVerified);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testResolvedLoopbackNameIsVerified);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLocalhostNameOnOtherAddressIsVerified);

	return pSuite;
}
