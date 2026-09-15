//
// ConnectionManager.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ConnectionManager
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/SingletonHolder.h"
#include "Poco/ScopedUnlock.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


ConnectionManager::ConnectionManager(Poco::ThreadPool& threadPool):
	_pSocketFactory(new SocketFactory),
	_idleTimeout(DEFAULT_IDLE_TIMEOUT, 0),
	_handshakeTimeout(DEFAULT_HANDSHAKE_TIMEOUT, 0),
	_threadPool(threadPool)
{
}


ConnectionManager::ConnectionManager(SocketFactory::Ptr pSocketFactory, Poco::ThreadPool& threadPool):
	_pSocketFactory(pSocketFactory),
	_idleTimeout(DEFAULT_IDLE_TIMEOUT, 0),
	_handshakeTimeout(DEFAULT_HANDSHAKE_TIMEOUT, 0),
	_threadPool(threadPool)
{
}


ConnectionManager::~ConnectionManager()
{
	try
	{
		shutdown();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void ConnectionManager::setIdleTimeout(Poco::Timespan timeout)
{
	_idleTimeout = timeout;
}


void ConnectionManager::setHandshakeTimeout(Poco::Timespan timeout)
{
	_handshakeTimeout = timeout;
}


void ConnectionManager::registerConnection(Connection::Ptr pConnection)
{
	Poco::Net::SocketAddress addr = pConnection->remoteAddress();

	// For a UNIX domain socket, pConnection->remoteAddress() will return 
	// the same "empty" address for every connection. Therefore, we do not
	// add these connections to the _connections map.

#if defined(POCO_OS_FAMILY_UNIX)
	if (addr.family() != Poco::Net::AddressFamily::UNIX_LOCAL)
#endif
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		ConnectionMap::iterator it = _connections.find(addr);
		if (it != _connections.end())
		{
			it->second->abort();
		}
		_connections[addr] = pConnection;
	}

	pConnection->setIdleTimeout(_idleTimeout);
}


void ConnectionManager::unregisterConnection(Connection::Ptr pConnection)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	for (ConnectionMap::iterator it = _connections.begin(); it != _connections.end(); ++it)
	{
		if (it->second == pConnection)
		{
			_connections.erase(it);
			break;
		}
	}
}


Connection::Ptr ConnectionManager::findConnection(const Poco::Net::SocketAddress& peerAddress)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Connection::Ptr pConnection;
	ConnectionMap::iterator it = _connections.find(peerAddress);
	if (it != _connections.end())
	{
		if (it->second->state() == Connection::STATE_ESTABLISHED)
		{
			pConnection = it->second;
		}
		else
		{
			_connections.erase(it);
		}
	}
	return pConnection;
}


Connection::Ptr ConnectionManager::getConnection(const Poco::URI& endpointURI)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::string auth;
	Poco::URI::decode(endpointURI.getAuthority(), auth);
	Poco::Net::SocketAddress addr(auth);

	while (_pendingConnections.find(addr) != _pendingConnections.end())
	{
		Poco::ScopedUnlock<Poco::FastMutex> unlock(_mutex);
		Poco::Thread::sleep(100);
	}

	Connection::Ptr pConnection;
	ConnectionMap::iterator it = _connections.find(addr);
	if (it != _connections.end())
	{
		if (it->second->state() == Connection::STATE_ESTABLISHED)
		{
			pConnection = it->second;
		}
		else
		{
			_connections.erase(it);
		}
	}
	if (!pConnection)
	{
		_pendingConnections.insert(addr);
		try
		{
			Poco::ScopedUnlock<Poco::FastMutex> unlock(_mutex);

			pConnection = createConnection(endpointURI);
		}
		catch (...)
		{
			_pendingConnections.erase(addr);
			throw;
		}
		_pendingConnections.erase(addr);
		_connections[addr] = pConnection;
	}
	return pConnection;
}


void ConnectionManager::shutdown()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	for (ConnectionMap::iterator it = _connections.begin(); it != _connections.end(); ++it)
	{
		it->second->close();
	}

	_connections.clear();
}


Connection::Ptr ConnectionManager::createConnection(const Poco::URI& endpointURI)
{
	Poco::Net::StreamSocket ss = _pSocketFactory->createSocket(endpointURI);
	Connection::Ptr pConnection = new Connection(ss, Connection::MODE_CLIENT);
	pConnection->setIdleTimeout(_idleTimeout);
	pConnection->setHandshakeTimeout(_handshakeTimeout);
	pConnection->addCapability(Frame::CAPA_REMOTING_PROTOCOL_1_0);
	pConnection->addCapability(Frame::CAPA_REMOTING_PROTOCOL_1_1);
	_threadPool.start(*pConnection);
	if (pConnection->waitReady())
	{
		return pConnection;
	}
	else
	{
		pConnection->close();
		throw Poco::TimeoutException("Timeout while waiting for handshake completion with endpoint", endpointURI.toString());
	}
}


namespace
{
	static SingletonHolder<ConnectionManager> sh;
}


ConnectionManager& ConnectionManager::defaultManager()
{
	return *sh.get();
}


} } } // namespace Poco::RemotingNG::TCP
