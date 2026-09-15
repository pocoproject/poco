//
// ConnectionManager.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ConnectionManager
//
// Definition of the ConnectionManager class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_ConnectionManager_INCLUDED
#define RemotingNG_TCP_ConnectionManager_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/Connection.h"
#include "Poco/RemotingNG/TCP/SocketFactory.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/ThreadPool.h"
#include <map>
#include <set>


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API ConnectionManager
	/// The ConnectionManager manages a collection of Connection objects.
	///
	/// Connection objects are either created by a ServerConnection as a result of
	/// an accepted connection request by the server, or by the ConnectionManager,
	/// in the case of client-side Connection objects.
	///
	/// For creation of Connection objects the ConnectionManager uses
	/// a SocketFactory.
	///
	/// A Poco::ThreadPool is used to obtain threads for driving the protocol
	/// on the client side, as well as for scheduling requests on the server
	/// and events on the client.
{
public:
	ConnectionManager(Poco::ThreadPool& threadPool = Poco::ThreadPool::defaultPool());
		/// Creates a ConnectionManager using the default
		/// SocketFactory.
		///
		/// Optionally, a Poco::ThreadPool can be specified
		/// if the default pool should not be used.

	ConnectionManager(SocketFactory::Ptr pSocketFactory, Poco::ThreadPool& threadPool = Poco::ThreadPool::defaultPool());
		/// Creates a ConnectionManager using the
		/// given SocketFactory.
		///
		/// Optionally, a Poco::ThreadPool can be specified
		/// if the default pool should not be used.

	~ConnectionManager();
		/// Destroys the ConnectionManager.

	void setIdleTimeout(Poco::Timespan timeout);
		/// Sets the timeout after an idle connection is closed.

	Poco::Timespan getIdleTimeout() const;
		/// Returns the idle connection timeout.

	void setHandshakeTimeout(Poco::Timespan timeout);
		/// Sets the timeout for the initial connection handshake.

	Poco::Timespan getHandshakeTimeout() const;
		/// Returns the timeout for the initial connection handshake.

	void registerConnection(Connection::Ptr pConnection);
		/// Registers an existing connection.

	void unregisterConnection(Connection::Ptr pConnection);
		/// Unregisters an existing connection.

	Connection::Ptr findConnection(const Poco::Net::SocketAddress& peerAddress);
		/// Searches for a connection to the given peer address.
		///
		/// If a connection exists, and the connection is in established state,
		/// it is returned. Otherwise, a null pointer is returned.

	Connection::Ptr getConnection(const Poco::URI& endpointURI);
		/// Searches for a connection to the given endpoint.
		///
		/// If a connection exists, and the connection is in established state,
		/// it is returned. Otherwise, a new connection to that endpoint
		/// is created.

	void shutdown();
		/// Closes all connections.

	ThreadPool& threadPool();
		/// Returns a reference to the ConnectionManager's thread pool.

	static ConnectionManager& defaultManager();
		/// Returns the default ConnectionManager instance.

protected:
	Connection::Ptr createConnection(const Poco::URI& endpointURI);

private:
	enum
	{
		DEFAULT_IDLE_TIMEOUT      = 60,
		DEFAULT_HANDSHAKE_TIMEOUT = 8
	};

	ConnectionManager(const ConnectionManager&);
	ConnectionManager& operator = (const ConnectionManager&);

	using ConnectionMap = std::map<Poco::Net::SocketAddress, Connection::Ptr>;
	using SocketAddressSet = std::set<Poco::Net::SocketAddress>;

	SocketFactory::Ptr _pSocketFactory;
	Poco::Timespan _idleTimeout;
	Poco::Timespan _handshakeTimeout;
	Poco::ThreadPool& _threadPool;
	ConnectionMap _connections;
	SocketAddressSet _pendingConnections;
	Poco::FastMutex _mutex;
};


//
// inlines
//
inline Poco::Timespan ConnectionManager::getIdleTimeout() const
{
	return _idleTimeout;
}


inline Poco::Timespan ConnectionManager::getHandshakeTimeout() const
{
	return _handshakeTimeout;
}


inline Poco::ThreadPool& ConnectionManager::threadPool()
{
	return _threadPool;
}


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_ConnectionManager_INCLUDED
