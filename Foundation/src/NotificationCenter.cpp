//
// NotificationCenter.cpp
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NotificationCenter.h"
#include "Poco/Notification.h"
#include "Poco/RWLock.h"


namespace Poco {


NotificationCenter::NotificationCenter() = default;


NotificationCenter::~NotificationCenter()
{
	try
	{
		clear();
	}
	catch(...)
	{
		poco_unexpected();
	}
}


void NotificationCenter::addObserver(const AbstractObserver& observer)
{
	AbstractObserverPtr pObserver = observer.clone();
	{
		RWLock::ScopedWriteLock lock(_mutex);
		_observers.push_back(pObserver);
	}

	pObserver->start();
}


void NotificationCenter::removeObserver(const AbstractObserver& observer)
{
	AbstractObserverPtr pObserverToDisable;
	{
		RWLock::ScopedWriteLock lock(_mutex);
		for (auto it = _observers.begin(); it != _observers.end(); ++it)
		{
			if (observer.equals(**it))
			{
				pObserverToDisable = std::move(*it);
				_observers.erase(it);
				break;
			}
		}
	}

	if (pObserverToDisable)
		pObserverToDisable->disable();
}


bool NotificationCenter::hasObserver(const AbstractObserver& observer) const
{
	RWLock::ScopedReadLock lock(_mutex);
	for (const auto& p: _observers)
		if (observer.equals(*p)) return true;

	return false;
}


NotificationCenter::ObserverList NotificationCenter::observersToNotify(const Notification::Ptr& pNotification) const
{
	ObserverList observers;
	{
		RWLock::ScopedReadLock lock(_mutex);
		observers = _observers;
	}

	ObserverList ret;
	for (auto& o : observers)
	{
		if (o->accepts(pNotification))
			ret.push_back(o);
	}
	return ret;
}


void NotificationCenter::postNotification(Notification::Ptr pNotification)
{
	poco_check_ptr (pNotification);

	notifyObservers(pNotification);
}


void NotificationCenter::notifyObservers(Notification::Ptr& pNotification)
{
	poco_check_ptr (pNotification);

	ObserverList observers = observersToNotify(pNotification);
	for (auto& p: observers)
		p->notify(pNotification);
}


bool NotificationCenter::hasObservers() const
{
	RWLock::ScopedReadLock lock(_mutex);

	return !_observers.empty();
}


std::size_t NotificationCenter::countObservers() const
{
	RWLock::ScopedReadLock lock(_mutex);

	return _observers.size();
}


int NotificationCenter::backlog() const
{
	int cnt = 0;

	_mutex.readLock();
	ObserverList observersToCount(_observers);
	_mutex.unlock();
	for (const auto& p : observersToCount)
		cnt += p->backlog();

	return cnt;
}


NotificationCenter& NotificationCenter::defaultCenter()
{
	static NotificationCenter nc;
	return nc;
}


void NotificationCenter::clear()
{
	ObserverList observers;
	{
		RWLock::ScopedWriteLock lock(_mutex);
		_observers.swap(observers);
	}

	for (auto& o: observers)
		o->disable();
}


} // namespace Poco
