//
// PoolableConnectionFactory.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  PoolableConnectionFactory
//
// Definition of the PoolableConnectionFactory class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_PoolableConnectionFactory_INCLUDED
#define MongoDB_PoolableConnectionFactory_INCLUDED


#include "Poco/MongoDB/Connection.h"
#include "Poco/ObjectPool.h"
#include "Poco/Timespan.h"


namespace Poco {


template<>
class PoolableObjectFactory<MongoDB::Connection, MongoDB::Connection::Ptr>
	/// PoolableObjectFactory specialisation for Connection. New connections
	/// are created with the given address or URI.
	///
	/// If a Connection::SocketFactory is given, it must live for the entire
	/// lifetime of the PoolableObjectFactory.
	///
	/// It is strongly recommended to use one of the timeout-aware constructors
	/// to avoid indefinite hangs when the server is unreachable.
{
public:
	PoolableObjectFactory(Net::SocketAddress& address):
		_address(address),
		_pSocketFactory(nullptr)
	{
	}

	PoolableObjectFactory(const std::string& address):
		_address(address),
		_pSocketFactory(nullptr)
	{
	}

	PoolableObjectFactory(Net::SocketAddress& address, Poco::Timespan connectTimeout, Poco::Timespan socketTimeout = 0):
		_address(address),
		_pSocketFactory(nullptr),
		_connectTimeout(connectTimeout),
		_socketTimeout(socketTimeout)
	{
	}

	PoolableObjectFactory(const std::string& address, Poco::Timespan connectTimeout, Poco::Timespan socketTimeout = 0):
		_address(address),
		_pSocketFactory(nullptr),
		_connectTimeout(connectTimeout),
		_socketTimeout(socketTimeout)
	{
	}

	PoolableObjectFactory(const std::string& uri, MongoDB::Connection::SocketFactory& socketFactory):
		_uri(uri),
		_pSocketFactory(&socketFactory)
	{
	}

	MongoDB::Connection::Ptr createObject()
	{
		if (_pSocketFactory)
		{
			return new MongoDB::Connection(_uri, *_pSocketFactory);
		}
		else
		{
			MongoDB::Connection::Ptr conn = new MongoDB::Connection();
			conn->connect(_address, _connectTimeout, _socketTimeout);
			return conn;
		}
	}

	bool validateObject(MongoDB::Connection::Ptr pObject)
	{
		return true;
	}

	void activateObject(MongoDB::Connection::Ptr pObject)
	{
	}

	void deactivateObject(MongoDB::Connection::Ptr pObject)
	{
	}

	void destroyObject(MongoDB::Connection::Ptr pObject)
	{
	}

private:
	Net::SocketAddress _address;
	std::string _uri;
	MongoDB::Connection::SocketFactory* _pSocketFactory = nullptr;
	Poco::Timespan _connectTimeout;
	Poco::Timespan _socketTimeout;
};


namespace MongoDB {


class PooledConnection
	/// Helper class for borrowing and returning a connection automatically from a pool.
	/// Note that the connection pool is not expected to be deleted during the lifetime
	/// of an instance of PooledConnection.
{
public:
	PooledConnection(Poco::ObjectPool<Connection, Connection::Ptr>& pool) : _pool(&pool)
	{
		_connection = _pool->borrowObject();
	}

	virtual ~PooledConnection()
	{
		try
		{
			if (_connection)
			{
				_pool->returnObject(_connection);
			}
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	operator Connection::Ptr ()
	{
		return _connection;
	}

	// Disable copy to prevent unwanted release of resources: C++11 way
	PooledConnection(const PooledConnection&) = delete;
	PooledConnection& operator=(const PooledConnection&) = delete;

	// Enable move semantics
	PooledConnection(PooledConnection&& other) = default;
	PooledConnection& operator=(PooledConnection&&) = default;

private:
	Poco::ObjectPool<Connection, Connection::Ptr>* _pool;
	Connection::Ptr _connection;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_PoolableConnectionFactory_INCLUDED
