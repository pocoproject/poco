//
// SocketAcceptorTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SocketAcceptorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/ParallelSocketAcceptor.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Observer.h"
#include "Poco/Thread.h"

using Poco::Net::SocketReactor;
using Poco::Net::SocketConnector;
using Poco::Net::SocketAcceptor;
using Poco::Net::ParallelSocketAcceptor;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::SocketNotification;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ErrorNotification;
using Poco::Net::ShutdownNotification;
using Poco::Observer;

namespace
{
	int REACTORS_COUNT = 8;

	class EchoServiceHandler
	{
	public:
		EchoServiceHandler(StreamSocket& socket, SocketReactor& reactor):
				_socket(socket),
				_reactor(reactor)
		{
			_reactor.addEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
		}

		EchoServiceHandler(StreamSocket& socket, SocketReactor& reactor, bool isBlocking):
				_socket(socket),
				_reactor(reactor)
		{
			_isBlocking = isBlocking;
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
			_data = std::string(buffer, n);
			_socket.sendBytes(buffer, sizeof(buffer));
			delete this;
		}

		static std::string _data;
		static bool _isBlocking;

	private:
		StreamSocket   _socket;
		SocketReactor& _reactor;
	};

	std::string EchoServiceHandler::_data;
	bool EchoServiceHandler::_isBlocking = true;

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
			_reactor.stop();
			delete this;
		}

		void onReadable(ReadableNotification* pNf)
		{
			if (pNf) pNf->release();
			char buffer[8];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			onShutdown(0);
		}

		void onWritable(WritableNotification* pNf)
		{
			if (pNf) pNf->release();
			_reactor.removeEventHandler(_socket, _ow);
			std::string data(8, 'x');
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

SocketAcceptorTest::SocketAcceptorTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketAcceptorTest::~SocketAcceptorTest()
{
}


void SocketAcceptorTest::testSocketAcceptor()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	EchoServiceHandler::_data.clear();
	reactor.run();
	assertTrue (EchoServiceHandler::_data == "xxxxxxxx");
}


void SocketAcceptorTest::testSocketAcceptorWithParameters()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAcceptor<EchoServiceHandler, bool> acceptor(ss, reactor, false);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	EchoServiceHandler::_isBlocking = true;
	reactor.run();
	assertTrue (EchoServiceHandler::_isBlocking == false);
}


void SocketAcceptorTest::testParallelSocketAcceptor()
{
	SocketAddress ssa("127.0.0.1:22087");
	ServerSocket ss(ssa);
	SocketReactor reactor;
	ParallelSocketAcceptor<EchoServiceHandler, SocketReactor> acceptor(ss, reactor, REACTORS_COUNT);

	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	EchoServiceHandler::_data.clear();
	reactor.run();
	assertTrue (EchoServiceHandler::_data == "xxxxxxxx");
}


void SocketAcceptorTest::testParallelSocketAcceptorWithParameters()
{
	SocketAddress ssa("127.0.0.1:22087");
	ServerSocket ss(ssa);
	SocketReactor reactor;
	ParallelSocketAcceptor<EchoServiceHandler, SocketReactor, bool> acceptor(ss, reactor, false, REACTORS_COUNT);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	EchoServiceHandler::_isBlocking = true;
	reactor.run();
	assertTrue (EchoServiceHandler::_isBlocking == false);
}



CppUnit::Test* SocketAcceptorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketAcceptorTest");

	CppUnit_addTest(pSuite, SocketAcceptorTest, testSocketAcceptor);
	CppUnit_addTest(pSuite, SocketAcceptorTest, testSocketAcceptorWithParameters);
	CppUnit_addTest(pSuite, SocketAcceptorTest, testParallelSocketAcceptor);
	CppUnit_addTest(pSuite, SocketAcceptorTest, testParallelSocketAcceptorWithParameters);

	return pSuite;
}