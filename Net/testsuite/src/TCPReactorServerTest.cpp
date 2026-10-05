//
// TCPReactorServerTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TCPReactorServerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/TCPReactorServer.h"
#include "Poco/Net/TCPReactorServerConnection.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>


using Poco::Net::TCPReactorServer;
using Poco::Net::TCPReactorServerConnection;
using Poco::Net::TcpReactorConnectionPtr;
using Poco::Net::TCPServerParams;
using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;


namespace
{
	class Recorder
		/// Keeps what the callbacks of the connections were given,
		/// and lets a test wait for it.
	{
	public:
		void received(const TcpReactorConnectionPtr& pConnection)
			/// Takes what the connection has in its buffer.
		{
			std::lock_guard<std::mutex> lock(_mutex);
			++_receives;
			_data += pConnection->buffer();
			pConnection->buffer().clear();
			_changed.notify_all();
		}

		void seen(const TcpReactorConnectionPtr& pConnection)
			/// Counts the call and leaves the buffer of the connection as it is.
		{
			std::lock_guard<std::mutex> lock(_mutex);
			++_receives;
			_changed.notify_all();
		}

		void closed(const TcpReactorConnectionPtr& pConnection)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			++_closes[pConnection.get()];
			// keeps the address from being given to a later connection,
			// which would then be counted as this one
			_seen.push_back(pConnection);
			_left.push_back(pConnection->buffer());
			_changed.notify_all();
		}

		bool waitForData(std::size_t size, int milliseconds = 10000)
			/// Waits until size bytes were received in all.
		{
			std::unique_lock<std::mutex> lock(_mutex);
			return _changed.wait_for(lock, std::chrono::milliseconds(milliseconds), [this, size] { return _data.size() >= size; });
		}

		bool waitForReceives(int count, int milliseconds = 10000)
			/// Waits until the receive callback was called count times in all.
		{
			std::unique_lock<std::mutex> lock(_mutex);
			return _changed.wait_for(lock, std::chrono::milliseconds(milliseconds), [this, count] { return _receives >= count; });
		}

		bool waitForCloses(std::size_t connections, int milliseconds = 10000)
			/// Waits until the given number of connections have closed.
		{
			std::unique_lock<std::mutex> lock(_mutex);
			return _changed.wait_for(lock, std::chrono::milliseconds(milliseconds), [this, connections] { return _closes.size() >= connections; });
		}

		std::string data() const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _data;
		}

		int receives() const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _receives;
		}

		std::size_t closedConnections() const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _closes.size();
		}

		bool everyCloseOnce() const
			/// Returns true if no connection reported its close more than once.
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (const auto& c: _closes)
			{
				if (c.second != 1) return false;
			}
			return true;
		}

		std::vector<std::string> leftAtClose() const
			/// Returns what the buffers of the connections held when they closed.
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _left;
		}

	private:
		std::string _data;
		int _receives = 0;
		std::map<const TCPReactorServerConnection*, int> _closes;
		std::vector<std::weak_ptr<TCPReactorServerConnection>> _seen;
		std::vector<std::string> _left;
		mutable std::mutex _mutex;
		std::condition_variable _changed;
	};


	class ManualReactor: public Poco::Net::SocketReactor
		/// A reactor that is not run: the test delivers the events itself,
		/// one at a time, and so knows what a connection does with each.
	{
	public:
		void readable(const Poco::Net::Socket& socket)
		{
			dispatch(socket, static_cast<Poco::Net::SocketNotification*>(getReadableNotification()));
		}

		using Poco::Net::SocketReactor::onError;
	};


	class HoldingSocketImpl: public Poco::Net::StreamSocketImpl
		/// A non-blocking socket that hands out data it holds already,
		/// as a TLS socket does with what it has decrypted. None of it is
		/// on the network, so the socket does not become readable.
	{
	public:
		HoldingSocketImpl(const std::string& data, bool secure):
			_data(data),
			_secure(secure)
		{
			init(AF_INET);
		}

		int receiveBytes(void* buffer, int length, int flags = 0) override
		{
			if (_pos == _data.size()) return -1;
			const std::size_t n = std::min(static_cast<std::size_t>(length), _data.size() - _pos);
			std::memcpy(buffer, _data.data() + _pos, n);
			_pos += n;
			return static_cast<int>(n);
		}

		int available() override
		{
			return static_cast<int>(_data.size() - _pos);
		}

		bool secure() const override
		{
			return _secure;
		}

		bool getBlocking() const override
		{
			return false;
		}

	private:
		std::string _data;
		std::size_t _pos = 0;
		bool _secure;
	};


	TCPServerParams::Ptr params(bool nonBlocking)
	{
		TCPServerParams::Ptr pParams = new TCPServerParams;
		pParams->setReactorMode(true);
		pParams->setUseSelfReactor(true);
		pParams->setNonBlocking(nonBlocking);
		return pParams;
	}


	std::string pattern(std::size_t size)
	{
		std::string data(size, '\0');
		for (std::size_t i = 0; i < size; ++i) data[i] = static_cast<char>('!' + i % 89);
		return data;
	}


	void sendAll(StreamSocket& socket, const std::string& data)
	{
		std::size_t sent = 0;
		while (sent < data.size())
		{
			int n = socket.sendBytes(data.data() + sent, static_cast<int>(data.size() - sent));
			if (n <= 0) throw Poco::IOException("connection closed while sending");
			sent += n;
		}
	}


	bool waitForAvailable(const StreamSocket& socket, std::size_t size, int milliseconds = 10000)
		/// Waits until size bytes have arrived at the socket.
	{
		Poco::Timestamp start;
		while (static_cast<std::size_t>(socket.available()) < size)
		{
			if (start.isElapsed(Poco::Timestamp::TimeDiff(milliseconds)*1000)) return false;
			Poco::Thread::sleep(1);
		}
		return true;
	}


	bool closedByPeer(StreamSocket& socket, int milliseconds = 10000)
		/// Returns true if the peer closes or resets the connection
		/// within the given time.
	{
		socket.setReceiveTimeout(Poco::Timespan(Poco::Timespan::TimeDiff(milliseconds)*1000));
		char buffer[256];
		try
		{
			return socket.receiveBytes(buffer, sizeof(buffer)) == 0;
		}
		catch (const Poco::TimeoutException&)
		{
			return false;
		}
		catch (const Poco::Net::NetException&)
		{
			return true;
		}
	}
}


TCPReactorServerTest::TCPReactorServerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TCPReactorServerTest::~TCPReactorServerTest()
{
}


void TCPReactorServerTest::testSuppliedSocket()
{
	Recorder recorder;
	ServerSocket ss(SocketAddress("127.0.0.1", 0));
	TCPReactorServer server(ss, params(false));
	server.setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConnection) { recorder.received(pConnection); });
	server.start();
	assertTrue (server.port() == ss.address().port());

	StreamSocket client(ss.address());
	const std::string hello("hello");
	sendAll(client, hello);
	bool arrived = recorder.waitForData(hello.size());
	server.stop();
	assertTrue (arrived);
	assertTrue (recorder.data() == hello);
}


void TCPReactorServerTest::testNonBlockingRead()
{
	Recorder recorder;
	ServerSocket ss(SocketAddress("127.0.0.1", 0));
	TCPReactorServer server(ss, params(true));
	server.setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConnection) { recorder.received(pConnection); });
	server.setCloseCallback([&recorder](const TcpReactorConnectionPtr& pConnection) { recorder.closed(pConnection); });
	server.start();

	// Far more than is read for one event, and more than the socket buffers
	// hold: the reads that find nothing must leave the connection open.
	const std::string data = pattern(1024*1024);
	StreamSocket client(ss.address());
	sendAll(client, data);
	bool arrived = recorder.waitForData(data.size());
	std::size_t closedBefore = recorder.closedConnections();
	client.close();
	bool closed = recorder.waitForCloses(1);
	server.stop();
	assertTrue (arrived);
	assertTrue (closedBefore == 0);
	assertTrue (closed);
	assertTrue (recorder.data() == data);
	assertTrue (recorder.everyCloseOnce());
}


void TCPReactorServerTest::testReadsPerEvent()
{
	// What one event may read is seen with a reactor that delivers nothing
	// by itself. The data is several times what one read takes.
	const std::string data = pattern(32*1024);
	for (int nonBlocking = 0; nonBlocking < 2; ++nonBlocking)
	{
		Recorder recorder;
		ManualReactor reactor;
		ServerSocket ss(SocketAddress("127.0.0.1", 0));
		StreamSocket client(ss.address());
		StreamSocket accepted = ss.acceptConnection();
		accepted.setBlocking(nonBlocking == 0);
		auto pConnection = std::make_shared<TCPReactorServerConnection>(accepted, reactor);
		pConnection->setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConn) { recorder.received(pConn); });
		pConnection->setCloseCallback([&recorder](const TcpReactorConnectionPtr& pConn) { recorder.closed(pConn); });
		pConnection->initialize();

		sendAll(client, data);
		assertTrue (waitForAvailable(accepted, data.size()));
		reactor.readable(accepted);
		if (nonBlocking)
		{
			// read until nothing was left, and still open
			assertTrue (recorder.data() == data);
			assertTrue (recorder.receives() > 1);
		}
		else
		{
			// read once
			assertTrue (recorder.receives() == 1);
			assertTrue (!recorder.data().empty());
			assertTrue (recorder.data().size() < data.size());
			assertTrue (data.compare(0, recorder.data().size(), recorder.data()) == 0);
		}
		assertTrue (recorder.closedConnections() == 0);

		pConnection->handleClose();
		assertTrue (recorder.closedConnections() == 1);
	}
}


void TCPReactorServerTest::testReadsHeldData()
{
	// Reading for one event ends after a number of reads, since what is
	// left on the network is signalled again. What a secure socket holds
	// itself is not, and has to be read to the end. The data is more than
	// those reads take.
	const std::string data = pattern(100*1024);
	for (int secure = 0; secure < 2; ++secure)
	{
		Recorder recorder;
		ManualReactor reactor;
		StreamSocket socket(new HoldingSocketImpl(data, secure != 0));
		auto pConnection = std::make_shared<TCPReactorServerConnection>(socket, reactor);
		pConnection->setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConn) { recorder.received(pConn); });
		pConnection->setCloseCallback([&recorder](const TcpReactorConnectionPtr& pConn) { recorder.closed(pConn); });
		pConnection->initialize();

		reactor.readable(socket);
		if (secure)
		{
			assertTrue (recorder.data() == data);
		}
		else
		{
			assertTrue (!recorder.data().empty());
			assertTrue (recorder.data().size() < data.size());
			// the rest with the events that follow
			for (int i = 0; i < 100 && recorder.data().size() < data.size(); ++i)
			{
				reactor.readable(socket);
			}
			assertTrue (recorder.data() == data);
		}
		// reads that find nothing leave the connection open
		reactor.readable(socket);
		assertTrue (recorder.closedConnections() == 0);

		pConnection->handleClose();
		assertTrue (recorder.closedConnections() == 1);
	}
}


void TCPReactorServerTest::testDataBeforeErrorIsDelivered()
{
	// A hang-up may be reported as an error, together with the last data
	// of the peer. That data must reach the receive callback before the
	// connection closes.
	Recorder recorder;
	ManualReactor reactor;
	ServerSocket ss(SocketAddress("127.0.0.1", 0));
	StreamSocket client(ss.address());
	StreamSocket accepted = ss.acceptConnection();
	accepted.setBlocking(false);
	auto pConnection = std::make_shared<TCPReactorServerConnection>(accepted, reactor);
	pConnection->setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConn) { recorder.received(pConn); });
	pConnection->setCloseCallback([&recorder](const TcpReactorConnectionPtr& pConn) { recorder.closed(pConn); });
	pConnection->initialize();

	const std::string data = pattern(24*1024);
	sendAll(client, data);
	client.close();
	assertTrue (waitForAvailable(accepted, data.size()));
	reactor.onError(0, "simulated error");
	assertTrue (recorder.data() == data);
	assertTrue (recorder.closedConnections() == 1);
	assertTrue (recorder.everyCloseOnce());

	// closed: nothing more is delivered, and nothing is reported twice
	reactor.onError(0, "simulated error");
	pConnection->handleClose();
	assertTrue (recorder.closedConnections() == 1);
	assertTrue (recorder.everyCloseOnce());
}


void TCPReactorServerTest::testCloseCallbackOnce()
{
	Recorder recorder;
	ServerSocket ss(SocketAddress("127.0.0.1", 0));
	TCPReactorServer server(ss, params(true));
	server.setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConnection)
	{
		if (pConnection->buffer() == "throw") throw Poco::ApplicationException("thrown by the receive callback");
		if (pConnection->buffer() == "keep")
			recorder.seen(pConnection);
		else
			recorder.received(pConnection);
	});
	server.setCloseCallback([&recorder](const TcpReactorConnectionPtr& pConnection) { recorder.closed(pConnection); });
	server.start();

	// One connection is closed by its peer, one because the receive callback
	// throws, and one by the server when it stops.
	StreamSocket byPeer(ss.address());
	sendAll(byPeer, "keep");
	byPeer.close();
	StreamSocket byException(ss.address());
	sendAll(byException, "throw");
	bool twoClosed = recorder.waitForCloses(2);
	StreamSocket byStop(ss.address());
	sendAll(byStop, "taken");
	bool arrived = recorder.waitForData(5);
	server.stop();

	assertTrue (twoClosed);
	assertTrue (arrived);
	assertTrue (recorder.closedConnections() == 3);
	assertTrue (recorder.everyCloseOnce());

	// what was not taken out of a buffer is still there at the close
	std::vector<std::string> left = recorder.leftAtClose();
	assertTrue (left.size() == 3);
	int kept = 0;
	int thrown = 0;
	int empty = 0;
	for (const auto& l: left)
	{
		if (l == "keep") ++kept;
		else if (l == "throw") ++thrown;
		else if (l.empty()) ++empty;
	}
	assertTrue (kept == 1);
	assertTrue (thrown == 1);
	assertTrue (empty == 1);
}


void TCPReactorServerTest::callbackCloses(bool nonBlocking)
{
	Recorder recorder;
	ServerSocket ss(SocketAddress("127.0.0.1", 0));
	TCPReactorServer server(ss, params(nonBlocking));
	server.setRecvMessageCallback([&recorder](const TcpReactorConnectionPtr& pConnection)
	{
		recorder.received(pConnection);
		pConnection->handleClose();
		pConnection->handleClose();
	});
	server.setCloseCallback([&recorder](const TcpReactorConnectionPtr& pConnection) { recorder.closed(pConnection); });
	server.start();

	// More than one read takes: reading must end with the close.
	StreamSocket client(ss.address());
	try
	{
		sendAll(client, pattern(64*1024));
	}
	catch (const Poco::Exception&)
	{
		// the server may close before everything is sent
	}
	bool closed = recorder.waitForCloses(1);
	bool seenByPeer = closedByPeer(client);
	server.stop();

	assertTrue (closed);
	assertTrue (seenByPeer);
	assertTrue (recorder.receives() == 1);
	assertTrue (recorder.closedConnections() == 1);
	assertTrue (recorder.everyCloseOnce());
}


void TCPReactorServerTest::testCallbackCloses()
{
	callbackCloses(false);
	callbackCloses(true);
}


void TCPReactorServerTest::setUp()
{
}


void TCPReactorServerTest::tearDown()
{
}


CppUnit::Test* TCPReactorServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TCPReactorServerTest");

	CppUnit_addTest(pSuite, TCPReactorServerTest, testSuppliedSocket);
	CppUnit_addTest(pSuite, TCPReactorServerTest, testNonBlockingRead);
	CppUnit_addTest(pSuite, TCPReactorServerTest, testReadsPerEvent);
	CppUnit_addTest(pSuite, TCPReactorServerTest, testReadsHeldData);
	CppUnit_addTest(pSuite, TCPReactorServerTest, testDataBeforeErrorIsDelivered);
	CppUnit_addTest(pSuite, TCPReactorServerTest, testCloseCallbackOnce);
	CppUnit_addTest(pSuite, TCPReactorServerTest, testCallbackCloses);

	return pSuite;
}
