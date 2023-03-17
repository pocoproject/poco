//
// SocketConnectorTest.cpp
//
// Copyright (c) 2005-2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SocketConnectorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Observer.h"
#include <iostream>


using Poco::Net::SocketReactor;
using Poco::Net::SocketConnector;
using Poco::Net::SocketAcceptor;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::SocketNotification;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ShutdownNotification;
using Poco::Observer;


namespace
{
	class EchoServiceHandler
	{
	public:
		EchoServiceHandler(StreamSocket& socket, SocketReactor& reactor):
			_socket(socket),
			_reactor(reactor)
		{
			_reactor.addEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
		}
		
		~EchoServiceHandler()
		{
			_reactor.removeEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
		}
		
		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[8];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n > 0) _socket.sendBytes(buffer, n);
			else delete this;
		}
		
	private:
		StreamSocket   _socket;
		SocketReactor& _reactor;
	};
	
	class ClientServiceHandler
	{
	public:
		ClientServiceHandler(StreamSocket& socket, SocketReactor& reactor):
			_socket(socket),
			_reactor(reactor),
			_or(*this, &ClientServiceHandler::onReadable),
			_ow(*this, &ClientServiceHandler::onWritable),
			_os(*this, &ClientServiceHandler::onShutdown)
		{
			_reactor.addEventHandler(_socket, _or);
			_reactor.addEventHandler(_socket, _ow);
			_reactor.addEventHandler(_socket, _os);

			doSomething();
		}
		
		~ClientServiceHandler()
		{
			_reactor.removeEventHandler(_socket, _os);
			_reactor.removeEventHandler(_socket, _ow);
			_reactor.removeEventHandler(_socket, _or);
		}

		void doSomething()
		{
			Thread::sleep(100);
		}

		void onShutdown(ShutdownNotification* pNf)
		{
			if (pNf) pNf->release();
			_reactor.removeEventHandler(_socket, _os);
			delete this;
		}

		void onReadable(ReadableNotification* pNf)
		{
			if (pNf) pNf->release();
			char buffer[32];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n <= 0) onShutdown(0);
		}
		
		void onWritable(WritableNotification* pNf)
		{
			if (pNf) pNf->release();
			_reactor.removeEventHandler(_socket, _ow);
			std::string data(5, 'x');
			_socket.sendBytes(data.data(), (int) data.length());
			_socket.shutdownSend();
		}
		
		StreamSocket                                         _socket;
		SocketReactor&                                       _reactor;
		Observer<ClientServiceHandler, ReadableNotification> _or;
		Observer<ClientServiceHandler, WritableNotification> _ow;
		Observer<ClientServiceHandler, ShutdownNotification> _os;
	};
}

SocketConnectorTest::SocketConnectorTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketConnectorTest::~SocketConnectorTest()
{
}


void SocketConnectorTest::testUnregisterConnector()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor1;
	SocketReactor reactor2;
	SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor1);
	Poco::Thread th;
	th.start(reactor1);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor2);
	Poco::Thread th2;
	th2.start(reactor2);
	Thread::sleep(200);
	reactor1.stop();
	reactor2.stop();
	th.join();
	th2.join();
}


void SocketConnectorTest::setUp()
{
}


void SocketConnectorTest::tearDown()
{
}


CppUnit::Test* SocketConnectorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketConnectorTest");

	CppUnit_addTest(pSuite, SocketConnectorTest, testUnregisterConnector);

	return pSuite;
}
