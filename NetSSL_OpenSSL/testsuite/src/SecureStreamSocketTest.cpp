//
// SecureStreamSocketTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SecureStreamSocketTest.h"
#include "ErrorQueueCleaner.h"
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
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <iostream>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SecureStreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SecureServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::Context;
using Poco::Net::Session;
using Poco::Net::SSLManager;
using Poco::Thread;
using Poco::Util::Application;


namespace
{
#ifdef POCO_NET_HAS_INTERFACE
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
#endif

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
			catch (Poco::Exception& exc)
			{
				std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
			}
		}
	};

	class CopyToStringConnection: public TCPServerConnection
	{
	public:
		CopyToStringConnection(const StreamSocket& s): 
			TCPServerConnection(s)
		{
		}

		void run()
		{
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				_data.clear();
			}
			StreamSocket& ss = socket();
			try
			{
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0)
				{
					{
						Poco::FastMutex::ScopedLock lock(_mutex);
						_data.append(buffer, n);
					}
					n = ss.receiveBytes(buffer, sizeof(buffer));
				}
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "CopyToStringConnection: " << exc.displayText() << std::endl;
			}
		}

		static const std::string data()
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _data;
		}

	private:
		static Poco::FastMutex _mutex;
		static std::string _data;
	};

	Poco::FastMutex CopyToStringConnection::_mutex;
	std::string CopyToStringConnection::_data;

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

	class DripFeeder: public Poco::Runnable
		/// Sends the header of a TLS handshake record of the largest size,
		/// then its content one byte at a time, each byte within the
		/// receive timeout of the peer. Stops after a limited time, so that
		/// a peer that waits for the whole record does not wait forever.
	{
	public:
		explicit DripFeeder(const StreamSocket& socket):
			_socket(socket),
			_accept(false)
		{
		}

		explicit DripFeeder(const ServerSocket& server):
			_server(server),
			_accept(true)
		{
		}

		void stop()
		{
			_stop.set();
		}

		void run() override
		{
			try
			{
				if (_accept)
				{
					if (!_server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ))
						return;
					_socket = _server.acceptConnection();
				}
				const unsigned char header[] = {0x16, 0x03, 0x01, 0x40, 0x00};
				_socket.sendBytes(header, sizeof(header));
				const char content = 0;
				const Poco::Timespan limit(6, 0);
				Poco::Timestamp start;
				while (!start.isElapsed(limit.totalMicroseconds()) && !_stop.tryWait(250))
				{
					_socket.sendBytes(&content, 1);
				}
				_socket.close();
			}
			catch (Poco::Exception&)
			{
			}
		}

	private:
		ServerSocket _server;
		StreamSocket _socket;
		bool _accept;
		Poco::Event _stop;
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


void SecureStreamSocketTest::testLazyHandshake()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setLazyHandshake(true);
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()));

	// the handshake is made with the first data
	std::string data("hello, world");
	ss.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	ss.close();
}


void SecureStreamSocketTest::testSendFile()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	std::string sentData = "Hello, world!";

	Poco::TemporaryFile file;
	Poco::FileOutputStream ostr(file.path());
	ostr.write(sentData.data(), sentData.size());
	ostr.close();
	
	Poco::FileInputStream istr(file.path());
	std::streamsize n = ss.sendFile(istr);
	assertTrue (n == file.getSize());

	istr.close();
	ss.close();

	Poco::Thread::sleep(200);
	Poco::Timestamp waitStart;
	while (srv.currentConnections() > 0 && !waitStart.isElapsed(10000000)) // 10s
	{
		Poco::Thread::sleep(100);
	}
	assertTrue(srv.currentConnections() == 0);
	srv.stop();

	assertTrue (CopyToStringConnection::data() == sentData);
}


void SecureStreamSocketTest::testSendFileLarge()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	std::string sentData;

	Poco::TemporaryFile file;
	Poco::FileOutputStream ostr(file.path());
	std::string data("0123456789abcdef");
	for (int i = 0; i < 10000; i++)
	{
		ostr.write(data.data(), data.size());
		sentData += data;
	}
	ostr.close();
	
	Poco::FileInputStream istr(file.path());
	std::streamsize n = ss.sendFile(istr);
	assertTrue (n == file.getSize());

	istr.close();
	ss.close();

	Poco::Thread::sleep(200);
	Poco::Timestamp waitStart;
	while (srv.currentConnections() > 0 && !waitStart.isElapsed(10000000)) // 10s
	{
		Poco::Thread::sleep(100);
	}
	assertTrue(srv.currentConnections() == 0);
	srv.stop();

	assertTrue (CopyToStringConnection::data() == sentData);
}


void SecureStreamSocketTest::testSendFileRange()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	std::string fileData;

	Poco::TemporaryFile file;
	Poco::FileOutputStream ostr(file.path());
	std::string data("0123456789abcdef");
	for (int i = 0; i < 1024; i++)
	{
		ostr.write(data.data(), data.size());
		fileData += data;
	}
	ostr.close();
	
	const std::streamoff offset = 4000;
	const std::streamsize count = 10000;

	Poco::FileInputStream istr(file.path());
	std::streamsize n = ss.sendFile(istr, offset, count);
	assertTrue (n == count);

	istr.close();
	ss.close();

	Poco::Thread::sleep(200);
	Poco::Timestamp waitStart;
	while (srv.currentConnections() > 0 && !waitStart.isElapsed(10000000)) // 10s
	{
		Poco::Thread::sleep(100);
	}
	assertTrue(srv.currentConnections() == 0);
	srv.stop();

	assertTrue (CopyToStringConnection::data() == fileData.substr(offset, count));
}


void SecureStreamSocketTest::testShutdownBidirectional()
{
	// Test that bidirectional shutdown ensures all data is received
	// by the peer before closing (regression test for GH #4883).
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<CopyToStringConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.connect(SocketAddress("127.0.0.1", srv.port()));

	const int chunkSize = 1024;
	const int chunkCount = 100;
	std::string sentData(chunkSize * chunkCount, 'A');
	for (int i = 0; i < chunkCount; i++)
	{
		const char* p = sentData.data() + i * chunkSize;
		int remaining = chunkSize;
		while (remaining > 0)
		{
			int n = ss.sendBytes(p, remaining);
			if (n <= 0) break;
			p += n;
			remaining -= n;
		}
	}

	// Close immediately after sending -- with bidirectional shutdown,
	// the receiver should still get all data.
	ss.close();

	Poco::Thread::sleep(200);
	Poco::Timestamp waitStart;
	while (srv.currentConnections() > 0 && !waitStart.isElapsed(10000000)) // 10s
	{
		Poco::Thread::sleep(100);
	}
	assertTrue(srv.currentConnections() == 0);
	srv.stop();

	assertTrue (CopyToStringConnection::data() == sentData);
}


void SecureStreamSocketTest::testPeerHostNameTooLong()
{
	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	// no certificate verification: only the SNI setup can reject this connection
	Context::Ptr pContext = new Context(Context::TLS_CLIENT_USE, "", Context::VERIFY_NONE);
	SecureStreamSocket ss(pContext);
	ss.setPeerHostName(std::string(256, 'a'));
	try
	{
		ss.connect(SocketAddress("127.0.0.1", svs.address().port()));
		fail("host name too long for SNI - must throw");
	}
	catch (Poco::Net::SSLException&)
	{
	}
}


void SecureStreamSocketTest::testStaleErrorQueue()
{
	ErrorQueueCleaner cleaner;

	SecureServerSocket svs(0);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SocketAddress sa("127.0.0.1", svs.address().port());
	SecureStreamSocket ss1(sa);
	ss1.setBlocking(false);

	// an unrelated failure leaves an entry in this thread's OpenSSL error queue
	BIO* pBIO = BIO_new(BIO_s_mem());
	assertNotNullPtr (pBIO);
	assertNullPtr (PEM_read_bio_X509(pBIO, nullptr, nullptr, nullptr));
	BIO_free(pBIO);
	assertTrue (ERR_peek_error() != 0);

	char buffer[256];
	int rc = ss1.receiveBytes(buffer, sizeof(buffer));
	assertEqual (static_cast<int>(SecureStreamSocket::ERR_SSL_WANT_READ), rc);
	assertTrue (ERR_peek_error() == 0);

	ss1.setBlocking(true);
	ss1.setReceiveTimeout(Poco::Timespan(10, 0));
	std::string data("hello, world");
	ss1.sendBytes(data.data(), static_cast<int>(data.size()));
	rc = ss1.receiveBytes(buffer, sizeof(buffer));
	assertTrue (std::string(buffer, rc) == data);

	ss1.close();
}


void SecureStreamSocketTest::testHandshakeTimeout()
{
	// The certificate of the test servers is not one for this name, and no
	// name service is needed to tell the name from the local host.
	const std::string otherName("192.0.2.1");

	{
		SecureServerSocket svs(0);
		TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
		srv.start();

		SecureStreamSocket ss;
		ss.setPeerHostName(otherName);
		try
		{
			ss.connect(SocketAddress("127.0.0.1", svs.address().port()));
			fail("certificate for another host - must throw");
		}
		catch (Poco::Net::CertificateValidationException&)
		{
		}
	}

	// A server that does not answer the handshake within the time allowed
	// for connecting: there is no connection then, whatever the server
	// does afterwards.
	HeldBackServer server;
	Thread thread;
	thread.start(server);

	SecureStreamSocket ss;
	ss.setPeerHostName(otherName);
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
	// the time allowed for connecting does not stay on the socket
	Poco::Timespan timeoutLeft(1, 0);
	try
	{
		timeoutLeft = ss.getReceiveTimeout();
	}
	catch (Poco::Exception&)
	{
	}
	// the server is let go before anything is asserted: its thread uses what is on this stack
	server.proceed();
	ss.close();
	thread.join();
	assertEqual (std::string("timeout"), outcome);
	assertTrue (timeoutLeft == Poco::Timespan());
	assertTrue (server.received().empty());
}


void SecureStreamSocketTest::testDefaultCipherListRefusesAnonymousServer()
{
	ErrorQueueCleaner cleaner;

	// a server without a certificate: it can only agree on a cipher suite without authentication,
	// which OpenSSL allows at security level 0 only
	Context::Params serverParams;
	serverParams.verificationMode = Context::VERIFY_NONE;
	serverParams.securityLevel = Context::SECURITY_LEVEL_NONE;
	serverParams.cipherList = "AECDH-AES256-SHA";
	Context::Ptr pServerContext = new Context(Context::TLS_SERVER_USE, serverParams);
	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	// the server does complete a handshake over that cipher suite: it is the client that refuses below
	Context::Params anonymousParams;
	anonymousParams.verificationMode = Context::VERIFY_NONE;
	anonymousParams.securityLevel = Context::SECURITY_LEVEL_NONE;
	anonymousParams.cipherList = serverParams.cipherList;
	Context::Ptr pAnonymousContext = new Context(Context::TLS_CLIENT_USE, anonymousParams);
	SecureStreamSocket anonymous(pAnonymousContext);
	anonymous.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
	anonymous.setReceiveTimeout(Poco::Timespan(10, 0));
	const std::string data("hello, world");
	anonymous.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	const int n = anonymous.receiveBytes(buffer, sizeof(buffer));
	assertTrue (std::string(buffer, n) == data);
	anonymous.close();

	// a client that accepts any server at any security level: only the cipher list can refuse
	Context::Params clientParams;
	clientParams.verificationMode = Context::VERIFY_NONE;
	clientParams.securityLevel = Context::SECURITY_LEVEL_NONE;
	Context::Ptr pClientContext = new Context(Context::TLS_CLIENT_USE, clientParams);
	SecureStreamSocket ss(pClientContext);
	try
	{
		ss.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
		fail("server without a certificate - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
		fail("the cipher list, not the certificate verification, must refuse the server");
	}
	catch (Poco::Net::SSLException&)
	{
	}
}


void SecureStreamSocketTest::testClientRejectsAnonymousServer()
{
	ErrorQueueCleaner cleaner;

	const std::string anonymousSuite("AECDH-AES256-SHA");

	// a server without a certificate: it can only agree on a cipher suite without authentication,
	// which OpenSSL allows at security level 0 only
	Context::Params serverParams;
	serverParams.verificationMode = Context::VERIFY_NONE;
	serverParams.securityLevel = Context::SECURITY_LEVEL_NONE;
	serverParams.cipherList = anonymousSuite;
	Context::Ptr pServerContext = new Context(Context::TLS_SERVER_USE, serverParams);
	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	const SocketAddress address("127.0.0.1", svs.address().port());
	const Poco::Timespan timeout(10, 0);

	// the client allows the suite, so that the handshake itself succeeds
	auto clientContext = [&anonymousSuite](Context::VerificationMode mode)
	{
		Context::Params params;
		params.verificationMode = mode;
		params.securityLevel = Context::SECURITY_LEVEL_NONE;
		params.cipherList = anonymousSuite;
		Context::Ptr pContext = new Context(Context::TLS_CLIENT_USE, params);
		pContext->disableProtocols(Context::PROTO_TLSV1_3);
		return pContext;
	};

	{
		SecureStreamSocket ss(clientContext(Context::VERIFY_NONE));
		ss.connect(address, timeout);
		assertFalse (ss.havePeerCertificate());
	}

	try
	{
		SecureStreamSocket ss(clientContext(Context::VERIFY_STRICT));
		ss.connect(address, timeout);
		fail("strict verification, server without a certificate - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}

	try
	{
		// not a loopback name: relaxed verification makes no exception for it
		SecureStreamSocket ss(clientContext(Context::VERIFY_RELAXED));
		ss.setPeerHostName("192.0.2.1");
		ss.connect(address, timeout);
		fail("relaxed verification, server without a certificate - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}

	try
	{
		Context::Ptr pContext = clientContext(Context::VERIFY_RELAXED);
		pContext->enableExtendedCertificateVerification(false);
		SecureStreamSocket ss(pContext);
		ss.connect(address, timeout);
		fail("no extended verification, server without a certificate - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}
}


void SecureStreamSocketTest::testRelaxedServerAcceptsClientWithoutCertificate()
{
	ErrorQueueCleaner cleaner;

	// the server asks for a client certificate and does not require one
	Context::Params serverParams;
	serverParams.privateKeyFile = Application::instance().config().getString("openSSL.server.privateKeyFile");
	serverParams.certificateFile = serverParams.privateKeyFile;
	serverParams.caLocation = Application::instance().config().getString("openSSL.server.caConfig");
	serverParams.verificationMode = Context::VERIFY_RELAXED;
	Context::Ptr pServerContext = new Context(Context::TLS_SERVER_USE, serverParams);
	SecureServerSocket svs(0, 64, pServerContext);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	Context::Params clientParams;
	clientParams.verificationMode = Context::VERIFY_NONE;
	Context::Ptr pClientContext = new Context(Context::TLS_CLIENT_USE, clientParams);
	SecureStreamSocket ss(pClientContext);
	const Poco::Timespan timeout(10, 0);
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()), timeout);
	ss.setReceiveTimeout(timeout);
	const std::string data("hello, world");
	ss.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	const int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);
}


void SecureStreamSocketTest::testLoopbackPeerHostName()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	const SocketAddress address("127.0.0.1", svs.address().port());

	// the certificate of the test server is not one for any of these names
	const std::vector<std::string> names = {"localhost", "LocalHost", "127.0.0.1", "::1"};
	for (const auto& name: names)
	{
		SecureStreamSocket ss;
		ss.setPeerHostName(name);
		ss.connect(address, Poco::Timespan(10, 0));
		ss.setReceiveTimeout(Poco::Timespan(10, 0));
		std::string data("hello, world");
		ss.sendBytes(data.data(), static_cast<int>(data.size()));
		char buffer[256];
		int n = ss.receiveBytes(buffer, sizeof(buffer));
		assertTrue (n > 0);
		assertTrue (std::string(buffer, n) == data);
		ss.close();
	}
}


void SecureStreamSocketTest::testTunneledLoopbackPeerIsVerified()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();
	const SocketAddress address("127.0.0.1", svs.address().port());

	// not tunneled: the peer is on this host, so the host name check is left out
	StreamSocket direct;
	direct.connect(address, Poco::Timespan(10, 0));
	SecureStreamSocket ss = SecureStreamSocket::attach(direct, "localhost", SSLManager::instance().defaultClientContext(), nullptr, false);
	ss.close();

	// tunneled: the peer address is that of a proxy, so the host name is checked
	StreamSocket tunnel;
	tunnel.connect(address, Poco::Timespan(10, 0));
	try
	{
		SecureStreamSocket tunneled = SecureStreamSocket::attach(tunnel, "localhost", SSLManager::instance().defaultClientContext(), nullptr, true);
		fail("certificate for another host, tunneled connection - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}
}


void SecureStreamSocketTest::testResolvedLoopbackNameIsVerified()
{
	// names that a host may resolve to a loopback address without being "localhost"
	const std::vector<std::string> candidates = {"ip6-localhost", "localhost.localdomain", "poco-test.localhost"};
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
	try
	{
		ss.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
		fail("certificate for another host, name resolves to loopback - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}
}


void SecureStreamSocketTest::testLocalhostNameOnOtherAddressIsVerified()
{
#ifdef POCO_NET_HAS_INTERFACE
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
	try
	{
		ss.connect(SocketAddress(host, svs.address().port()), Poco::Timespan(10, 0));
		fail("certificate for another host, peer not on loopback - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}
#else
	std::cout << "no network interface detection on this platform; test skipped" << std::endl;
#endif
}


void SecureStreamSocketTest::testCompleteHandshakeVerifiesPeer()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setLazyHandshake(true);
	// the certificate of the test server is not one for this name
	ss.setPeerHostName("192.0.2.1");
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	try
	{
		ss.completeHandshake();
		fail("certificate for another host - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}

	// the failed check does not leave a connection that can be used
	try
	{
		ss.sendBytes("hello", 5);
		fail("certificate for another host, send after the failed handshake - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}
}


void SecureStreamSocketTest::testCompleteHandshakeVerifiesPeerNB()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	// the certificate of the test server is not one for this name
	ss.setPeerHostName("192.0.2.1");
	ss.connectNB(SocketAddress("127.0.0.1", svs.address().port()));

	const Poco::Timespan timeout(10, 0);
	assertTrue (ss.poll(timeout, Socket::SELECT_WRITE));
	Poco::Timestamp start;
	int rc = SecureStreamSocket::ERR_SSL_WANT_WRITE;
	try
	{
		while (rc < 0 && !start.isElapsed(timeout.totalMicroseconds()))
		{
			const int mode = rc == SecureStreamSocket::ERR_SSL_WANT_WRITE ? Socket::SELECT_WRITE : Socket::SELECT_READ;
			ss.poll(Poco::Timespan(0, 100000), mode);
			rc = ss.completeHandshake();
		}
		fail("certificate for another host - must throw");
	}
	catch (Poco::Net::CertificateValidationException&)
	{
	}
}


void SecureStreamSocketTest::testCompleteHandshakeAcceptsPeer()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setLazyHandshake(true);
	// the name in the certificate of the test server
	ss.setPeerHostName("www.appinf.com");
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	assertEqual (1, ss.completeHandshake());

	std::string data("hello, world");
	ss.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	ss.close();
}


void SecureStreamSocketTest::testServerCompleteHandshakeVerifiesClientCertificate()
{
	ErrorQueueCleaner cleaner;

	// the server requires a client certificate and completes the handshake itself
	Context::Params serverParams;
	serverParams.privateKeyFile = Application::instance().config().getString("openSSL.server.privateKeyFile");
	serverParams.certificateFile = serverParams.privateKeyFile;
	// the certificate of the test client is self-signed
	serverParams.caLocation = Application::instance().config().getString("openSSL.client.privateKeyFile");
	serverParams.verificationMode = Context::VERIFY_STRICT;
	Context::Ptr pServerContext = new Context(Context::TLS_SERVER_USE, serverParams);
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0), 64, pServerContext);

	// the certificate of the test client is not one for its IP address
	SecureStreamSocket client;
	client.connectNB(svs.address());

	const Poco::Timespan timeout(10, 0);
	assertTrue (svs.poll(timeout, Socket::SELECT_READ));
	SecureStreamSocket server(svs.acceptConnection());
	server.setBlocking(false);
	assertTrue (client.poll(timeout, Socket::SELECT_WRITE));

	Poco::Timestamp start;
	int clientRc = SecureStreamSocket::ERR_SSL_WANT_WRITE;
	int serverRc = SecureStreamSocket::ERR_SSL_WANT_READ;
	bool refused = false;
	while (!refused && serverRc < 0 && !start.isElapsed(timeout.totalMicroseconds()))
	{
		// the client accepts the server: an exception of the client is an error of the test
		if (clientRc < 0) clientRc = client.completeHandshake();
		server.poll(Poco::Timespan(0, 100000), Socket::SELECT_READ);
		try
		{
			serverRc = server.completeHandshake();
		}
		catch (Poco::Net::CertificateValidationException&)
		{
			refused = true;
		}
	}
	if (!refused) fail("client certificate for another host - must throw");
}


void SecureStreamSocketTest::testServerHandshakeDripFeed()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	StreamSocket client;
	client.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
	assertTrue (svs.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	// the handshake of an accepted socket is made with its first data
	StreamSocket socket = svs.acceptConnection();
	socket.setReceiveTimeout(Poco::Timespan(1, 0));

	DripFeeder feeder(client);
	Thread thread;
	thread.start(feeder);

	std::string outcome("received");
	Poco::Timestamp start;
	try
	{
		char buffer[256];
		socket.receiveBytes(buffer, sizeof(buffer));
	}
	catch (Poco::TimeoutException&)
	{
		outcome = "timeout";
	}
	catch (Poco::Exception& exc)
	{
		outcome = exc.displayText();
	}
	const Poco::Timespan elapsed = start.elapsed();
	const bool blocking = socket.getBlocking();
	feeder.stop();
	thread.join();
	assertEqual (std::string("timeout"), outcome);
	// the receive timeout limits the handshake, not the wait for the next byte
	assertTrue (elapsed < Poco::Timespan(4, 0));
	assertTrue (blocking);
}


void SecureStreamSocketTest::testClientHandshakeDripFeed()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	DripFeeder feeder(server);
	Thread thread;
	thread.start(feeder);

	SecureStreamSocket ss;
	std::string outcome("connected");
	Poco::Timestamp start;
	try
	{
		ss.connect(SocketAddress("127.0.0.1", server.address().port()), Poco::Timespan(1, 0));
	}
	catch (Poco::TimeoutException&)
	{
		outcome = "timeout";
	}
	catch (Poco::Exception& exc)
	{
		outcome = exc.displayText();
	}
	const Poco::Timespan elapsed = start.elapsed();
	const bool blocking = ss.getBlocking();
	feeder.stop();
	thread.join();
	assertEqual (std::string("timeout"), outcome);
	// the time allowed for connecting limits the handshake, not the wait for the next byte
	assertTrue (elapsed < Poco::Timespan(4, 0));
	assertTrue (blocking);
}


void SecureStreamSocketTest::testHandshakeLeavesSocketBlocking()
{
	SecureServerSocket svs(SocketAddress("127.0.0.1", 0));
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	SecureStreamSocket ss;
	ss.setLazyHandshake(true);
	ss.connect(SocketAddress("127.0.0.1", svs.address().port()), Poco::Timespan(10, 0));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	assertEqual (1, ss.completeHandshake());
	assertTrue (ss.getBlocking());

	std::string data("hello, world");
	ss.sendBytes(data.data(), static_cast<int>(data.size()));
	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n) == data);

	ss.close();
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
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testNB);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLazyHandshake);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendFile);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendFileLarge);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testSendFileRange);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testShutdownBidirectional);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testPeerHostNameTooLong);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testStaleErrorQueue);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testHandshakeTimeout);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testDefaultCipherListRefusesAnonymousServer);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testClientRejectsAnonymousServer);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testRelaxedServerAcceptsClientWithoutCertificate);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLoopbackPeerHostName);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testTunneledLoopbackPeerIsVerified);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testResolvedLoopbackNameIsVerified);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLocalhostNameOnOtherAddressIsVerified);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testCompleteHandshakeVerifiesPeer);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testCompleteHandshakeVerifiesPeerNB);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testCompleteHandshakeAcceptsPeer);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testServerCompleteHandshakeVerifiesClientCertificate);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testServerHandshakeDripFeed);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testClientHandshakeDripFeed);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testHandshakeLeavesSocketBlocking);

	return pSuite;
}
