//
// ORB.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  ORB
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/Proxy.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/RemotingNG/EventListener.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/Format.h"
#include "Poco/URI.h"
#include "Poco/StringTokenizer.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {


ORB::ORB():
	_enabled(true),
	_logger(Poco::Logger::get("RemotingNG.ORB"s))
{
}


ORB::~ORB()
{
}


ORB& ORB::instance()
{
	static Poco::SingletonHolder<ORB> sh;
	return *sh.get();
}


void ORB::shutdown()
{
	_logger.information("Shutting down"s);
	_enabled = false;

	ListenerMap::iterator it = _listeners.begin();
	ListenerMap::iterator itEnd = _listeners.end();
	for (; it != itEnd; ++it)
	{
		it->second->stop();
	}

	_listeners.clear();
	_remoteObjects.clear();
	_remoteObjectURIs.clear();
	_skeletons.clear();
	_uriAliases.clear();
}


bool ORB::invoke(const Listener& listener, const std::string& uri, ServerTransport& transport) const
{
	poco_assert (!uri.empty());

	Poco::URI theURI(uri);
	std::string uriPath = theURI.getPath();

	Skeleton::Ptr pSkeleton;
	RemoteObject::Ptr pRemoteObject;
	std::string objectPath;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		URIAliases::const_iterator ita = findAlias(uriPath);
		if (ita == _uriAliases.end())
		{
			Poco::StringTokenizer tok(theURI.getPathEtc(), "/", Poco::StringTokenizer::TOK_IGNORE_EMPTY);
			if (tok.count() == 3)
			{
				objectPath.reserve(256);
				objectPath += listener.protocol();
				objectPath += '/';
				objectPath += listener.endPoint();
				objectPath += '/';
				objectPath += tok[1];
				objectPath += '/';
				objectPath += tok[2];
			}
			else return false;
		}
		else
		{
			objectPath = ita->second;
		}
		RemoteObjects::const_iterator it = _remoteObjects.find(objectPath);
		if (it == _remoteObjects.end()) return false;
		pRemoteObject = it->second->pRemoteObject;
		pSkeleton = it->second->pSkeleton;
	}

	if (_logger.information())
	{
		_logger.information("Invoking method on object: %s"s, objectPath);
	}

	pSkeleton->invoke(transport, pRemoteObject);
	return true;
}


bool ORB::invoke(const std::string& objectPath, ServerTransport& transport) const
{
	poco_assert (!objectPath.empty());

	Skeleton::Ptr pSkeleton;
	RemoteObject::Ptr pRemoteObject;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		RemoteObjects::const_iterator it = _remoteObjects.find(objectPath);
		if (it == _remoteObjects.end()) return false;
		pRemoteObject = it->second->pRemoteObject;
		pSkeleton = it->second->pSkeleton;
	}

	if (_logger.information())
	{
		_logger.information("Invoking method on object: %s"s, objectPath);
	}

	pSkeleton->invoke(transport, pRemoteObject);
	return true;
}


Identifiable::Ptr ORB::findObject(const std::string& uri) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_enabled) throw ORBDisabledException();

	RemoteObjects::const_iterator itRO = _remoteObjectURIs.find(uri);
	if (itRO != _remoteObjectURIs.end())
	{
		return itRO->second->pRemoteObject;
	}

	Identifiable::TypeId tid;
	Identifiable::ObjectId oid;
	std::string protocol;
	URIUtility::parseURIPath(uri, oid, tid, protocol);

	RemoteObjectInfo::Ptr pRemoteObjectInfo = findLocalObject(tid, oid, protocol);
	if (pRemoteObjectInfo && pRemoteObjectInfo->pListener->handlesURI(uri))
		return pRemoteObjectInfo->pRemoteObject;
	else
		return _proxyFactories.createProxy(tid, oid, protocol, uri);
}


Identifiable::Ptr ORB::findObject(const std::string& uri, const Identifiable::TypeId& tid, const std::string& protocol) const
{
	if (!_enabled) throw ORBDisabledException();

	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		RemoteObjects::const_iterator itRO = _remoteObjectURIs.find(uri);
		if (itRO != _remoteObjectURIs.end())
		{
			return itRO->second->pRemoteObject;
		}
	}

	Identifiable::ObjectId oid;
	return _proxyFactories.createProxy(tid, oid, protocol, uri);
}


ORB::RemoteObjectInfo::Ptr ORB::findLocalObject(const Identifiable::TypeId& tid, const Identifiable::ObjectId& oid, const std::string& protocol) const
{
	for (const auto& lp: _listeners)
	{
		if (lp.second->protocol() == protocol)
		{
			Listener::Ptr pListener = lp.second;
			std::string uri = pListener->createURI(tid, oid);
			RemoteObjects::const_iterator itRO = _remoteObjectURIs.find(uri);
			if (itRO != _remoteObjectURIs.end())
			{
				return itRO->second;
			}
		}
	}
	return 0;
}


std::string ORB::registerListener(Listener::Ptr pListener)
{
	poco_check_ptr (pListener);

	if (_enabled)
	{
		std::string listenerId = pListener->protocol() + ":" + pListener->endPoint();
		std::pair<ListenerMap::iterator, bool> res = _listeners.insert(std::make_pair(listenerId, pListener));
		if (res.second)
			pListener->start();
		else
			throw Poco::IllegalStateException("Listener already registered for that endpoint");

		_logger.information("Listener registered for protocol: %s and endpoint: %s."s, pListener->protocol(), pListener->endPoint());

		return listenerId;
	}
	throw ORBDisabledException();
}


void ORB::unregisterListener(const std::string& listenerId, bool autoRemoveObjects)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	ListenerMap::iterator itListener = _listeners.find(listenerId);
	if (itListener == _listeners.end()) return;
	Listener::Ptr pListener = itListener->second;

	RemoteObjects::iterator itRO = _remoteObjects.begin();
	RemoteObjects::iterator endRO = _remoteObjects.end();
	while (itRO != endRO)
	{
		if (itRO->second->pListener == pListener)
		{
			if (autoRemoveObjects)
			{
				itRO->second->pListener->unregisterObject(itRO->second->pRemoteObject);
				RemoteObjects::iterator delIt = itRO;
				++itRO;
				_remoteObjectURIs.erase(delIt->second->uri);
				_remoteObjects.erase(delIt);
				continue;
			}
			else throw RemotingException("Listener has registered objects and cannot be unregistered");
		}
		++itRO;
	}
	pListener->stop();
	_listeners.erase(itListener);
}


ORB::ListenerVec ORB::listeners() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	ListenerVec result;
	for (const auto& lp: _listeners)
	{
		result.push_back(lp.second);
	}
	return result;
}


std::vector<std::string> ORB::listenerIds() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::vector<std::string> result;
	for (const auto& lp: _listeners)
	{
		result.push_back(lp.first);
	}
	return result;
}


std::vector<std::string> ORB::listenerIds(const std::string& protocol) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::vector<std::string> result;
	for (const auto& lp: _listeners)
	{
		if (lp.second->protocol() == protocol)
			result.push_back(lp.first);
	}
	return result;
}


Listener::Ptr ORB::findListener(const std::string& listenerId) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Listener::Ptr pListener;
	ListenerMap::const_iterator it = _listeners.find(listenerId);
	if (it != _listeners.end())
	{
		pListener = it->second;
	}
	return pListener;
}


void ORB::registerSkeleton(const Identifiable::TypeId& tid, Skeleton::Ptr pSkeleton)
{
	if (!_enabled) throw ORBDisabledException();

	poco_check_ptr (pSkeleton);

	Poco::FastMutex::ScopedLock lock(_mutex);

	_skeletons.insert(make_pair(tid, pSkeleton));
}


void ORB::unregisterSkeleton(const Identifiable::TypeId& tid, bool autoRemoveObjects)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Skeletons::iterator itSkel = _skeletons.find(tid);
	if (itSkel == _skeletons.end()) return;
	Skeleton::Ptr pSkeleton = itSkel->second;

	RemoteObjects::iterator itRO = _remoteObjects.begin();
	RemoteObjects::iterator endRO = _remoteObjects.end();
	while (itRO != endRO)
	{
		if (itRO->second->pSkeleton == pSkeleton)
		{
			if (autoRemoveObjects)
			{
				itRO->second->pListener->unregisterObject(itRO->second->pRemoteObject);
				RemoteObjects::iterator delIt = itRO;
				++itRO;
				_remoteObjectURIs.erase(delIt->second->uri);
				_remoteObjects.erase(delIt);
				continue;
			}
			else throw RemotingException("Skeleton is still in use and cannot be unregistered");
		}
		++itRO;
	}
}


Skeleton::Ptr ORB::skeletonForClass(const Identifiable::TypeId& tid) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Skeletons::const_iterator it = _skeletons.find(tid);
	if (it == _skeletons.end())
	{
		throw UnknownTypeException("No skeleton registered for type", tid);
	}
	return it->second;
}


void ORB::registerProxyFactory(const Identifiable::TypeId& tid, ProxyFactory::Ptr pProxyFactory)
{
	if (!_enabled) throw ORBDisabledException();

	poco_check_ptr (pProxyFactory);

	Poco::FastMutex::ScopedLock lock(_mutex);

	_proxyFactories.registerProxyFactory(tid, pProxyFactory);
}


inline void ORB::unregisterProxyFactory(const Identifiable::TypeId& tid)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_proxyFactories.unregisterProxyFactory(tid);
}


void ORB::registerClass(const Identifiable::TypeId& tid, ProxyFactory::Ptr pProxyFactory, Skeleton::Ptr pSkeleton)
{
	if (!_enabled) throw ORBDisabledException();

	poco_check_ptr (pProxyFactory);
	poco_check_ptr (pSkeleton);

	Poco::FastMutex::ScopedLock lock(_mutex);

	_skeletons.insert(make_pair(tid, pSkeleton));
	_proxyFactories.registerProxyFactory(tid, pProxyFactory);
}


void ORB::unregisterClass(const Identifiable::TypeId& tid, bool autoRemoveObjects)
{
	unregisterSkeleton(tid, autoRemoveObjects);
	unregisterProxyFactory(tid);
}


std::string ORB::registerObject(RemoteObject::Ptr pRemoteObject, const std::string& listenerId)
{
	if (!_enabled) throw ORBDisabledException();

	poco_check_ptr (pRemoteObject);

	Poco::FastMutex::ScopedLock lock(_mutex);

	ListenerMap::iterator itListener = _listeners.find(listenerId);
	if (itListener == _listeners.end())
	{
		throw RemotingException("Listener not found", listenerId);
	}
	Listener::Ptr pListener = itListener->second;

	std::string objectPath;
	objectPath.reserve(256);
	objectPath += pListener->protocol();
	objectPath += '/';
	objectPath += pListener->endPoint();
	objectPath += '/';
	objectPath += pRemoteObject->remoting__typeId();
	objectPath += '/';
	objectPath += pRemoteObject->remoting__objectId();

	RemoteObjects::iterator it = _remoteObjects.find(objectPath);
	if (it != _remoteObjects.end())
	{
		throw RemotingException("The RemoteObject has already been registered", pRemoteObject->remoting__typeId());
	}

	Skeletons::iterator itSkel = _skeletons.find(pRemoteObject->remoting__typeId());
	if (itSkel == _skeletons.end())
	{
		throw UnknownTypeException("No Skeleton available for class", pRemoteObject->remoting__typeId());
	}
	Skeleton::Ptr pSkeleton = itSkel->second;

	std::string uri(pListener->createURI(pRemoteObject->remoting__typeId(), pRemoteObject->remoting__objectId()));

	pListener->registerObject(pRemoteObject, pSkeleton);

	RemoteObjectInfo::Ptr pInfo(new RemoteObjectInfo);
	pInfo->pRemoteObject = pRemoteObject;
	pInfo->pSkeleton     = pSkeleton;
	pInfo->pListener     = pListener;
	pInfo->objectPath    = objectPath;
	pInfo->uri           = uri;

	_remoteObjects.insert(std::make_pair(objectPath, pInfo));
	_remoteObjectURIs.insert(std::make_pair(uri, pInfo));

	if (pRemoteObject->remoting__getURI().empty())
	{
		pRemoteObject->remoting__setURI(Poco::URI(uri));
	}
	else
	{
		_uriAliases.insert(std::make_pair(pRemoteObject->remoting__getURI().getPath(), objectPath));
		_logger.debug("Registered alias path: %s"s, pRemoteObject->remoting__getURI().getPath());
	}

	if (_logger.information())
	{
		_logger.information("Registered RemoteObject, path: %s, URI: %s"s, objectPath, uri);
	}

	try
	{
		ObjectRegistration reg;
		reg.uri = uri;
		reg.alias = pRemoteObject->remoting__getURI().toString();
		reg.pRemoteObject = pRemoteObject;
		reg.pListener = pListener;
		objectRegistered(this, reg);
	}
	catch (Poco::Exception& exc)
	{
		_logger.warning("objectRegistered event handler leaked exception: %s"s, exc.displayText());
	}

	return uri;
}


void ORB::unregisterObject(const std::string& uri)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	RemoteObjects::iterator itRO = _remoteObjectURIs.find(uri);
	if (itRO != _remoteObjectURIs.end())
	{
		ObjectRegistration reg;
		reg.uri = uri;
		reg.alias = itRO->second->pRemoteObject->remoting__getURI().toString();
		reg.pRemoteObject = itRO->second->pRemoteObject;
		reg.pListener = itRO->second->pListener;

		itRO->second->pListener->unregisterObject(itRO->second->pRemoteObject);
		_remoteObjects.erase(itRO->second->objectPath);
		URIAliases::iterator itAl = _uriAliases.find(itRO->second->pRemoteObject->remoting__getURI().getPath());
		if (itAl != _uriAliases.end())
		{
			_uriAliases.erase(itAl);
		}
		_remoteObjectURIs.erase(itRO);

		try
		{
			objectUnregistered(this, reg);
		}
		catch (Poco::Exception& exc)
		{
			_logger.warning("objectUnegistered event handler leaked exception: %s"s, exc.displayText());
		}
	}
}


void ORB::registerEventDispatcher(const std::string& uri, EventDispatcher::Ptr pDispatcher)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	RemoteObjects::iterator itRO = _remoteObjectURIs.find(uri);
	if (itRO != _remoteObjectURIs.end())
	{
		itRO->second->eventDispatchers[pDispatcher->protocol()] = pDispatcher;
		pDispatcher->setOwner(itRO->second->pRemoteObject);
	}
	else throw Poco::NotFoundException("remote object", uri);
}


void ORB::unregisterEventDispatcher(const std::string& uri, const std::string& protocol)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	RemoteObjects::iterator itRO = _remoteObjectURIs.find(uri);
	if (itRO != _remoteObjectURIs.end())
	{
		itRO->second->eventDispatchers.erase(protocol);
	}
	else throw Poco::NotFoundException("remote object", uri);
}


void ORB::unregisterEventDispatchers(const std::string& uri)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	RemoteObjects::iterator itRO = _remoteObjectURIs.find(uri);
	if (itRO != _remoteObjectURIs.end())
	{
		itRO->second->eventDispatchers.clear();
	}
	else throw Poco::NotFoundException("remote object", uri);
}


EventDispatcher::Ptr ORB::findEventDispatcher(const std::string& uri, const std::string& protocol) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::string eventProto = protocol;
	RemoteObjectInfo::Ptr pRemoteObjectInfo;
	RemoteObjects::const_iterator itRO = _remoteObjectURIs.find(uri);
	if (itRO != _remoteObjectURIs.end())
	{
		pRemoteObjectInfo = itRO->second;
	}
	else
	{
		Identifiable::TypeId tid;
		Identifiable::ObjectId oid;
		std::string proto;
		URIUtility::parseURIPath(uri, oid, tid, proto);
		pRemoteObjectInfo = findLocalObject(tid, oid, proto);
		if (eventProto.empty()) eventProto = proto;
	}
	if (pRemoteObjectInfo)
	{
		EventDispatchers::const_iterator itED = pRemoteObjectInfo->eventDispatchers.find(eventProto);
		if (itED != pRemoteObjectInfo->eventDispatchers.end())
		{
			return itED->second;
		}
		else throw Poco::NotFoundException(Poco::format("event dispatcher for URI: %s and protocol: %s"s, uri, protocol));
	}
	else throw Poco::NotFoundException("event dispatcher for URI", uri);
}


ORB::URIAliases::const_iterator ORB::findAlias(const std::string& path) const
{
	for (URIAliases::const_iterator it = _uriAliases.begin(); it != _uriAliases.end(); ++it)
	{
		if (URIUtility::matchPath(path, it->first))
		{
			return it;
		}
	}
	return _uriAliases.end();
}


} } // namespace Poco::RemotingNG
