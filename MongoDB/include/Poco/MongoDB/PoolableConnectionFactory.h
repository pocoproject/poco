#ifndef _MongoDB_PoolableConnectionFactory_included
#define _MongoDB_PoolableConnectionFactory_included

#include "Poco/MongoDB/Connection.h"
#include "Poco/ObjectPool.h"

namespace Poco
{
namespace MongoDB
{
	
	
template<>
class PoolableObjectFactory<Connection, Connection::Ptr>
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


	Connection::Ptr createObject()
	{
		return new Connection(_address);
	}
	
	
	bool validateObject(Connection::Ptr pObject)
	{
		return true;
	}
	

	void activateObject(Connection::Ptr pObject)
	{
	}
	

	void deactivateObject(Connection::Ptr pObject)
	{
	}
	

	void destroyObject(Connection::Ptr pObject)
	{
	}
	

private:

	Net::SocketAddress _address;
};
	
	
}} // Poco::MongoDB

#endif //_MongoDB_PoolableConnectionFactory_included
