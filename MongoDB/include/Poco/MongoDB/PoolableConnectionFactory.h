#ifndef _MongoDB_PoolableConnectionFactory_included
#define _MongoDB_PoolableConnectionFactory_included

#include "Poco/MongoDB/Connection.h"
#include "Poco/ObjectPool.h"

namespace Poco
{


template<>
class MongoDB_API PoolableObjectFactory<MongoDB::Connection, MongoDB::Connection::Ptr>
	/// PoolableObjectFactory specialisation for Connection. New connections
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


	MongoDB::Connection::Ptr createObject()
	{
		return new MongoDB::Connection(_address);
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
};

namespace MongoDB
{


class MongoDB_API PooledConnection
	/// Helper class for borrowing and returning a connection automatically from a pool.
{
public:
	PooledConnection(Poco::ObjectPool<Connection, Connection::Ptr>& pool) : _pool(pool)
	{
		_connection = _pool.borrowObject();
	}

	virtual ~PooledConnection()
	{
		_pool.returnObject(_connection);
	}

	operator Connection::Ptr () { return _connection; }

private:
	Poco::ObjectPool<Connection, Connection::Ptr>& _pool;

	Connection::Ptr _connection;
};

} // MongoDB


} // Poco

#endif //_MongoDB_PoolableConnectionFactory_included
