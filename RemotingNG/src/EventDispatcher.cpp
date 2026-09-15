//
// EventDispatcher.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  EventDispatcher
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/EventDispatcher.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {


EventDispatcher::EventDispatcher(const std::string& protocol):
	_protocol(protocol)
{
}


EventDispatcher::~EventDispatcher()
{
}


void EventDispatcher::setOwner(Poco::AutoPtr<Poco::RefCountedObject> pOwner)
{
	_pOwner = pOwner;
}


void EventDispatcher::subscribe(const std::string& subscriberURI, const std::string& endpointURI, Poco::Clock expireTime)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	SubscriberMap::iterator it = _subscribers.find(subscriberURI);
	if (it == _subscribers.end())
	{
		SubscriberInfo::Ptr pInfo = new SubscriberInfo;
		pInfo->endpoint = endpointURI;
		pInfo->expireTime = expireTime;
		_subscribers[subscriberURI] = pInfo;
	}
	else 
	{
		it->second->expireTime = expireTime;
	}
}


void EventDispatcher::unsubscribe(const std::string& subscriberURI)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	SubscriberMap::iterator it = _subscribers.find(subscriberURI);
	if (it != _subscribers.end())
	{
		_subscribers.erase(it);
	}
	else throw Poco::NotFoundException("event subscriber", subscriberURI);
}


void EventDispatcher::setEventFilterImpl(const std::string& subscriberURI, const std::string& event, const Poco::Any& filter)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	SubscriberMap::iterator it = _subscribers.find(subscriberURI);
	if (it != _subscribers.end())
	{
		it->second->filters[event] = filter;
	}
	else throw Poco::NotFoundException("event subscriber", subscriberURI);
}


void EventDispatcher::removeEventFilter(const std::string& subscriberURI, const std::string& event)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	SubscriberMap::iterator it = _subscribers.find(subscriberURI);
	if (it != _subscribers.end())
	{
		it->second->filters.erase(event);
	}
}


Transport& EventDispatcher::transportForSubscriber(const std::string& subscriberURI)
{
	// Note: the mutex will have already been locked by the caller
	
	static TransportFactoryManager& tfm = TransportFactoryManager::instance();
	
	SubscriberMap::iterator it = _subscribers.find(subscriberURI);
	if (it != _subscribers.end())
	{
		if (!it->second->pTransport)
		{
			it->second->pTransport = tfm.createTransport(_protocol, it->second->endpoint);
		}
		return *it->second->pTransport;
	}
	else throw Poco::NotFoundException("event subscriber", subscriberURI);
}


} } // namespace Poco::RemotingNG
