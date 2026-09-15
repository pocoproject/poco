//
// TransportFactoryManager.cpp
//
// Library: RemotingNG
// Package: Transport
// Module:  TransportFactoryManager
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/Transport.h"


using Poco::SingletonHolder;
using Poco::FastMutex;


namespace Poco {
namespace RemotingNG {


TransportFactoryManager::TransportFactoryManager()
{
}


TransportFactoryManager::~TransportFactoryManager()
{
}


Transport* TransportFactoryManager::createTransport(const std::string& protocol)
{
	FastMutex::ScopedLock lock(_mutex);
	Factories::iterator it = _factories.find(protocol);
	if (it != _factories.end())
	{
		return it->second->createTransport();
	}
	throw UnknownTransportException(protocol);
}


Transport* TransportFactoryManager::createTransport(const std::string& protocol, const std::string& endPoint)
{
	Transport* pTransport = createTransport(protocol);
	try
	{
		pTransport->connect(endPoint);
		return pTransport;
	}
	catch (...)
	{
		delete pTransport;
		throw;
	}
}


void TransportFactoryManager::registerFactory(const std::string& protocol, TransportFactory::Ptr pFactory)
{
	FastMutex::ScopedLock lock(_mutex);
	_factories.insert(make_pair(protocol, pFactory));
}


void TransportFactoryManager::unregisterFactory(const std::string& protocol)
{
	FastMutex::ScopedLock lock(_mutex);
	Factories::iterator it = _factories.find(protocol);
	if (it != _factories.end())
	{
		_factories.erase(it);
	}
}


bool TransportFactoryManager::hasFactory(const std::string& protocol) const
{
	FastMutex::ScopedLock lock(_mutex);
	return _factories.find(protocol) != _factories.end();

}


TransportFactoryManager& TransportFactoryManager::instance()
{
	static SingletonHolder<TransportFactoryManager>sh;
	return *sh.get();
}


} } // namespace Poco::RemotingNG
