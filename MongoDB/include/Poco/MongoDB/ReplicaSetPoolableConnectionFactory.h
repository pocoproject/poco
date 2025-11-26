//
// ReplicaSetPoolableConnectionFactory.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSetPoolableConnectionFactory
//
// Definition of the ReplicaSetPoolableConnectionFactory class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ReplicaSetPoolableConnectionFactory_INCLUDED
#define MongoDB_ReplicaSetPoolableConnectionFactory_INCLUDED


#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/ObjectPool.h"


namespace Poco {


template<>
class PoolableObjectFactory<MongoDB::ReplicaSetConnection, MongoDB::ReplicaSetConnection::Ptr>
	/// PoolableObjectFactory specialization for ReplicaSetConnection.
	///
	/// New connections are created from the given ReplicaSet with the
	/// specified ReadPreference.
	///
	/// Usage example:
	///   Poco::SharedPtr<ReplicaSet> rs(new ReplicaSet(config));
	///   PoolableObjectFactory<ReplicaSetConnection, ReplicaSetConnection::Ptr> factory(*rs, ReadPreference::PrimaryPreferred);
	///   Poco::ObjectPool<ReplicaSetConnection, ReplicaSetConnection::Ptr> pool(factory, 10, 20);
	///
	///   {
	///       PooledReplicaSetConnection conn(pool);
	///       conn->sendRequest(request, response);
	///   }  // Automatically returned to pool
	///
	/// IMPORTANT:
	/// The ReplicaSet instance must outlive the PoolableObjectFactory and the ObjectPool.
	/// Using a SharedPtr for the ReplicaSet is recommended.
{
public:
	PoolableObjectFactory(MongoDB::ReplicaSet& replicaSet, const MongoDB::ReadPreference& readPref):
		_replicaSet(replicaSet),
		_readPreference(readPref)
	{
	}

	MongoDB::ReplicaSetConnection::Ptr createObject()
	{
		return new MongoDB::ReplicaSetConnection(_replicaSet, _readPreference);
	}

	bool validateObject(MongoDB::ReplicaSetConnection::Ptr pObject)
	{
		// Check if the connection is still valid
		return pObject->isConnected();
	}

	void activateObject(MongoDB::ReplicaSetConnection::Ptr pObject)
	{
		// No action needed - connection is established lazily
	}

	void deactivateObject(MongoDB::ReplicaSetConnection::Ptr pObject)
	{
		// No action needed - keep connection alive for reuse
	}

	void destroyObject(MongoDB::ReplicaSetConnection::Ptr pObject)
	{
		// Connection is destroyed automatically when Ptr goes out of scope
	}

private:
	MongoDB::ReplicaSet& _replicaSet;
	MongoDB::ReadPreference _readPreference;
};


namespace MongoDB {


class PooledReplicaSetConnection
	/// Helper class for borrowing and returning a ReplicaSetConnection
	/// automatically from a pool.
	///
	/// This class uses RAII to automatically return the connection to the
	/// pool when it goes out of scope.
	///
	/// Usage:
	///   {
	///       PooledReplicaSetConnection conn(pool);
	///       conn->sendRequest(request, response);
	///   }  // Connection automatically returned to pool
	///
	/// Note: The connection pool must outlive the PooledReplicaSetConnection instance.
{
public:
	PooledReplicaSetConnection(Poco::ObjectPool<ReplicaSetConnection, ReplicaSetConnection::Ptr>& pool):
		_pool(&pool)
	{
		_connection = _pool->borrowObject();
	}

	virtual ~PooledReplicaSetConnection()
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

	operator ReplicaSetConnection::Ptr ()
	{
		return _connection;
	}

	ReplicaSetConnection::Ptr operator->()
	{
		return _connection;
	}

	ReplicaSetConnection& operator*()
	{
		return *_connection;
	}

	// Disable copy to prevent unwanted release of resources
	PooledReplicaSetConnection(const PooledReplicaSetConnection&) = delete;
	PooledReplicaSetConnection& operator=(const PooledReplicaSetConnection&) = delete;

	// Enable move semantics
	PooledReplicaSetConnection(PooledReplicaSetConnection&& other) noexcept = default;
	PooledReplicaSetConnection& operator=(PooledReplicaSetConnection&& other) noexcept = default;

private:
	Poco::ObjectPool<ReplicaSetConnection, ReplicaSetConnection::Ptr>* _pool;
	ReplicaSetConnection::Ptr _connection;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_ReplicaSetPoolableConnectionFactory_INCLUDED
