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
#include "Poco/Observer.h"
#include "Poco/AutoPtr.h"


namespace Poco {


NotificationCenter::NotificationCenter()
{
}


NotificationCenter::~NotificationCenter()
{
	try
	{
		Mutex::ScopedLock lock(_mutex);
		for (auto& o: _observers)
			o->disable();

		_observers.clear();
	}
	catch(...)
	{
		poco_unexpected();
	}
}


void NotificationCenter::addObserver(const AbstractObserver& observer)
{
	Mutex::ScopedLock lock(_mutex);
	_observers.push_back(observer.clone());
}


void NotificationCenter::removeObserver(const AbstractObserver& observer)
{
	Mutex::ScopedLock lock(_mutex);
	for (ObserverList::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		if (observer.equals(**it))
		{
			(*it)->disable();
			_observers.erase(it);
			return;
		}
	}
}


bool NotificationCenter::hasObserver(const AbstractObserver& observer) const
{
	Mutex::ScopedLock lock(_mutex);
	for (const auto& p: _observers)
		if (observer.equals(*p)) return true;

	return false;
}


void NotificationCenter::postNotification(Notification::Ptr pNotification)
{
	poco_check_ptr (pNotification);

	ScopedLockWithUnlock<Mutex> lock(_mutex);
	ObserverList observersToNotify(_observers);
	lock.unlock();
	for (auto& p: observersToNotify)
	{
		p->notify(pNotification);
	}
}


bool NotificationCenter::hasObservers() const
{
	Mutex::ScopedLock lock(_mutex);

	return !_observers.empty();
}


std::size_t NotificationCenter::countObservers() const
{
	Mutex::ScopedLock lock(_mutex);

	return _observers.size();
}


NotificationCenter& NotificationCenter::defaultCenter()
{
	static NotificationCenter nc;
	return nc;
}


} // namespace Poco
