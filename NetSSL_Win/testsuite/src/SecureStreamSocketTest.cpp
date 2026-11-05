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
#include "SchannelPeer.h"
#include <functional>
#include <iostream>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::Socket;
using Poco::Net::ServerSocket;
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

	class HandshakeServer: public Poco::Runnable
		/// Takes one connection and makes the TLS handshake with it.
	{
	public:
		HandshakeServer():
			_socket(SocketAddress("127.0.0.1", 0))
		{
		}

		[[nodiscard]] Poco::UInt16 port() const
		{
			return _socket.address().port();
		}

		[[nodiscard]] bool failed() const
			/// Returns true if the handshake ended with an SSLException.
			/// Valid after the thread running the server has been joined.
		{
			return _failed;
		}

		void run() override
		{
			try
			{
				SecureStreamSocket socket(_socket.acceptConnection());
				socket.completeHandshake();
			}
			catch (Poco::Net::SSLException&)
			{
				_failed = true;
			}
			catch (Poco::Exception&)
			{
			}
		}

	private:
		SecureServerSocket _socket;
		bool _failed = false;
	};

	class PlainServer: public Poco::Runnable
		/// Takes one connection and answers the first data with a plain
		/// HTTP response.
	{
	public:
		PlainServer():
			_socket(SocketAddress("127.0.0.1", 0))
		{
		}

		[[nodiscard]] Poco::UInt16 port() const
		{
			return _socket.address().port();
		}

		[[nodiscard]] bool clientDone() const
			/// Returns true if the client closed the connection, or sent
			/// something, within ten seconds of the response.
			/// Valid after the thread running the server has been joined.
		{
			return _clientDone;
		}

		void run() override
		{
			try
			{
				StreamSocket socket = _socket.acceptConnection();
				socket.setReceiveTimeout(Poco::Timespan(10, 0));
				char buffer[1024];
				if (socket.receiveBytes(buffer, sizeof(buffer)) > 0)
				{
					const std::string response("HTTP/1.1 400 Bad Request\r\n\r\n");
					socket.sendBytes(response.data(), static_cast<int>(response.size()));
					_clientDone = socket.poll(Poco::Timespan(10, 0), Socket::SELECT_READ);
				}
			}
			catch (Poco::Exception&)
			{
			}
		}

	private:
		Poco::Net::ServerSocket _socket;
		bool _clientDone = false;
	};

	class SchannelServer: public Poco::Runnable
		/// Accepts one connection on a plain socket and runs a scenario on it with a
		/// SchannelPeer in the server role, in a thread of its own. The destructor
		/// waits for the thread, so that a test that ends early does not leave the
		/// scenario running on a destroyed object.
	{
	public:
		using Scenario = std::function<void(SchannelPeer&, StreamSocket&)>;

		explicit SchannelServer(const Scenario& scenario):
			_socket(SocketAddress("127.0.0.1", 0)),
			_scenario(scenario)
		{
			_thread.start(*this);
		}

		~SchannelServer() override
		{
			try
			{
				join();
			}
			catch (...)
			{
			}
		}

		Poco::UInt16 port() const
		{
			return _socket.address().port();
		}

		void join()
			/// Waits for the scenario to end.
		{
			if (!_joined)
			{
				_thread.join();
				_joined = true;
			}
		}

		const std::string& failure() const
			/// Returns the text of the exception that ended the scenario,
			/// or an empty string. Valid after join().
		{
			return _failure;
		}

		void run() override
		{
			try
			{
				if (!_socket.poll(Poco::Timespan(10, 0), Socket::SELECT_READ))
				{
					_failure = "no connection within 10 s";
					return;
				}
				StreamSocket socket = _socket.acceptConnection();
				socket.setReceiveTimeout(Poco::Timespan(10, 0));
				SchannelPeer peer(SchannelPeer::SERVER, socket, SSLManager::instance().defaultServerContext());
				_scenario(peer, socket);
			}
			catch (Poco::Exception& exc)
			{
				_failure = exc.displayText();
			}
			catch (std::exception& exc)
			{
				_failure = exc.what();
			}
		}

	private:
		ServerSocket _socket;
		Scenario _scenario;
		Thread _thread;
		bool _joined = false;
		std::string _failure;
	};

	void expectReceived(SchannelPeer& peer, const std::string& expected)
		/// Receives the next application data from the peer's partner and throws
		/// if it is not the expected text. An empty text expects the end of the stream.
	{
		const std::string received = peer.receive();
		if (received != expected)
			throw Poco::DataException("received '" + received + "' instead of '" + expected + "'");
	}

	std::string sessionTicket(SchannelPeer& peer)
		/// Runs the handshake and returns the session ticket that a TLS 1.3 server
		/// sends at its end, without sending it.
	{
		const std::string ticket = peer.handshake(false);
		if (ticket.empty())
			throw Poco::IllegalStateException("the handshake produced no session ticket");
		return ticket;
	}
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


void SecureStreamSocketTest::testServerDropsPlainClient()
{
	HandshakeServer server;
	Thread thread;
	thread.start(server);

	// the start of a plain HTTP request reads like the header of a record of 8239 bytes
	StreamSocket client(SocketAddress("127.0.0.1", server.port()));
	const std::string request("GET / HTTP/1.1\r\n\r\n");
	client.sendBytes(request.data(), static_cast<int>(request.size()));
	// the server closes the connection when its handshake has ended
	const bool dropped = client.poll(Poco::Timespan(10, 0), Socket::SELECT_READ);
	client.close();
	thread.join();
	assertTrue (dropped);
	assertTrue (server.failed());
}


void SecureStreamSocketTest::testClientLeavesPlainServer()
{
	PlainServer server;
	Thread thread;
	thread.start(server);

	// the server keeps the connection open after its response
	SecureStreamSocket client;
	bool failed = false;
	try
	{
		client.connect(SocketAddress("127.0.0.1", server.port()));
	}
	catch (Poco::Net::SSLException&)
	{
		failed = true;
	}
	client.close();
	thread.join();
	assertTrue (failed);
	assertTrue (server.clientDone());
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


void SecureStreamSocketTest::testDataWithCloseNotify()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket& socket)
	{
		peer.handshake();
		// the operands of + are evaluated in no defined order, the records must be made in this one
		const std::string data = peer.encrypt("one");
		peer.send(data + peer.closeNotify());
		socket.shutdownSend();
		// the connection stays open until the client's close_notify
		expectReceived(peer, "");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	char buffer[256];
	const int n = ss.receiveBytes(buffer, sizeof(buffer));
	const std::string data(buffer, n > 0 ? n : 0);
	const int end = ss.receiveBytes(buffer, sizeof(buffer));
	const int closeNotifyReceived = ss.shutdown();
	server.join();
	assertEqual (std::string(), server.failure());
	assertEqual (3, n);
	assertEqual ("one", data);
	assertEqual (0, end);
	assertEqual (1, closeNotifyReceived);
}


void SecureStreamSocketTest::testCloseNotifyAlone()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket& socket)
	{
		peer.handshake();
		peer.send(peer.closeNotify());
		socket.shutdownSend();
		expectReceived(peer, "");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	char buffer[256];
	const int end = ss.receiveBytes(buffer, sizeof(buffer));
	const int closeNotifyReceived = ss.shutdown();
	server.join();
	assertEqual (std::string(), server.failure());
	assertEqual (0, end);
	assertEqual (1, closeNotifyReceived);
}


void SecureStreamSocketTest::testReceiveAfterShutdown()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket&)
	{
		peer.handshake();
		expectReceived(peer, "data");
		// the operands of + are evaluated in no defined order, the records must be made in this one
		const std::string reply = peer.encrypt("reply");
		peer.send(reply + peer.closeNotify());
		expectReceived(peer, "");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	const int sent = ss.sendBytes("data", 4);
	// the client's close_notify goes out before the reply is read
	ss.shutdown();
	std::string received;
	std::string failure;
	int n = 0;
	try
	{
		do
		{
			char buffer[256];
			n = ss.receiveBytes(buffer, sizeof(buffer));
			if (n > 0)
				received.append(buffer, n);
		}
		while (n > 0);
	}
	catch (Poco::Exception& exc)
	{
		failure = exc.displayText();
	}
	const int closeNotifyReceived = ss.shutdown();
	server.join();
	assertEqual (std::string(), server.failure());
	assertEqual (4, sent);
	assertEqual (std::string(), failure);
	assertEqual ("reply", received);
	assertEqual (0, n);
	assertEqual (1, closeNotifyReceived);
}


void SecureStreamSocketTest::testNonBlockingReceiveOfTicketWithData()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket&)
	{
		const std::string ticket = sessionTicket(peer);
		peer.send(ticket + peer.encrypt("one"));
		expectReceived(peer, "done");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	ss.setBlocking(false);
	const bool readable = ss.poll(Poco::Timespan(10, 0), Socket::SELECT_READ);
	// the ticket is processed without waiting for the socket again
	char buffer[256];
	const int n = ss.receiveBytes(buffer, sizeof(buffer));
	const std::string data(buffer, n > 0 ? n : 0);
	ss.setBlocking(true);
	const int sent = ss.sendBytes("done", 4);
	server.join();
	assertEqual (std::string(), server.failure());
	assertTrue (readable);
	assertEqual (3, n);
	assertEqual ("one", data);
	assertEqual (4, sent);
}


void SecureStreamSocketTest::testEmptyRecordIsNotEndOfStream()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket&)
	{
		peer.send(sessionTicket(peer));
		expectReceived(peer, "go");
		peer.send(peer.encrypt(""));
		expectReceived(peer, "go");
		peer.send(peer.encrypt("two"));
		expectReceived(peer, "done");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	ss.setBlocking(false);
	char buffer[256];
	const bool ticketReadable = ss.poll(Poco::Timespan(10, 0), Socket::SELECT_READ);
	const int ticketResult = ss.receiveBytes(buffer, sizeof(buffer));
	const int sentAfterTicket = ss.sendBytes("go", 2);
	const bool emptyRecordReadable = ss.poll(Poco::Timespan(10, 0), Socket::SELECT_READ);
	const int emptyRecordResult = ss.receiveBytes(buffer, sizeof(buffer));
	const int sentAfterEmptyRecord = ss.sendBytes("go", 2);
	int n = SecureStreamSocket::ERR_SSL_WOULD_BLOCK;
	while (n < 0 && ss.poll(Poco::Timespan(10, 0), Socket::SELECT_READ))
	{
		n = ss.receiveBytes(buffer, sizeof(buffer));
	}
	const std::string data(buffer, n > 0 ? n : 0);
	const int sentDone = ss.sendBytes("done", 4);
	server.join();
	assertEqual (std::string(), server.failure());
	assertTrue (ticketReadable);
	assertTrue (ticketResult < 0);
	assertEqual (2, sentAfterTicket);
	assertTrue (emptyRecordReadable);
	// an empty record is no end of the stream
	assertTrue (emptyRecordResult != 0);
	assertEqual (2, sentAfterEmptyRecord);
	assertEqual ("two", data);
	assertEqual (4, sentDone);
}


void SecureStreamSocketTest::testOversizedRecordThrows()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket&)
	{
		peer.handshake();
		// a record header that announces more than the client's receive buffer holds, and the bytes to fill it
		const int length = 18000;
		std::string record("\x17\x03\x03", 3);
		record += static_cast<char>(length >> 8);
		record += static_cast<char>(length & 0xFF);
		record.append(17000, 'x');
		peer.send(record);
		expectReceived(peer, "");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	std::string outcome;
	try
	{
		char buffer[256];
		outcome = "receiveBytes() returned " + std::to_string(ss.receiveBytes(buffer, sizeof(buffer)));
	}
	catch (Poco::Net::SSLException&)
	{
		outcome = "SSLException";
	}
	catch (Poco::TimeoutException&)
	{
		outcome = "timeout";
	}
	ss.close();
	server.join();
	assertEqual (std::string(), server.failure());
	assertEqual ("SSLException", outcome);
}


void SecureStreamSocketTest::testCoalescedTicketAndData()
{
	SchannelServer server([](SchannelPeer& peer, StreamSocket&)
	{
		const std::string ticket = sessionTicket(peer);
		const std::string one = peer.encrypt("one");
		peer.send(ticket + one + peer.encrypt("two"));
		expectReceived(peer, "done");
	});
	SecureStreamSocket ss(SocketAddress("127.0.0.1", server.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	std::string received;
	int n = 0;
	do
	{
		char buffer[256];
		n = ss.receiveBytes(buffer, sizeof(buffer));
		if (n > 0)
			received.append(buffer, n);
	}
	while (n > 0 && received.size() < 6);
	const int sent = ss.sendBytes("done", 4);
	server.join();
	assertEqual (std::string(), server.failure());
	assertTrue (n > 0);
	assertEqual ("onetwo", received);
	assertEqual (4, sent);
}


void SecureStreamSocketTest::testReconnectAfterCloseNotify()
{
	SchannelServer first([](SchannelPeer& peer, StreamSocket&)
	{
		peer.handshake();
		peer.send(peer.closeNotify());
		expectReceived(peer, "");
	});
	SchannelServer second([](SchannelPeer& peer, StreamSocket&)
	{
		peer.handshake();
		peer.send(peer.encrypt("two"));
		expectReceived(peer, "");
	});
	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", first.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	char buffer[256];
	const int end = ss.receiveBytes(buffer, sizeof(buffer));
	ss.close();
	// the second connection is made by the same socket implementation, as HTTPSClientSession does
	ss.connect(SocketAddress("127.0.0.1", second.port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	const int n = ss.receiveBytes(buffer, sizeof(buffer));
	const std::string data(buffer, n > 0 ? n : 0);
	ss.close();
	first.join();
	second.join();
	assertEqual (std::string(), first.failure());
	assertEqual (std::string(), second.failure());
	assertEqual (0, end);
	assertEqual (3, n);
	assertEqual ("two", data);
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
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testServerDropsPlainClient);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testClientLeavesPlainServer);
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
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testDataWithCloseNotify);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testCloseNotifyAlone);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testReceiveAfterShutdown);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testNonBlockingReceiveOfTicketWithData);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testEmptyRecordIsNotEndOfStream);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testOversizedRecordThrows);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testCoalescedTicketAndData);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testReconnectAfterCloseNotify);

	return pSuite;
}
