//
// TCPServerTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TCPServerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/RejectCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include <iostream>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::StreamSocket;
using Poco::Net::SecureStreamSocket;
using Poco::Net::SecureServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::Context;
using Poco::Net::Session;
using Poco::Net::SSLManager;
using Poco::Thread;
using Poco::Util::Application;


namespace
{
	static Poco::FastMutex cerrMutex;

	class CollectingConnection: public TCPServerConnection
		/// Reads until the peer closes and stores what arrived, without
		/// ever writing back.
	{
	public:
		CollectingConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			try
			{
				StreamSocket& ss = socket();
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0)
				{
					Poco::FastMutex::ScopedLock l(_mutex);
					_data.append(buffer, n);
					n = ss.receiveBytes(buffer, sizeof(buffer));
				}
			}
			catch (const Poco::Exception& exc)
			{
				Poco::FastMutex::ScopedLock l(_mutex);
				_error = exc.displayText();
			}
		}

		static void reset()
		{
			Poco::FastMutex::ScopedLock l(_mutex);
			_data.clear();
			_error.clear();
		}

		static std::string data()
		{
			Poco::FastMutex::ScopedLock l(_mutex);
			return _data;
		}

		static std::string error()
		{
			Poco::FastMutex::ScopedLock l(_mutex);
			return _error;
		}

	private:
		static Poco::FastMutex _mutex;
		static std::string _data;
		static std::string _error;
	};

	Poco::FastMutex CollectingConnection::_mutex;
	std::string CollectingConnection::_data;
	std::string CollectingConnection::_error;


	class HandshakeOnlyConnection: public TCPServerConnection
		/// Completes the handshake, then shuts down without reading or
		/// writing any application data.
	{
	public:
		HandshakeOnlyConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			try
			{
				SecureStreamSocket sss(socket());
				sss.completeHandshake();
				sss.shutdown();
			}
			catch (const Poco::Exception& exc)
			{
				Poco::FastMutex::ScopedLock l(_mutex);
				_error = exc.displayText();
			}
			Poco::FastMutex::ScopedLock l(_mutex);
			_done = true;
		}

		static void reset()
		{
			Poco::FastMutex::ScopedLock l(_mutex);
			_done = false;
			_error.clear();
		}

		static bool done()
		{
			Poco::FastMutex::ScopedLock l(_mutex);
			return _done;
		}

		static std::string error()
		{
			Poco::FastMutex::ScopedLock l(_mutex);
			return _error;
		}

	private:
		static Poco::FastMutex _mutex;
		static bool _done;
		static std::string _error;
	};

	Poco::FastMutex HandshakeOnlyConnection::_mutex;
	bool HandshakeOnlyConnection::_done = false;
	std::string HandshakeOnlyConnection::_error;


	class EchoConnection: public TCPServerConnection
	{
	public:
		EchoConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			StreamSocket& ss = socket();
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
			catch (const Poco::Exception& exc)
			{
				Poco::FastMutex::ScopedLock l(cerrMutex);

				std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
			}
		}
	};

	class NullConnection: public TCPServerConnection
	{
	public:
		NullConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			SecureStreamSocket ss = socket();
			try
			{
				ss.completeHandshake();
			}
			catch (...)
			{
			}
		}
	};
}


TCPServerTest::TCPServerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TCPServerTest::~TCPServerTest()
{
}


void TCPServerTest::testOneConnection()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.currentThreads() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 1);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testTwoConnections()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	SecureStreamSocket ss2(sa);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	ss2.sendBytes(data.data(), (int) data.size());

	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	assertTrue (srv.currentConnections() == 2);
	assertTrue (srv.currentThreads() == 2);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 2);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.currentThreads() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 2);
	ss2.close();

	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testMultiConnections()
{
	SecureServerSocket svs(0);
	TCPServerParams* pParams = new TCPServerParams;
	pParams->setMaxThreads(4);
	pParams->setMaxQueued(4);
	pParams->setThreadIdleTime(100);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs, pParams);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	SecureStreamSocket ss2(sa);
	SecureStreamSocket ss3(sa);
	SecureStreamSocket ss4(sa);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	ss2.sendBytes(data.data(), (int) data.size());
	ss3.sendBytes(data.data(), (int) data.size());
	ss4.sendBytes(data.data(), (int) data.size());

	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	n = ss2.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	n = ss3.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	n = ss4.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	assertTrue (srv.currentConnections() == 4);
	assertTrue (srv.currentThreads() == 4);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 4);

	SecureStreamSocket ss5;
	ss5.setLazyHandshake();
	ss5.connect(sa);
	Thread::sleep(200);
	assertTrue (srv.queuedConnections() == 1);
	SecureStreamSocket ss6;
	ss6.setLazyHandshake();
	ss6.connect(sa);
	Thread::sleep(200);
	assertTrue (srv.queuedConnections() == 2);

	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 4);
	assertTrue (srv.currentThreads() == 4);
	assertTrue (srv.queuedConnections() == 1);
	assertTrue (srv.totalConnections() == 5);

	ss2.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 4);
	assertTrue (srv.currentThreads() == 4);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 6);

	ss3.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 3);
	assertTrue (srv.currentThreads() == 3);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 6);

	ss4.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 2);
	assertTrue (srv.currentThreads() == 2);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 6);

	ss5.close();
	ss6.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testReuseSocket()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.currentThreads() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 1);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);

	ss1.connect(sa);
	ss1.sendBytes(data.data(), (int) data.size());
	n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 2);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testReuseSession()
{
	// ensure OpenSSL machinery is fully setup
	Context::Ptr pDefaultServerContext = SSLManager::instance().defaultServerContext();
	Context::Ptr pDefaultClientContext = SSLManager::instance().defaultClientContext();

	Context::Ptr pServerContext = new Context(
		Context::SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pServerContext->disableProtocols(Context::PROTO_TLSV1_3);
	pServerContext->enableSessionCache(true, "TestSuite");
	pServerContext->setSessionTimeout(10);
	pServerContext->setSessionCacheSize(1000);
	pServerContext->disableStatelessSessionResumption();

	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	Context::Ptr pClientContext = new Context(
		Context::CLIENT_USE,
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.caConfig"),
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pClientContext->enableSessionCache(true);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa, pClientContext);
	assertTrue (!ss1.sessionWasReused());
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.currentThreads() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 1);

	Session::Ptr pSession = ss1.currentSession();
	if (!pSession || !pSession->isResumable())
	{
		std::cerr << "WARNING: Server did not return a session or session is not resumable. Aborting test." << std::endl;
		return;
	}

	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);

	ss1.useSession(pSession);
	ss1.connect(sa);
	ss1.sendBytes(data.data(), (int) data.size());
	n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (ss1.sessionWasReused());
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 2);
	pSession = ss1.currentSession();
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);

	Thread::sleep(15000); // wait for session to expire
	pServerContext->flushSessionCache();

	ss1.useSession(pSession);
	ss1.connect(sa);
	ss1.sendBytes(data.data(), (int) data.size());
	n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (!ss1.sessionWasReused());
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 3);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testReuseSessionTLS13()
{
	// ensure OpenSSL machinery is fully setup
	Context::Ptr pDefaultServerContext = SSLManager::instance().defaultServerContext();
	Context::Ptr pDefaultClientContext = SSLManager::instance().defaultClientContext();

	Context::Ptr pServerContext = new Context(
		Context::TLS_SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pServerContext->requireMinimumProtocol(Context::PROTO_TLSV1_3);
	pServerContext->enableSessionCache(true, "TestSuite");
	pServerContext->setSessionTimeout(10);
	pServerContext->setSessionCacheSize(1000);

	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	Context::Ptr pClientContext = new Context(
		Context::TLS_CLIENT_USE,
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.caConfig"),
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pClientContext->enableSessionCache(true);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa, pClientContext);
	assertTrue (!ss1.sessionWasReused());
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	char buffer[256];
	// the echo reply also carries the session ticket issued after the handshake
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	Session::Ptr pSession = ss1.currentSession();
	assertTrue (!pSession.isNull());
	// guard against the test silently degrading to the TLS 1.2 ticket path
	assertTrue (SSL_SESSION_get_protocol_version(pSession->sslSession()) == TLS1_3_VERSION);
	assertTrue (pSession->isResumable());
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);

	ss1.useSession(pSession);
	ss1.connect(sa);
	ss1.sendBytes(data.data(), (int) data.size());
	n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (ss1.sessionWasReused());
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testNoSessionTicketsTLS13()
{
	// ensure OpenSSL machinery is fully setup
	Context::Ptr pDefaultServerContext = SSLManager::instance().defaultServerContext();
	Context::Ptr pDefaultClientContext = SSLManager::instance().defaultClientContext();

	// server session cache not enabled: no session tickets are issued
	Context::Ptr pServerContext = new Context(
		Context::TLS_SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pServerContext->requireMinimumProtocol(Context::PROTO_TLSV1_3);

	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	Context::Ptr pClientContext = new Context(
		Context::TLS_CLIENT_USE,
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.caConfig"),
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pClientContext->enableSessionCache(true);

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa, pClientContext);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	// without a server session cache no ticket is issued, so the session
	// exists but cannot be resumed
	Session::Ptr pSession = ss1.currentSession();
	assertTrue (!pSession.isNull());
	assertTrue (!pSession->isResumable());
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);

	ss1.useSession(pSession);
	ss1.connect(sa);
	ss1.sendBytes(data.data(), (int) data.size());
	n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (!ss1.sessionWasReused());
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	ss1.close();
	Thread::sleep(300);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testClientClosesWithoutReadingTLS13()
{
	// A TLS 1.3 client that sends data and closes without reading must not
	// make the server-side handshake fail, and its data must still arrive.
	// The server session cache is enabled, so this covers the connections
	// for which a session ticket is issued.
	Context::Ptr pDefaultServerContext = SSLManager::instance().defaultServerContext();
	Context::Ptr pDefaultClientContext = SSLManager::instance().defaultClientContext();

	Context::Ptr pServerContext = new Context(
		Context::TLS_SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pServerContext->requireMinimumProtocol(Context::PROTO_TLSV1_3);
	pServerContext->enableSessionCache(true, "TestSuite");

	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CollectingConnection>(), svs);
	srv.start();

	Context::Ptr pClientContext = new Context(
		Context::TLS_CLIENT_USE,
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.caConfig"),
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pClientContext->requireMinimumProtocol(Context::PROTO_TLSV1_3);

	CollectingConnection::reset();
	const std::string data("hello, world");
	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa, pClientContext);
	ss1.sendBytes(data.data(), (int) data.size());
	ss1.close();   // close without ever reading

	Poco::Timestamp waitStart;
	while (srv.currentConnections() > 0 && !waitStart.isElapsed(10000000))
		Thread::sleep(100);
	assertTrue (srv.currentConnections() == 0);
	assertTrue (CollectingConnection::data() == data);
	assertTrue (CollectingConnection::error().empty());
}


void TCPServerTest::testShutdownWithoutDataTLS13()
{
	// A server that completes the handshake and shuts down without reading or
	// writing must not fail: requesting a session ticket puts the connection
	// back into the handshake state, and SSL_shutdown() fails while in it.
	Context::Ptr pDefaultServerContext = SSLManager::instance().defaultServerContext();
	Context::Ptr pDefaultClientContext = SSLManager::instance().defaultClientContext();

	Context::Ptr pServerContext = new Context(
		Context::TLS_SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pServerContext->requireMinimumProtocol(Context::PROTO_TLSV1_3);
	pServerContext->enableSessionCache(true, "TestSuite");

	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<HandshakeOnlyConnection>(), svs);
	srv.start();

	Context::Ptr pClientContext = new Context(
		Context::TLS_CLIENT_USE,
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.caConfig"),
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pClientContext->requireMinimumProtocol(Context::PROTO_TLSV1_3);

	HandshakeOnlyConnection::reset();
	SocketAddress sa("127.0.0.1", svs.address().port());
	{
		SecureStreamSocket ss1(sa, pClientContext);
		ss1.sendBytes("x", 1);   // make the server-side handshake complete
		Poco::Timestamp waitStart;
		while (!HandshakeOnlyConnection::done() && !waitStart.isElapsed(10000000))
			Thread::sleep(100);
		ss1.close();
	}

	assertTrue (HandshakeOnlyConnection::done());
	assertTrue (HandshakeOnlyConnection::error().empty());
}


void TCPServerTest::testContextInvalidCertificateHandler()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<NullConnection>(), svs);
	srv.start();

	Context::Ptr pClientContext = new Context(
		Context::CLIENT_USE,
		"",
		"",
		"",
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

	pClientContext->setInvalidCertificateHandler(new Poco::Net::RejectCertificateHandler(false));

	SocketAddress sa("127.0.0.1", svs.address().port());

	try
	{
		SecureStreamSocket ss1(sa, pClientContext);
		fail("must throw with RejectCertificateHandler");
	}
	catch (...)
	{
	}

	pClientContext->setInvalidCertificateHandler(new Poco::Net::AcceptCertificateHandler(false));

	try
	{
		SecureStreamSocket ss1(sa, pClientContext);
	}
	catch (...)
	{
		fail("must not throw with AcceptCertificateHandler");
	}

	srv.stop();
}


void TCPServerTest::testAddCertificateAuthority()
{
	Context::Ptr pServerContext = new Context(
		Context::SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		false,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	// Client context with no CA location and no default CAs
	Context::Ptr pClientContext = new Context(
		Context::CLIENT_USE,
		"",
		"",
		"",
		Context::VERIFY_RELAXED,
		9,
		false,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

	// Add root CA via file-path overload
	std::string rootCertPath = Application::instance().config().getString("openSSL.server.caConfig");
	pClientContext->addCertificateAuthority(rootCertPath);

	pClientContext->setInvalidCertificateHandler(new Poco::Net::AcceptCertificateHandler(false));

	SocketAddress sa("127.0.0.1", svs.address().port());

	SecureStreamSocket ss1(sa, pClientContext);
	std::string data("hello, world");
	ss1.sendBytes(data.data(), (int) data.size());
	char buffer[256];
	int n = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
	ss1.close();
	Thread::sleep(300);

	srv.stop();
}


void TCPServerTest::setUp()
{
}


void TCPServerTest::tearDown()
{
}


CppUnit::Test* TCPServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TCPServerTest");

	CppUnit_addTest(pSuite, TCPServerTest, testOneConnection);
	CppUnit_addTest(pSuite, TCPServerTest, testTwoConnections);
	CppUnit_addTest(pSuite, TCPServerTest, testMultiConnections);
	CppUnit_addTest(pSuite, TCPServerTest, testReuseSocket);
	CppUnit_addTest(pSuite, TCPServerTest, testReuseSession);
	CppUnit_addTest(pSuite, TCPServerTest, testReuseSessionTLS13);
	CppUnit_addTest(pSuite, TCPServerTest, testNoSessionTicketsTLS13);
	CppUnit_addTest(pSuite, TCPServerTest, testClientClosesWithoutReadingTLS13);
	CppUnit_addTest(pSuite, TCPServerTest, testShutdownWithoutDataTLS13);
	CppUnit_addTest(pSuite, TCPServerTest, testContextInvalidCertificateHandler);
	CppUnit_addTest(pSuite, TCPServerTest, testAddCertificateAuthority);

	return pSuite;
}
