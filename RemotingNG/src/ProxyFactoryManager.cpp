//
// ProxyFactoryManager.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  ProxyFactoryManager
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/ProxyFactoryManager.h"
#include "Poco/RemotingNG/ProxyFactory.h"
#include "Poco/RemotingNG/Proxy.h"
#include "Poco/RemotingNG/RemotingException.h"


using Poco::FastMutex;


namespace Poco {
namespace RemotingNG {


ProxyFactoryManager::ProxyFactoryManager()
{
}


ProxyFactoryManager::~ProxyFactoryManager()
{
}


void ProxyFactoryManager::registerProxyFactory(const Identifiable::TypeId& tid, ProxyFactory::Ptr pProxyFactory)
{
	FastMutex::ScopedLock lock(_mutex);
	_factories.insert(make_pair(tid, pProxyFactory));
}


void ProxyFactoryManager::unregisterProxyFactory(const Identifiable::TypeId& tid)
{
	FastMutex::ScopedLock lock(_mutex);
	Factories::iterator it = _factories.find(tid);
	if (it != _factories.end())
	{
		_factories.erase(it);
	}
}


Proxy* ProxyFactoryManager::createProxy(const Identifiable::TypeId& tid, const RemotingNG::Identifiable::ObjectId& oid, const std::string& protocol, const std::string& uri) const
{
	Proxy* pProxy = createProxy(tid, oid);
	try
	{
		pProxy->remoting__connect(protocol, uri);
		return pProxy;
	}
	catch (...)
	{
		delete pProxy;
		throw;
	}
}


Proxy* ProxyFactoryManager::createProxy(const Identifiable::TypeId& tid, const RemotingNG::Identifiable::ObjectId& oid) const
{
	FastMutex::ScopedLock lock(_mutex);

	Factories::const_iterator it = _factories.find(tid);
	if (it != _factories.end())
	{
		return it->second->createProxy(oid);
	}
	throw UnknownTypeException(tid);
}


} } // namespace Poco::RemotingNG
