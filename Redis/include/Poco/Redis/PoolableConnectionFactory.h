//
// PoolableConnectionFactory.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  PoolableConnectionFactory
//
// Definition of the PoolableConnectionFactory class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Redis_PoolableConnectionFactory_INCLUDED
#define Redis_PoolableConnectionFactory_INCLUDED


#include "Poco/Redis/Client.h"
#include "Poco/ObjectPool.h"


namespace Poco {


template<>
class PoolableObjectFactory<Redis::Client, Redis::Client::Ptr>
	/// PoolableObjectFactory specialisation for Client. New connections
	/// are created with the given address.
{
public:
	PoolableObjectFactory(Net::SocketAddress& address)
		: _address(address)
	{
	}

	PoolableObjectFactory(const std::string& address)
		: _address(address)
	{
	}

	Redis::Client::Ptr createObject()
	{
		return new Redis::Client(_address);
	}

	bool validateObject(Redis::Client::Ptr pObject)
	{
		return true;
	}

	void activateObject(Redis::Client::Ptr pObject)
	{
	}

	void deactivateObject(Redis::Client::Ptr pObject)
	{
	}

	void destroyObject(Redis::Client::Ptr pObject)
	{
	}

private:
	Net::SocketAddress _address;
};


namespace Redis {


class PooledConnection
	/// Helper class for borrowing and returning a connection automatically from a pool.
{
public:
	PooledConnection(ObjectPool<Client, Client::Ptr>& pool, long timeoutMilliseconds = 0) : _pool(pool)
	{
		_client = _pool.borrowObject(timeoutMilliseconds);
	}

	virtual ~PooledConnection()
	{
		try
		{
			_pool.returnObject(_client);
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	operator Client::Ptr ()
	{
		return _client;
	}

private:
	ObjectPool<Client, Client::Ptr>& _pool;
	Client::Ptr _client;
};


} // namespace Redis
} // namespace Poco

#endif // Redis_PoolableConnectionFactory_INCLUDED
