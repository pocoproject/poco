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
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Redis_PoolableConnectionFactory_INCLUDED
#define Redis_PoolableConnectionFactory_INCLUDED


#include "Poco/Redis/Client.h"
#include "Poco/ObjectPool.h"
#include "Poco/Version.h"


namespace Poco {


template<>
class PoolableObjectFactory<Redis::Client, Redis::Client::Ptr>
	/// PoolableObjectFactory specialisation for Client. New connections
	/// are created with the given address.
{
public:
	PoolableObjectFactory(Net::SocketAddress& address):
        _address(address)
	{
	}

    PoolableObjectFactory(Net::SocketAddress& address, const std::string& password):
        _address(address),
        _password(password)
	{
	}

	PoolableObjectFactory(const std::string& address):
        _address(address)
	{
	}

    PoolableObjectFactory(const std::string& address, const std::string& password):
        _address(address),
        _password(password)
	{
	}

	Redis::Client::Ptr createObject()
	{
        Redis::Client::Ptr redis = new Redis::Client(_address);

        if (_password.empty()) return redis;

        if (redis->sendAuth(_password)) return redis;

        return NULL;
	}

	bool validateObject(Redis::Client::Ptr pObject)
	{
		if (_password.empty()) return true;

        return pObject->isAuthenticated();
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
    std::string _password;
};


namespace Redis {


class PooledConnection
	/// Helper class for borrowing and returning a connection automatically from a pool.
{
public:
	PooledConnection(ObjectPool<Client, Client::Ptr>& pool, long timeoutMilliseconds = 0) : _pool(pool)
	{
#if POCO_VERSION >= 0x01080000
		_client = _pool.borrowObject(timeoutMilliseconds);
#else
		_client = _pool.borrowObject();
#endif
	}

	virtual ~PooledConnection()
	{
		try
		{
            if (_client)
            {
                _pool.returnObject(_client);
            }
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	operator Client::Ptr()
	{
		return _client;
	}

private:
	ObjectPool<Client, Client::Ptr>& _pool;
	Client::Ptr _client;
};


} } // namespace Poco::Redis


#endif // Redis_PoolableConnectionFactory_INCLUDED
