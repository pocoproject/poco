//
// SocketReactorTest.h
//
// Definition of the SocketReactorTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SocketReactorTest_INCLUDED
#define SocketReactorTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Thread.h"
#include "Poco/Observer.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "CppUnit/TestCase.h"


class SocketReactorTest: public CppUnit::TestCase
{
public:
	SocketReactorTest(const std::string& name);
	~SocketReactorTest();

	void testSocketReactor();
	void testSocketReactorPoll();
	void testSetSocketReactor();
	void testParallelSocketReactor();
	void testSocketConnectorFail();
	void testSocketConnectorTimeout();
	void testDataCollection();
	void testCompletionHandler();
	void testTimedCompletionHandler();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

	template <class SocketType>
	class IOHandler
	{
	public:
		IOHandler(const SocketType& socket, Poco::Net::SocketReactor& reactor):
				_socket(socket),
				_reactor(reactor),
				_or(*this, &IOHandler::onReadable),
				_ow(*this, &IOHandler::onWritable),
				_ot(*this, &IOHandler::onTimeout),
				_os(*this, &IOHandler::onShutdown)
		{
			_reactor.addEventHandler(_socket, _or);
			_reactor.addEventHandler(_socket, _ow);
			_reactor.addEventHandler(_socket, _ot);
			_reactor.addEventHandler(_socket, _os);
		}

		~IOHandler()
		{
			_reactor.removeEventHandler(_socket, _or);
			_reactor.removeEventHandler(_socket, _ow);
			_reactor.removeEventHandler(_socket, _ot);
			_reactor.removeEventHandler(_socket, _os);
		}

		void onReadable(Poco::Net::ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[32];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n > 0) _data.append(buffer, n);
		}

		void onWritable(Poco::Net::WritableNotification* pNf)
		{
			pNf->release();
			_writable = true;
		}

		void onTimeout(Poco::Net::TimeoutNotification* pNf)
		{
			pNf->release();
			_timeout = true;
		}

		void onShutdown(Poco::Net::ShutdownNotification* pNf)
		{
			pNf->release();
			_shutdown = true;
		}

		const std::string& data()
		{
			return _data;
		}

		bool writable() const
		{
			return _writable;
		}

		bool timeout() const
		{
			return _timeout;
		}

		bool shutdown() const
		{
			return _shutdown;
		}

	private:

		SocketType _socket;
		Poco::Net::SocketReactor& _reactor;
		Poco::Observer<IOHandler, Poco::Net::ReadableNotification> _or;
		Poco::Observer<IOHandler, Poco::Net::WritableNotification> _ow;
		Poco::Observer<IOHandler, Poco::Net::TimeoutNotification>  _ot;
		Poco::Observer<IOHandler, Poco::Net::ShutdownNotification> _os;
		bool _writable = false;
		bool _timeout = false;
		bool _shutdown = false;
		std::string _data;
	};

	template <typename ServerType, typename SocketType>
	void testIOHandler()
	{
		using Poco::Thread;
		using Poco::Net::SocketReactor;
		using Poco::Net::SocketAddress;

		ServerType echoServer;
		SocketType ds;
		ds.connect(SocketAddress("127.0.0.1", echoServer.port()));
		SocketReactor reactor;
		IOHandler<SocketType> ih(ds, reactor);
		int n = ds.sendBytes("hello", 5);
		assertTrue (n == 5);
		assertFalse(ih.writable());
		Thread t; t.start(reactor);
		Thread::sleep(500);
		assertTrue(ih.writable());
		assertFalse(ih.shutdown());
		reactor.stop(); t.join();
		assertTrue(ih.shutdown());
		assertTrue (ih.data() == "hello");
		n = ds.sendBytes("hello", 5);
		assertTrue (n == 5);
		while (ih.data().size() < 10)
		{
			reactor.poll();
			Thread::sleep(10);
		}
		assertTrue (ih.data() == "hellohello");
		reactor.poll();
		assertTrue (ih.timeout());
		ds.close();
	}
};


#endif // SocketReactorTest_INCLUDED
