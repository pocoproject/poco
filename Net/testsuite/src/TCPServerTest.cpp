//
// TCPServerTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TCPServerTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Thread.h"
#include <iostream>


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
	
	class RejectFilter: public TCPServerConnectionFilter
	{
	public:
		bool accept(const StreamSocket&)
		{
			return false;
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

	return pSuite;
}
