//
// TCPServerTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/ServerSocketImpl.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Exception.h"
#include "Poco/Stopwatch.h"
#include "Poco/Timespan.h"
#include <atomic>
#include <iostream>
#include <string>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnectionFilter;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;


namespace
{
	static Poco::FastMutex cerrMutex;

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

	class RejectFilter: public TCPServerConnectionFilter
	{
	public:
		bool accept(const StreamSocket&)
		{
			return false;
		}
	};

	class CountingErrorHandler: public Poco::ErrorHandler
		/// Counts the exceptions reported to it. It is installed as the
		/// ErrorHandler for its lifetime.
	{
	public:
		POCO_NODISCARD_CTOR CountingErrorHandler():
			_pPrevious(Poco::ErrorHandler::set(this))
		{
		}

		~CountingErrorHandler() override
		{
			Poco::ErrorHandler::set(_pPrevious);
		}

		CountingErrorHandler(const CountingErrorHandler&) = delete;
		CountingErrorHandler(CountingErrorHandler&&) = delete;
		CountingErrorHandler& operator = (const CountingErrorHandler&) = delete;
		CountingErrorHandler& operator = (CountingErrorHandler&&) = delete;

		void exception(const Poco::Exception&) override
		{
			++_reports;
		}

		void exception(const std::exception&) override
		{
			++_reports;
		}

		void exception() override
		{
			++_reports;
		}

		[[nodiscard]] int reports() const
		{
			return _reports;
		}

	private:
		Poco::ErrorHandler* _pPrevious;
		std::atomic<int> _reports{0};
	};

	class FailingAcceptSocketImpl: public Poco::Net::ServerSocketImpl
		/// Fails every accept with the given error code, as accept() does
		/// when the process runs out of descriptors or memory, until
		/// stopFailing() is called. The pending connection stays queued.
	{
	public:
		explicit FailingAcceptSocketImpl(int errorCode):
			_errorCode(errorCode)
		{
		}

		FailingAcceptSocketImpl() = delete;

		FailingAcceptSocketImpl(const FailingAcceptSocketImpl&) = delete;
		FailingAcceptSocketImpl(FailingAcceptSocketImpl&&) = delete;
		FailingAcceptSocketImpl& operator = (const FailingAcceptSocketImpl&) = delete;
		FailingAcceptSocketImpl& operator = (FailingAcceptSocketImpl&&) = delete;

		[[nodiscard]] Poco::Net::SocketImpl* acceptConnection(Poco::Net::SocketAddress& clientAddr) override
		{
			if (_failing) throw Poco::IOException("Resource error", _errorCode);
			return ServerSocketImpl::acceptConnection(clientAddr);
		}

		void stopFailing()
		{
			_failing = false;
		}

	protected:
		~FailingAcceptSocketImpl() override = default;

	private:
		const int _errorCode;
		std::atomic<bool> _failing{true};
	};

	class FailingAcceptSocket: public ServerSocket
		/// A ServerSocket whose accepts fail until
		/// failingImpl().stopFailing() is called.
	{
	public:
		explicit FailingAcceptSocket(int errorCode):
			ServerSocket(new FailingAcceptSocketImpl(errorCode), true)
		{
		}

		FailingAcceptSocket() = delete;
		~FailingAcceptSocket() override = default;

		[[nodiscard]] FailingAcceptSocketImpl& failingImpl() const
		{
			return *static_cast<FailingAcceptSocketImpl*>(impl());
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
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>());
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", srv.socket().address().port());
	StreamSocket ss1(sa);
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
	Thread::sleep(1000);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testTwoConnections()
{
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>());
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", srv.socket().address().port());
	StreamSocket ss1(sa);
	StreamSocket ss2(sa);
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
	Thread::sleep(1000);
	assertTrue (srv.currentConnections() == 1);
	assertTrue (srv.currentThreads() == 1);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 2);
	ss2.close();

	Thread::sleep(1000);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testMultiConnections()
{
	ServerSocket svs(0);
	TCPServerParams* pParams = new TCPServerParams;
	pParams->setMaxThreads(4);
	pParams->setMaxQueued(4);
	pParams->setThreadIdleTime(100);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs, pParams);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.maxThreads() >= 4);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", svs.address().port());
	StreamSocket ss1(sa);
	StreamSocket ss2(sa);
	StreamSocket ss3(sa);
	StreamSocket ss4(sa);
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

	StreamSocket ss5(sa);
	Thread::sleep(200);
	assertTrue (srv.queuedConnections() == 1);
	StreamSocket ss6(sa);
	Thread::sleep(200);
	assertTrue (srv.queuedConnections() == 2);

	ss1.close();
	Thread::sleep(2000);
	assertTrue (srv.currentConnections() == 4);
	assertTrue (srv.currentThreads() == 4);
	assertTrue (srv.queuedConnections() == 1);
	assertTrue (srv.totalConnections() == 5);

	ss2.close();
	Thread::sleep(2000);
	assertTrue (srv.currentConnections() == 4);
	assertTrue (srv.currentThreads() == 4);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 6);

	ss3.close();
	Thread::sleep(2000);
	assertTrue (srv.currentConnections() == 3);
	assertTrue (srv.currentThreads() == 3);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 6);

	ss4.close();
	Thread::sleep(2000);
	assertTrue (srv.currentConnections() == 2);
	assertTrue (srv.currentThreads() == 2);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 6);

	ss5.close();
	ss6.close();
	Thread::sleep(1000);
	assertTrue (srv.currentConnections() == 0);
}


void TCPServerTest::testThreadCapacity()
{
	ServerSocket svs(0);
	TCPServerParams* pParams = new TCPServerParams;
	pParams->setMaxThreads(64);
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs, pParams);
	srv.start();
	assertTrue (srv.maxThreads() >= 64);
}


void TCPServerTest::testFilter()
{
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>());
	srv.setConnectionFilter(new RejectFilter);
	srv.start();
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);

	SocketAddress sa("127.0.0.1", srv.socket().address().port());
	StreamSocket ss(sa);

	char buffer[256];
	int n = ss.receiveBytes(buffer, sizeof(buffer));

	assertTrue (n == 0);
	assertTrue (srv.currentConnections() == 0);
	assertTrue (srv.currentThreads() == 0);
	assertTrue (srv.queuedConnections() == 0);
	assertTrue (srv.totalConnections() == 0);
}


void TCPServerTest::testAcceptResourceError()
{
	checkPauseAfterAcceptError(POCO_EMFILE);
	checkPauseAfterAcceptError(POCO_ENOBUFS);
#if defined(POCO_OS_FAMILY_UNIX)
	checkPauseAfterAcceptError(ENFILE);
	checkPauseAfterAcceptError(ENOMEM);
#endif
}


void TCPServerTest::checkPauseAfterAcceptError(int errorCode)
{
	constexpr long WINDOW_MS = 500;
	constexpr int MAX_REPORTS = 100;

	const std::string context = "error code " + std::to_string(errorCode) + ": ";
	CountingErrorHandler handler;
	FailingAcceptSocket svs(errorCode);
	svs.bind(SocketAddress("127.0.0.1", 0));
	svs.listen();
	TCPServer srv(new TCPServerConnectionFactoryImpl<EchoConnection>(), svs);
	srv.start();

	StreamSocket client;
	client.connect(SocketAddress("127.0.0.1", svs.address().port()));
	Poco::Stopwatch sw;
	sw.start();
	while (handler.reports() == 0 && sw.elapsedSeconds() < 10)
	{
		Thread::sleep(10);
	}
	if (handler.reports() == 0) failmsg(context + "the failed accept was not reported");

	// A measurement window, not synchronization: about 10 reports with the
	// pause after a failed accept, thousands without it.
	const int reportsBefore = handler.reports();
	Thread::sleep(WINDOW_MS);
	const int reports = handler.reports() - reportsBefore;
	if (reports >= MAX_REPORTS)
		failmsg(context + std::to_string(reports) + " failed accepts in the window, the acceptor does not pause");

	svs.failingImpl().stopFailing();
	client.setReceiveTimeout(Poco::Timespan(5, 0));
	const char sent = 'x';
	client.sendBytes(&sent, 1);
	char received = 0;
	if (client.receiveBytes(&received, 1) != 1 || received != sent)
		failmsg(context + "the connection was not served after the accepts succeeded again");
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
	CppUnit_addTest(pSuite, TCPServerTest, testThreadCapacity);
	CppUnit_addTest(pSuite, TCPServerTest, testFilter);
	CppUnit_addTest(pSuite, TCPServerTest, testAcceptResourceError);

	return pSuite;
}
