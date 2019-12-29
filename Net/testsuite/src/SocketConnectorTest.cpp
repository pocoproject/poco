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
			if (n > 0)
			{
				_socket.sendBytes(buffer, n);
			}
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
			_ow(*this, &ClientServiceHandler::onWritable)
		{
			_reactor.addEventHandler(_socket, _or);
			_reactor.addEventHandler(_socket, _ow);

			do_something();
			_reactor.stop();
		}
		
		~ClientServiceHandler()
		{
		}

		void do_something(){
			volatile long i = 0;
			while(true){
				i++;
				// if(i >= 1000000) break;
				if(i >= 10000000) break;
			}
		}
		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			_reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, ReadableNotification>(*this, &ClientServiceHandler::onReadable));
		}
		
		void onWritable(WritableNotification* pNf)
		{
			pNf->release();
			_reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, WritableNotification>(*this, &ClientServiceHandler::onWritable));
		}
		
		StreamSocket                                         _socket;
		SocketReactor&                                       _reactor;
		Observer<ClientServiceHandler, ReadableNotification> _or;
		Observer<ClientServiceHandler, WritableNotification> _ow;
	};
}

SocketConnectorTest::SocketConnectorTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketConnectorTest::~SocketConnectorTest()
{
}

void SocketConnectorTest::testCallUnregisterConnectorInSameTime()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor1;
	SocketReactor reactor2;
	SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor1);
	Poco::Thread th;
	th.start(reactor1);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> *connector = new SocketConnector<ClientServiceHandler>(sa, reactor2);
	Poco::Thread th2;
	th2.start(reactor2);
	Poco::Thread::sleep(1);
	delete connector;

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

	CppUnit_addTest(pSuite, SocketConnectorTest, testCallUnregisterConnectorInSameTime);

	return pSuite;
}
