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
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/EventDispatcher.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/Exception.h"


namespace
{
	class Unlocker
		/// Unlocks a mutex that tryLock() has locked.
	{
	public:
		explicit Unlocker(Poco::FastMutex& mutex):
			_mutex(mutex)
		{
		}

		~Unlocker()
		{
			_mutex.unlock();
		}

		Unlocker(const Unlocker&) = delete;
		Unlocker& operator = (const Unlocker&) = delete;

	private:
		Poco::FastMutex& _mutex;
	};
}


namespace Poco {
namespace RemotingNG {


EventDispatcher::EventDispatcher(const std::string& protocol):
	_protocol(protocol)
{
}


EventDispatcher::~EventDispatcher()
{
}


void EventDispatcher::setOwner(const Poco::AutoPtr<Poco::RefCountedObject>& pOwner)
{
	_pOwner = pOwner;
}


void EventDispatcher::subscribe(const std::string& subscriberURI, const std::string& endpointURI, Poco::Clock expireTime)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	addSubscriber(subscriberURI, endpointURI, expireTime);
}


void EventDispatcher::unsubscribe(const std::string& subscriberURI)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	removeSubscriber(subscriberURI);
}


bool EventDispatcher::trySubscribe(const std::string& subscriberURI, const std::string& endpointURI, Poco::Clock expireTime)
{
	if (!_mutex.tryLock()) return false;
	Unlocker unlocker(_mutex);

	addSubscriber(subscriberURI, endpointURI, expireTime);
	return true;
}


bool EventDispatcher::tryUnsubscribe(const std::string& subscriberURI)
{
	if (!_mutex.tryLock()) return false;
	Unlocker unlocker(_mutex);

	removeSubscriber(subscriberURI);
	return true;
}


void EventDispatcher::addSubscriber(const std::string& subscriberURI, const std::string& endpointURI, Poco::Clock expireTime)
{
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


void EventDispatcher::removeSubscriber(const std::string& subscriberURI)
{
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
