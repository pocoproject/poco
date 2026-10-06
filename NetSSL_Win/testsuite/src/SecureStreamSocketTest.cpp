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
#include "Poco/Net/Context.h"
#include "Poco/Net/RejectCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"
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

	// the handshake is made with the first data
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
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testHandshakeTimeout);
	CppUnit_addTest(pSuite, SecureStreamSocketTest, testLazyHandshake);

	return pSuite;
}
