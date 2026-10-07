//
// ConnectionTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ConnectionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/TCP/Connection.h"
#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/ThreadPool.h"
#include "Poco/Stopwatch.h"
#include "Poco/Timespan.h"
#include "Poco/Event.h"
#include "Poco/Format.h"
#include "Poco/URI.h"
#include <atomic>
#include <functional>
#include <thread>


using Poco::RemotingNG::TCP::Connection;
using Poco::RemotingNG::TCP::ConnectionManager;
using Poco::RemotingNG::TCP::Listener;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Timespan;


namespace
{
	class Peer
		/// Something that listens on a port of the loopback address and,
		/// on a thread of its own, does with the first connection it
		/// accepts what it was told to. Not a RemotingNG endpoint.
	{
	public:
		using Behavior = std::function<void(StreamSocket&)>;

		explicit Peer(Behavior behavior):
			_socket(SocketAddress("127.0.0.1", 0)),
			_thread([this, behavior]()
			{
				try
				{
					while (!_stopped)
					{
						if (_socket.poll(Timespan(0, 100000), Poco::Net::Socket::SELECT_READ))
						{
							StreamSocket connection = _socket.acceptConnection();
							behavior(connection);
							break;
						}
					}
				}
				catch (Poco::Exception&)
				{
				}
			})
		{
		}

		~Peer()
		{
			_stopped = true;
			_thread.join();
		}

		Poco::URI uri() const
		{
			return Poco::URI(Poco::format("remoting.tcp://127.0.0.1:%hu", _socket.address().port()));
		}

	private:
		ServerSocket _socket;
		std::atomic<bool> _stopped{false};
		std::thread _thread;
	};


	class Endpoint
		/// A Listener on a port of the loopback address, with a
		/// ConnectionManager and a thread pool of its own, stopped and
		/// its threads joined when it goes out of scope.
	{
	public:
		Endpoint():
			_pool("endpoint", 1, 8),
			_manager(_pool),
			_socket(SocketAddress("127.0.0.1", 0)),
			_pListener(new Listener(_socket.address().toString(), _socket, new Poco::Net::TCPServerParams, _manager))
		{
			_pListener->start();
		}

		~Endpoint()
		{
			_pListener->stop();
			_manager.shutdown();
			_pool.joinAll();
		}

		Poco::URI uri() const
		{
			return Poco::URI(Poco::format("remoting.tcp://%s", _socket.address().toString()));
		}

		Poco::ThreadPool& pool()
		{
			return _pool;
		}

	private:
		Poco::ThreadPool _pool;
		ConnectionManager _manager;
		ServerSocket _socket;
		Listener::Ptr _pListener;
	};


	class Client
		/// A ConnectionManager with a thread pool of its own, shut down
		/// and its threads joined when it goes out of scope.
	{
	public:
		Client():
			_pool("client", 1, 8),
			_manager(_pool)
		{
		}

		~Client()
		{
			_manager.shutdown();
			_pool.joinAll();
		}

		ConnectionManager& manager()
		{
			return _manager;
		}

	private:
		Poco::ThreadPool _pool;
		ConnectionManager _manager;
	};
}


ConnectionTest::ConnectionTest(const std::string& name): CppUnit::TestCase(name)
{
}


ConnectionTest::~ConnectionTest()
{
}


void ConnectionTest::testHandshakeFailure()
{
	// A peer that takes the handshake and closes the connection instead of
	// answering it: the caller learns so at once, and what it learns is
	// what happened, not that it waited in vain.
	Peer peer([](StreamSocket& connection)
	{
		char buffer[256];
		connection.receiveBytes(buffer, sizeof(buffer));
		connection.close();
	});
	Client client;
	client.manager().setHandshakeTimeout(Timespan(600, 0));

	try
	{
		Connection::Ptr pConnection = client.manager().getConnection(peer.uri());
		fail("a closed connection must not be handed out");
	}
	catch (Poco::TimeoutException&)
	{
		fail("the handshake has failed, not timed out");
	}
	catch (Poco::Exception&)
	{
	}
}


void ConnectionTest::testHandshakeTimeout()
{
	// A peer that keeps the connection and says nothing: the caller gets
	// the timeout when the handshake timeout it has set is over.
	Poco::Event done;
	Peer peer([&done](StreamSocket&)
	{
		done.tryWait(60000);
	});
	Client client;
	client.manager().setHandshakeTimeout(Timespan(0, 250000));

	Poco::Stopwatch stopwatch;
	stopwatch.start();
	try
	{
		Connection::Ptr pConnection = client.manager().getConnection(peer.uri());
		done.set();
		fail("a connection without a handshake must not be handed out");
	}
	catch (Poco::TimeoutException&)
	{
		done.set();
	}
	catch (...)
	{
		done.set();
		throw;
	}
	assertTrue (stopwatch.elapsedSeconds() < 8);
}


void ConnectionTest::testListenerThreadPool()
{
	// The connections a Listener accepts run on the thread pool of the
	// ConnectionManager the Listener was given.
	Endpoint endpoint;
	Client client;
	assertEqual (0, endpoint.pool().used());

	Connection::Ptr pConnection = client.manager().getConnection(endpoint.uri());
	assertTrue (pConnection->state() == Connection::STATE_ESTABLISHED);
	assertTrue (endpoint.pool().used() > 0);
}


void ConnectionTest::setUp()
{
}


void ConnectionTest::tearDown()
{
}


CppUnit::Test* ConnectionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ConnectionTest");

	CppUnit_addTest(pSuite, ConnectionTest, testHandshakeFailure);
	CppUnit_addTest(pSuite, ConnectionTest, testHandshakeTimeout);
	CppUnit_addTest(pSuite, ConnectionTest, testListenerThreadPool);

	return pSuite;
}
